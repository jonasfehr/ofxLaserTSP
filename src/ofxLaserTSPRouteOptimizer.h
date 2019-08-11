#pragma once

#include <vector>
#include "ofMain.h"
#include "ofxLaserTSPDefs.h"
#include "ofxLaserTSPRouteNode.h"

/*
 * Polyline Route Optimizer.
 * Written by Bryce Summers on 11/18/2016.
 */

namespace bryce_tsp
{

    class RouteOptimizer
    {

    private:
        std::vector<ofPolyline> route;
		std::vector<ofPolyline> outputRoute;
        bool closed;

        // Represents the index of the start of the linked list.
        // This is important for non closed solutions, so that they can align the permutation correctly.
        // This is changed when a route is returned to a user.
        size_t start_index;

    public:
        RouteOptimizer(const std::vector<ofPolyline> & route, bool closed = true);

        // Returns a heuristically optimized route.
        // This is guranteed to be a new route in memory.
        const std::vector<ofPolyline>& optimize(int passes);

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

}
