#pragma once
/*
 *  IdleModeView.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 11/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"

class IdleModeView : public UI::View {
public:
	
	UIElement recordButton;
	UIElement videoOffButton;
	UIElement instructionSprite;
	
	void setup();
	void draw();
	
};

