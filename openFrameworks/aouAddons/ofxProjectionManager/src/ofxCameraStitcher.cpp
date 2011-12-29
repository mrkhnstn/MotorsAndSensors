/*
 *  ofxCameraStitcher.cpp
 *  TestBed_Mark
 *
 *  Created by TAQA_CH_2 on 11/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxCameraStitcher.h"

void ofxCameraStitcher::setup(int fboWidth, int fboHeight){
	fbo.allocate(fboWidth, fboHeight, GL_RGB);
	fbo2.allocate(fboWidth, fboHeight, GL_RGB);
	pixels = NULL;
}

void ofxCameraStitcher::addCamera(string cameraName, ofTexture& cameraTexture, string xmlFolderpath){
	ofxProjectionManager* projectionManager = new ofxProjectionManager(); //TODO: clean up!!
	projectionManager->name = cameraName;
	projectionManager->setup(cameraTexture.getWidth(), cameraTexture.getHeight());
	projectionManager->addOutput(cameraName+" Output", xmlFolderpath);
	cameras.add(cameraName, projectionManager);
	cameraTextures.add(cameraName, &cameraTexture);
	
	CameraIdPair pair;
	pair.id = cameras.size()-1;
	pair.projectionManager = projectionManager;
	sortedCameras.push_back(pair);
}

void ofxCameraStitcher::setupGUI(){
	for (int i=0; i<cameras.size(); i++) {
		cameras[i]->setupGUI();
	}
}

void ofxCameraStitcher::postGUI(){
	for (int i=0; i<cameras.size(); i++) {
		cameras[i]->postGUI();
	}
}

void ofxCameraStitcher::update(){
	for (int i=0; i<cameras.size(); i++) {
		cameras[i]->update();
	}	
}

void ofxCameraStitcher::drawFbos(){
	
	//TODO: order drawin
	sort(sortedCameras.begin(),sortedCameras.end(),sortCamera);
	
	// only if editing draw camera textures into projection fbos
	for (int j=0; j<sortedCameras.size(); j++) {
		int i = sortedCameras[j].id;
		if (cameras[i]->outputs[0]->isEditing()) {
			cameras[i]->begin();
			glPushMatrix();
			glScalef(1, -1, 1);
			glTranslatef(0, -cameraTextures[i]->getHeight(), 0);
			cameraTextures[i]->draw(0,0);
			glPopMatrix();
			cameras[i]->end();
			
			cameras[i]->outputs[0]->draw();
			
			//if (cameras[i]->outputs[0]->editingGrid) {
			//	cameras[i]->drawOutputHUD(cameras[i]->outputs[0]);
			//}
		}
	}
	
	glColor4f(1, 1, 1, 1);
	// draw cameras into main fbo
	fbo.begin();
	fbo.clear(0, 0, 0, 1); //< necessary?!
	//for (int i=0; i<cameras.size(); i++) {
	for (int j=0; j<sortedCameras.size(); j++) {
		int i = sortedCameras[j].id;
		if (cameras[i]->outputs[0]->drawOutput) {
			if (cameras[i]->outputs[0]->isEditing()) {
				cameras[i]->drawer.drawSub(0, cameras[i]->outputs[0], false);
			} else {
				cameras[i]->drawer.drawSub(0, cameras[i]->outputs[0], false, cameraTextures[i]);
			}
			
			cameras[i]->outputs[0]->drawHUD();
			
		}
	}
	
		
	/*
	 glPixelStorei(GL_PACK_ALIGNMENT, 1);
	 glReadPixels(0, 0, fbo.getWidth(), fbo.getHeight(), GL_RED, GL_UNSIGNED_BYTE, testPixels);
	 */
	
	fbo.end();
	
	
	
	// mirror y  for fbo read back
	fbo2.begin();
	ofPushMatrix();
	ofDisableAlphaBlending();
	ofSetColor(255, 255, 255);
	ofScale(1, -1, 1);
	fbo.draw(0, -fbo.getHeight());
	ofPopMatrix();
	if (pixels != NULL) {
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, fbo2.getWidth(), fbo2.getHeight(), GL_RED, GL_UNSIGNED_BYTE, pixels);
	}
	
	fbo2.end();
}

void ofxCameraStitcher::draw(){
	// draws main fbo
	fbo.draw(0, 0);
}

void ofxCameraStitcher::draw(float x, float y, float width, float height){
	fbo.draw(x,y,width,height);
	for (int i=0; i<cameras.size(); i++) {
		if (cameras[i]->outputs[0]->isEditing()) {
			cameras[i]->drawOutputHUDOutside(cameras[i]->outputs[0]);
		}
	}
}