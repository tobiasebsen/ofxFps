#pragma once

#include "ofMain.h"

typedef std::pair<string, unsigned long long> Tick;

// FRAME RATE
class ofxFps {
public:
	ofxFps();
    
    unsigned long long getTimeMicros();

	void begin();
    void tick(string name);
	void end();

	float getFps();
	float getLoad();
	double getFrameTime();
	float getFrameTimef();
	unsigned int getFrameTimeMicros();
	unsigned int getFrameTimeMillis();
    
    string toString(int fpsPrecision = 1, int loadPrecision = 0, bool useTicks = true);
    
    void draw(int x, int y);
    void draw(int x, int y, string label, bool drawTicks = true);

	void setWarnings(float minFramerate, float maxLoad);

private:
	unsigned long long timeBegin;
	unsigned long long timeEnd;
	unsigned int timeFrame;
	unsigned int timeUpdate;

	float minFramerate;
	float maxLoad;
    
    map<string, unsigned long long> ticks;
    vector<Tick> ticks_sorted;
};

// HISTORY
class ofxFpsHistory {
public:
    
    ofxFpsHistory(int size = 100, bool autoMax = true);
    
    void setSize(int size);
    void setMax(float max);
    void setAutoMax(bool autoMax);
    
    void add(float f);
    void draw(float x, float y, float height);

private:
    int size;
    list<float> history;
    bool autoMax;
    float max;
};