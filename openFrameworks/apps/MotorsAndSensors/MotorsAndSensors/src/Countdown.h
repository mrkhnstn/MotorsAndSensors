#pragma once
/*
 *  Countdown.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 23/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "UIElement.h"

class Countdown : public UI::UIElement {

public:

	ofTrueTypeFont myFont;
	
	void setup(){
		UIElement::setup();
		myFont.loadFont("fonts/Hlcb____.ttf",128,true,false,true);
	}
	
	void draw(){
	
		drawOverlay();
		
		float scale = 2; // scale the normal label
		
		// draw label
		if(label != ""){
			ofPushStyle();
			
			ofFill();
			ofSetLineWidth(0);
			
			// set color
			if(isInteractive)
				ofSetColor(255, 0, 0); // red if interactive
			else {
				ofSetColor(255, 255, 255); // white for non interactive
			}
			
			ofRectangle rect = getTextRect(label,myFont);
			
			ofPushMatrix();
			ofTranslate(x+0.5*width-0.5*rect.width*scale,y+0.5*height+0.5*rect.height*scale,0);
			ofScale(scale, scale, 1);
			myFont.drawStringAsShapes(label,0,0);
			ofPopMatrix();
			
			ofPopStyle();
		}
	}
	
};

