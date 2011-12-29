#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0,0,0);
	cam.initGrabber(CAM_W, CAM_H);
	camImg.allocate(CAM_W, CAM_H);
	warpedImg.allocate(CAM_W, CAM_H);
	warper.setup(&camImg, &warpedImg);
}

//--------------------------------------------------------------
void testApp::update(){
	cam.grabFrame();
	if(cam.isFrameNew()){
		camImg.setFromPixels(cam.getPixels(), CAM_W, CAM_H);
		warper.warp();
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	camImg.draw(20, 60, 320, 240);
	warper.draw(20, 60, 320, 240);
	warpedImg.draw(360, 60, 320, 240);
	string info = "press 's' to save\npress'l' to load";
	ofDrawBitmapString(info, 20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key == 's')warper.save();
	if(key == 'l')warper.load();
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

