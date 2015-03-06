#include "ofxFps.h"

void ofxFps::begin() {
	timeBegin = ofGetElapsedTimeMicros();
}

void ofxFps::end() {
	unsigned long long now = ofGetElapsedTimeMicros();
	timeFrame = now - timeEnd;
	timeUpdate = now - timeBegin;
	timeEnd = now;
}

float ofxFps::getFps() {
	return 1000000. / timeFrame;
}

float ofxFps::getLoad() {
	return ((float)timeUpdate / (float)timeFrame);
}