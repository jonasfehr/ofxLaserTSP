#pragma once
#include <cstddef>
#include <memory>
namespace bryce_tsp
{
	class RouteNode: public std::enable_shared_from_this<RouteNode>
    {
    public:
        RouteNode(size_t id, size_t pid1, size_t pid2);
//        ~RouteNode();

        bool marked = false;

        // A route is assumed to consist of "domino" nodes linked between each other.
        // These nodes contain indices into point data arrays.
        // Consider the route A -> B -> C
        // The polylines would be:
        // 1. A.start -> A.end,
        // 2. A.end   -> B.start,
        // 3. B.start -> B.end,
        // 4. B.end   -> C.start,
        // 5. C.start -> C.end,
        // 6. C.end   -> A.start.
        size_t id;

        // Stores whether this node's polyline should be interpreted forwards, or backwards.
        
        bool flipped = false;
        size_t index_start;
        size_t index_end;


        // Flips this node.
        void flip();// Flips starting and ending points, but keeps location in domino list.

        // Reverses orientation in domino list. Maintains the connections between paired points.
        void reverse();
	
		
		std::shared_ptr<RouteNode> getNext();
		std::shared_ptr<RouteNode> getPrev();
		
		void setNext(std::shared_ptr<RouteNode> next);
		void setPrev(std::shared_ptr<RouteNode> prev);
		
		
	private:
		// Circular Doubly Linked List.
		// Next pointers are consistent.
		std::weak_ptr<RouteNode> next;
		std::weak_ptr<RouteNode> prev;

		
    };
}
