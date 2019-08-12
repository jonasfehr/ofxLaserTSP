# ofxLaserTSP

Optimizes the traveling salesman problem for a set of double ended paths.

# Status
Being tested in practice now.

### Updated (Aug 10, 2019) by Roy Macdonald
I made several modifications to this addon, while keeping the original algorithm it improves performance by avoiding copies and using a more eficient memory handling. Also now there is no need to manage memory as it is done internaly by the use of smart pointers. The whole addon is templated so to use `ofPolylines` or any class that inherits from it, making it easier to use with custom polyline derived types. Although it still has to be use how it was originaly, it is not really backwards compatible.
aa

# Closed problem (Cycle).
![alt text](https://github.com/Bryce-Summers/ofxLaserTSP/blob/master/screenshots/Skeleton_Optimization_closed.png "Skeleton Optimization Closed Problem Image")

# Open problem (Path).
![alt text](https://github.com/Bryce-Summers/ofxLaserTSP/blob/master/screenshots/Skeleton_Optimization_open.png "Skeleton Optimization Open Problem Image")

Please note that a solution to the open problem is simply a solution to the closed problem without its longest edge.

# Future Work.
I've got a bunch of ideas for making this faster, more efficient, and working for larger problem sizes.
Right now it is designed for the needs of those who have originally commissioned this work.
