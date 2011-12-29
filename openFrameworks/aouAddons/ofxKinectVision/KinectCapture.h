#pragma once
/*
 *  KinectCapture.h
 *  TestBed_SharedMemory
 *
 *  Created by Mark Hauenstein on 03/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "ofxKinect.h"
#include "ofxSharedObject.h"
#include "KinectShare.h"
#include "ofxValueObserverT.h"
#include "ofxOpenCv.h"
#include "SharedObjectManager.h"
#include "Singleton.h"
#include "ofxKinectRecorder.h"
#include "ofxKinectPlayer.h"
#include <exception>

#define KINECT_RECORD_FRAMES 800 //< number of frames that can be recorded into RAM
	
	/**
	 - records and playsback kinect depth frame from an array (located in RAM)
	 - the array can be saved and loaded to disc
	 */
	
	class KinectRecorder : public ofxBaseObject {
	
	public:
		
		unsigned char* pixels;
		
		int captureFrame;
		int playFrame;
		bool isSetup;
		bool doPlay;
		bool doStepFrame;
		
		KinectRecorder(){
			isSetup = false;
			captureFrame = 0;
			playFrame = 0;
			doPlay = false;
			doStepFrame = false;
		}
		
		~KinectRecorder(){
			if (isSetup) {
				delete[] pixels;
			}
		}
		
		void setup(){
			if (!isSetup) {
				ofLog(OF_LOG_VERBOSE, "KinectRecorder::setup()");
				captureFrame = 0;
				playFrame = 0;
				pixels = new unsigned char[KINECT_RECORD_FRAMES*KINECT_WIDTH*KINECT_HEIGHT];
				isSetup = true;
			}
		}
			
		void save(string filename){
			if (!isSetup) setup();
			FILE* f = fopen(ofToDataPath(filename).c_str(),"wb");
			fwrite(pixels,KINECT_RECORD_FRAMES*KINECT_WIDTH*KINECT_HEIGHT,1,f);
			fclose(f);
		}
		
		void load(string filename){
			if (!isSetup) setup();
			
				FILE* f = fopen(ofToDataPath(filename).c_str(), "rb");
				if (f != NULL) {
					long lSize = ftell (f);
					//if (lSize == KINECT_RECORD_FRAMES*KINECT_WIDTH*KINECT_HEIGHT) {
						fread(pixels,KINECT_RECORD_FRAMES*KINECT_WIDTH*KINECT_HEIGHT,1,f);
					//} else {
					//	ofLog(OF_LOG_ERROR, "KinectRecorder::load >> size of file does not match array size (lsize: " + ofToString((int)lSize) + " array size:" + ofToString(KINECT_RECORD_FRAMES*KINECT_WIDTH*KINECT_HEIGHT));
					//}
					fclose(f);
				} else {
					ofLog(OF_LOG_ERROR, "KinectRecorder::load >> couldn't open file");
				}

		}
		
		void recordFrame(unsigned char * recordPixels){
			if (!isSetup) setup();
			memcpy(pixels+(captureFrame*KINECT_WIDTH*KINECT_HEIGHT),recordPixels,KINECT_WIDTH*KINECT_HEIGHT);
			captureFrame++;
			if (captureFrame >= KINECT_RECORD_FRAMES) {
				captureFrame = 0;
			}
		}
		
		unsigned char * getPixels(){
			if (!isSetup) setup();
			if(doPlay || doStepFrame){
				playFrame++;
				if (playFrame >= KINECT_RECORD_FRAMES) {
					playFrame = 0;
				}
				doStepFrame = false;
			}
			return pixels+(playFrame*KINECT_WIDTH*KINECT_HEIGHT);
		}
		
	};
	
	// Kinect Capture ///////////////////////////////////////////////////////
	
	#define KinectCapture_CAPTURE 0
	#define	KinectCapture_RECORD 1
	#define KinectCapture_PLAYBACK 2
	
	/**
	 captures depth images from one kinect and copies them to shared memory
	 */
	class KinectCapture : public ofxBaseObject {
		public:
		
		int kinectId;
		
		float* cameraAngle;
		float* nearClipping;
		float* farClipping;
		//bool enableDepthNearValueWhite;
		
		ofxValueObserverT<float> cameraAngleObserver;		
		ofxValueObserverT<float> nearClippingObserver;
		ofxValueObserverT<float> farClippingObserver;		
		//ofxValueObserverT<bool> nearWhiteObserver;
		
		ofxKinect kinect;
		//ofxSharedObject<KinectShare> sharedObject;
		SharedObjectManager*		sharedObjectManager;
		
		bool doDraw;
		ofImage debugImage;
		bool clippingChanged;
		bool cameraAngleChanged;
		
		bool doHideWindow;
		bool doShowWindow;
		
		int counter;
		bool hidden;
		
		ofxCvGrayscaleImage captureImage[2];
		int activeCapture; // ping pong index for captureImage
		
		ofxCvGrayscaleImage bufferedImage;
		
		ofxCvGrayscaleImage blackPlateImage;
		ofxCvGrayscaleImage bgPlateImage;
		
		//ofxKinectRecorder	kinectRecorder;
		//ofxKinectPlayer		kinectPlayer;
		bool				doRecord;
		bool				doPlay;
		bool				_doPlay;
		
		bool mirrorX;
		bool mirrorY;
		
		KinectRecorder recorder;
		
		bool doSave;
		bool doLoad;
		
		string recordFilename;
		
		bool doCapture;//
		bool doCapture_;
		
		bool doDoubleBuffer;//
		
		KinectCapture(){
			kinectId = 0;

			/*
			cameraTileAngle = 0;
			nearClipping = 26;
			farClipping = nearClipping + 255;
			//enableDepthNearValueWhite = true;
			*/
			
			doHideWindow = false;
			doShowWindow = false;
			counter = 0;
			hidden = false;
			doDoubleBuffer = true;
			
			for (int i=0; i<2; i++) {
				captureImage[i].allocate(640, 480);
				captureImage[i].setUseTexture(false);
				cvSet(captureImage[i].getCvImage(),cvScalar(0));
				captureImage[i].flagImageChanged();
			}			
			
			activeCapture = 0;
			
			bufferedImage.allocate(640,480);
			bufferedImage.setUseTexture(false);
			cvSet(bufferedImage.getCvImage(),cvScalar(0));
			bufferedImage.flagImageChanged();

			bgPlateImage.allocate(640, 480);
			cvSet(bgPlateImage.getCvImage(),cvScalar(0));
			bgPlateImage.flagImageChanged();
			
			
			blackPlateImage.allocate(640, 480);
			cvSet(blackPlateImage.getCvImage(),cvScalar(0));
			blackPlateImage.flagImageChanged();
			
			/*
			cvSub(bgPlateImage.getCvImage(), blackPlateImage.getCvImage(), blackPlateImage.getCvImage());
			blackPlateImage.flagImageChanged();
			*/
			
			doRecord = false;
			doPlay = false;
			_doPlay = false;
			
			recordFilename = "kinectTest.kct";
			
			mirrorX = false;
			mirrorY = false;
			
			doCapture = true;
			doCapture_ = doCapture;
		}
		
		void setup(){
			// attach shared memory
			//sharedObject.attach(KINECT_SHARE_PATH, false); // setting up as client share
			
			sharedObjectManager = Singleton<SharedObjectManager>::instance();
			sharedObjectManager->setup();
			
			nearClipping = &sharedObjectManager->sharedObject.getObject().kinectSettings[kinectId].nearClipping;
			farClipping = &sharedObjectManager->sharedObject.getObject().kinectSettings[kinectId].farClipping;
			cameraAngle = &sharedObjectManager->sharedObject.getObject().kinectSettings[kinectId].cameraAngle;
			
			nearClippingObserver.setup(*nearClipping,this,&KinectCapture::onClippingChanged);
			farClippingObserver.setup(*farClipping,this,&KinectCapture::onClippingChanged);
			cameraAngleObserver.setup(*cameraAngle,this,&KinectCapture::onCameraAngleChanged);
			// start kinect
			kinect.init(false,false);
			kinect.setVerbose(false);
			kinect.open(kinectId); 
			
			debugImage.allocate(KINECT_WIDTH, KINECT_HEIGHT, OF_IMAGE_GRAYSCALE);
			
			//createMenu();
			
			//recorder.setup();
		}
		
		void setupGUI(){
			gui.page(1).addPageShortcut(gui.addPage("kinect_"+ofToString(kinectId)));
			gui.addTitle("kinect capture");
			gui.addDebug("kinect id",kinectId);
			gui.addToggle("capture enabled", doCapture);
			gui.addToggle("double buffer", doDoubleBuffer);
			gui.addToggle("mirror x", mirrorX);
			gui.addToggle("mirror y", mirrorY);
			gui.addToggle("draw", doDraw);
			gui.addToggle("record", doRecord);
			gui.addToggle("use file", doPlay);
			gui.addToggle("play file", recorder.doPlay);
			gui.addButton("step file", recorder.doStepFrame);
			gui.addDebug("capture frame", recorder.captureFrame);
			gui.addDebug("play frame", recorder.playFrame);
			gui.addTextInput("filename", recordFilename);
			gui.addToggle("save", doSave);
			gui.addToggle("load", doLoad);
			sharedObjectManager->sharedObject.getObject().setupKinectSettingsGUI();
		}
		
		/*
		void hideWindow(ofEventArgs& e){
			doHideWindow = true;
		}
		
		void showWindow(ofEventArgs& e){
			doShowWindow = true;
			ofLog(OF_LOG_VERBOSE, "show");
		}
		*/
		
		void postGUI(){
			doDraw = false;
			doRecord = false;
			doPlay = false;
		}
		
		void update(){
			
			// minimize window after 10 frames
			if (ofGetFrameNum() == 10) {
				glutIconifyWindow();
			}
			
			if(doCapture != doCapture_){
				doCapture_ = doCapture;
				if (doCapture) {
					kinect.open(kinectId); 
				} else {
					kinect.close();
				}
			}
			
			if (clippingChanged) {
				//ofxKinectCalibration::setClippingInCentimeters(nearClipping,farClipping);
				ofxKinectCalibration::setClippingRaw(*nearClipping,*farClipping);
				clippingChanged = false;
			}
			
			
			if (!doPlay) {
				
				if (cameraAngleChanged) {
					kinect.setCameraTiltAngle(*cameraAngle);
					cameraAngleChanged = false;
				}
				
				kinect.update();
				
				captureImage[activeCapture].setFromPixels(kinect.getDepthPixels(), KINECT_WIDTH, KINECT_HEIGHT);
				
				if(doRecord){
					recorder.setup();
					recorder.recordFrame(kinect.getDepthPixels());
				}				
			
			} else {
				recorder.setup();
				captureImage[activeCapture].setFromPixels(recorder.getPixels(), KINECT_WIDTH, KINECT_HEIGHT);
			}
			
			captureImage[activeCapture].mirror(mirrorX, mirrorY);
			
			if(doDoubleBuffer){
				cvMax(captureImage[0].getCvImage(), captureImage[1].getCvImage(), bufferedImage.getCvImage());
				//bufferedImage.scaleIntoMe(captureImage, <#int interpolationMethod#>)
				// copy pixels from kinect into shared memory
				memcpy(sharedObjectManager->sharedObject.getObject().pixels[kinectId],bufferedImage.getPixels(),KINECT_WIDTH*KINECT_HEIGHT*KINECT_CHANNELS);			
			} else {
				memcpy(sharedObjectManager->sharedObject.getObject().pixels[kinectId],captureImage[activeCapture].getPixels(),KINECT_WIDTH*KINECT_HEIGHT*KINECT_CHANNELS);			
			
			}
			if(doSave){
				recorder.setup();
				recorder.save(recordFilename);
				doSave = false;
			}
			
			if (doLoad) {
				recorder.setup();
				recorder.load(recordFilename);
				doLoad = false;
			}
			
			activeCapture = !activeCapture;
			
			//cvMax(captureImage[activeCapture].getCvImage(), bgPlateImage.getCvImage(), bgPlateImage.getCvImage());
			//bgPlateImage.flagImageChanged();
		}
		
		void draw(){
			
			//bgPlateImage.draw(640, 0);
			
			if(doDraw){
				
				
				
				// need to copy kinect pixels to debugImage pixels as texture of kinect switched off for performance reason
				/*
				if (!doPlay) {
					debugImage.setFromPixels(kinect.getDepthPixels(), KINECT_WIDTH, KINECT_HEIGHT, OF_IMAGE_GRAYSCALE, true);
				} else {
					recorder.setup();
					debugImage.setFromPixels(recorder.getPixels(), KINECT_WIDTH, KINECT_HEIGHT, OF_IMAGE_GRAYSCALE, true);
				}
				*/
				debugImage.setFromPixels(sharedObjectManager->sharedObject.getObject().pixels[kinectId], KINECT_WIDTH, KINECT_HEIGHT, OF_IMAGE_GRAYSCALE, true);
				debugImage.draw(0, 0);

			}
		}
		
		void onClippingChanged(ofEventArgs& e){
			clippingChanged = true;
		}
		
		void onCameraAngleChanged(ofEventArgs& e){
			cameraAngleChanged = true;
		}

		
	};