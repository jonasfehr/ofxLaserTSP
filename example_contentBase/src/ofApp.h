#pragma once

#include "ofMain.h"
#include "ofxLaserTSP.h"
#include "ContentBase.hpp"
#include "OptimiserTSP.h"


class ofApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void optimize(int passes = 4);

	
    ContentBase cb;

    vector<ContentBase::PointGroup>	rawPointGroups;

    vector<ContentBase::PointGroup>	optimisedPointGroups;

	void drawPolys(vector<ofPolyline>& polys);
	size_t drawIndex = 0;
	
    vector<float> drawingLengths;
    
    void drawPoints(vector<ContentBase::PointGroup> & pointGroups);
	
	float rawDrawingLength;
	
	
    bool closed = false;
    
    float computeLengthOfPoints(vector<ContentBase::PointGroup>  &pointGroups) {
        float len = 0;
        glm::vec2 prevP = glm::vec2(ofGetWidth()/2);

        for(auto & pg : pointGroups){
               for(auto p : pg.points){
                   
                   len += glm::distance(prevP, p);
                   prevP = p;
               }
           }
       
        return len;
    }
    
    OptimiserTSP optimiser;
};
