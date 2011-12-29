/*
 *  TextureAtlas.cpp
 *  TestBed_Mark2
 *
 *  Created by TAQA_CH_0 on 16/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TextureAtlas.h"

TextureAtlas::TextureAtlas(){
	isSetup = false;
	col = 0;
	row = 0;
	cols = 0;
	rows = 0;
	colSpan = 0;
	rowSpan = 0;
	displayListsCreated = false;
	displayListStartIndex = 0;
}

TextureAtlas::~TextureAtlas(){
	if(isSetup){
		delete[] textureCoords;
		ClearPointerVector(frameSequences);
		resetDisplayLists();
	}
}

void TextureAtlas::setup(int width, int height, int cols, int rows){
	if (isSetup) return; // only allow one setup for now
	//ofEnableArbTex();
	texture.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
	this->cols = cols;
	this->rows = rows;
	this->rowSpan = width / (float)rows;
	this->colSpan = height / (float)cols;
	
	textureCoords = new float[8 * cols * rows]; // 4 corners * 2uv * cols * rows
	
	isSetup = true;
}

TextureAtlasFrameSequence& TextureAtlas::addFolder(string folderpath){
	
	ofxDirList   dir;
	
	int nImages = dir.listDir(folderpath);
	vector<ofImage>images;
	for(int i=0; i<nImages; i++){
		ofImage image;
		image.loadImage(dir.getPath(i));
		image.setImageType(OF_IMAGE_COLOR_ALPHA);
		images.push_back(image);
	}
	
	return addImages(images);
}

TextureAtlasFrameSequence& TextureAtlas::addImages(vector<ofImage>& images){
	TextureAtlasFrameSequence* frameSequence = new TextureAtlasFrameSequence();
	
	int frameIndex = row * cols + col;
	int framesLeft = rows * cols - frameIndex;
	
	
	frameSequence->startFrame = ofClamp(frameIndex, 0, rows * cols - 1);
	frameSequence->endFrame = ofClamp(frameIndex, 0, rows * cols - 1);
	
	for (int i=0; i<images.size() && i<framesLeft; i++) {
		
		// texture coordinates to use
		int left = col*colSpan;
		int right = (col+1) * colSpan;
		int top = row * rowSpan;
		int bottom = (row+1) * rowSpan;
		
		// copy src image into texture image
		ImageUtil::copyPixel(images[i],0,0,colSpan,rowSpan,texture,left,top,true);
		
		// adjust frame sequence
		frameSequence->endFrame = frameIndex;
		
		// set texture coordinates into cached array
		float* tempTexCoord = textureCoords + (frameIndex * 8);
		
		if(frameIndex * 8 > (cols * rows - 1) * 8)
			ofLog(OF_LOG_ERROR, "meeep");
		
		//top left
		*tempTexCoord++ = left;
		*tempTexCoord++ = top;
		
		//top right
		*tempTexCoord++ = right;
		*tempTexCoord++ = top;
		
		//bottom right
		*tempTexCoord++ = right;
		*tempTexCoord++ = bottom;
		
		//bottom left
		*tempTexCoord++ = left;
		*tempTexCoord++ = bottom;
		
		col++;
		if(col >= cols){
			col = 0;
			row++;
		}
		
		frameIndex++;
	}
	
	texture.update();
	
	frameSequences.push_back(frameSequence);
	
	return *frameSequence;
}

float* TextureAtlas::getTextureCoord(int frameIndex){
	float* temp = textureCoords;
	int index = ofClamp(frameIndex,0,cols*rows-1) * 8;
	temp += index;
	return textureCoords + index;
}

ofTexture& TextureAtlas::getTexture(){
	return texture.getTextureReference();
}