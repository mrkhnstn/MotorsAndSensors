/*
 *  ofxSimpleGuiPageShortcut.cpp
 *  surface
 *
 *  Created by Mark Hauenstein on 10/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSimpleGuiPageShortcut.h"
#include "ofxSimpleGuiToo.h"
#include "ofxSimpleGuiPage.h"

void ofxSimpleGuiPageShortcut::onPress(int x, int y, int button) {
	//if(!beToggle) (*value) = false;
	//ofNotifyEvent(onReleased, e, this);
	gui.setPage(shortcutPageName);
}

/*
void ofxSimpleGuiPageShortcut::onKeyEnter() {
	//if(!beToggle) (*value) = false;
	//ofNotifyEvent(onReleased, e, this);
	gui.setPage(shortcutPageName);
}
*/

ofxSimpleGuiPage& ofxSimpleGuiPageShortcut::getPage(){
	return *page;
}

int	 ofxSimpleGuiPageShortcut::getPriority(){
	return getPage().getPriority(); //<<TODO: optimize performance
}

ofxSimpleGuiPageShortcut::ofxSimpleGuiPageShortcut(string name, string shortcutPageName) : ofxSimpleGuiButton(name,emptyBool){
	this->shortcutPageName = shortcutPageName;
	page = &gui.page(shortcutPageName);
}

void ofxSimpleGuiPageShortcut::draw(float x, float y) {
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