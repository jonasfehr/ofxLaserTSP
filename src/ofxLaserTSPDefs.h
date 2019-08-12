#pragma once
#include "ofMain.h"

namespace ofxLaserTSP
{
	template<typename T>
	std::shared_ptr<T> getFromWeak(std::weak_ptr<T> weak){
		if(!weak.expired()){
			return weak.lock();
		}
		return nullptr;
	}

	template<typename T>
	typename std::enable_if<std::is_base_of<ofPolyline, T>::value || std::is_same<ofPolyline, T>::value, void >::type
	reverse_polyline(T & route){
		auto& v = route.getVertices();
		// we are swaping elements of the polyline so we just need to loop through half of these.
		size_t mid = floor(v.size()/2.0f);
		for(size_t i = 0; i < mid; i++){
			std::swap(v[i], v[v.size() - 1 - i]);
		}
	}

	

}

