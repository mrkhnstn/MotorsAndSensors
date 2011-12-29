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

void ofxSimpleGuiPageShortcut::onRelease(int x, int y, int button) {
	//if(!beToggle) (*value) = false;
	//ofNotifyEvent(onReleased, e, this);
	gui.setPage(shortcutPageName);
}