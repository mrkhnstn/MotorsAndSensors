/*
 *  UIElement.cpp
 *  TestBed
 *
 *  Created by Mark Hauenstein on 21/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "UIElement.h"

namespace UI {

	bool UIElement::fontLoaded = false;
	ofTrueTypeFont UIElement::font;
	ofTrueTypeFont UIElement::labelFont;
	
	bool UIElement::doDrawNames = true;
	bool UIElement::doDrawOutlines = true;
	bool UIElement::doDrawCrosses = false;

}