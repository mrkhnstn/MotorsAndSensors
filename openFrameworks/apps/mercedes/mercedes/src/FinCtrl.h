#pragma once
/*
 *  FinCtrl.h
 *  mercedes
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "Fin.h"

class FinCtrl : public ofxBaseObject {
public:
	
	Constants* constants;
	
	vector<Fin*> fins;
	
	int servoMin;
	int servoMax;
	
	void setup(){
		constants = Singleton<Constants>::instance();
		
		servoMin = 600;
		servoMax = 2000;
		
		int bank = 0;
		int ch = 1;
		for (int i=0; i<constants->numFins; i++) {
			Fin* fin = new Fin();
			fin->index = i;
			fin->setup();
			fin->bank = bank;
			fin->ch = ch;
			fins.push_back(fin);
			
			ch++;
			if(ch > 18) {
				ch = 1;
				bank++;
			}
		}
	}
	
	void setupGUI(){
		
		gui.page(1).addPageShortcut(gui.addPage("FinCtrl"));
		gui.addTitle("servo limits");
		gui.addSlider("servoMin", servoMin, 500, 2500);
		gui.addSlider("servoMax", servoMax, 500, 2500);
		
		
		for (int j=0; j<4; j++) {
			int min = j*18;
			int max = (j+1)*18;
			gui.page(1).addPageShortcut(gui.addPage("Fins_"+ofToString(min)+"-"+ofToString(max-1)));

			gui.addTitle("angleN");
			for(int i=min; i<max && i < fins.size(); i++)
			{
				gui.addSlider("angleN_"+ofToString(i), fins[i]->angleN, 0, 1);
			}

			gui.addTitle("bank").setNewColumn(true);
			for(int i=min; i<max && i < fins.size(); i++)
			{
				gui.addSlider("bank_"+ofToString(i), fins[i]->bank, 0, 3);
			}
			
			gui.addTitle("channel").setNewColumn(true);
			for(int i=min; i<max && i < fins.size(); i++)
			{
				gui.addSlider("channel_"+ofToString(i), fins[i]->ch, 1, 84);
			}
			
			gui.addTitle("enabled");
			for(int i=min; i<max && i < fins.size(); i++)
			{
				gui.addToggle("enabled_"+ofToString(i), fins[i]->enabled);
			}
			
			
		}
		/*
		for (int i=0; i<Singleton<Constants>::instance()->numFins; i++) {
			fins[i]->setupGUI();
		}
		*/
	}
	
	void postGUI(){
		for (int i=0; i<Singleton<Constants>::instance()->numFins; i++) {
			fins[i]->postGUI();
		}
	}
	
	void update(){
		for (int i=0; i<Singleton<Constants>::instance()->numFins; i++) {
			fins[i]->update();
		}
	}
	
	void draw(){
		
		ofPushMatrix();
		ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5, 0);
		for (int i=0; i<Singleton<Constants>::instance()->numFins; i++) {
			fins[i]->draw();
		}
		ofPopMatrix();
	}
};