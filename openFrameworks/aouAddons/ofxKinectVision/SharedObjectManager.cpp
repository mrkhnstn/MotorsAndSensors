/*
 *  SharedObjectManager.cpp
 *  TestBed_Mark2
 *
 *  Created by TAQA_CH_0 on 10/02/2011.
 *  Copyright 2011 AllofUs. All rights reserved.
 *
 */

#include "SharedObjectManager.h"

SharedObjectManager::SharedObjectManager(){
	isComputerVision = false;
	doDrawCameraInputs = false;
}

void SharedObjectManager::setup(){
	sharedObject.attach(KINECT_SHARE_PATH, false); 
	for(int i=0; i<3; i++){
		testImage[i].allocate(640, 480, OF_IMAGE_GRAYSCALE);
	}
	compositeImage.allocate(1440, 384, OF_IMAGE_GRAYSCALE);
	
	computerVision = &sharedObject.getObject().computerVision;
	computerVision->reloadSettings = true;
	
	cameraStitcher.setup(CAMERA_STITCHER_WIDTH, CAMERA_STITCHER_HEIGHT);
	for (int i=0; i<3; i++) {
		cameraStitcher.addCamera("Camera Warp " + ofToString(i), testImage[i].getTextureReference(), gui.getFolderPath()+"camera/");
	}
	cameraStitcher.pixels = sharedObject.getObject().inputImage.pixels;
	sharedObject.getObject().inputImage.width = CAMERA_STITCHER_WIDTH;
	sharedObject.getObject().inputImage.height = CAMERA_STITCHER_HEIGHT;
}

void SharedObjectManager::setupGUI(){
	
	
	gui.page(1).addPageShortcut(gui.addPage("Shared Object Manager"));
	//gui.addToggle("stitch cameras in cv", sharedObject.getObject().stitchInComputerVision);
	//gui.addToggle("stitch cameras in cv", computerVision->stitchInComputerVision);
	//gui.addToggle("draw camera stitcher", computerVision->doDrawCameraStitcher);
	gui.addToggle("draw camera inputs", doDrawCameraInputs);
	//gui.addToggle("draw preview image", doDrawCompositeImage);
	
	
	sharedObject.getObject().setupGlobalVariablesGUI();
	sharedObject.getObject().setupKinectSettingsGUI();
	cameraStitcher.setupGUI();
}

void SharedObjectManager::postGUI(){
	cameraStitcher.postGUI();
	computerVision->doDrawCameraStitcher = false;
	computerVision->stitchInComputerVision = true;
	computerVision->doOuterContours = false;
	computerVision->doOpFlow = false;
	computerVision->scale = 3;
	computerVision->shiftX = 0;
	computerVision->shiftY = 0;
	computerVision->reset_background = true;
	computerVision->currentSnapshotFrame = 0;
}

void SharedObjectManager::drawFbos(){
	if (computerVision->stitchInComputerVision == isComputerVision) {
		cameraStitcher.drawFbos();
	}
}

void SharedObjectManager::update(){
	if (computerVision->stitchInComputerVision == isComputerVision){
		// load camera images into textures
		//if (doDrawCameraInputs){
			for (int i=0; i<3; i++) {
				testImage[i].setFromPixels(sharedObject.getObject().pixels[i], 640, 480, OF_IMAGE_GRAYSCALE, true);
			}
		//}
		
		// update camera stitcher
		sharedObject.getObject().inputImage.width = CAMERA_STITCHER_WIDTH;
		sharedObject.getObject().inputImage.height = CAMERA_STITCHER_HEIGHT;
		cameraStitcher.update();
		
	}
}

void SharedObjectManager::draw(){
	
	if (computerVision->stitchInComputerVision == isComputerVision) {
		
		if(doDrawCameraInputs){
			ofPushStyle();
			ofEnableAlphaBlending();
			ofSetColor(computerVision->previewImageColor.r, computerVision->previewImageColor.g, computerVision->previewImageColor.b, computerVision->previewImageColor.a);
			for (int i=0; i<3; i++) {
				testImage[i].draw(i*320, 0,320,240);
			}
			ofPopStyle();
		}
		
		if (computerVision->shareImageId) {
			ofPushStyle();
			ofEnableAlphaBlending();
			ofSetColor(computerVision->previewImageColor.r, computerVision->previewImageColor.g, computerVision->previewImageColor.b, computerVision->previewImageColor.a);
			CompositeImage& sharedImage = sharedObject.getObject().compositeImage;
			if (compositeImage.width != sharedImage.width || compositeImage.height != sharedImage.height) {
				compositeImage.allocate(sharedImage.width, sharedImage.height, OF_IMAGE_GRAYSCALE);
			}
			compositeImage.setFromPixels(sharedImage.pixels, sharedImage.width, sharedImage.height, OF_IMAGE_GRAYSCALE);
			//		compositeImage.draw(computerVision->shiftX,computerVision->shiftY,sharedImage.width*computerVision->scale,sharedImage.height*computerVision->scale);compositeImage.draw(computerVision->shiftX,computerVision->shiftY,sharedImage.width*computerVision->scale,sharedImage.height*computerVision->scale);
			ofPushMatrix();
			ofScale(computerVision->scale, computerVision->scale, 1);
			compositeImage.draw(0,0);//-sharedImage.height*computerVision->scale
			ofPopMatrix();
			ofPopStyle();
		}
		
		if (computerVision->doDrawCameraStitcher) {
			ofPushStyle();
			ofEnableAlphaBlending();
			//ofSetColor(imageColor.r, imageColor.g, imageColor.b, imageColor.a);
			ofSetColor(computerVision->cameraStitcherColor.r, computerVision->cameraStitcherColor.g, computerVision->cameraStitcherColor.b, computerVision->cameraStitcherColor.a);
			cameraStitcher.draw(computerVision->shiftX,computerVision->shiftY,960*computerVision->scale,256*computerVision->scale); //<TODO: take out hardcoding
			ofPopStyle();
		}
		
	}
}