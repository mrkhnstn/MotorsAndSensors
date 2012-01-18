#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "ofxTestBed.h"
#include "MotorsAndSensors.h"

// include file of your scene here
#include "IdleScene.h"
#include "PalindromeScene.h"
#include "StepScene.h"
#include "SimpleInteractiveScene.h"
#include "ExampleScene.h"
#include "RotationRangeScene.h"
#include "WaveScene.h"

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

	//////////////////////////////////////////////
	// ADD SCENES
	
	Scene::add(new IdleScene());
	Scene::add(new PalindromeScene());
	Scene::add(new StepScene());
	Scene::add(new SimpleInteractiveScene());
	Scene::add(new ExampleScene());
	Scene::add(new RotationRangeScene());
	Scene::add(new WaveScene());
	
	//////////////////////////////////////////////
	
	// add the "main app" to the test bed 
	MotorsAndSensors motorsAndSensors;
	testBed.addTestObject(motorsAndSensors);

	ofRunApp(&testBed);
}