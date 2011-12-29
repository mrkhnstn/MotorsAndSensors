/* 
 * Copyright (c) 2009, Rui Madeira
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * http://creativecommons.org/licenses/LGPL/2.1/
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"


#define OFX_CV_EASY_WARPER_DEFAULT_FILE_NAME "ofxCvEasyWarperSettings.xml"
#define CORNER_RADIUS 15

class ofxCvEasyWarper{
protected:
	ofxCvImage*sourceImg, *destImg;
	ofPoint imgOffSet;
	int cornerHitId;
	ofPoint sourcePts[4], destPts[4];
	string fileName;
	float imageWidth, imageHeight;
	bool bEventsAdded;
	float scaleX, scaleY;
	ofxXmlSettings xml;
	void onMousePressed(ofMouseEventArgs& mouseArgs);
	void onMouseReleased(ofMouseEventArgs& mouseArgs);
	void onMouseDragged(ofMouseEventArgs& mouseArgs);
	int checkCornerHit(float x, float y);
	bool bEnabled;
public:
	ofxCvEasyWarper();
	
	void setup(ofxCvImage*_sourceImg, ofxCvImage*_destImg);
	
	void warp();
	void draw(float x=0, float y=0);
	void draw(float x, float y, float w, float h);
	
	void save();
	void load();
	
	void reset();
	void enable();
	void disable();
	
	//normally you shouldn't have to mess with these.
	//internally it uses mouse events, use this if you want to remove these events
	void removeEvents();
	//use this if after removing the events you wish to add them again
	void addEvents();
	
	//if you want to specify a file name to save the settings
	//otherwise its defaulted to the DEFAULT_FILE_PATH defined above
	void setFileName(string _file = OFX_CV_EASY_WARPER_DEFAULT_FILE_NAME);
};



