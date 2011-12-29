/*
#include "ofAppGlutWindow.h"
#include "ofxTestBed.h"
#include "SharedMemoryTestBed.h"
#include "KinectSharedMemoryTestBed.h"
#include "ofxArgs.h"

#define KINECT_EXAMPLE //comment to use normal video grabber

int main(int argc, char *argv[]) { //< uses terminal arguments to determine instance id

	ofxArgs* args = new ofxArgs(argc, argv);
	
	ofAppGlutWindow window;
	window.setGlutDisplayString("rgba double samples>=4 depth"); // enables anti-aliasing using multi-sampling
	ofSetupOpenGL(&window, 640, 480, OF_WINDOW);
	
	ofxTestBed testBed;
	
#ifdef KINECT_EXAMPLE
	KinectSharedMemoryTestBed sharedMemoryTestBed;
#else
	SharedMemoryTestBed sharedMemoryTestBed;
#endif
	
	sharedMemoryTestBed.id = args->getInt("-id", 0); //< set instance id
	testBed.addTestObject(sharedMemoryTestBed);
	
	ofRunApp( &testBed);
		
}
*/

//#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "ofxTestBed.h"
#include "KinectCapture.h"
#include "KinectStitch.h"
#include "ofxArgs.h"

#define KINECT_EXAMPLE //comment to use normal video grabber

int main(int argc, char *argv[]) { //< uses terminal arguments to determine instance id
	
	ofxArgs* args = new ofxArgs(argc, argv);
	
	ofAppGlutWindow window;
	window.setGlutDisplayString("rgba double samples>=4 depth"); // enables anti-aliasing using multi-sampling
	
	
	ofxTestBed testBed;
	
	if (args->getInt("-master", 0) == 1) { // if defined as master then initialize stitcher
		MultiKinect::KinectStitch kinectStitch;
		testBed.addTestObject(kinectStitch);
		ofSetupOpenGL(&window, 3 * 640, 480, OF_WINDOW);
		ofRunApp(&testBed);
	} else { // initialize as capture instance
		ofSetupOpenGL(&window, 640, 480, OF_WINDOW);
		MultiKinect::KinectCapture kinectCapture;
		kinectCapture.kinectId = args->getInt("-kinectId", 0); //< set kinect id based on passed argument
		testBed.addTestObject(kinectCapture);
		ofRunApp(&testBed);
	}

	
}