#include "ofxLaserTSPRouteOptimizer.h"
#include <stdexcept>

namespace ofxLaserTSP
{
//----------------------------------------------------------------------------------
    RouteOptimizer::RouteOptimizer(const std::vector<ofPolyline> &  route, bool closed)
    {
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
    const std::vector<ofPolyline>& RouteOptimizer::optimize(int passes){
		outputRoute.clear();
		if(aplyOptimizationPasses(passes)){
			toRoute();
		}
		return outputRoute;
    }
//----------------------------------------------------------------------------------
    bool RouteOptimizer::aplyOptimizationPasses(int passes)
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
   void RouteOptimizer::toRoute()
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

    bool RouteOptimizer::attemptFlip(int i)
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
    bool RouteOptimizer::attemptSwap(int i1, int i2)
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
    float RouteOptimizer::metric(const std::vector<std::shared_ptr<RouteNode>>& nodes)
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
    size_t RouteOptimizer::getLongestEdgeIndex(const std::vector<std::shared_ptr<RouteNode>>& nodes)
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
    float RouteOptimizer::metric(const size_t& id1, const size_t& id2)
    {
        // FIXME: Square distance would be faster.
		if (useSquareDistance()){
			return glm::distance2(points[id1], points[id2]);
		}
		return glm::distance(points[id1], points[id2]);
    }
//----------------------------------------------------------------------------------
    void RouteOptimizer::permute(std::vector<size_t> & permutation_in_out)
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
	bool& RouteOptimizer::useSquareDistance(){
		static std::unique_ptr<bool> i =  make_unique<bool>(false);
		return *i;
	}
}
