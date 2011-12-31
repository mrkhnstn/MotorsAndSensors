#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "ofxTestBed.h"
#include "MercedesApp.h"
#include "FinCtrl.h"
#include "OscFinCtrl.h"
#include "SceneCtrl.h"
#include "SerialTest.h"
#include "SensorCtrl.h"
#include "ofxOscManager.h"
#include "ThreeDTest.h"


int main( ){

    ofAppGlutWindow window;
	window.setGlutDisplayString("rgba double samples>=4 depth"); // enables anti-aliasing using multi-sampling
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	
	ofxTestBed testBed;
	testBed.fps = 60;
	testBed.vSync = false;
	testBed.settingsFolder = "settings/";
	testBed.dataFolder = "../../../data/";
	testBed.logLevel = 0;	

	/*
	testBed.addTestObject(*Singleton<ofxOscManager>::instance());
	Singleton<ofxOscManager>::instance()->port = 3334;
	
	testBed.addTestObject(*Singleton<FinCtrl>::instance());
	testBed.addTestObject(*Singleton<OscFinCtrl>::instance());
	testBed.addTestObject(*Singleton<SceneCtrl>::instance());
	//testBed.addTestObject(*Singleton<SerialTest>::instance());
	testBed.addTestObject(*Singleton<SensorCtrl>::instance());
	*/
	
	ThreeDTest threeDTest;
	testBed.addTestObject(threeDTest);
	
	
	ofRunApp(&testBed);
	
	/*
	delete Singleton<FinCtrl>::instance();
	delete Singleton<OscFinCtrl>::instance();
	delete Singleton<SceneCtrl>::instance();
	delete Singleton<SensorCtrl>::instance();
	delete Singleton<ofxOscManager>::instance();
	//delete Singleton<SerialTest>::instance();
	*/
}
