#pragma once

#include <vector>
#include "ofMain.h"
#include "ofxLaserTSPDefs.h"
#include "ofxLaserTSPRouteNode.h"

/*
 * Polyline Route Optimizer.
 * Written by Bryce Summers on 11/18/2016.
 */

namespace ofxLaserTSP
{
	template<typename T>
	class RouteOptimizer
	{
		static_assert(std::is_base_of<ofPolyline, T>::value || std::is_same<ofPolyline, T>::value,
					  "Templated class needs to be ofPolyline or inherit from it");
	private:
		std::vector<T> route;
		std::vector<T> outputRoute;
		bool closed;
		
		// Represents the index of the start of the linked list.
		// This is important for non closed solutions, so that they can align the permutation correctly.
		// This is changed when a route is returned to a user.
		size_t start_index;
		
	public:
		RouteOptimizer(const std::vector<T> & route, bool closed = true);
		
		// Returns a heuristically optimized route.
		// This is guranteed to be a new route in memory.
		const std::vector<T>& optimize(int passes);
		
		// Permutes the given permutation by the permutation constructed by this RouteOptimizer.
		void permute(std::vector<size_t> &permutation);
		
		static bool & useSquareDistance();
		
	private:
		// A local allocation of memory for all of the route nodes,
		// which constitute a modifiable linked list.
		std::vector<std::shared_ptr<RouteNode>>  nodes;	 // Stored in by index, nodes[i].id = i;
		std::vector<glm::vec3>  points;  // Stored in by index, points[nodex[i].start]
		
		// Flips node i if it decreases the global metric.
		bool attemptFlip(int i);
		
		// Attempts to rearrange the pointer for node i1 -> i2.
		bool attemptSwap(int i1, int i2);
		
		// global metric.
		// Returns a consistent for how long the path is.
		// We wish to minimize this value.
		float metric(const std::vector<std::shared_ptr<RouteNode>> & nodes);
		
		// This is where I design the macro optimization passes.
		bool aplyOptimizationPasses(int passes);
		
		// Returns a consistent heuristic for the length of a path from points[id1] to points[id2].
		float metric(const size_t& id1, const size_t& id2);
		
		// returns the index of the node where the longest edge points finishes in the forward ordering.
		// The return value will indicate the proper start_index for open problems.
		size_t getLongestEdgeIndex(const std::vector<std::shared_ptr<RouteNode>> & nodes);
		
		// Returns a consistent heuristic for the length of a path, with a penalty for sharp angles.
		// float metric(int id1_p, int id1, int id2, int id2_p);
		
		// Converts from this RouteOptimizer's internal structures back to a route representation.
		void toRoute();
		
	};
	
//----------------------------------------------------------------------------------
//              IMPLEMENTATION
//----------------------------------------------------------------------------------
	template<typename T>
	RouteOptimizer<T>::RouteOptimizer(const std::vector<T> &  route, bool closed){
		this -> route = route;
		this -> closed = closed;
		int start_index = 0;
		
		nodes.clear();
		points.clear();
		
		size_t id  = 0; // next Route ID.
		size_t pid = 0; // next Point ID.
		
		// Allocate all of the Route Nodes and Points.
		for(auto& r: route){
			auto node = std::make_shared<RouteNode>(id, pid, pid + 1);
			id  += 1;
			pid += 2;
			nodes.push_back(node);
			
			points.push_back(r.getVertices().front());
			points.push_back(r.getVertices().back());
			
		}
		
		// Link up the entire list of nodes.
		size_t len = nodes.size();
		for (int i = 0; i < len; i++)
		{
			nodes[i]->setPrev(nodes[(i + len - 1) % len]);
			nodes[i]->setNext(nodes[(i + len + 1) % len]);
		}
	}
	
	//----------------------------------------------------------------------------------
	template<typename T>
	const std::vector<T>& RouteOptimizer<T>::optimize(int passes){
		outputRoute.clear();
		if(aplyOptimizationPasses(passes)){
			toRoute();
		}
		return outputRoute;
	}
	//----------------------------------------------------------------------------------
	template<typename T>
	bool RouteOptimizer<T>::aplyOptimizationPasses(int passes)
	{
		// Do some optimization here...
		int len = nodes.size();
		
		// Original Metric.
		float score = metric(nodes);
		
		// Optimize through routeNode flipping.
		// O(n).
		for (int i = 0; i < len; i++)
		{
			attemptFlip(i);
			
			float new_score = metric(nodes);
			if (new_score > score + 2)
			{
				ofLogError("ofxLaserTSP::RouteOptimizer::aplyOptimizationPasses") << "A Deoptimization has Occured while attempting flip!";
				return false;
			}
			score = new_score;
		}
		
		// Try swapping route pointers...
		// O(n^3)
		for (int pass = 0; pass < passes; pass++)
		{
			for (int i1 = 0; i1 < len; i1++){
				for (int i2 = i1 + 1; i2 < len; i2++)
				{
					attemptSwap(i1, i2);
					
					float new_score = metric(nodes);
					if (new_score > score + 2)
					{
						ofLogError("ofxLaserTSP::RouteOptimizer::aplyOptimizationPasses") << "A Deoptimization has Occured while attempting swap!";
						return false;
					}
					new_score = score;
					
				}
			}
		}
		return true;
	}
	//----------------------------------------------------------------------------------
	template<typename T>
	void RouteOptimizer<T>::toRoute()
	{
		outputRoute.clear();
		// Convert RouteOptimizer Structures back to a route representation.
		
		// retrive the start index, such that we start after the most distant edge.
		this -> start_index = getLongestEdgeIndex(nodes);
		
		auto first   = nodes[this -> start_index];
		auto current = first;
		do
		{
			size_t& ID = current -> id;
			
			// NOTE: We could easily make an output format that gives the users the flipped bools, instead of flipped lists.
			// I like the lists, because we could potentially chop up paths in the future for other applications.
			outputRoute.push_back(this -> route[ID]);
			
			if (current -> flipped)
			{
				reverse_polyline(outputRoute.back());
			}
			// Iterate.
			current = current -> getNext();
		}while(current != first);
	}
	//----------------------------------------------------------------------------------
	// -- Optimization Helper Functions.
	template<typename T>
	bool RouteOptimizer<T>::attemptFlip(int i)
	{
		auto & node = nodes[i];
		
		// The back connection.
		const size_t & ip0 = node->getPrev()->index_end;
		const size_t & ip1 = node->index_start;
		
		// The forward connection.
		const size_t & ip2 = node->index_end;
		const size_t & ip3 = node->getNext()-> index_start;
		
		
		if((metric(ip0, ip2) + metric(ip1, ip3)) < (metric(ip0, ip1) + metric(ip2, ip3))){
			node->flip();
			return true;
		}
		return false;
	}
	//----------------------------------------------------------------------------------
	// Attempts to point i1 to i2 and
	template<typename T>
	bool RouteOptimizer<T>::attemptSwap(int i1, int i2)
	{
		auto& node1 = nodes[i1];
		auto& node2 = nodes[i2];
		
		auto next1 = node1 -> getNext();
		auto next2 = node2 -> getNext();
		
		// Swaps are impossible for successive nodes.
		if(next1.get() == node2.get())
		{
			return false;
		}
		
		auto prev1 = node1 -> getPrev();
		auto prev2 = node2 -> getPrev();
		
		float metric_0 = metric(node1 -> index_end, next1 -> index_start) +
		metric(node2 -> index_end, next2 -> index_start);
		float metric_1 = metric(node1 -> index_end, node2 -> index_end) +
		metric(next1 -> index_start, next2 -> index_start);
		// No benifit.
		if (metric_0 < metric_1)
		{
			return false;
		}
		
		node1 -> setNext(node2);
		next1 -> setPrev(next2);// reversed later.
		next2 -> setPrev(next1);// Link the latter part back. This won't need to be reversed.
		node2 -> setNext(node1);// reversed later.
		
		// Reverse next1 through node2.
		
		auto current = next1;
		auto end = node2;
		do
		{
			auto next = current -> getNext();
			current -> reverse();
			current = next;
		}while(current != end);
		
		current -> reverse();// Reverse the end.
		
		// Perform some opportunistic flips.
		attemptFlip(node1 -> id);
		attemptFlip(next1 -> id);
		attemptFlip(node2 -> id);
		attemptFlip(next2 -> id);
		
		return true;
	}
	//----------------------------------------------------------------------------------
	// global metric.
	// Returns a consistent for how long the path is.
	// We wish to minimize this value.
	// We use this metric for open and closed problems, then simply remove the longest edge afterwards.
	template<typename T>
	float RouteOptimizer<T>::metric(const std::vector<std::shared_ptr<RouteNode>>& nodes)
	{
		// Accumulator.
		float accum = 0.0f;
		// Warning, this is going in arbitrary original order, because the order that we add the segments up doesn't matter.
		// If you change this to rely on the permuted ordering of nodes, transverse this using next pointers.
		for(auto& node: nodes){
			accum += metric(node->index_end, node->getNext()-> index_start);
		}
		
		return accum;
	}
	//----------------------------------------------------------------------------------
	template<typename T>
	size_t RouteOptimizer<T>::getLongestEdgeIndex(const std::vector<std::shared_ptr<RouteNode>>& nodes)
	{
		size_t index = 0;
		bool bFound = false;
		float max = 0;
		
		for(auto& node: nodes)
		{
			auto next = node->getNext();
			float dist = metric(node->index_end,  next->index_start);
			
			if (dist > max){
				bFound = true;
				index = next -> id;
				max = dist;
			}
		}
		if(!bFound){
			ofLogError("ofxLaserTSP::RouteOptimizer::getLongestEdgeIndex") << "No maximum edge distance was found.";
		}
		return index;
	}
	//----------------------------------------------------------------------------------
	// Returns a consistent heuristic for the length of a path from id1 to id2.
	template<typename T>
	float RouteOptimizer<T>::metric(const size_t& id1, const size_t& id2)
	{
		if (useSquareDistance()){
			return glm::distance2(points[id1], points[id2]);
		}
		return glm::distance(points[id1], points[id2]);
	}
	//----------------------------------------------------------------------------------
	template<typename T>
	void RouteOptimizer<T>::permute(std::vector<size_t> & permutation_in_out)
	{
		
		std::vector<size_t> permutation;
		std::vector<size_t> permutation_out;
		
		// First compute the relative permutation from the input route to this optimizer to the present route.
		// ASSERTION( This loop goes n times, where n = nodes.length;
		auto start = nodes[this -> start_index];
		auto current = start;
		do
		{
			permutation.push_back(current -> id);
			current = current -> getNext();
		} while (current != start);
		
		// Next permute the input permutation by the relative permutation.
		// i is the local index -> relative index -> global index.
		size_t len = permutation_in_out.size();
		for (size_t i = 0; i < len; i++)
		{
			size_t& relative_index = permutation[i];
			size_t& global_index   = permutation_in_out[relative_index];
			permutation_out.push_back(global_index);
		}
		
		// Finnally copy the resultant global permutation onto the input_output buffer.
		for (int i = 0; i < len; i++){
			permutation_in_out[i] = permutation_out[i];
		}
	}
	//----------------------------------------------------------------------------------
	template<typename T>
	bool& RouteOptimizer<T>::useSquareDistance(){
		static std::unique_ptr<bool> i =  make_unique<bool>(true);
		return *i;
	}
}
