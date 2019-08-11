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
	const std::vector<ofPolyline>& LaserProgram::getRoute() const{
		return route;
	}
	//----------------------------------------------------------------------------------
	std::vector<ofPolyline>& LaserProgram::getRoute(){
		return route;
	}
	//----------------------------------------------------------------------------------
	std::vector<LaserCommand>&  LaserProgram::getCommandList(){
		updateCommandList();
		return commandList;
	}
	// Note: This command list is valid for closed and open paths,
	// because closure is a function of user interpretation.
	//----------------------------------------------------------------------------------
	void LaserProgram::updateCommandList()
	{
		if(!bCommandListBuilt){
			bCommandListBuilt = true;
			
			// Instructs the artist to turn the laser off.
			LaserCommand off;
			off.type = OFF;
			
			// Instructs the artist to turn the laser on for drawing.
			LaserCommand on;
			on.type = ON;
			
			commandList.clear();
			
			for(size_t index = 0; index < route.size(); index++)
			{
				auto& ID = path_permutation[index];
				
				// Populate the off command.
				off.ID = ID;
				commandList.push_back(off);
				
			
				auto& polyline = route[index];
				auto& v = polyline.getVertices();
				if(v.size()){
					
			
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
					for(int i = 1; i < v.size(); i++){
						move.pt = v[i];
						commandList.push_back(move);
					}
				}
			}
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
		
		RouteOptimizer optimizer(this -> route, this -> closed);
		this -> route = optimizer.optimize(passes);
		
		// Get the permutation for concerned artists who want their original colors, strokes, etc.
		optimizer.permute(path_permutation);
		
	}
	//----------------------------------------------------------------------------------
	const size_t& LaserProgram::lookup_original_index(const size_t & current_index)
	{
		return path_permutation[current_index];
	}
}
