#pragma once
/*
 *  Button.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 11/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "UIElement.h"

namespace UI {
	
	class Button : public UIElement {
	public:
		
		/*
		ofEvent<ofEventArgs> onReleased;
		ofEvent<ofEventArgs> onPressed;
		ofEventArgs e;
		
		Button(){
			disableAppEvents();
		}
		
		void enable(){
			enabled = true;
			enableMouseEvents();
		}
		
		void disable(){
			enabled = false;
			disableMouseEvents();
		}
		
		void onPress(int x, int y, int button){
			ofLog(OF_LOG_VERBOSE, "onPress");
			ofNotifyEvent(onPressed, e, this);
		}
		
		void onRelease(int x, int y, int button){
			ofLog(OF_LOG_VERBOSE, "onRelease");
			ofNotifyEvent(onReleased, e, this);
		}
		*/
	};
	
}

