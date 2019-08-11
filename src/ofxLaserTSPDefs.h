#pragma once

//#include <vector>
#include "ofMain.h"

/*
* A series of Typedefs in one place to make the Type descriptions better.
* This also imports all of the Laser .h files.
* Written by Bryce Summers on 11/28/2016.
*/
namespace ofxLaserTSP
{
	template<typename T>
	std::shared_ptr<T> getFromWeak(std::weak_ptr<T> weak){
		if(!weak.expired()){
			return weak.lock();
		}
		return nullptr;
	}

//	typedef std::vector<glm::vec3> Polyline;
//    typedef std::vector<Polyline *> Route;
	

//    Route * copy(Route * route);

//    Polyline * copy_polyline(Polyline * route);
	
//    Polyline * of_polyline_to_polyline(ofPolyline * route);

    void reverse_polyline(ofPolyline & route);

	
    // Free an allocated Route.
//    void free_route(Route * route);

}

