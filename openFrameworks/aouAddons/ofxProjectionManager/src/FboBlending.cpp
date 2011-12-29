/*
 *  FboBlending.cpp
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 16/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "FboBlending.h"
#include "FboWarpOutput.h"

FboBlending::FboBlending(){
	doDraw = false;
	backgroundImage = NULL;
	parentName = "Projection Manager";
}

void FboBlending::setup(ofRectangle window, float borderH, float borderV){
	top.width = borderV;
	bottom.width = borderV;
	left.width = borderH;
	right.width = borderH;
	redraw(window);
	
	gammaTop = 0;
	gammaBottom = window.height;
	gammaLeft = 0;
	gammaRight = window.width;
	
	_gammaTop = 0;
	_gammaBottom = 0;
	_gammaLeft = 0;
	_gammaRight = 0;
	
}

void FboBlending::setBackgroundImage(ofImage* bg){
	this->backgroundImage = bg;
	backgroundUpdated();
}

void FboBlending::backgroundUpdated(){
	update(true);
}

void FboBlending::redraw(ofRectangle window){
	rect = window;
	//don't resize the edges!
	top.position.x = window.x + window.width*0.5;
	top.position.y = window.y;
	top.setAngle(PI*1.5);
	top.height = window.width;
	_top = top;
	
	bottom.position.x = window.x + window.width*0.5;
	bottom.position.y = window.y + window.height;
	bottom.setAngle(PI*0.5);
	bottom.height = window.width;
	_bottom = bottom;
	
	left.position.x = window.x;
	left.position.y = window.y + window.height*0.5;
	left.setAngle(PI);
	left.height = window.height;
	_left = left;
	
	right.position.x = window.x + window.width;
	right.position.y = window.y + window.height*0.5;
	right.setAngle(0);
	right.height = window.height;
	_right = right;
	
	blendImage.allocate(window.width, window.height, OF_IMAGE_COLOR_ALPHA);
	update(true);
}

void FboBlending::draw(FboWarpOutput* output){
	if(doDraw){
		ofPushStyle();
		ofSetColor(output->edgeColor.r, output->edgeColor.g, output->edgeColor.b);
		_top.draw();
		_bottom.draw();
		_left.draw();
		_right.draw();
		ofPopStyle();
	}
}

void FboBlending::update(bool forceUpdate){
	bool modified = forceUpdate;
	
	if(_gammaTop != gammaTop || _gammaBottom != gammaBottom || _gammaLeft != gammaLeft || _gammaRight != gammaRight){
	_gammaTop = gammaTop;
	_gammaBottom = gammaBottom;
	_gammaLeft = gammaLeft;
	_gammaRight = gammaRight;
		modified = true;
	}
	
	if(top.gammaR.g != gammaR || top.gammaG.g != gammaG || top.gammaB.g != gammaB){
		top.setGamma(gammaR, gammaG, gammaB);
		bottom.setGamma(gammaR, gammaG, gammaB);
		left.setGamma(gammaR, gammaG, gammaB);
		right.setGamma(gammaR, gammaG, gammaB);
		modified = true;
	}
	if(!(_top == top)){
		_top = (top);
		modified = true;
	}
	if(!(_bottom == bottom)){
		_bottom = (bottom);
		modified = true;
	}
	if(!(_left == left)){
		_left = (left);
		modified = true;
	}
	if(!(_right == right)){
		_right = (right);
		modified = true;
	}
	
	if(modified){
		buildImage();
		//rebuild blending image!
	}
}

void FboBlending::buildImage(){
	//TODO: do all of this on graphics card without having to read it back
	
	
	if(backgroundImage != NULL){
		/*
		 bool valid =
		 containsRect(ofRectangle(0, 0, backgroundImage->width, backgroundImage->height), rect) &&
		 blendImage.height >= rect.height && blendImage.width >= rect.width;
		 */
		
		float srcX = ofClamp(rect.x, 0, backgroundImage->width-1);
		float srcY = ofClamp(rect.y, 0, backgroundImage->height-1);
		float srcX2 = ofClamp(rect.x + rect.width, 0, backgroundImage->width-1);
		float srcY2 = ofClamp(rect.y + rect.height, 0, backgroundImage->height-1);
		float srcWidth = srcX2 - srcX;
		float srcHeight = srcY2 - srcY;
		bool valid = (srcWidth > 0) && (srcHeight > 0);
		
		if(valid){ 
			ImageUtil::copyPixel(*backgroundImage, srcX, srcY, srcWidth, srcHeight, blendImage, 0, 0, false);
			//ImageUtil::copyPixel(*backgroundImage, rect.x, rect.y, rect.width, rect.height, blendImage, 0, 0, false);
			ImageUtil::flipVertically(blendImage);
		} else {
			unsigned char color[4];
			color[0] = 255;
			color[1] = 255;
			color[2] = 255;
			color[3] = 255;
			ImageUtil::setPixels(blendImage.getPixels(), blendImage.width, blendImage.height, 0, 0, blendImage.width, blendImage.height, color);
		}
	} else {
		
		unsigned char color[4];
		color[0] = 255;
		color[1] = 255;
		color[2] = 255;
		color[3] = 255;
		ImageUtil::setPixels(blendImage.getPixels(), blendImage.width, blendImage.height, 0, 0, blendImage.width, blendImage.height, color);
	}
	
	/*
	 unsigned char color[4];
	 color[0] = 255;
	 color[1] = 255;
	 color[2] = 255;
	 color[3] = 255;
	 
	 ImageUtil::setPixels(blendImage.getPixels(), blendImage.width, blendImage.height, 0, 0, blendImage.width, blendImage.height, color);
	 */
	
	buildEdge(_top);
	buildEdge(_bottom);
	buildEdge(_left);
	buildEdge(_right);
	
	buildLevelsMap();
	
	blendImage.update();
}

void FboBlending::buildEdge(FboBlendEdge& edge){
	if(edge.enabled){
		
		unsigned char color[4];
		
		unsigned char* pixels = blendImage.getPixels();
		
		ofPoint point;
		for(int j = 0; j < blendImage.height; ++j){
			for(int i = 0; i < blendImage.width; ++i){
				point.x = rect.x + i;
				point.y = rect.y + blendImage.height - j - 1;
				ImageUtil::getPixel(blendImage, i, j, color);
				
				//float mul = edge.gammaBlend(point);
				float b = edge.blend(point);
				/*
				color[0] *= edge.gammaR.igammaNormalized(b);
				color[1] *= edge.gammaG.igammaNormalized(b);
				color[2] *= edge.gammaB.igammaNormalized(b);
				*/
				
				color[0] *= b;//edge.gammaR.igammaNormalized(b);
				color[1] *= b;//edge.gammaG.igammaNormalized(b);
				color[2] *= b;//edge.gammaB.igammaNormalized(b);
				//color[3] = 1;
				//color[3] *= mul;
				
				ImageUtil::setPixel(pixels, i, j, blendImage.width, blendImage.height, color);
			}
		}
	}
}

void FboBlending::buildLevelsMap(){
	unsigned char color[4];	
	unsigned char* pixels = blendImage.getPixels();
	
	for(int j = 0; j < blendImage.height; ++j){
		for(int i = 0; i < blendImage.width; ++i){			
			ImageUtil::getPixel(blendImage, i, j, color);
			color[3] = (i >= gammaLeft && i <= gammaRight && j >= gammaTop && j <= gammaBottom) ? 255 : 0;			
			ImageUtil::setPixel(pixels, i, j, blendImage.width, blendImage.height, color);
		}
	}
	
}

void FboBlending::setupGUI(string subname){
	//gui.page(parentName).addPageShortcut(gui.addPage(subname+" Edge Blending"));
	
	gui.addTitle("levels rect");
	gui.addSlider("left", gammaLeft, 0, 1920);
	gui.addSlider("right", gammaRight, 0, 1920);
	gui.addSlider("top", gammaTop, 0, 1200);
	gui.addSlider("bottom", gammaBottom, 0, 1200);
	
	gui.addTitle("Edge Blending");
	gui.addToggle("Draw", doDraw);
	//gui.addTitle("Blending");
	//gui.addSlider("GammaR", gammaR, 1, 3);
	//gui.addSlider("GammaG", gammaG, 1, 3);
	//gui.addSlider("GammaB", gammaB, 1, 3);
	
	top.setupGUI("Top");
	bottom.setupGUI("Bottom");
	left.setupGUI("Left");
	right.setupGUI("Right");
	gui.addContent("BlendImage", blendImage, 256);
	//gui.addSlider("Shape", shape, 1, 5);
	//gui.addSlider("BlendPoint", blendPoint, 0, 1);
	//gui.addToggle("Enable Top", enableT);
	//gui.addToggle("Enable Bottom", enableB);
	//gui.addToggle("Enable Left", enableL);
	//gui.addToggle("Enable Right", enableR);
}