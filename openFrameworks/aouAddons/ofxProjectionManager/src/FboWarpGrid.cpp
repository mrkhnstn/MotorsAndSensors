/*
 *  FboWarpGrid.cpp
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 24/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "FboWarpGrid.h"

void FboWarpGrid::setup(ofRectangle window, ofRectangle src, ofRectangle dst, int cols, int rows){
	//		this->offsetX = offsetX;
	//		this->offsetY = offsetY;
	//		this->width = width;
	//		this->height = height;
	this->window = window;
	this->src = src;
	this->dst = dst;
	this->rows = rows;
	this->cols = cols;
	reset();
}

void FboWarpGrid::reset(){
	radialAmount = 0;
	radialInverseAmount = 0;
	radialCenterOffsetX = 0;
	radialCenterOffsetY = 0;
	radialScale = 1;
	radialBias = 0;
	
	shift.set(0, 0);
	stretchTL.set(0, 0);
	stretchTR.set(0, 0);
	stretchBR.set(0, 0);
	stretchBL.set(0, 0);
	
	buildGrids(true, true, true);
}

bool FboWarpGrid::isValidPoint(int index){
	return index >= 0 && index < targetGrid.size();
}

bool FboWarpGrid::operator== (FboWarpGrid& b){
	return
	rows == b.rows && cols == b.cols &&
	compareRect(src, b.src) && compareRect(dst, b.dst) && compareRect(window, b.window) &&
	//width == b.width && height == b.height &&
	//offsetX == b.offsetX && offsetY == b.offsetY &&
	radialAmount == b.radialAmount &&
	radialInverseAmount == b.radialInverseAmount &&
	radialCenterOffsetX == b.radialCenterOffsetX &&
	radialCenterOffsetY == b.radialCenterOffsetY &&
	radialScale == b.radialScale &&
	radialBias == b.radialBias &&
	shift == b.shift &&
	stretchTL ==b.stretchTL && stretchTR == b.stretchTR && stretchBR == b.stretchBR && stretchBL == b.stretchBL && 
	equal(originalGrid.begin(), originalGrid.end(), b.originalGrid.begin()) &&
	equal(targetGrid.begin(), targetGrid.end(), b.targetGrid.begin()) &&
	equal(warpGrid.begin(), warpGrid.end(), b.warpGrid.begin());
}

void FboWarpGrid::copyFrom(FboWarpGrid& b){
	rows = b.rows;
	cols = b.cols;
	src = b.src;
	dst = b.dst;
	window = b.window;
	//width = b.width;
	//height = b.height;
	//offsetX = b.offsetX;
	//offsetY = b.offsetY;
	
	originalGrid.clear();
	originalGrid = b.originalGrid;
	
	targetGrid.clear();
	targetGrid = b.targetGrid;
	
	warpGrid.clear();
	warpGrid = b.warpGrid;
	
	
	shift = b.shift;
	stretchTL = b.stretchTL;
	stretchTR = b.stretchTR;
	stretchBL = b.stretchBL;
	stretchBR = b.stretchBR;
	
	
	radialAmount = b.radialAmount;
	radialInverseAmount = b.radialInverseAmount;
	radialCenterOffsetX = b.radialCenterOffsetX;
	radialCenterOffsetY = b.radialCenterOffsetY;
	radialScale = b.radialScale;
	radialBias = b.radialBias;
}

void FboWarpGrid::saveXML(string path){
	ofxXmlSettings xml;
	
	xml.addValue("rows", rows);
	xml.addValue("cols", cols);
	xml.addValue("srcX", src.x);
	xml.addValue("srcY", src.y);
	xml.addValue("srcW", src.width);
	xml.addValue("srcH", src.height);
	
	xml.addValue("dstX", dst.x);
	xml.addValue("dstY", dst.y);
	xml.addValue("dstW", dst.width);
	xml.addValue("dstH", dst.height);
	
	xml.addValue("windowX", window.x);
	xml.addValue("windowY", window.y);
	xml.addValue("windowW", window.width);
	xml.addValue("windowH", window.height);
	
	//xml.addValue("width", width);
	//xml.addValue("height", height);
	//xml.addValue("offsetX", offsetX);
	//xml.addValue("offsetY", offsetY);
	
	xml.addValue("shift", stringifyVec2f(shift));
	xml.addValue("stretchTL", stringifyVec2f(stretchTL));
	xml.addValue("stretchTR", stringifyVec2f(stretchTR));
	xml.addValue("stretchBL", stringifyVec2f(stretchBL));
	xml.addValue("stretchBR", stringifyVec2f(stretchBR));
	
	xml.addValue("radialAmount", radialAmount);
	xml.addValue("radialInverseAmount", radialInverseAmount);
	xml.addValue("radialCenterOffsetX", radialCenterOffsetX);
	xml.addValue("radialCenterOffsetY", radialCenterOffsetY);
	xml.addValue("radialScale", radialScale);
	xml.addValue("radialBias", radialBias);
	
	string originalString = stringifyVec2fVec(originalGrid);
	string targetString = stringifyVec2fVec(targetGrid);
	string warpString = stringifyVec2fVec(warpGrid);
	
	xml.addValue("originalGrid", originalString);
	xml.addValue("targetGrid", targetString);
	xml.addValue("warpGrid", warpString);
	
	xml.saveFile(path);
}

void FboWarpGrid::loadXML(string path){
	ofxXmlSettings xml;
	xml.loadFile(path);
	
	rows = xml.getValue("rows", 1);
	cols = xml.getValue("cols", 1);
	src.x = xml.getValue("srcX", src.x);
	src.y = xml.getValue("srcY", src.y);
	src.width = xml.getValue("srcW", src.width);
	src.height = xml.getValue("srcH", src.height);
	dst.x = xml.getValue("dstX", dst.x);
	dst.y = xml.getValue("dstY", dst.y);
	dst.width = xml.getValue("dstW", dst.width);
	dst.height = xml.getValue("dstH", dst.height);
	window.x = xml.getValue("windowX", window.x);
	window.y = xml.getValue("windowY", window.y);
	window.width = xml.getValue("windowW", window.width);
	window.height = xml.getValue("windowH", window.height);
	//width = xml.getValue("width", 1024.0);
	//		height = xml.getValue("height", 768.0);
	//		offsetX = xml.getValue("offsetX", 0.0);
	//		offsetY = xml.getValue("offsetY", 0.0);
	
	string temp;
	loadStringIntoVec2f(temp = xml.getValue("shift", ""), shift);
	loadStringIntoVec2f(temp = xml.getValue("stretchTL", ""), stretchTL);
	loadStringIntoVec2f(temp = xml.getValue("stretchTR", ""), stretchTR);
	loadStringIntoVec2f(temp = xml.getValue("stretchBL", ""), stretchBL);
	loadStringIntoVec2f(temp = xml.getValue("stretchBR", ""), stretchBR);
	
	radialAmount = xml.getValue("radialAmount", 0.0);
	radialInverseAmount = xml.getValue("radialInverseAmount", 0.0);
	radialCenterOffsetX = xml.getValue("radialCenterOffsetX", 0.0);
	radialCenterOffsetY = xml.getValue("radialCenterOffsetY", 0.0);
	radialScale = xml.getValue("radialScale", 1.0);
	radialBias = xml.getValue("radialBias", 0.0);
	
	string originalString = xml.getValue("originalGrid", "");
	originalGrid.clear();
	loadStringIntoVec2fVec(originalString, originalGrid);
	
	string targetString = xml.getValue("targetGrid", "");
	targetGrid.clear();
	loadStringIntoVec2fVec(targetString, targetGrid);
	
	string warpString = xml.getValue("warpGrid", "");
	warpGrid.clear();
	loadStringIntoVec2fVec(warpString, warpGrid);
	
}

ofxVec2f FboWarpGrid::forwardsRadial(ofxVec2f& before){
	ofxVec2f result = before;
	
	
	ofxVec2f normalized;
	normalized.x = (before.x - dst.x)/dst.width;
	normalized.y = (before.y - dst.y)/dst.height;
	
	
	
	//recenter the point
	result.x -= dst.x + dst.width*0.5 + radialCenterOffsetX*dst.width;
	result.y -= dst.y + dst.height*0.5 + radialCenterOffsetY*dst.height;
	
	
	
	//shift the entire grid
	result += shift;
	
	
	
	//stretch the grid as a single quad from the corners
	float stretchL = 1 - normalized.x;
	float stretchR = normalized.x;
	float stretchT = 1 - normalized.y;
	float stretchB = normalized.y;
	result += stretchTL*stretchT*stretchL + stretchTR*stretchT*stretchR + stretchBR*stretchB*stretchR + stretchBL*stretchB*stretchL;
	
	
	
	//some numbers needed to bias the lens distortion
	float r2 = result.lengthSquared()/(dst.width*dst.height);
	float r = sqrt(r2);
	float radialBiasX = 1 - radialBias;
	float radialBiasY = 1 + radialBias;
	radialBiasX = sqrt(radialBiasX);
	radialBiasY = sqrt(radialBiasY);
	
	
	
	//calculate reverse distortion
	float radialScaleInverseX2 = r/(1 - radialBiasX*radialInverseAmount*r2);
	float radialScaleInverseX = 1/(1 - radialBiasX*radialInverseAmount*radialScaleInverseX2*radialScaleInverseX2);
	
	float radialScaleInverseY2 = r/(1 - radialBiasY*radialInverseAmount*r2);
	float radialScaleInverseY = 1/(1 - radialBiasY*radialInverseAmount*radialScaleInverseY2*radialScaleInverseY2);
	
	result.x = result.x*radialScaleInverseX;
	result.y = result.y*radialScaleInverseY;
	
	
	
	//calculate forwards distortion
	float radialScaleX = (1 + radialBiasX*radialAmount*r2);
	float radialScaleY = (1 + radialBiasY*radialAmount*r2);
	
	result.x = result.x*radialScaleX;
	result.y = result.y*radialScaleY;
	
	
	
	//apply some scaling
	result.x *= radialScale;
	result.y *= radialScale;
	
	
	
	//recenter the point
	result.x += dst.x + dst.width*0.5 + radialCenterOffsetX*dst.width;
	result.y += dst.y + dst.height*0.5 + radialCenterOffsetY*dst.height;
	
	
	return result;
}

void FboWarpGrid::buildGrids(bool original, bool target, bool warp){
	float srcSpacingH = src.width/(float)cols;
	float srcSpacingV = src.height/(float)rows;
	float dstSpacingH = dst.width/(float)cols;
	float dstSpacingV = dst.height/(float)rows;
	
	if(original){
		originalGrid.clear();
	}
	if(target){
		targetGrid.clear();
	}
	if(warp){
		warpGrid.clear();
	}
	
	//		float srcX = src.x;
	//		float srcY = 0;
	//		float dstX = dst.x;
	//		float dstY = 0;
	
	for(int j = 0; j <= rows; ++j){
		//			srcX = src.x;
		//			dstX = dst.x;
		for(int i = 0; i <= cols; ++i){
			
			ofxVec2f point;
			//				point.x = x;
			//				point.y = offsetY + y;
			
			if(target){
				point.x = dst.x + dstSpacingH*i;
				point.y = dst.y + dstSpacingV*j;
				targetGrid.push_back(point);
			}
			
			//				point.y = offsetY + height - y;
			
			if(original){
				point.x = src.x + srcSpacingH*i;
				point.y = window.height - srcSpacingV*j - src.y;
				originalGrid.push_back(point);
			}
			
			if(warp){
				point.x = 0;
				point.y = 0;
				warpGrid.push_back(point);
			}
			
			//x += spacingH;
		}
		//y += spacingV;
	}
	
}

void FboWarpGrid::buildRadialGrid(){
	warpedLensedGrid.clear();
	for(int i = 0; i < targetGrid.size(); ++i){
		ofxVec2f point = targetGrid[i] + warpGrid[i];
		warpedLensedGrid.push_back(forwardsRadial(point));
	}
}

ofxVec2f FboWarpGrid::originalToTarget(ofxVec2f& original){
	ofxVec2f result;
	result.x = (original.x - src.x)*dst.width/(1.0f*src.width) + dst.x;
	result.y = (original.y - src.y)*dst.height/(1.0f*src.height) + dst.y;
	return result;
}

ofxVec2f FboWarpGrid::targetToOriginal(ofxVec2f& target){
	ofxVec2f result;
	result.x = (target.x - dst.x)*(1.0f*src.width)/dst.width + src.x;
	result.y = (target.y - dst.y)*(1.0f*src.height)/dst.height + src.y;
	return result;
}