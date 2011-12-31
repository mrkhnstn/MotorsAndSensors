#pragma once
/*
 *  ofxBaseContainer.h
 *  mercedes
 *
 *  Created by Mark Hauenstein on 31/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"

class ofxBaseContainer : public ofxBaseObject {
	
public:
	
	vector<ofxBaseObject*> objects;
	
	virtual void addObject(ofxBaseObject& object){
		objects.push_back(&object);
	}
	
	virtual void removeObject(ofxBaseObject& object){
		vector<ofxBaseObject*> temp;
		
		for(int i=0; i<objects.size(); i++)
		{
			if(objects[i] != &object)
			{
				temp.push_back(objects[i]);
			}
		}
		
		objects.clear();
		
		for(int i=0; i<temp.size(); i++)
		{
			objects.push_back(temp[i]);
		}
	}
	
	virtual void setup(){
		for (int i=0; i<objects.size(); i++) {
			objects[i]->setup();
		}
	}
	
	virtual void setupGUI(){
		for (int i=0; i<objects.size(); i++) {
			objects[i]->setupGUI();
		}
	}
	
	virtual void postGUI(){
		for (int i=0; i<objects.size(); i++) {
			objects[i]->postGUI();
		}
	}
	
	virtual void update(){
		for (int i=0; i<objects.size(); i++) {
			objects[i]->update();
		}
	}
	
	virtual void draw(){
		for (int i=0; i<objects.size(); i++) {
			objects[i]->draw();
		}
	}
	
	virtual void exit(){
		for (int i=0; i<objects.size(); i++) {
			objects[i]->exit();
		}
	}
	
};
