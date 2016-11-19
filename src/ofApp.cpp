#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // The screen that I test this on was 1920 by 1080, in case anybody out there wants to know...
    int width  = ofGetWidth();
    int height = ofGetHeight();

    // Minnimal testing code. Interior points in polylines are ommitted,
    // because they are currently not considered in the algorithm.
    
    // We generate a random 10 route, using a random seed for consistent testing.
    ofSeedRandom(0);

    laser::Route * route = new laser::Route();

    int path_num = 10;

    for (int i = 0; i < path_num; i++)
    {
        laser::Polyline * path = new laser::Polyline();
        for (int i = 0; i < 2; i++)
        {
            path -> push_back(ofPoint(ofRandom(width), ofRandom(height)));
        }
        route -> push_back(path);
    }

    program = new laser::LaserProgram(route);
    program -> optimize();
    this -> route = program -> getRoute();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    // Draw the route each frame.
    ofPath p;// The path.
    p.setStrokeColor(color);
    p.setStrokeWidth(strokeWidth);

    ofPath p_connectors;// The connection paths.
    p_connectors.setStrokeColor(128);
    p_connectors.setStrokeWidth(strokeWidth / 2);


    // Draws the given paths, while also drawing the connecting routes between them in dual.
    auto iter = route -> cbegin();
    auto first_line_iter = iter;
    auto first_point_iter = (*first_line_iter) -> cbegin();
    p_connectors.moveTo(*first_point_iter);
    for (; iter != route -> cend(); ++iter)
    {
        laser::Polyline * polyline = *iter;

        auto point_iter = polyline -> cbegin();

        // Notice that p always moves while p_connectos draws ("lineTo"), and vise versa.
        // There is beautiful combinatorial cyclic symmetry here.
        p_connectors.lineTo(*point_iter);
        p.moveTo(*point_iter);
        ++iter;
        for (; point_iter != polyline -> cend(); ++point_iter)
        {
            p.lineTo(*point_iter);
            p_connectors.moveTo(*point_iter);
        }
    }
    p_connectors.lineTo(*first_point_iter);

    p.draw();
    p_connectors.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
