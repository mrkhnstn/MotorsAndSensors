#include "ofxTestBed.h"

ofxTestBed::ofxTestBed(){
	logLevel = OF_LOG_VERBOSE;
	fps = 30;
	vSync = true;
	dataFolder = "";
	settingsFolder = "settings/";
	guiAutoDraw = true;
	noCursor = false;
	_noCursor = noCursor;
}

void ofxTestBed::setup(){
	setupOF();
	
	gui.setup();
	gui.setFolderPath(settingsFolder);
	//gui.addFPSCounter();
	//gui.setPage("Keyboard Shortcuts");
	gui.page(1).addPageShortcut(gui.addPage("App"));
	gui.addButton("toggle fullscreen", 'f', this, &ofxTestBed::toggleFullscreen);
	gui.addToggle("hide cursor", noCursor);
	gui.addButton("toggle cursor", 'm', this, &ofxTestBed::toggleMouse);
	gui.setDefaultKeys(true);
	gui.setAutoSave(true);
	
	for (vector<ofxBaseObject*>::iterator it = baseObjects.begin(); it != baseObjects.end(); ++it) {
		(*it)->setup();
	}
	setupGUI();
	postGUI();
}

void ofxTestBed::setupOF(){
	ofBackground(0, 0, 0);
	ofSetVerticalSync(vSync);
	ofSetFrameRate(fps);
	ofSetLogLevel(logLevel);
	ofSetDataPathRoot(dataFolder);
	if(_noCursor)
		ofHideCursor();
}

void ofxTestBed::addTestObject(ofxBaseObject& baseObject){
	baseObjects.push_back(&baseObject);
}

void ofxTestBed::setupGUI(){

	
	for (vector<ofxBaseObject*>::iterator it = baseObjects.begin(); it != baseObjects.end(); ++it) {
		(*it)->setupGUI();
	}	
	gui.loadFromXML();
	//gui.show();
}

void ofxTestBed::postGUI(){
	for (vector<ofxBaseObject*>::iterator it = baseObjects.begin(); it != baseObjects.end(); ++it) {
		(*it)->postGUI();
	}	
}

void ofxTestBed::update(){
	if(noCursor != _noCursor){
		_noCursor = noCursor;
		if (_noCursor) {
			ofHideCursor();
		} else {
			ofShowCursor();
		}
	}
	for (vector<ofxBaseObject*>::iterator it = baseObjects.begin(); it != baseObjects.end(); ++it) {
		(*it)->update();
	}	
}

void ofxTestBed::draw(){
	for (vector<ofxBaseObject*>::iterator it = baseObjects.begin(); it != baseObjects.end(); ++it) {
		(*it)->draw();
	}	
	if (guiAutoDraw) {
		gui.draw();
	}
}

void ofxTestBed::keyPressed  (int key){
};

void ofxTestBed::toggleFullscreen(ofEventArgs& e){
	ofToggleFullscreen();
};

void ofxTestBed::toggleMouse(ofEventArgs& e){
	noCursor = !noCursor;
};

void ofxTestBed::updateVSync(ofEventArgs& e){
	
}
