#include "ofxFps.h"

struct less_first : std::binary_function<Tick,Tick,bool>
{
    inline bool operator()( const Tick& lhs, const Tick& rhs )
    {
        return lhs.second < rhs.second;
    }
};

void ofxFps::begin() {
	timeBegin = ofGetElapsedTimeMicros();
    map<string, unsigned long long>::iterator it = ticks.begin();
    for (; it!=ticks.begin(); ++it) {
        it->second = timeBegin;
    }
}

void ofxFps::tick(string name) {
    ticks[name] = ofGetElapsedTimeMicros();
}

void ofxFps::end() {
	unsigned long long now = ofGetElapsedTimeMicros();
	timeFrame = now - timeEnd;
	timeUpdate = now - timeBegin;
	timeEnd = now;
    
    ticks_sorted.clear();
    ticks_sorted.assign(ticks.begin(), ticks.end());
    sort(ticks_sorted.begin(), ticks_sorted.end(), less_first());
}

float ofxFps::getFps() {
	return 1000000. / timeFrame;
}

float ofxFps::getLoad() {
	return ((float)timeUpdate / (float)timeFrame);
}

string ofxFps::toString(int fpsPrecision, int loadPrecision, bool useTicks) {
    stringstream ss;
    ss << ofToString(getFps(), 1);
    ss << " fps (";

    if (useTicks && ticks.size() > 0) {
        vector<Tick>::iterator it=ticks_sorted.begin();
        unsigned long long previous = timeBegin;
        for (; it!=ticks_sorted.end(); ) {
            ss << it->first;
            ss << ": ";
            if (it->second > previous)
                ss << ofToString(100. * (float)(it->second - previous) / (float)timeFrame, loadPrecision);
            else
                ss << "0";
            ss << " %";
            previous = it->second;
            if (++it != ticks_sorted.end())
                ss << ", ";
        }
        ss << ")";
    }
    else {
        ss << ofToString(getLoad()*100.f, 0);
        ss << " %)";
    }
    return ss.str();
}

void ofxFps::draw(int x, int y) {
    if (getFps() < 20.f || getLoad() > 0.9f)
        ofSetColor(255, 0, 0);
    else
        ofSetColor(255);
    ofDrawBitmapString(toString(), x, y);
}

void ofxFps::draw(int x, int y, string label, bool drawTicks) {

    if (getFps() < 15.f || getLoad() > 0.9f)
        ofSetColor(255, 0, 0);
    else
        ofSetColor(255);

    ofDrawBitmapString(label + ": " + toString(1, 0, drawTicks), x, y);
}

ofxFpsHistory::ofxFpsHistory(int size, bool autoMax) {
    setSize(size);
    this->autoMax = autoMax;
}

void ofxFpsHistory::setSize(int size) {
    this->size = size;
}

void ofxFpsHistory::setMax(float max) {
    this->max = max;
}

void ofxFpsHistory::setAutoMax(bool autoMax) {
    this->autoMax = autoMax;
}

void ofxFpsHistory::add(float f) {
    history.push_front(f);
    while (history.size() > size)
        history.pop_back();
}

void ofxFpsHistory::draw(float x, float y, float height) {

    if (autoMax) {
        max = 0.f;
        list<float>::iterator it=history.begin();
        for (; it!=history.end(); ++it) {
            if (*it > max)
                max = *it;
        }
    }
    list<float>::iterator it=history.begin();
    for (; it!=history.end(); ++it) {
        ofLine(x, y+height, x, y+height-(*it/max)*height);
        x++;
    }
}
