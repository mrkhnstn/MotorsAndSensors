/*
 ------------------------------------------------------------------------------------------------
 ofxKinectVision.cpp | ofxKinectVision | 26/01/2011
 
 created by MATT JAKOB - www.mattjakob.com
 Copyright 2011. All rights reserved.
 ------------------------------------------------------------------------------------------------
 */

#include "ofxKinectVision.h"
#include "ofxSimpleGuiToo.h"

GlobalVariables* KinectVision::global = NULL;



// kinect vision //////////////////////////////////////////////////

KinectVision::KinectVision(){
}

KinectVision::~KinectVision(){
	delete sharedObjectManager;
}

void KinectVision::setup(){
	
	sharedObjectManager = Singleton<SharedObjectManager>::instance();
	sharedObjectManager->setup();
	sharedObjectManager->isComputerVision = true;	
	sharedObject = &sharedObjectManager->sharedObject;
	
	global = &sharedObject->getObject().computerVision;
	innerManager = &innerManager_;//&sharedObject->getObject().innerManager;	
	
	//Blobs manager
	//innerManager = BlobManager();
	outerManager = BlobManager();
	
	doAllocation(960, 256); //TODO: take out hard coding
	
	global->currentSnapshotFrame = 0;
	
	clearBackground();
}

void KinectVision::setupGUI(){
	
	// setting up a specific gui page that allows to preview all cv steps
	
	ofxSimpleGuiPage& page_imgproc1 = gui.addPage("KV Preview");
	gui.page(1).addPageShortcut(page_imgproc1);
	
	sharedObjectManager->sharedObject.getObject().addPreviewImageCombobox();
	gui.addToggle("Speed Mode", global->speedMode);
	gui.addTitle("Camera feed");
	gui.addContent("Feed", grayImg);
	
	gui.addTitle("Colour pass");
	gui.addSlider("c_min", global->colourMin, 0, 255);
	gui.addSlider("c_max", global->colourMax, 0, 255);
	gui.addContent("result", rangeImg);
	
	gui.addTitle("Static subtract");
	gui.addToggle("Take snapshot", global->snap_background);
	//gui.addKeyboardShortcut('s', this, &KinectVision::takeBackgroundSnapshot);
	gui.addButton("Clear snapshot", global->reset_background);
	gui.addContent("Static Background",bg_staticImg);
	gui.addContent("result", subtractSImg);
	
	gui.addTitle("Smoothing");
	gui.addSlider("s_value",global->smooth,0,100);
	gui.addContent("result", smoothImg);
	
	gui.addTitle("Highpass");
	gui.addSlider("h_blur", global->highpassBlur, 0, 100);
	gui.addSlider("h_noise", global->highpassNoise, 0, 100);
	gui.addContent("result", highpassImg);
	
	gui.addTitle("Amplify");
	gui.addSlider("a_value", global->amplify, 0, 10);
	gui.addContent("result", amplifyImg);
	
	gui.addTitle("Treshold");
	gui.addSlider("t_value", global->treshold, 0, 255);
	gui.addContent("result", tresholdImg);
	
	//ofxSimpleGuiPage& page_imgproc2 = gui.addPage("Image processing 2");
	//gui.page(1).addPageShortcut(page_imgproc2);
	
	gui.addTitle("Dynamic subtract");	
	gui.addToggle("Dynamic learning", global->dynamic_learning);
	gui.addSlider("Learning speed", global->learning_speed, 1, 1000);
	gui.addContent("Dynamic background",bg_dynamicImg);
	gui.addContent("result",subtractDImg);
	
	gui.addTitle("Erode");
	gui.addSlider("e_value", global->erode,0,100);
	gui.addContent("result", erodeImg);
	gui.addTitle("Dilate");
	gui.addSlider("d_value", global->dilate, 0, 100);
	gui.addContent("result", dilateImg);
	
	gui.addTitle("Final image");
	gui.addContent("result", cvImg);
	
	sharedObjectManager->setupGUI();
}

void KinectVision::postGUI(){
	sharedObjectManager->postGUI();
}

void KinectVision::update(){
	
	sharedObjectManager->update();
	
	doSettingsUpdate(); //< reloads settings on bool flag
	
	if(global->reset_background)
		clearBackground();
		//setBackground(true); // clear static and dynamic background image (does not snapshot)
	
	//UPDATE ROUTINES
	//doSettings();
	
	doFrame();
	
	//IMAGE PROCESSING
	doFilters();
	
	//COMPUTER VISION
	doContours();
	
	//OPTICAL FLOW
	doOpticalFlow();
	
	sharedObjectManager->drawFbos();
	
	//SHARE DATA
	doShareData();
	
}

void KinectVision::doAllocation(int _camW, int _camH)
{
	camW = _camW;
	camH = _camH;
	
	//Image buffers allocation
	surface_fbo.allocate(camW,camH);
	//pixels = new unsigned char[camW*camH*4];
	innermaskImg.allocate(camW, camH);
	outermaskImg.allocate(camW, camH);
	
	//colorImg.allocate(camW, camH);
	cvImg.allocate(camW, camH);
	cvImg_prev.allocate(camW, camH);
	
	grayImg.allocate(camW, camH);
	
	innerImg.allocate(camW, camH);
	outerImg.allocate(camW, camH);
	
	bg_staticImg.allocate(camW, camH);
	bg_dynamicImg.allocate(camW, camH);
	
	floatImg.allocate(camW, camH);
	//tmp1Img.allocate(camW, camH, OF_IMAGE_GRAYSCALE); //ofImg
	tmp2Img.allocate(camW, camH);
	
	rangeImg.allocate(camW, camH);
	subtractSImg.allocate(camW, camH);
	subtractDImg.allocate(camW, camH);
	smoothImg.allocate(camW, camH);
	dilateImg.allocate(camW, camH);
	erodeImg.allocate(camW, camH);
	tresholdImg.allocate(camW, camH);
	highpassImg.allocate(camW, camH);
	amplifyImg.allocate(camW, camH);
	
	cvImg.set(0);
	cvImg_prev.set(0);
	
	opflow.setup(camW, camH);
}

void KinectVision::doSettingsUpdate(){
	if (global->reloadSettings) {
		gui.loadFromXML();
		global->reloadSettings = false;
	}
}

void KinectVision::doFrame()
{
	cvImg.setFromPixels(sharedObject->getObject().inputImage.pixels, sharedObject->getObject().inputImage.width, sharedObject->getObject().inputImage.height);
	cvImg.flagImageChanged();
}

void KinectVision::doFilter(int filter_name)
{
	float strech;
	switch (filter_name) {
			
		case FILTER_RANGE:
			cvInRangeS(cvImg.getCvImage(), cvScalar(global->colourMin), cvScalar(global->colourMax), tmp2Img.getCvImage());
			cvConvertScale(tmp2Img.getCvImage(), tmp2Img.getCvImage(), 1/255.0f, 0);
			cvMul(tmp2Img.getCvImage(),cvImg.getCvImage(),cvImg.getCvImage());
			/*
			 strech = 255.0f/(*colourMax-*colourMin);
			 cvConvertScale(cvImg.getCvImage(), cvImg.getCvImage(), strech, (-*colourMin * strech));
			 */
			//cvInRangeS(cvImg.getCvImage(),cvScalar(*colourMin), cvScalar(*colourMax), cvImg.getCvImage());
			if (!global->speedMode) rangeImg = cvImg;
			break;
			
		case FILTER_SUBTRACT_STATIC:
			tmp2Img = cvImg;
			tmp2Img.flagImageChanged();
			cvSub(cvImg.getCvImage(),bg_staticImg.getCvImage(),cvImg.getCvImage());
			cvImg.flagImageChanged();
			if (!global->speedMode) subtractSImg = cvImg;
			break;
			
		case FILTER_SUBTRACT_DYNAMIC:
			floatImg.addWeighted(cvImg, float(1.0f/(global->learning_speed)));
			cvConvertScale( floatImg.getCvImage(), bg_dynamicImg.getCvImage(), 255.0f/65535.0f, 0 ); 
			bg_dynamicImg.flagImageChanged();
			cvSub(cvImg.getCvImage(), bg_dynamicImg.getCvImage(), cvImg.getCvImage());
			if (!global->speedMode) subtractDImg = cvImg;
			break;
		
		case FILTER_SMOOTH:
			cvSmooth(cvImg.getCvImage(), cvImg.getCvImage(), CV_BLUR , int(ofMap(global->smooth, 0, 100, 0, 20))*2 + 1);
			if (!global->speedMode) smoothImg = cvImg;
			break;
		
		case FILTER_DILATE:
			if (global->dilate) cvDilate(cvImg.getCvImage(), cvImg.getCvImage(), NULL, int(ofMap(global->dilate, 0, 100, 0, 10)));
			if (!global->speedMode) dilateImg = cvImg;
			break;
		
		case FILTER_ERODE:
			if (global->erode) cvErode(cvImg.getCvImage(), cvImg.getCvImage(), NULL, int(ofMap(global->erode, 0, 100, 0, 10)));
			if (!global->speedMode) erodeImg = cvImg;
			break;
		
		case FILTER_THRESHOLD:
			cvThreshold(cvImg.getCvImage(), cvImg.getCvImage(), ofMap(global->treshold, 0, 100, 0, 255), 255, CV_THRESH_BINARY);
			if (!global->speedMode) tresholdImg = cvImg;
			break;
			
		case FILTER_HIGHPASS:
			//Blur Original Image
			cvSmooth( cvImg.getCvImage(), tmp2Img.getCvImage(), CV_BLUR , (global->highpassBlur * 2) + 1);
			tmp2Img.flagImageChanged();
			cvImg.flagImageChanged();
			//Original Image - Blur Image = Highpass Image
			cvSub( cvImg.getCvImage(), tmp2Img.getCvImage(), cvImg.getCvImage() );
			//Blur Highpass to remove noise
			cvSmooth(cvImg.getCvImage(), cvImg.getCvImage(), CV_BLUR , (global->highpassNoise * 2) + 1);
			if (!global->speedMode) highpassImg = cvImg;
			break;
			
		case FILTER_AMPLIFY:
			float scalef = global->amplify / 128.0f;
			cvMul(cvImg.getCvImage(), cvImg.getCvImage(), cvImg.getCvImage(), scalef);
			if (!global->speedMode) amplifyImg = cvImg;
			break;
			
	}
		
	cvImg.flagImageChanged();

	takeSnapShot(filter_name);
}

void KinectVision::doFilters()
{
	// Input
	if (!global->speedMode)
		grayImg = cvImg;
	
	takeSnapShot(FILTER_INPUT); //< copies cvImg into composite image if FILTER_INPUT 
	

	
	//GRAB SNAPSHOT OF THE BACKGROUND (for subtraction algorithm)
	if(global->snap_background || global->currentSnapshotFrame < global->snapshotFrame ){
		snapBackground();
		global->currentSnapshotFrame++;
	}
	
	doFilter(FILTER_SUBTRACT_STATIC);
	
	doFilter(FILTER_RANGE);
	
	//Smooth
	doFilter(FILTER_SMOOTH);
	
	//Highpass
	doFilter(FILTER_HIGHPASS);
	
	//Amplify
	doFilter(FILTER_AMPLIFY);
	
	//Treshold
	doFilter(FILTER_THRESHOLD);
	
	//Backgournd subtraction
	if(global->dynamic_learning)
		doFilter(FILTER_SUBTRACT_DYNAMIC);
	
	//Erode
	doFilter(FILTER_ERODE);
	
	//Dilate
	doFilter(FILTER_DILATE);
	
}

void KinectVision::clearBackground(){
	ofLog(OF_LOG_VERBOSE, "BACKGROUND: cleared");
	bg_staticImg.set(0);
	bg_dynamicImg.set(0);
	floatImg.set(0.0); 
	global->reset_background = false;
	bg_staticImg.flagImageChanged();
	bg_dynamicImg.flagImageChanged();
}

void KinectVision::snapBackground()
{
	/*
	if(clear)
	{
		ofLog(OF_LOG_VERBOSE, "BACKGROUND: cleared");
		bg_staticImg.set(0);
		bg_dynamicImg.set(0);
		floatImg.set(0.0); 
		global->reset_background = false;
		bg_staticImg.flagImageChanged();
		bg_dynamicImg.flagImageChanged();
	}
	else
	{
	 */
		//ofLog(OF_LOG_VERBOSE, "BACKGROUND: snapshot");
		//bg_staticImg = cvImg;
		cvMax(cvImg.getCvImage(), bg_staticImg.getCvImage(), bg_staticImg.getCvImage());
		//global->snap_background = false;
		bg_staticImg.flagImageChanged();
	//}
}

void KinectVision::doContours()
{
	if(global->doInnerContours){
		innermaskImg.set(1);
		//INNER
		cvMul(cvImg.getCvImage(),innermaskImg.getCvImage(),innerImg.getCvImage());
		innerImg.flagImageChanged();
		
		//Contour finders
		//innerFinder.findContours(innerImg, *innerMin, *innerMax, *innerMaxCount, false);
		innerFinder.findContours(cvImg, global->innerMin, global->innerMax, global->innerMaxCount, false);
		//Add objects to the manager
		innerManager->blobsCount = 0;
		for(int i =0; i<innerFinder.blobs.size(); i++)
			innerManager->add(&innerFinder.blobs[i], true);
		
		innerManager->update();	
	}

	if(global->doOuterContours){
		//OUTER
		cvMul(cvImg.getCvImage(),outermaskImg.getCvImage(),outerImg.getCvImage());
		outerImg.flagImageChanged();
		
		//Contour finders
		outerFinder.findContours(outerImg, global->outerMin, global->outerMax, global->outerMaxCount, false);
		
		//Add objects to the manager
		outerManager.blobsCount = 0;
		for(int i =0; i<outerFinder.blobs.size(); i++)
			outerManager.add(&outerFinder.blobs[i], true);
		
		outerManager.update();	
	}
}

void KinectVision::doOpticalFlow()
{
	if(global->doOpFlow)
	{
		opflow.calc(cvImg, cvImg_prev);
		opflow.filterPoints(global->minFlow, global->maxFlow);
		if(global->normalise)	
			opflow.normalizePoints();
		
		cvImg_prev = cvImg;
		cvImg_prev.flagImageChanged();
	}
}

void KinectVision::doShareData()
{
	updateAnchors(&(sharedObject->getObject().anchorManager));
}

void KinectVision::updateAnchors(AnchorManager* anchorManager)
{
	anchorManager->outerCount  = outerManager.blobsCount;
	anchorManager->innerCount  = innerManager->blobsCount;
	anchorManager->opflowCount = opflow.flowPoints.size() < KINECT_MAX_CORNERS ? opflow.flowPoints.size() : KINECT_MAX_CORNERS;
	
	for(int i=0; i<anchorManager->outerCount; i++)
	{
		anchorManager->outer[i].id = outerManager.blobs[i].id;
		anchorManager->outer[i].life = outerManager.blobs[i].life;
		anchorManager->outer[i].x = outerManager.blobs[i].x;
		anchorManager->outer[i].y = outerManager.blobs[i].y;
		anchorManager->outer[i].x_world = outerManager.blobs[i].x_world;
		anchorManager->outer[i].y_world = outerManager.blobs[i].y_world;
		anchorManager->outer[i].w = outerManager.blobs[i].w;
		anchorManager->outer[i].h = outerManager.blobs[i].h;
		anchorManager->outer[i].w_world = outerManager.blobs[i].w_world;
		anchorManager->outer[i].h_world = outerManager.blobs[i].h_world;	
		
		anchorManager->outer[i].state = outerManager.blobs[i].state;
		anchorManager->outer[i].ghost = outerManager.blobs[i].ghost;
	}
	
	for(int i=0; i<anchorManager->innerCount; i++)
	{
		anchorManager->inner[i].id = innerManager->blobs[i].id;
		anchorManager->inner[i].life = innerManager->blobs[i].life;
		anchorManager->inner[i].x = innerManager->blobs[i].hand.x;
		anchorManager->inner[i].y = innerManager->blobs[i].hand.y;
		anchorManager->inner[i].x_world = innerManager->blobs[i].hand.x_world;
		anchorManager->inner[i].y_world = innerManager->blobs[i].hand.y_world;
		anchorManager->inner[i].w = innerManager->blobs[i].hand.w;
		anchorManager->inner[i].h = innerManager->blobs[i].hand.h;
		anchorManager->inner[i].w_world = innerManager->blobs[i].hand.w_world;
		anchorManager->inner[i].h_world = innerManager->blobs[i].hand.h_world;
		anchorManager->inner[i].closestBorder = innerManager->blobs[i].hand.closest_border;
		anchorManager->inner[i].state = innerManager->blobs[i].state;
		anchorManager->inner[i].ghost = innerManager->blobs[i].ghost;
	}
	
	for(int i=0; i<anchorManager->opflowCount; i++)
	{
		anchorManager->opflow[i].id = i;
		anchorManager->opflow[i].life = 0;
		anchorManager->opflow[i].x = opflow.flowPoints[i].x;
		anchorManager->opflow[i].y = opflow.flowPoints[i].y;
		anchorManager->opflow[i].x_world = opflow.flowPoints[i].x *global->scale;;
		anchorManager->opflow[i].y_world = opflow.flowPoints[i].y *global->scale;;
		anchorManager->opflow[i].w = 0;
		anchorManager->opflow[i].h = 0;
		anchorManager->opflow[i].w_world = 0;
		anchorManager->opflow[i].h_world = 0;	
	}
}

void KinectVision::takeBackgroundSnapshot(ofEventArgs& e){
	takeBackgroundSnapshot();
}

void KinectVision::takeBackgroundSnapshot(){
	global->snap_background = true;
}

void KinectVision::takeSnapShot(int filter_name)
{
	if(global->shareImageId-1 == filter_name){
		CompositeImage& compositeImage = sharedObject->getObject().compositeImage;
		compositeImage.width = cvImg.getWidth();
		compositeImage.height = cvImg.getHeight();
		memcpy(compositeImage.pixels, cvImg.getPixels(), compositeImage.width * compositeImage.height * sizeof(unsigned char));
	}
}

void KinectVision::draw()
{
	ofBackground(128, 128, 128);
	ofSetColor(255,255,255);

	// draw composite image / camera stitcher
	ofPushMatrix();
	ofScale(1/global->scale, 1/global->scale, 1);
	sharedObjectManager->draw();
	ofPopMatrix();
	
	//Objects in camera-space units
	ofPushMatrix();
	
	ofPushStyle();
	ofEnableAlphaBlending();
	
	if(global->hud_tablesurface)
		(global->table).draw();
	

	
	if (global->hud_blobsContours) {
		//Blob *iB;
		//Blob *oB;
		ofSetColor(255, 255, 0);
		ofNoFill();
		for(int i=0; i<innerManager->blobsCount; i++)
		{
			Blob *iB = &innerManager->blobs[i];
			if (iB->state != BLOB_GHOST) //Do not draw ghost blobs
				iB->drawContour();
		}
		/* // draw people
		ofSetColor(255, 0, 0);
		ofNoFill();
		for(int i=0; i<outerManager.blobsCount; i++)
		{
			oB = &outerManager.blobs[i];
			if (oB->state != BLOB_GHOST) //Do not draw ghost blobs
				oB->drawContour();
		}
		*/
	}
	

	/*
	if(global->hud_parameters)
	{
		ofSetColor(0, 0, 255);
		ofFill();
		Hand* h;
		for(int i=0; i<innerManager->blobsCount; i++)
		{
			h = &(innerManager->blobs[i]).hand;
			if ((innerManager->blobs[i]).state != BLOB_GHOST) //Do not draw ghost blobs
				h->drawContour();
		}
	}	
	*/
	
	if(global->hud_hands)
		innerManager->drawHands();
	
	if (global->hud_handsInfo) {
		innerManager->drawHandsInfo();
	}
	
	
	/*
	if (global->hud_people) {
		Blob *oB;
		ofSetColor(255, 0, 255,180);
		ofFill();
		
		for(int i=0; i<outerManager.blobsCount; i++)
		{
			oB = &outerManager.blobs[i];
			oB->drawBox();
		}
	}
	
	if(global->doOpFlow)
		opflow.draw();
	*/
	
	//HUD ON TABLE
	//glTranslatef(global->table.corners[2].x-80, global->table.corners[2].y-20,0);
	if(global->hud_hands)
		innerManager->drawCounts();
	
	/*
	glTranslatef(0, -32 ,0);
	if(global->hud_parameters)
		innerManager->drawParameters(); 
	*/
	
	ofPopStyle();
	ofPopMatrix();
	

	
}

void KinectVision::exit()
{
	
}

