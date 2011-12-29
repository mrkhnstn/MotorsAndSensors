/*
 *  ImageList.cpp
 *  backdrop
 *
 *  Created by Mark on 14/02/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageList.h"
#include "ofxDirList.h"

ImageList::ImageList(){
	current_ = 0;
}

ImageList::~ImageList(){
}

ofImage& ImageList::operator[](unsigned i) {
	return images[i];
}

ofImage& ImageList::current(){
	return images[current_];
}

ofImage& ImageList::next(){
	current_++;
	if(current_ >= size())
		current_ = 0;
	return images[current_];
}

ofImage& ImageList::previous(){
	current_--;
	if(current_ < 0)
		current_ = size() - 1;
	return images[current_];
}

unsigned ImageList::size() const{
	return images.size();
}

void ImageList::addFolder(string folderpath, int ofImageType, int width, int height){
	
	ofxDirList dir;
	int nImages = dir.listDir(folderpath);
	
	for(int i=0; i<nImages; i++){
		ofImage img;		
		img.loadImage(dir.getPath(i));
		img.setImageType(ofImageType);
		if(width != 0 && height != 0){
			img.resize(width, height);
		}
		images.push_back(img);
	}
}