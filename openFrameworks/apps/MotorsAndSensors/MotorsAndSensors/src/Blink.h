#pragma once
/*
 *  Blink.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 23/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "UIElement.h"

class Blink : public UI::UIElement {
	
public:
	
	float refTime;
	bool labelOn;
	
	void enable(){
		UIElement::enable();
		refTime = ofGetElapsedTimef() + 1;
		labelOn = true;
	}
	
	void draw(){
		drawOverlay();

		// draw label every other second
		
		if (ofGetElapsedTimef() >= refTime) {
			labelOn = !labelOn;
			refTime = ofGetElapsedTimef() + 1;
		}

		if(!labelOn) return;
		if(label == "") return;
		
			ofPushStyle();
			
			ofFill();
			ofSetLineWidth(0);
			
			// set color
			if(isInteractive)
				ofSetColor(255, 0, 0); // red if interactive
			else {
				ofSetColor(255, 255, 255); // white for non interactive
			}
			
			ofRectangle rect = getTextRect(label,labelFont);//font.getStringBoundingBox(name, 0, 0);
			
			labelFont.drawStringAsShapes(label,x+0.5*width-0.5*rect.width,y+0.5*height+0.5*rect.height);
			
			ofPopStyle();
		
	}
	
};

