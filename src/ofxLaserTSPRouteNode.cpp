#include "ofxLaserTSPRouteNode.h"
#include "ofxLaserTSPDefs.h"
namespace ofxLaserTSP
{
//----------------------------------------------------------------------------------
    RouteNode::RouteNode(size_t id, size_t pid1, size_t pid2)
    {
        this -> id = id;
        this -> index_start = pid1;
        this -> index_end   = pid2;
    }
//----------------------------------------------------------------------------------
    void RouteNode::flip()
    {
        // This is used to signal the output to invert the input polyline.
        flipped = !flipped;

		std::swap(index_start, index_end);
    }
//----------------------------------------------------------------------------------
    void RouteNode::reverse(){
		std::swap(prev, next);
        flip();
    }
//----------------------------------------------------------------------------------
	std::shared_ptr<RouteNode> RouteNode::getNext(){
		return getFromWeak(next);
	}
//----------------------------------------------------------------------------------
	std::shared_ptr<RouteNode> RouteNode::getPrev(){
		return getFromWeak(prev);
	}
//----------------------------------------------------------------------------------
	void RouteNode::setNext(std::shared_ptr<RouteNode> next){
		this->next = next;
	}
//----------------------------------------------------------------------------------
	void RouteNode::setPrev(std::shared_ptr<RouteNode> prev){
		this->prev = prev;
	}
	
}
