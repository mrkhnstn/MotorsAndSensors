#pragma once
/*
 *  UIElement.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 21/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxMSAInteractiveObject.h"
#include "ofxSimpleGuiToo.h"
#include "ofxVec2f.h"
#include "Coords.h"

namespace UI {
	
	class UIElement : public ofxMSAInteractiveObject {
		
	public:
		
		static bool doDrawNames;
		static bool doDrawOutlines;
		static bool doDrawCrosses;
		
		string	name;	// name of the object instance
		string	label;	// label to be displayed
		bool	isInteractive; // if true then enables / disables mouse and key events on enable() / disable()
		
		ofEvent<ofEventArgs> onReleased;
		ofEvent<ofEventArgs> onPressed;
		ofEventArgs e;
		
		UIElement(){
			loadFont();
			disableAppEvents();
			name = "Element";
			label = "";
			enabled = false;
			isInteractive = false;
		}
		
		virtual void setup(){}
		
		virtual void setupGUI(){
			gui.addTitle(name);
			gui.addSlider(name+"_x", x, 0, Coords::width);
			gui.addSlider(name+"_y", y, 0, Coords::height);
			gui.addSlider(name+"_w", width, 0, Coords::width);
			gui.addSlider(name+"_h", height, 0, Coords::height);
		}
		
		virtual void postGUI(){}
		
		virtual void draw(){
			drawOverlay();
			
			// draw label if set
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
				
				ofRectangle rect = getTextRect(label,labelFont);//font.getStringBoundingBox(name, 0, 0);
				
				labelFont.drawStringAsShapes(label,x+0.5*width-0.5*rect.width,y+0.5*height+0.5*rect.height);
				
				ofPopStyle();
			}
		}
		
		// fixes bug in ofTrueTypeFont::getStringBoundingBox with multiple words
		static ofRectangle& getTextRect(string& text, ofTrueTypeFont& ttf)
		{
			string temp = text;
			size_t found = temp.find(" ");
			while (found != string::npos) {
				temp.replace(found,1,"_");
				found = temp.find(" ");
			}
			//ofLog(OF_LOG_VERBOSE, temp);
			ofRectangle rect = ttf.getStringBoundingBox(temp, 0, 0);
			return rect;
		}
		
		virtual void update(){}
		
		virtual void drawOverlay(){
			
			ofPushStyle();
			
			// set color
			if(isInteractive)
				ofSetColor(255, 0, 0); // red if interactive
			else {
				ofSetColor(255, 255, 255); // white for non interactive
			}
			
			// draw outline
			ofNoFill();
			ofSetLineWidth(1);
			if(doDrawOutlines)
				ofRect(x,y,width,height);
			if(doDrawCrosses){
				ofLine(x, y, x+width, y+height);
				ofLine(x+width, y, x, y+height);
			}
			if (doDrawNames) {
				// draw name as text
				ofFill();
				ofSetLineWidth(0);
				ofRectangle rect = font.getStringBoundingBox(name, 2, 2);
				
				// draw rectangle around name
				font.drawString(name, x,y+rect.height);
				ofNoFill();
				ofSetLineWidth(1);
				ofRect(x, y, rect.width+5, rect.height+5);
				ofPopStyle();
			}
		}
		
		virtual void enable(){
			enabled = true;
			if(isInteractive){
				enableMouseEvents();
				enableKeyEvents();
			}
		}
		
		virtual void disable(){
			enabled = false;
			if(isInteractive){
				disableMouseEvents();
				disableKeyEvents();
			}
		}
		
		void onPress(int x, int y, int button){
			//ofLog(OF_LOG_VERBOSE, "onPress");
			ofNotifyEvent(onPressed, e, this);
		}
		
		void onRelease(int x, int y, int button){
			//ofLog(OF_LOG_VERBOSE, "onRelease");
			ofNotifyEvent(onReleased, e, this);
		}
		
		virtual void _mouseMoved(ofMouseEventArgs &e){
			ofxMSAInteractiveObject::_mouseMoved(mapMouseEvent(e));
		}
		
		virtual void _mousePressed(ofMouseEventArgs &e){
			ofxMSAInteractiveObject::_mousePressed(mapMouseEvent(e));
		}
		
		virtual void _mouseDragged(ofMouseEventArgs &e){
			ofxMSAInteractiveObject::_mouseDragged(mapMouseEvent(e));
		}
		
		virtual void _mouseReleased(ofMouseEventArgs &e){
			ofxMSAInteractiveObject::_mouseReleased(mapMouseEvent(e));
		}
		
		ofMouseEventArgs& mapMouseEvent(ofMouseEventArgs &e){
			ofxVec2f temp;
			temp.x = e.x - Coords::x;
			temp.y = e.y - Coords::y;
			//if(verbose) printf("mapMouseEvent(x: %i, y: %i)\n", e.x, e.y);
			ofPoint origin;
			origin.x = 0;
			origin.y = 0;
			temp.map(origin, Coords::vx, Coords::vy);
			ofMouseEventArgs m;
			m.x = temp.x;
			m.y = temp.y;
			m.button = e.button;
			return m;
		}
		
	protected:
		
		void loadFont(){
			if(fontLoaded) return;
			font.loadFont("fonts/Hlcb____.ttf",12,true,true,true);
			labelFont.loadFont("fonts/Hlcb____.ttf",24,true,true,true);
			fontLoaded = true;
		}
		
		static bool fontLoaded;
		static ofTrueTypeFont font;
		static ofTrueTypeFont labelFont;
	};
	
}