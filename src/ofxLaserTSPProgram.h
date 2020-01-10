#pragma once

#include "ofMain.h"
#include "ofxLaserTSPRouteOptimizer.h"
#include "ContentBase.hpp"
//namespace ofxLaserTSP
//{

	template<typename T>
    class ofxLaserTSPLaserProgram
    {
		static_assert(std::is_base_of<ContentBase::PointGroup, T>::value || std::is_same<ContentBase::PointGroup, T>::value,
					  "Templated class needs to be ContentBase::PointGroup or inherit from it");
		
    private:
        // Private Route object.
		std::vector<T> route;
        bool closed;

        // path_permutations[route[id]] = id of route in original input.
        std::vector<size_t> path_permutation;
    public:
        // ASSUMES: route allocated via new. The Laser Program now owns the input route and will deal with deallocation.
        // ASSUMES: every route contains at least 2 points.
		ofxLaserTSPLaserProgram(const std::vector<T>& route, bool closed = true)
		{
			this -> route = route;
			this -> closed = closed;
			
			for (size_t i = 0; i < route.size(); i++)
			{
				path_permutation.push_back(i);
			}
		}

        // -- Instructs the LaserProgram to Optimize its route.
        // Updates the internal route pointer to a new route.
        // It frees the original route.

        // FIXME: I would like to add some inputs for maximum time or maximum distance.
        // I could add a boolean return if it meets the input requirements.
//		template<typename T>
        void optimize(int passes)
		{
			if (passes <= 0){
				return;
			}
			
			ofxLaserTSP::RouteOptimizer<T> optimizer(this -> route, this -> closed);
			this -> route = optimizer.optimize(passes);
			
			// Get the permutation for concerned artists who want their original colors, strokes, etc.
			optimizer.permute(path_permutation);
			
		}

        // Returns the current route. This will be an optimized route if the optimize function has been used.
        const std::vector<T>& getRoute() const {return route;}
			  std::vector<T>& getRoute() { return route; }

        // Determines the index of the given index in the original data after the optimization permutation has been applied.
        const size_t& lookup_original_index(const size_t & current_index)
		{
			return path_permutation[current_index];
		}


       
    };
//#include "ofxLaserTSPProgram.inl"
//}
