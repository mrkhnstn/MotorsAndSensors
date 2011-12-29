/*
 *  FboMaskManager.cpp
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 26/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "FboMaskManager.h"

FboMaskManager::FboMaskManager(){
	drawing = false;
	editing = false;
	_editing = false;
	displayZoom = 0;
	displayCoords.x = 0;
	displayCoords.y = 0;
	worldFbo = NULL;
	autoload = false;
	hasAutoloaded = false;
	load = false;
	save = false;
	clickDistance = FBOMASKCLICKDISTANCE;
	loadPath = "FboMaskShape.xml";
	maskImageTime = 0;
	//drawToOutputs = false;
	blankOutput = false;
	invert = false;
	blurEnabled = false;
}

void FboMaskManager::setup(ofxFBOTexture& worldFbo, string filepath){
	this->worldFbo = &worldFbo;
	this->loadPath = filepath;
	maskFbo.allocate(worldFbo.getWidth(), worldFbo.getHeight(), false);
	fbo2.allocate(worldFbo.getWidth(), worldFbo.getHeight(), false);
	maskImage.allocate(worldFbo.getWidth(), worldFbo.getHeight(), OF_IMAGE_COLOR_ALPHA);
	
	shaderH.loadShader("shaders/simpleBlurHorizontal");
	shaderV.loadShader("shaders/simpleBlurVertical");
	
	noPasses = 1;
	blurDistance = 2.0;
}

void FboMaskManager::setupGUI(string name){
	gui.page(1).addPageShortcut(gui.addPage("FboMask"));
	gui.addToggle("drawing", drawing);
	gui.addToggle("editing", editing);
	gui.addToggle("invert", invert);
	gui.addToggle("drawEditOverlay", drawEditOverlay);
	gui.addToggle("hideMouseWhileEditing", hideMouseWhileEditing);
	
	
	gui.addSlider("displayZoom", displayZoom, -2, 2);
	gui.addSlider("displayX", displayCoords.x, -ofGetWidth(), ofGetWidth());
	gui.addSlider("displayY", displayCoords.y, -ofGetHeight(), ofGetHeight());
	//gui.addToggle("drawToOutputs", drawToOutputs);
	gui.addTitle("Loading");
	gui.addButton("save", save);
	gui.addButton("load", load);
	gui.addToggle("autoload", autoload);
	gui.addTitle("Blurring");
	gui.addToggle("blurEnabled", blurEnabled);
	gui.addSlider("blurDistance",blurDistance,0,3);
	gui.addSlider("noPass", noPasses, 0, 8);
};

void FboMaskManager::update(){
	if(!hasAutoloaded){
		editing = false;
		if(autoload){
			mask.load(loadPath);
			drawToFboBlending();
		}
		hasAutoloaded = true;
	}
	//if(drawToOutputs){
	//			drawToFboBlending();
	//			drawToOutputs = false;
	//		}
	
	if (_editing != editing) {
		_editing = editing;
		if (_editing) {
			addListeners();
		} else {
			removeListeners();
		}
	}
	
	if(editing){
		
		if(!blankOutput){
			blankOutput = true;
			drawToFboBlending();
		}
		
		if(hideMouseWhileEditing){
			ofHideCursor();
		} else {
			ofShowCursor();
		}
		
		if(load){
			mask.load(loadPath);
			load = false;
		}
		if(save){
			mask.save(loadPath);
			save = false;
		}
		
	} else {
		ofShowCursor();
		if(blankOutput){
			blankOutput = false;
			drawToFboBlending();
		}
		
	}
	
	
}

void FboMaskManager::draw(){
	if(editing && drawEditOverlay){
		
		if(worldFbo != NULL){
			glPushMatrix();
			float zoom = getZoom();
			glScalef(zoom, zoom, zoom);
			glTranslatef(-displayCoords.x, -displayCoords.y, 0);
			
			ofSetColor(255, 255, 255);
			worldFbo->draw(0, 0);
			
			drawEditShapes();
			
			glPopMatrix();
		}
		drawToWorld();
	}
	//maskImage.draw(0, 0);
}

void FboMaskManager::drawEditShapes(){
	mask.draw(true,invert);
	
	if(dragging){
		mask.drawSelection(dragPoint, false, true);
	} else {
		FboMaskSelection selection = mask.getNearestPoint(lastMouse);
		if(selection.valid){
			mask.drawSelection(selection, false, true);
		}
	}
}

void FboMaskManager::drawToWorld(){
	if(drawing){
		if(editing){
			drawEditShapes();
			
			ofSetColor(0, 255, 0);
			ofFill();
			ofSetLineWidth(0);
			ofCircle(lastMouse.x, lastMouse.y, 4);
		}
	}
	//ofSetColor(255, 255, 255);
	//maskImage.draw(0, 0);
}

void FboMaskManager::drawToFboBlending(){
	//return;
	maskFbo.begin();
	if (invert) {
		maskFbo.clear(0, 0, 0, 255);
	} else {
		maskFbo.clear(255, 255, 255, 255);
	}
	if(blankOutput){
		//draw nothing?!
	} else {
		mask.draw(false,invert);
	}
	maskFbo.end();
	
	if(blurEnabled){
		
		ofxFBOTexture * src, * dst;
		src = &maskFbo;
		dst = &fbo2;
		
		for(int i = 0; i < noPasses; i++){
			float blurPer =  blurDistance * ofMap(i, 0, noPasses, 1.0/noPasses, 1.0);
			
			//first the horizontal shader 
			shaderH.setShaderActive(true);
			shaderH.setUniformVariable1f("blurAmnt", blurPer);
			
			dst->begin();
			src->draw(0, 0);
			dst->end();
			
			shaderH.setShaderActive(false);
			
			//now the vertical shader
			shaderV.setShaderActive(true);	
			shaderV.setUniformVariable1f("blurAmnt", blurPer);
			
			src->begin();
			dst->draw(0,0);
			src->end();
			
			shaderV.setShaderActive(false);
			
			//			ofxFBOTexture  * tmp = src;
			//			src = dst;
			//			dst = tmp;
		}		
		
	}
	
	
	maskFbo.begin();
	//maskImage.grabScreen(0, 0, maskImage.width, maskImage.height);//THE OLD BROKED WAY
	ImageUtil::grabFBO(maskImage, 0, 0, maskImage.width, maskImage.height);//THE NEW WORKING WAY
	
	maskFbo.end();
	
	maskImageTime = ofGetElapsedTimeMillis();
}

float FboMaskManager::getZoom(){
	return powf(2, displayZoom);
}

ofxVec2f FboMaskManager::mouseVec(float mouseX, float mouseY){
	ofxVec2f mouse;
	float zoom = getZoom();
	mouse.x = (mouseX/zoom + displayCoords.x);
	mouse.y = (mouseY/zoom + displayCoords.y);
	return mouse;
}

void FboMaskManager::mouseMoved(ofMouseEventArgs &e){
	mouseMove(e.x, e.y);
}

void FboMaskManager::mousePressed(ofMouseEventArgs &e){
	mouseDown(e.x, e.y);
}	

void FboMaskManager::mouseReleased(ofMouseEventArgs &e){
	mouseUp(e.x, e.y);
};

void FboMaskManager::keyPressed(ofKeyEventArgs &e){
	if (editing) {
		if(!dragging){
			float scale = checkShiftKey() ? 1.0f : 0.1f;
			switch (e.key) {
				case OF_KEY_UP:
					shiftSelection(0, -1 * scale);
					break;	
				case OF_KEY_DOWN:
					shiftSelection(0, 1 * scale);
					break;
				case OF_KEY_LEFT:
					shiftSelection(-1 * scale, 0);
					break;
				case OF_KEY_RIGHT:
					shiftSelection(1 * scale, 0);
					break;
				default:
					break;
			}
			
		}
	}
}

void FboMaskManager::shiftSelection(float dx, float dy){
	FboMaskSelection selection = mask.getNearestPoint(lastMouse);
	if (selection.valid) {
		mask.shiftSelection(selection,dx,dy);
	}
}

void FboMaskManager::keyReleased(ofKeyEventArgs &e){};

void FboMaskManager::mouseMove(float mouseX, float mouseY){
	if(editing){
		ofxVec2f mouse = mouseVec(mouseX, mouseY);
		ofxVec2f deltaMouse = mouse - lastMouse;
		
		if(dragging){
			dragPoint.point += deltaMouse;
			mask.replacePoint(dragPoint);
		}
		lastMouse = mouse;
	}
}

void FboMaskManager::mouseDown(float mouseX, float mouseY){
	if(editing){
		ofxVec2f mouse = mouseVec(mouseX, mouseY);
		
		if(checkShiftKey()){
			
		} else {
			FboMaskSelection nearest = mask.getNearestPoint(mouse);
			float distance = nearest.distance(mouse);
			
			if(nearest.valid && !nearest.midPoint && distance < clickDistance){
				dragging = true;
				dragPoint = nearest;
			}
		}
		
		lastMouse = mouse;
	}
}

void FboMaskManager::mouseUp(float mouseX, float mouseY){
	if(editing){
		ofxVec2f mouse = mouseVec(mouseX, mouseY);
		
		if(dragging){
			
		} else {
			FboMaskSelection nearest = mask.getNearestPoint(mouse);
			float distance = nearest.distance(mouse);
			
			if(nearest.valid){
				if(distance < clickDistance){
					if(!nearest.midPoint){
						if(checkShiftKey()){
							//delete this point!
							mask.deletePoint(nearest);
						}
						
					} else {
						if(!checkShiftKey()){
							//insert a new point at the midpoint
							mask.insertPoint(nearest);
						}
					}
				}
			} else {
				//we need to insert a new shape
				
				if(checkShiftKey()){
					nearest.setPoint(mouse);
					mask.insertShape(nearest);
				}
			}
		}
		
		dragging = false;
		lastMouse = mouse;
	}
}

void FboMaskManager::addListeners() {
	
	//	ofAddListener(ofEvents.setup, this, &FboMaskManager::setup);
	//  ofAddListener(ofEvents.update, this, &FboMaskManager::update);
	//	ofAddListener(ofEvents.draw, this, &FboMaskManager::draw);
	//	ofAddListener(ofEvents.exit, this, &FboMaskManager::exit);
	
	ofAddListener(ofEvents.mousePressed, this, &FboMaskManager::mousePressed);
	ofAddListener(ofEvents.mouseMoved, this, &FboMaskManager::mouseMoved);
	ofAddListener(ofEvents.mouseDragged, this, &FboMaskManager::mouseMoved);
	ofAddListener(ofEvents.mouseReleased, this, &FboMaskManager::mouseReleased);
	
	ofAddListener(ofEvents.keyPressed, this, &FboMaskManager::keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &FboMaskManager::keyReleased);
}

void FboMaskManager::removeListeners() {
	
	//	ofRemoveListener(ofEvents.setup, this, &FboMaskManager::setup);
	//  ofRemoveListener(ofEvents.update, this, &FboMaskManager::update);
	//	ofRemoveListener(ofEvents.draw, this, &FboMaskManager::draw);
	//	ofRemoveListener(ofEvents.exit, this, &FboMaskManager::exit);
	
	ofRemoveListener(ofEvents.mousePressed, this, &FboMaskManager::mousePressed);
	ofRemoveListener(ofEvents.mouseMoved, this, &FboMaskManager::mouseMoved);
	ofRemoveListener(ofEvents.mouseDragged, this, &FboMaskManager::mouseMoved);
	ofRemoveListener(ofEvents.mouseReleased, this, &FboMaskManager::mouseReleased);
	
	ofRemoveListener(ofEvents.keyPressed, this, &FboMaskManager::keyPressed);
	ofRemoveListener(ofEvents.keyReleased, this, &FboMaskManager::keyReleased);
}
