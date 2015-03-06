#pragma once

#include "ofMain.h"

class ofxFps {
public:

	void begin();
	void end();

	float getFps();
	float getLoad();

private:
	unsigned long long timeBegin;
	unsigned long long timeEnd;
	unsigned int timeFrame;
	unsigned int timeUpdate;
};