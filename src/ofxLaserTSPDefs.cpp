#include "ofxLaserTSPDefs.h"

namespace ofxLaserTSP
{
//    Route * copy(Route * route)
//    {
//        Route * output = new Route();
//        for (auto iter = route->begin(); iter != route->end(); iter++)
//        {
//            output->push_back(copy_polyline(*iter));
//        }
//
//        return output;
//    };
//
//    Polyline * copy_polyline(Polyline * route)
//    {
//        Polyline * output = new Polyline();
//        for (auto iter = route->cbegin(); iter != route->cend(); iter++)
//        {
//            output->push_back(*iter);
//        }
//
//        return output;
//    };
//
//    Polyline * of_polyline_to_polyline(ofPolyline * route)
//    {
//        Polyline * output = new Polyline();
//        for (auto iter = route->begin(); iter != route->end(); iter++)
//        {
//            output->push_back(*iter);
//        }
//
//        return output;
//    };
//
//	
	
//    Polyline * reverse_polyline(Polyline * route)
//    {
//        Polyline * output = new Polyline();
//
//        for (auto iter = route->rbegin(); iter != route->rend(); ++iter)
//        {
//            output->push_back(*iter);
//        }
//
//        return output;
//    };

	void reverse_polyline(ofPolyline & route){
		auto& v = route.getVertices();
		size_t mid = floor(v.size()/2.0f);
		for(size_t i = 0; i < mid; i++){
			std::swap(v[i], v[v.size() - 1 - i]);
		}
	}
    // Free an allocated Route.
//    void free_route(Route * route)
//    {
//        // Delete all of the allocated polylines.
//        for (auto iter = route->begin(); iter != route->end(); ++iter)
//        {
//            delete *iter;
//        }
//
//        // delete the route.
//        delete route;
//    };
}
