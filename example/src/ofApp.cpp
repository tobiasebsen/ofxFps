#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	history = ofxFpsHistory(1920, false);
	history.setMax(70);
}

//--------------------------------------------------------------
void ofApp::update() {
	if (ofGetFrameNum() % 60 != 0) return;
	ofLogNotice() << "getLoad:\t\t\t" << ofToString(fps.getLoad() * 100, 0) << "%";
	ofLogNotice() << "getFps:\t\t\t" << (int)fps.getFps() << "fps";
	ofLogNotice() << "getFrameTimef:\t" << int(fps.getFrameTimef() * 1000) << "ms\n";
}

//--------------------------------------------------------------
void ofApp::draw() {
	history.add(fps.getFps());
	int h = 10;
	int y = ofGetHeight() - h; 
	history.draw(0, y, h);
	ofDrawLine(0, y, 1902, y);

	fps.begin();
	ofSleepMillis(1);
	fps.end();
	fps.draw(10, 20);
}

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