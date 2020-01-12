#pragma once

#include "ofMain.h"
#include "ofxLaserTSP.h"



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

    void populateExampleVectorOfPolylines();
    void optimizeDrawing(int passes = 4);
    float computeLengthOfDrawing(const vector<ofPolyline>& aDrawing);
    void createRandomPolylines();
	void optimize();
	
	
    vector<ofPolyline>	theRawDrawing;

    vector<vector<ofPolyline>>	theOptimizedDrawings;
    

	void drawPolys(vector<ofPolyline>& polys);
	size_t drawIndex = 0;
	
    vector<float> drawingLengths;
	
	float rawDrawingLength;
	
	
    bool closed = false;
};
