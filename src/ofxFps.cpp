#include "ofxFps.h"

#ifdef TARGET_WIN32
#ifndef _MSC_VER
#include <unistd.h> // this if for MINGW / _getcwd
#include <sys/param.h> // for MAXPATHLEN
#endif
#endif


#if defined(TARGET_OF_IOS) || defined(TARGET_OSX ) || defined(TARGET_LINUX) || defined(TARGET_EMSCRIPTEN)
#include <sys/time.h>
#endif

#ifdef TARGET_OSX
#ifndef TARGET_OF_IOS
#include <mach-o/dyld.h>
#include <sys/param.h> // for MAXPATHLEN
#endif
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#ifdef TARGET_WIN32
#include <mmsystem.h>
#ifdef _MSC_VER
#include <direct.h>
#endif

#endif

struct less_first : std::binary_function<Tick,Tick,bool>
{
    inline bool operator()( const Tick& lhs, const Tick& rhs )
    {
        return lhs.second < rhs.second;
    }
};

ofxFps::ofxFps() {
	this->timeBegin = 0;
	this->timeEnd = 0;
	this->timeFrame = 0;
	this->timeUpdate = 0;
	this->minFramerate = 10.f;
	this->maxLoad = 0.9f;
}

unsigned long long ofxFps::getTimeMicros() {
#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI)) || defined(TARGET_EMSCRIPTEN)
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000000 + now.tv_nsec / 1000;
#elif defined(TARGET_OSX)
    clock_serv_t cs;
    mach_timespec_t now;
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cs);
    clock_get_time(cs, &now);
    mach_port_deallocate(mach_task_self(), cs);
    return now.tv_sec * 1000000 + now.tv_nsec / 1000;
#elif defined( TARGET_WIN32 )
    LARGE_INTEGER freq;
    LARGE_INTEGER counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (counter.QuadPart / freq.QuadPart)*1000000 + (counter.QuadPart % freq.QuadPart)*1000000/freq.QuadPart;
#else
    struct timeval now;
    gettimeofday( &now, NULL );
    return now.tv_sec * 1000000 + now.tv_usec;
#endif
}

void ofxFps::begin() {
	register auto now = getTimeMicros();
	timeFrame = now - timeBegin;
	timeBegin = now;
    for (auto it=ticks.begin(); it!=ticks.begin(); ++it) {
        it->second = timeBegin;
    }
}

void ofxFps::tick(string name) {
    ticks[name] = getTimeMicros();
}

void ofxFps::end() {
	register unsigned long long now = getTimeMicros();
	timeFrame = now - timeEnd;
	timeUpdate = now - timeBegin;
	timeEnd = now;
    
    ticks_sorted.clear();
    ticks_sorted.assign(ticks.begin(), ticks.end());
    std::sort(ticks_sorted.begin(), ticks_sorted.end(), less_first());
}

float ofxFps::getFps() {
	return timeFrame != 0 ? 1000000. / timeFrame : 0.f;
}

float ofxFps::getLoad() {
	return ((float)timeUpdate / (float)timeFrame);
}

double ofxFps::getFrameTime() {
	return timeFrame / 1000000.;
}

float ofxFps::getFrameTimef() {
	return timeFrame / 1000000.;
}

unsigned int ofxFps::getFrameTimeMicros() {
	return timeFrame;
}

unsigned int ofxFps::getFrameTimeMillis() {
	return timeFrame / 1000;
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
            if (it->second > previous && timeFrame != 0) {
				float load = 100. * (float)(it->second - previous) / (float)timeFrame;
                ss << ofToString(load, loadPrecision);
			}
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

    if (getFps() < minFramerate || getLoad() > maxLoad)
        ofSetColor(255, 0, 0);
    else
        ofSetColor(255);
    ofDrawBitmapString(toString(), x, y);
	ofSetColor(255);
}

void ofxFps::draw(int x, int y, string label, bool drawTicks) {

    if (getFps() <minFramerate || getLoad() > maxLoad)
        ofSetColor(255, 0, 0);
    else
        ofSetColor(255);
    ofDrawBitmapString(label + ": " + toString(1, 0, drawTicks), x, y);
	ofSetColor(255);
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
