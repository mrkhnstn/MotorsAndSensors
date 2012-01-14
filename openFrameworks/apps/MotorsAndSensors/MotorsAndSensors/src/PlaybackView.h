#pragma once
/*
 *  PlaybackView.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 14/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "Blink.h"

class PlaybackView : public UI::View {
public:
	
	UIElement clearButton;
	UIElement emailButton;
	UIElement videoOffButton;
	UIElement playbackIconSprite;
	
	void setup();
	void draw();
	
};