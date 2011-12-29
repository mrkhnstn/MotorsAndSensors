#pragma once

/*
 *  ForceFieldDrawer2f.h
 *  atmosphere
 *
 *  Created by Mark Hauenstein on 03/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "ForceField2f.h"
#include "ForceFieldSet2f.h"
#include <set>

class ForceFieldDrawer2f : public ofxBaseObject {
	
public:
	
	ofRectangle boundary;				// boundary of area to test
	int hPoints; 						// horizontal points
	int vPoints;						// vertical points
	ForceFieldSet2f fields;
	//set<ForceEffector2f*> effectors; 	// effectors used to test force field
	float forceVectorScale;					// factor to scale the resulting force vector at a test point
	float forceVectorLimit;
	
	float* vertexArray;
	float* colourArray;
	int pointCount;
	
	bool doDraw;
	
	//////////////////////////////////////////////////////////////////////
	
	virtual void setupGUI(){
		//gui.page(1).addPageShortcut(gui.addPage(name_));
		gui.addPage("ForceFieldDrawer");
		gui.addToggle("draw", doDraw);
		gui.addSlider("horizontal points", hPoints, 0, 512);
		gui.addSlider("vertical points", vPoints, 0, 512);
		gui.addSlider("x", boundary.x, 0, 1920);
		gui.addSlider("y", boundary.y, 0, 1200);
		gui.addSlider("width", boundary.width, 0, 1920);
		gui.addSlider("height", boundary.height, 0, 1200);
		gui.addSlider("scale", forceVectorScale, 0, 1000);
		gui.addSlider("limit", forceVectorLimit, 0, 1000);
	}
	
	virtual void setup(){
		hPoints = 0;
		vPoints = 0;
		forceVectorScale = 1;
		boundary.x = 0;
		boundary.y = 0;
		boundary.width = ofGetWidth();
		boundary.height = ofGetHeight();
		pointCount = 0;
		vertexArray = NULL;
		colourArray = NULL;
		doDraw = true;
		
		ofAddListener(ofEvents.keyPressed, this, &ForceFieldDrawer2f::keyPressed);
	}
	
	virtual void update(){
		
		if(doDraw){
			
			if (pointCount != vPoints * hPoints) {
				// update vertexArray and colourArray
				if (vertexArray != NULL) {
					delete[] vertexArray;
					delete[] colourArray;
				}
				
				pointCount = vPoints * hPoints;
				
				vertexArray = new float[pointCount*2*2];
				
				colourArray = new float[pointCount*4*2];
				
				// initialize colour array (does not need to be updated on a frame basis)
				float* colourPointer = &colourArray[0];
				
				for(int i=0; i<pointCount; i++){
					// point 1
					*colourPointer++ = 1;
					*colourPointer++ = 1;
					*colourPointer++ = 1;
					*colourPointer++ = 1;
					// point 2
					*colourPointer++ = 0;
					*colourPointer++ = 0;
					*colourPointer++ = 0;
					*colourPointer++ = 1;
				}
				
			}
			
			// update vertices
			
			float* vertexPointer = &vertexArray[0];
			float limit2 = forceVectorLimit * forceVectorLimit;
			
			for(int y=0; y<vPoints; y++)
				for(int x=0; x<hPoints; x++){
					float x1 = ofMap(x, 0, hPoints-1, boundary.x, boundary.x + boundary.width, false);
					float y1 = ofMap(y, 0, vPoints-1, boundary.y, boundary.y + boundary.height, false);
					
					ofxVec2f pos (x1, y1);
					ofxVec2f force = fields.getForce(pos);
					//for(set<ForceEffector2f*>::iterator it = effectors.begin(); it != effectors.end(); ++it){
					//	ForceEffector2f* effector = *it;
					//	force += effector->getForce(pos);
					//}
					
					//scale force vector
					force *= forceVectorScale;
					
					//limit force vector
					float d2 = force.x * force.x + force.y * force.y; 
					if(d2 > limit2){ // scaled force vector longer than limit
						float d = sqrtf(d2);
						if(d != 0){
							force *= forceVectorLimit / d;
						}
					}
					
					float x2 = x1 + force.x;
					float y2 = y1 + force.y;
					
					*vertexPointer++ = x1;
					*vertexPointer++ = y1;
					*vertexPointer++ = x2;
					*vertexPointer++ = y2;
				}
		}
		
	}
	
	virtual void draw(){
		
		if(doDraw){
			
			//draw boundary outline
			ofSetColor(255, 0, 0);
			ofSetLineWidth(1);
			ofNoFill();
			ofRect(boundary.x, boundary.y, boundary.width, boundary.height);
			
			
			// draw with vertex array
			
#ifdef FORCE_FIELD_DRAWER_DEBUG_DRAW
			
			//iterate through grid points
			//calculate accumulated force vector and draw vector
			ofSetColor(0, 0, 255);
			float radius = 2; // radius of source point
			float limit2 = forceVectorLimit * forceVectorLimit;
			
			//TODO: separate calculation and optimize drawing
			
			for(int y=0; y<vPoints; y++)
				for(int x=0; x<hPoints; x++){
					float x1 = ofMap(x, 0, hPoints-1, boundary.x, boundary.x + boundary.width, false);
					float y1 = ofMap(y, 0, vPoints-1, boundary.y, boundary.y + boundary.height, false);
					
					ofxVec2f pos (x1, y1);
					ofxVec2f force = fields.getForce(pos);
					//for(set<ForceEffector2f*>::iterator it = effectors.begin(); it != effectors.end(); ++it){
					//	ForceEffector2f* effector = *it;
					//	force += effector->getForce(pos);
					//}
					
					//scale force vector
					force *= forceVectorScale;
					
					//limit force vector
					float d2 = force.x * force.x + force.y * force.y; 
					if(d2 > limit2){ // scaled force vector longer than limit
						float d = sqrtf(d2);
						if(d != 0){
							force *= forceVectorLimit / d;
						}
					}
					
					float x2 = x1 + force.x;
					float y2 = y1 + force.y;
					
					/*
					 // draw source point
					 ofFill();
					 ofSetLineWidth(0);
					 ofCircle(x1, y1, radius);
					 */
					
					ofNoFill();
					ofSetLineWidth(1);
					//draw force line
					ofLine(x1, y1, x2, y2);
				}
#else
			
			if(pointCount > 0){
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(4,GL_FLOAT,0, &colourArray[0]);
				
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(2, GL_FLOAT, 0, &vertexArray[0]);
				
				glDrawArrays(GL_LINES, 0, pointCount*2);
				
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
			}
#endif
			
		}
	}
	
	void keyPressed(ofKeyEventArgs& e){
		keyPressed(e.key);
	}
	
	void keyPressed(int key){
		switch(key) {
			case 'v':
				doDraw = !doDraw;
				break;
		}
	}
	
};
