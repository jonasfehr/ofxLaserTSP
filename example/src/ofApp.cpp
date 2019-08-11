#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	populateExampleVectorOfPolylines();
	
	optimize();
}

//--------------------------------------------------------------
void ofApp::update() {
	
}

//--------------------------------------------------------------
void ofApp::optimize(){
	cout << "---------------" << endl;
	bryce_tsp::RouteOptimizer::useSquareDistance() = false;
	optimizeDrawing();
	bryce_tsp::RouteOptimizer::useSquareDistance() = true;
	optimizeDrawing();
	
}
//--------------------------------------------------------------
void ofApp::optimizeDrawing(int passes) {
	auto t = ofGetElapsedTimeMicros();
	
	bryce_tsp::LaserProgram program(theRawDrawing, this -> closed);
	
	theOptimizedDrawings.clear();
	drawingLengths.clear();
	for (int i = 0; i <= passes; i++){
		
		program.optimize(i);
		
		theOptimizedDrawings.push_back(program.getRoute());
		
	}
	cout << "optimizeDrawing: time taken: " << ofGetElapsedTimeMicros() - t <<endl;
	
	for (int i = 0; i <= passes; i++){
		drawingLengths.push_back(computeLengthOfDrawing(theOptimizedDrawings[i]));
		cout << "Length after " << i << " passes = " << drawingLengths.back() << endl;
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

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(60);
	ofPushMatrix();
	ofScale(3.0, 3.0);
	
	
	drawPolys(theOptimizedDrawings[drawIndex]);
	
	ofTranslate(ofGetWidth()/6, 0);
	drawPolys(theRawDrawing);
	ofPopMatrix();
	
	ofSetColor(0, 0, 0);
	stringstream ss;
	ss << "Raw Drawing Length: " << rawDrawingLength << endl;
	ss << "Index: " <<  drawIndex <<endl;
	ss << "Optimized drawings lengths:" << endl;
	for(size_t i = 0; i < drawingLengths.size(); i++){
		ss << "    " << i << "  -  " << drawingLengths[i] << endl;
	}
	ofDrawBitmapString(ss.str(), 50, 50);
	
}






//--------------------------------------------------------------
void ofApp::populateExampleVectorOfPolylines() {
	
	// Constructs theRawDrawing from some hardcoded data.
	
	float xy0[] = { 57.71,24.79, 54.04,27.46, 52.71,30.46, 52.71,33.29, 51.04,33.29, 46.38,33.29, 44.54,38.29, 44.54,42.46, 41.54,48.12, 40.21,50.96, 40.21,54.12, 38.21,56.62, 38.21,60.29, 37.54,63.48, 41.04,65.62, 43.04,68.12, 45.38,70.79, 47.62,74.29, 47.62,77.12, 49.64,79.62, 50.88,80.96, 53.38,85.46, 55.04,87.96, 56.88,91.62, 58.71,93.79, 60.38,97.62, 62.71,101.79, 65.21,104.12 };
	float xy1[] = { 17.04,106.12, 19.38,102.79, 21.04,101.46, 22.04,97.79, 22.04,96.29, 23.54,93.79, 24.54,90.12, 25.88,85.96, 25.88,83.96, 27.38,81.96, 29.38,78.96, 30.71,73.96, 33.54,68.62, 37.54,63.48 };
	float xy2[] = { 82.21,61.29, 79.21,59.79, 74.71,58.46, 71.71,56.96, 67.71,55.46, 64.04,53.46, 60.21,52.12, 57.71,50.62, 53.71,48.29, 48.54,44.12, 44.54,42.46 };
	float xy3[] = { 78.54,44.79, 74.88,41.62, 72.04,40.29, 68.88,38.96, 64.54,37.79, 61.04,36.46, 55.88,35.46, 52.71,33.29 };
	float xy4[] = { 134.75,22.12, 131.99,24.79, 131.38,25.88, 131.38,29.12, 131.99,32.12, 130,37.12, 132.38,44.79, 130.88,47.25, 131.12,53.75, 131,59.12, 131,65.25, 134.75,70.25, 137,75.12, 138.75,79.25, 139.88,84.8, 140.5,89, 142.88,93.5, 142.88,98.25, 143.5,104.12, 145,107.62, 145,112.5, 146,115.12, 148.5,116.38 };
	float xy5[] = { 111.62,117.12, 114.25,115, 115.5,114.38, 117.75,112.38, 117.75,108.75, 119.25,106.62, 119.25,102.88, 120.75,100, 121.75,95.38, 122.75,90, 122.75,88.5, 123.88,83.5, 125.25,78.88, 126.62,74.38, 128,70, 129.25,65.62, 131,65.25 };
	float xy6[] = { 126.62,27.88, 131.38,27.12 };
	float xy7[] = { 90.75,13.25, 93.5,15.12, 94.75,17.5, 94.75,19.38, 92.12,22.12 };
	float xy8[] = { 94.75,19.38, 96.75,21.25, 98.88,23.25, 100.39,24.79, 103.62,27.5, 107.25,30.75, 111.25,33.29, 115.5,36.25, 115.5,37.12, 121.31,39.04, 126.62,43.12, 130,45.88, 130.88,47.25 };
	float xy9[] = { 169.75,10.25, 168.38,11.75, 167,14.25, 166.12,17, 165.38,19.38, 162.62,22.75, 160.88,24.79, 158.12,27.5, 155.12,30, 151.75,33.29, 148.5,36, 144.38,39.04, 137.75,42.46, 132.38,44.79 };
	
	int lengths[] = { 56,28,22,16,46,34,4,10,26,28 };
	float *xyValues[10];
	xyValues[0] = xy0;
	xyValues[1] = xy1;
	xyValues[2] = xy2;
	xyValues[3] = xy3;
	xyValues[4] = xy4;
	xyValues[5] = xy5;
	xyValues[6] = xy6;
	xyValues[7] = xy7;
	xyValues[8] = xy8;
	xyValues[9] = xy9;
	
	theRawDrawing.clear();
	for (int i = 0; i<10; i++) {
		ofPolyline aPolyline;
		aPolyline.clear();
		for (int j = 0; j<lengths[i]; j += 2) {
			float px = xyValues[i][j + 0];
			float py = xyValues[i][j + 1];
			aPolyline.addVertex(px, py);
		}
		
		theRawDrawing.push_back(aPolyline);
	}
	
	rawDrawingLength = computeLengthOfDrawing(theRawDrawing);
}


//--------------------------------------------------------------
float ofApp::computeLengthOfDrawing(const vector<ofPolyline>&  aDrawing) {
	
	float len = 0;
	size_t drawingSize = aDrawing.size();
	for (size_t i = 0; i < drawingSize; i++) {
		const ofPolyline& aPolyline = aDrawing[i];
		
		len += aPolyline.getPerimeter();
		
		if (closed || i < drawingSize - 1)
		{
			auto& lastOfThis = aPolyline[aPolyline.size() - 1];
			
			auto& firstOfNext = (aDrawing[(i + 1) % drawingSize])[0];
			
			len += glm::distance(lastOfThis, firstOfNext);
		}
		
	}
	return len;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if(key == OF_KEY_UP){
		++drawIndex %= theOptimizedDrawings.size();
	}else if(key == OF_KEY_DOWN){
		drawIndex = (drawIndex + theOptimizedDrawings.size() - 1)% theOptimizedDrawings.size();
	}else if(key == ' '){
		optimize();
	}
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
