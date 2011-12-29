#pragma once
/*
 *  View.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 10/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "UIContainer.h"

namespace UI {
	
	class View : public UIContainer {
		
	public:

		virtual void setupGUI(){
			gui.page(1).addPageShortcut(gui.addPage(name));
			for(int i=0; i<elements.size(); i++){
				elements[i]->setupGUI();
			}
		}
		
		virtual void postGUI(){
			width = Coords::width;
			height = Coords::height;
			UIContainer::postGUI();
		}
		
	};
	
}