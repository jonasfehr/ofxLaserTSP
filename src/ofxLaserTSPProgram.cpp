#include "ofxLaserTSPProgram.h"

namespace bryce_tsp
{
	//----------------------------------------------------------------------------------
	LaserProgram::LaserProgram(const std::vector<ofPolyline>& route, bool closed)
	{
		this -> route = route;
		this -> closed = closed;
		bCommandListBuilt = false;
		
		for (size_t i = 0; i < route.size(); i++)
		{
			path_permutation.push_back(i);
		}
	}
	//----------------------------------------------------------------------------------
	
	//    LaserProgram::~LaserProgram()
	//    {
	//        bryce_tsp::free_route(this -> route);
	//    }
	
	//----------------------------------------------------------------------------------
	const std::vector<ofPolyline>& LaserProgram::getRoute() const{
		return route;
	}
	//----------------------------------------------------------------------------------
	std::vector<ofPolyline>& LaserProgram::getRoute(){
		return route;
	}
	//----------------------------------------------------------------------------------
	//	const std::vector<LaserCommand>&  LaserProgram::getCommandList() const{
	//		updateCommandList();
	//		return commandList;
	//	}
	//----------------------------------------------------------------------------------
	std::vector<LaserCommand>&  LaserProgram::getCommandList(){
		updateCommandList();
		return commandList;
	}
	//----------------------------------------------------------------------------------
	//    Route * LaserProgram::getRoute()
	//    {
	//        // Copy the private route into one that we can pass permission to the caller.
	//
	//        return copy(route);
	//    }
	
	// Note: This command list is valid for closed and open paths,
	// because closure is a function of user interpretation.
	//----------------------------------------------------------------------------------
	void LaserProgram::updateCommandList()
	{
		if(!bCommandListBuilt){
			bCommandListBuilt = true;
			//        Program * program = new Program();
			
			// Instructs the artist to turn the laser off.
			LaserCommand off;
			off.type = OFF;
			
			// Instructs the artist to turn the laser on for drawing.
			LaserCommand on;
			on.type = ON;
			
			commandList.clear();
			
			for(size_t index = 0; index < route.size(); index++)
				//        for (auto iter = route -> cbegin(); iter != route -> cend(); ++iter)
			{
				auto& ID = path_permutation[index];
				//            index++;
				
				// Populate the off command.
				off.ID = ID;
				commandList.push_back(off);
				
				//            Polyline * polyline = *iter;
				auto& polyline = route[index];
				auto& v = polyline.getVertices();
				if(v.size()){
					
					//            auto point_iter = polyline.cbegin();
					LaserCommand move;
					move.type = MOVETO;
					move.pt = v[0];
					move.ID = ID;
					
					// Move without the laser on.
					commandList.push_back(move);
					
					// Turn the laser on.
					on.ID = ID;
					commandList.push_back(on);
					
					// Draw the rest of the movements with the laser on.
					//            point_iter++;
					//            while (point_iter != polyline -> cend())
					for(int i = 1; i < v.size(); i++){
						//                move.pt = *point_iter;
						move.pt = v[i];
						commandList.push_back(move);
						//                point_iter++;
					}
				}
			}
			
			//        return program;
		}
	}
	
	
	// -- Optimization.
	
	
	// -- Instructs the LaserProgram to Optimize its route.
	// Updates the internal route pointer to a new route.
	// It frees the original route.
	//----------------------------------------------------------------------------------
	void LaserProgram::optimize(int passes)
	{
		if (passes <= 0)
		{
			return;
		}
		
		// For deletion later.
		//        Route * old_route = route;
		
		RouteOptimizer optimizer(this -> route, this -> closed);
		this -> route = optimizer.optimize(passes);
		
		// Get the permutation for concerned artists who want their original colors, strokes, etc.
		optimizer.permute(path_permutation);
		
		//        bryce_tsp::free_route(old_route);
	}
	//----------------------------------------------------------------------------------
	const size_t& LaserProgram::lookup_original_index(const size_t & current_index)
	//    int LaserProgram::lookup_original_index(int current_index)
	{
		return path_permutation[current_index];
	}
}
