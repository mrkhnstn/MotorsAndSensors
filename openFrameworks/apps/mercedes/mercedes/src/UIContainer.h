#pragma once
/*
 *  UIContainer.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 21/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "UIElement.h"

namespace UI {

	class UIContainer : public UIElement {
	public:
		
		vector<UIElement*> elements;			// all elements
		
		virtual void setupGUI(){
			UIElement::setupGUI();
			for(int i=0; i<elements.size(); i++){
				elements[i]->setupGUI();
			}
		}
		
		virtual void postGUI(){
			UIElement::postGUI();
			for(int i=0; i<elements.size(); i++){
				elements[i]->postGUI();
			}
		}
		
		void addElement(UIElement& e){
			elements.push_back(&e);
		}
		
		void removeElement(UIElement& e){
			//TODO: implement
		}

		virtual void enable(){
			UIElement::enable();
			for(int i=0; i<elements.size(); i++){
				elements[i]->enable();
			}
		}
		
		virtual void disable(){
			UIElement::disable();
			for(int i=0; i<elements.size(); i++){
				elements[i]->disable();
			}
		}
		
		virtual void update(){
			if (!enabled) return;
			UIElement::update();
			for(int i=0; i<elements.size(); i++){
				elements[i]->update();
			}
		}
		
		virtual void draw(){
			if (!enabled) return;
			UIElement::draw();
			for(int i=0; i<elements.size(); i++){
				elements[i]->draw();
			}
		}
		
		
	};
	
}