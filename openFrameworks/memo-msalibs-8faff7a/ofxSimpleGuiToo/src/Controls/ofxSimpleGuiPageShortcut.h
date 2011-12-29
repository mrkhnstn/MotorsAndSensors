#pragma once
/*
 *  ofxSimpleGuiPageShortcut.h
 *  surface
 *
 *  Created by Mark Hauenstein on 10/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSimpleGuiButton.h"
//#include "ofxSimpleGuiPage.h"

class ofxSimpleGuiPage;

class ofxSimpleGuiPageShortcut : public ofxSimpleGuiButton {

public:
	
	string shortcutPageName;
	ofxSimpleGuiPage* page;
	
	ofxSimpleGuiPageShortcut(string name, string shortcutPageName);
	
	ofxSimpleGuiPage& getPage();
	
	void onPress(int x, int y, int button);	
	
	void draw(float x, float y);
	
	int	 getPriority();	

protected:
	
	bool emptyBool;
	
}; 