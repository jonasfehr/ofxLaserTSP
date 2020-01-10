#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
    
    for(int i = 0; i< 20; ++i){
           glm::vec2 pos = glm::vec2(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
           for(int p = 0; p< 5; ++p){
               cb.addPoint(pos, 1);
               pos+=glm::vec2(1);
           }
           cb.closePointGroup();
       }
    
    rawPointGroups = cb.getPointGroups();
       optimize(3);
}

//--------------------------------------------------------------
void ofApp::update() {
    cb.clear();
    for(int i = 0; i< 40; ++i){
            glm::vec2 pos = glm::vec2(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
            for(int p = 0; p<ofRandom(2,100); ++p){
                cb.addPoint(pos, 1);
                pos+=glm::vec2(1);
            }
            cb.closePointGroup();
        }

     rawPointGroups = cb.getPointGroups();

    optimize(3);
    
//    optimiser.optimise(cb);
//    optimiser.update();
}
//--------------------------------------------------------------
void ofApp::optimize(int passes) {
	
	ofxLaserTSPLaserProgram<ContentBase::PointGroup> program(rawPointGroups, this -> closed);
	
	optimisedPointGroups.clear();
	drawingLengths.clear();
	auto t = ofGetElapsedTimeMicros();

	for (int i = 0; i <= passes; i++){
		program.optimize(i);
        //program.getRoute();
        optimisedPointGroups.clear();
        optimisedPointGroups = program.getRoute();
        drawingLengths.push_back(computeLengthOfPoints(optimisedPointGroups));
	}
    optimisedPointGroups.clear();
    optimisedPointGroups = program.getRoute();
    drawingLengths.push_back(computeLengthOfPoints(optimisedPointGroups));
	cout << "optimizeDrawing: time taken: " << ofGetElapsedTimeMicros() - t <<endl;
	
    cout << "Length  = " << computeLengthOfPoints(rawPointGroups) << endl;

	for (int i = 0; i <= passes; i++){
        cout << "Length after " << i << " passes = " << drawingLengths[i] << endl;
	}
}

//--------------------------------------------------------------
void ofApp::drawPolys(vector<ofPolyline>& polys){
	for(size_t i = 0; i < polys.size(); i++){
		ofSetColor(0);
		polys[i].draw();
		
		if (closed || i < polys.size() - 1)
		{
			// draw indicator from end of this line, to start of next
			ofSetColor(255, 0, 0, 160);
			ofSetLineWidth(1.0);
			auto& lastPointOfThis = polys[i].getVertices().back();
			auto& firstPointOfNext = (polys[(i + 1) % polys.size()])[0];
			ofDrawLine(lastPointOfThis, firstPointOfNext);
		}
	}
}

void ofApp::drawPoints(vector<ContentBase::PointGroup> & pointGroups){
    glm::vec2 prevP = glm::vec2(ofGetWidth()/2);

    for(auto & pg : pointGroups){
        for(auto p : pg.points){
            ofDrawLine(prevP, p);
            ofDrawCircle(p, 3);
            prevP = p;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(60);
    
    ofSetColor(255,0,0);
    drawPoints(rawPointGroups);
    ofSetColor(255,0,155);
    drawPoints(optimisedPointGroups);
    
//    ofSetColor(255,0,0);
//    drawPoints(cb.getPointGroupsRef());
//    ofSetColor(255,255,155);
//    drawPoints(optimiser.contentBase.getPointGroupsRef());
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
	
}






//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
	
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	
}
