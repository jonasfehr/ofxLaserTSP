#pragma once

#include "ofMain.h"
#include "ofxLaserTSPDefs.h"
#include "ofxLaserTSPRouteOptimizer.h"
#include "ofxLaserTSPRouteNode.h"

//#include <vector>

namespace ofxLaserTSP
{

    // Laser Commands tell the robot to turn on the laser, turn off the laser,
    // or move the laser to a new location.
    // they will then contain a relevant ofPoint if it is a move command.
    enum LaserCommandType
    {
        ON, OFF, MOVETO
    };

    class LaserCommand
    {
    public:
        LaserCommandType type;

        // Only relavant to moveToCommands.
		glm::vec3 pt;

        // MoveTo commands contain the index of the polyline they belong
        // to based on the ordering of the route passed into the constructor.
        // OFF commands contain the ID of the next point in the route, namely the first point in the next path.
        // ON commands contain the ID of the next point.
        int ID;
    };

    class LaserProgram
    {

    private:
        // Private Route object.
		std::vector<ofPolyline> route;
        bool closed;

        // path_permutations[route[id]] = id of route in original input.
        std::vector<size_t> path_permutation;
		std::vector<LaserCommand> commandList;
		bool bCommandListBuilt = false;
		void updateCommandList();
    public:
        // ASSUMES: route allocated via new. The Laser Program now owns the input route and will deal with deallocation.
        // ASSUMES: every route contains at least 2 points.
        LaserProgram(const std::vector<ofPolyline>& route, bool closed = true);

        // -- Instructs the LaserProgram to Optimize its route.
        // Updates the internal route pointer to a new route.
        // It frees the original route.

        // FIXME: I would like to add some inputs for maximum time or maximum distance.
        // I could add a boolean return if it meets the input requirements.
        void optimize(int passes);

        // Returns the current route. This will be an optimized route if the optimize function has been used.
        const std::vector<ofPolyline>& getRoute() const;
			  std::vector<ofPolyline>& getRoute();
        // Converts the current route into a command list form.

			  std::vector<LaserCommand>&  getCommandList() ;
        // Determines the index of the given index in the original data after the optimization permutation has been applied.
        const size_t& lookup_original_index(const size_t & current_index);

    // Optimization functions.
    private:

       
    };
}
