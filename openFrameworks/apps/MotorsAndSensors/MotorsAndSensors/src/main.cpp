#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "ofxTestBed.h"
#include "MotorsAndSensors.h"

int main( ){

    ofAppGlutWindow window;
	window.setGlutDisplayString("rgba double samples>=4 depth"); // enables anti-aliasing using multi-sampling
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	
	// setup generic oF environment (see ofxTestBed)
	ofxTestBed testBed;
	testBed.fps = 60;
	testBed.vSync = true;
	testBed.settingsFolder = "settings/";
	testBed.dataFolder = "../../../data/";
	testBed.logLevel = 0;	

	// add the "main app" to the test bed 
	MotorsAndSensors motorsAndSensors;
	testBed.addTestObject(motorsAndSensors);

	ofRunApp(&testBed);
}
