#pragma once

#include "ofMain.h"
#include "ofxBaseObject.h"
#include "ofxSimpleGuiToo.h"

class ofxTestBed : public ofBaseApp {

public:
	
	bool	vSync;
	int		fps;
	int		logLevel;
	string	dataFolder; //< not implemented yet
	string	settingsFolder;
	bool	noCursor;
	
	bool	guiAutoDraw; ///< if true then the gui gets drawn after all base objects (set to false if a base object takes care of drawing the gui)
	
	ofxTestBed();
	
	/***
	 use this method to add a test object in the constructor
	 */
	virtual void addTestObject(ofxBaseObject& baseObject);
	
	/***
	 override this method if you want to change the base setup (e.g. framerate, background....)
	 */
	virtual void setupOF();	
	virtual void setupGUI();
	virtual void postGUI();
	
	///////////////////////////////////////////////////////////
	// no need to override methods below
	
	virtual void setup();	
	virtual void update();	
	virtual void draw();	
	
	virtual void keyPressed  (int key);	
	
	void toggleFullscreen(ofEventArgs& e);
	void toggleMouse(ofEventArgs& e);
	
	void updateVSync(ofEventArgs& e);
	
protected:
	
	bool _noCursor;
	
	vector<ofxBaseObject*> baseObjects;

	
	
};
