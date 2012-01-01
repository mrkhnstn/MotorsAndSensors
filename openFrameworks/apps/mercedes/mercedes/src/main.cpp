#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "ofxTestBed.h"
#include "MercedesApp.h"

int main( ){

    ofAppGlutWindow window;
	window.setGlutDisplayString("rgba double samples>=4 depth"); // enables anti-aliasing using multi-sampling
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	
	ofxTestBed testBed;
	testBed.fps = 60;
	testBed.vSync = true;
	testBed.settingsFolder = "settings/";
	testBed.dataFolder = "../../../data/";
	testBed.logLevel = 0;	

	MercedesApp mercedesApp;
	testBed.addTestObject(mercedesApp);
	ofRunApp(&testBed);
}
