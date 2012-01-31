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
#include "InteractiveScene.h"
#include "RotationRangeScene.h"
#include "MovingWaveScene.h"
#include "SineWaveScene.h"
#include "OpenPortionScene.h"
#include "SnoringScene.h"
#include "InvertedInteractiveSceen.h"
#include "SwipeScene.h"
#include "MovingWindowScene.h"
#include "SerialTest2.h"

int main( ){

    ofAppGlutWindow window;
	window.setGlutDisplayString("rgba double samples>=4 depth"); // enables anti-aliasing using multi-sampling
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	
	// setup generic oF environment (see ofxTestBed)
	ofxTestBed testBed;
	testBed.fps = 30;
	testBed.vSync = true;
	testBed.settingsFolder = "settings/";
	testBed.dataFolder = "../../../data/";
	testBed.logLevel = 0;	

	//////////////////////////////////////////////
	// ADD SCENES
	
	Scene::add(new IdleScene());
	Scene::add(new ExampleScene());
	Scene::add(new InteractiveScene());
	Scene::add(new InteractiveScene());
	Scene::add(new InteractiveScene());
	Scene::add(new SwipeScene());
	Scene::add(new SwipeScene());
	Scene::add(new SwipeScene());
	//Scene::add(new InvertedInteractiveScene());
	//Scene::add(new PalindromeScene());
	//Scene::add(new StepScene());
	//Scene::add(new SimpleInteractiveScene());
	
	//Scene::add(new RotationRangeScene());
	//Scene::add(new MovingWaveScene());
	//Scene::add(new MovingWaveScene());
	
	Scene::add(new SineWaveScene());
	Scene::add(new SineWaveScene());
	Scene::add(new SineWaveScene());
	Scene::add(new MovingWindowScene());
	Scene::add(new MovingWindowScene());
	Scene::add(new MovingWindowScene());
	Scene::add(new MovingWaveScene());
	Scene::add(new OpenPortionScene());
	Scene::add(new SnoringScene());
	
	
	
	//////////////////////////////////////////////
	
	// add the "main app" to the test bed 
	//MotorsAndSensors motorsAndSensors;
	testBed.addTestObject(*Singleton<MotorsAndSensors>::instance());
	
	//SerialTest2 serialTest2;
	//testBed.addTestObject(serialTest2);

	ofRunApp(&testBed);
	
	delete Singleton<MotorsAndSensors>::instance();
}