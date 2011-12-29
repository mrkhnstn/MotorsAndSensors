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

class ofxSimpleGuiPageShortcut : public ofxSimpleGuiButton {

public:
	
	string shortcutPageName;
	
	ofxSimpleGuiPageShortcut(string name, string shortcutPageName) : ofxSimpleGuiButton(name,emptyBool) {
		this->shortcutPageName = shortcutPageName;
	}
	
	void onRelease(int x, int y, int button);	
	
	void draw(float x, float y) {
		setPos(x, y);
		
		glPushMatrix();
		glTranslatef(x, y, 0);
		
		ofEnableAlphaBlending();
		ofFill();
		setTextBGColor();
		ofRect(0, 0, width, height);
		
		// if a toggle
		if((*value) && beToggle) {
			setTextColor();
			//ofLine(0, 0, box.width, box.height);
			//ofLine(box.width, 0, 0, box.height);
		}
		
		setTextColor();
		ofDrawBitmapString(">> " + name, 3, 15);
		
		ofDisableAlphaBlending();
		
		glPopMatrix();
	}

protected:
	
	bool emptyBool;
	
}; 