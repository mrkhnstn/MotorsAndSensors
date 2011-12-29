#pragma once

/*
 *  TextureAtlas.h
 *  TestBed_Mark2
 *
 *  Created by TAQA_CH_0 on 16/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Utils.h"
#include "ImageUtil.h"
#include "ofxDirList.h"

////////////////////////////////////////////////////////////////////

class TextureAtlasFrameSequence {
	
public:
	
	//string name; //< usually folderpath
	int startFrame;
	int endFrame;
};

////////////////////////////////////////////////////////////////////

class TextureAtlas {
	
public:
	
	bool isSetup;
	int cols;
	int rows;
	float colSpan;
	float rowSpan;
	ofImage texture;
	vector<TextureAtlasFrameSequence*> frameSequences;
	int col;
	int row;
	float* textureCoords;
	bool displayListsCreated;
	GLuint displayListStartIndex;
	
	TextureAtlas();
	
	~TextureAtlas();
	
	void setup(int width, int height, int cols, int rows);
	
	TextureAtlasFrameSequence& addFolder(string folderpath);
	
	TextureAtlasFrameSequence& addImages(vector<ofImage>& images);
	
	float* getTextureCoord(int frameIndex);
	
	ofTexture& getTexture();	
	
	/**
	 Carefull!!
	 this function does not check whether display lists have been created nor whether i is within the boundaries
	 */
	void drawDisplayList(uint i){
		glCallList(displayListStartIndex + i);
	}
	
	void resetDisplayLists(){
		if (displayListsCreated) {
			// delete existing display lists
			int frames = row * cols + col;
			glDeleteLists(displayListStartIndex, frames);
			displayListStartIndex = 0;
		}
	}
	
	void createDisplayLists(){
		
		int frames = row * cols + col;
		
		if (frames == 0) {
			return;
		}
		
		resetDisplayLists();
		
		// create new display lists

		displayListStartIndex = glGenLists(frames);
		
		if (displayListStartIndex == 0) {
			displayListsCreated = false;
			return;
		}
		
		
		for (uint i=0; i<frames; i++) {
			float* tc = textureCoords + (i * 8);
			glNewList(displayListStartIndex + i, GL_COMPILE);
			glBegin(GL_QUADS);
			
			glVertex2f(-.1, -.1);	glTexCoord2f(tc[0],tc[1]);
			glVertex2f(.1,-.1);		glTexCoord2f(tc[2],tc[3]);	
			glVertex2f(.1,.1);		glTexCoord2f(tc[4],tc[5]);
			glVertex2f(-.1,.1);		glTexCoord2f(tc[6],tc[7]);
			glEnd();
			glEndList();
		}
		displayListsCreated = true;
	}
};
