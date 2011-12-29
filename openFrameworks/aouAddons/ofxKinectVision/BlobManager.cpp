/*
 *  BlobManager.cpp
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 19/04/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "BlobManager.h"
#include "ofxKinectVision.h"
#include "ofxSimpleGuiToo.h"
#include "GlobalVariables.h"



// hand //////////////////////////////////////////////////

float pointLine(ofxVec2f *pt, ofxVec2f *lineStart, ofxVec2f *lineEnd)
{
	float vx = lineStart->x-pt->x, vy = lineStart->y-pt->y, ux = lineEnd->x-lineStart->x, uy = lineEnd->y-lineStart->y;
	float length = ux*ux+uy*uy;
	
	float det = (-vx*ux)+(-vy*uy); //if this is < 0 or > length then its outside the line segment
	if(det<0 || det>length)
	{
        ux=lineEnd->x-pt->x;
        uy=lineEnd->y-pt->y;
        return std::min<float>(vx*vx+vy*vy, ux*ux+uy*uy);
	}
	
	det = ux*vy-uy*vx;
	return (det*det)/length;
}

Hand::Hand()
{
	x = y = w = h = 0;
	//vertices = new ofPoint[500];
	vertices_count = 0;
}

void Hand::update(ofxCvBlob* blob)
{
	vertices_count = 0;
	
	//c.x = ofGetWidth()/2;
	//c.y = ofGetHeight()/2;
	
	p.x = blob->centroid.x;
	p.y = blob->centroid.y;
	
	r1.x = blob->boundingRect.x;								//top left
	r1.y = blob->boundingRect.y;
	
	r2.x = blob->boundingRect.x + blob->boundingRect.width;		//top right
	r2.y = blob->boundingRect.y;
	
	r3.x = blob->boundingRect.x;								//bottom left
	r3.y = blob->boundingRect.y + blob->boundingRect.height;
	
	r4.x = blob->boundingRect.x + blob->boundingRect.width;		//bottom right
	r4.y = blob->boundingRect.y + blob->boundingRect.height;
	
	c1.x = 0;
	c1.y = 0;
	
	c2.x = 960;
	c2.y = 0;
	
	c3.x = 0;
	c3.y = 256;
	
	c4.x = 960;
	c4.y = 256;
	
	float dist, base;
	float tmp;
	
	//TODO: this should only happen on the first frame!!!
	
	//Test with top margin (1)
	tmp = pointLine(&r1,&c1,&c2);
	dist = tmp;
	base = r3.y - KinectVision::KinectVision::global->handLength;
	closest_border = BlobManager_BORDER_TOP;
	
	/* left and right sides disabled!!!
	 //Test with left margin (2)
	 tmp = pointLine(&r3,&c1,&c3);
	 if(tmp < dist)
	 {
	 dist = tmp;
	 base = r2.x - KinectVision::global->handLength;
	 closest_border = BlobManager_BORDER_LEFT;
	 }	
	 
	 //Test with right margin (3)
	 tmp = pointLine(&r2,&c2,&c4);
	 if(tmp < dist)
	 {
	 dist = tmp;
	 base = r1.x + KinectVision::global->handLength;
	 closest_border = BlobManager_BORDER_RIGHT;
	 } 
	 */
	
	//Test with bottom margin (4)
	tmp = pointLine(&r4,&c3,&c4);
	if(tmp < dist)
	{
		dist = tmp;
		base = r1.y + KinectVision::global->handLength;
		closest_border = BlobManager_BORDER_BOTTOM;
	}
	
	if(base < 0)
		base = 0;
	
	p = getCentroid(blob, base, closest_border);
	x = p.x;
	y = p.y;
	w = blob->boundingRect.width;
	h = blob->boundingRect.height;
	x_world = x*KinectVision::global->scale + KinectVision::global->shiftX;
	//y_world = (256-y)*KinectVision::global->scale + KinectVision::global->shiftY; //TODO: take out hard coding
	y_world = y*KinectVision::global->scale + KinectVision::global->shiftY; //TODO: take out hard coding
	w_world = w*KinectVision::global->scale;
	h_world = h*KinectVision::global->scale;
	
}

ofxVec2f Hand::getCentroid(ofxCvBlob* blob, int base, int border)
{
	float tx,ty;
	int count = 0;
	int px,py;
	
	for(int i=0; i<blob->nPts && count < KinectVision::global->maxHandPoints; i+=KinectVision::global->handPointsInterval) //< hardcoded 8 determines the number of points to jump
	{
		px = blob->pts[i].x;
		py = blob->pts[i].y;
		
		switch (border) {
			case BlobManager_BORDER_TOP:				//hand is top to bottom
				if(py > base)
				{
					vertices[count] = blob->pts[i];
					count++;
				}
				break;
			case BlobManager_BORDER_LEFT:				//hand is left to rigth
				if(px > base)
				{
					vertices[count] = blob->pts[i];
					count++;
				}
				break;
			case BlobManager_BORDER_RIGHT:				//hand is right to left
				if(px < base && px > 0)
				{
					vertices[count] = blob->pts[i];
					count++;
				}
				break;
			case BlobManager_BORDER_BOTTOM:				//hand is bottom to top
				if(py < base && py > 0)
				{
					vertices[count] = blob->pts[i];
					count++;
				}
				break;
		}
	}
	
	vertices_count = count;
	
	ofxVec2f centroid;
	
	if(count > 0){
		for (int i=0; i<count; ++i)
		{
			centroid += vertices[i];
		}
		centroid *= 1. / count;
	}
	
	
	/*
	 //http://stackoverflow.com/questions/2792443/finding-the-centroid-of-a-polygon
	 centroid.set(0,0);
	 double signedArea = 0.0;
	 double x0 = 0.0; // Current vertex X
	 double y0 = 0.0; // Current vertex Y
	 double x1 = 0.0; // Next vertex X
	 double y1 = 0.0; // Next vertex Y
	 double a = 0.0;  // Partial signed area
	 
	 // For all vertices except last
	 int i=0;
	 for (i=0; i<count-1; ++i)
	 {
	 x0 = vertices[i].x;
	 y0 = vertices[i].y;
	 x1 = vertices[i+1].x;
	 y1 = vertices[i+1].y;
	 a = x0*y1 - x1*y0;
	 signedArea += a;
	 centroid.x += (x0 + x1)*a;
	 centroid.y += (y0 + y1)*a;
	 }
	 
	 // Do last vertex
	 x0 = vertices[i].x;
	 y0 = vertices[i].y;
	 x1 = vertices[0].x;
	 y1 = vertices[0].y;
	 a = x0*y1 - x1*y0;
	 signedArea += a;
	 centroid.x += (x0 + x1)*a;
	 centroid.y += (y0 + y1)*a;
	 
	 signedArea *= 0.5;
	 centroid.x /= (6*signedArea);
	 centroid.y /= (6*signedArea);
	 */
	
	return centroid;
}

void Hand::drawContour()
{
	ofBeginShape();
	for(int i=0; i<vertices_count; i++)
	{
		ofVertex(vertices[i].x, vertices[i].y);
	}
	ofEndShape(true);
}

// blob //////////////////////////////////////////////////

Blob::Blob()
{
	x = y = w = h = 0;
	hand = Hand();	
}

void Blob::set(ofxCvBlob* _cvBlob, int _id, int _state)
{
	cvBlob = *_cvBlob;
	x = cvBlob.centroid.x;
	y = cvBlob.centroid.y;
	w = cvBlob.boundingRect.width;
	h = cvBlob.boundingRect.height;
	
	x_world = x*KinectVision::global->scale + KinectVision::global->shiftX;
	y_world = y*KinectVision::global->scale + KinectVision::global->shiftY;
	w_world = w*KinectVision::global->scale;
	h_world = h*KinectVision::global->scale;
	
	id = _id;
	state = _state;
	
	switch (state) {
		case BLOB_NEW:
			life = ghost = 0;
			color = KinectVision::global->blobNewColor;
			break;
		case BLOB_STATIC:
			life++;
			ghost = 0;
			color = KinectVision::global->blobActiveColor;
			break;
		case BLOB_MOVEMENT:
			life++;
			ghost = 0;
			color = KinectVision::global->blobActiveColor;
			break;
		case BLOB_GHOST:
			life++;
			ghost++;
			color = KinectVision::global->blobGhostColor;
			break;
	}
	
	hand.update(&cvBlob);
}

void Blob::set(int _state)
{
	state = _state;
}

void Blob::set(Blob* newBlob, Blob* oldBlob, int _state)
{
	cvBlob = (*newBlob).cvBlob;
	id = oldBlob->id;
	life = oldBlob->life;
	ghost = oldBlob->ghost;
	set(&cvBlob,id,_state);
}

void Blob::drawContour()
{
	ofSetColor(color.r, color.g, color.b, 128);
	ofNoFill();
	ofBeginShape();
	for (int i = 0; i < cvBlob.nPts; i++){
		ofVertex(cvBlob.pts[i].x, cvBlob.pts[i].y);
	}
	ofEndShape(true); 
	ofFill();
	hand.drawContour();
}

void Blob::drawBox()
{
	ofRect(x-w/2.0, y-h/2.0, w, h);
}

// blob manager //////////////////////////////////////////////////

BlobManager::BlobManager()
{
	for (int i = 0; i < MAX_BLOB_COUNT_LIMIT; i++) {
		blobs[i] = prev_blobs[i] = new_blobs[i] = Blob();
	}
	blobsCount = prev_blobsCount = new_blobsCount = 0;
	IDcounter = 0;
}

void BlobManager::add(ofxCvBlob* _cvBlob, bool _isHand )
{
	new_blobs[new_blobsCount].set(_cvBlob);
	new_blobsCount++;
}

void BlobManager::update()
{
	blobsCount = 0; 
	
	ofxVec2f pt1, pt2;
	float sqDist;
	float sqRad = (KinectVision::global->maxSearchRad)*(KinectVision::global->maxSearchRad);
	int minIndex;
	
	//Existing blobs
	for (int i = 0; i < prev_blobsCount; i++) {
		if(prev_blobs[i].ghost < (KinectVision::global->maxGhost))
		{
			pt1.x = prev_blobs[i].x;
			pt1.y = prev_blobs[i].y;
			minIndex = -1;
			
			//Searching withing maxSearchRad
			for(int j = 0; j < new_blobsCount; j++)
			{
				if(new_blobs[j].state != BLOB_PROCESSED)	//Not the best thing... sticks it to the first closest blob, might be there are some blobs closer
				{
					pt2.x = new_blobs[j].x;
					pt2.y = new_blobs[j].y;
					sqDist = abs(pt1.squareDistance(pt2));
					if(sqDist < sqRad)
						minIndex = j;
				}
				
				if(minIndex > -1)
					break;
			}
			
			if(minIndex > -1)
			{
				pt2.x = new_blobs[minIndex].x;
				pt2.y = new_blobs[minIndex].y;
				sqDist = abs(pt1.squareDistance(pt2));
				if(sqDist < (KinectVision::global->minMovement)*(KinectVision::global->minMovement))
				{
					blobs[blobsCount].set(&new_blobs[minIndex], &prev_blobs[i], BLOB_STATIC);
					new_blobs[minIndex].set(BLOB_PROCESSED);
				}
				else
				{
					blobs[blobsCount].set(&new_blobs[minIndex], &prev_blobs[i], BLOB_MOVEMENT);
					new_blobs[minIndex].set(BLOB_PROCESSED);
				}
				
				if(blobs[blobsCount].life < (KinectVision::global->minLife))
					blobs[blobsCount].set(BLOB_NEW); //<<?
				
				blobsCount++;
			}
			else // minIndex == -1
			{
				blobs[blobsCount].set(&prev_blobs[i], &prev_blobs[i], BLOB_GHOST);
				blobsCount++;
			}
		}
	}
	
	//New blobs
	for(int i = 0; i < new_blobsCount; i++)
	{
		if(new_blobs[i].state != BLOB_PROCESSED)
		{
			string ss = "BLOB: new" + ofToString(IDcounter);
			ofLog(OF_LOG_VERBOSE, ss);
			blobs[blobsCount].set(&(new_blobs[i].cvBlob), IDcounter, BLOB_NEW);
			new_blobs[i].state = BLOB_PROCESSED;
			IDcounter++;
			blobsCount++;
		}
	}
	
	// moves current array to old
	memcpy(prev_blobs, blobs, sizeof(prev_blobs)); 
	prev_blobsCount = blobsCount;
	new_blobsCount = 0;
}

void BlobManager::drawCounts()
{
	string ss;
	Blob* b;
	
	countN = countG = countA = 0;
	
	for(int i=0; i<blobsCount; i++)
	{
		b = &blobs[i];
		
		switch (b->state) {
			case BLOB_NEW:
				countN++;
				break;
			case BLOB_STATIC:
				countA++;
				break;
			case BLOB_MOVEMENT:
				countA++;
				break;
			case BLOB_GHOST:
				countG++;
				break;
		}
	
	}
	
	float lineHeight = 15;
	
	ofPushStyle();
	
	ofPushMatrix();
	ofTranslate(940-72, 256+20, 0);
	
	ofSetColor(255,255,255,255);
	ss = "Blobs : " + ofToString(blobsCount);
	ofDrawBitmapString(ss,0,0);
	
	ofColor tempColor = KinectVision::global->blobNewColor;
	ofSetColor(tempColor.r,tempColor.g,tempColor.b,tempColor.a);
	ss = "New   : " + ofToString(countN);
	ofDrawBitmapString(ss,0,lineHeight);
	
	tempColor = KinectVision::global->blobActiveColor;
	ofSetColor(tempColor.r,tempColor.g,tempColor.b,tempColor.a);
	ss = "Active: " + ofToString(countA);
	ofDrawBitmapString(ss,0,2*lineHeight);
	
	tempColor = KinectVision::global->blobGhostColor;
	ofSetColor(tempColor.r,tempColor.g,tempColor.b,tempColor.a);
	ss = "Ghost : " + ofToString(countG);
	ofDrawBitmapString(ss,0,3*lineHeight);
	
	ofPopStyle();
	ofPopMatrix();
}

void BlobManager::drawHands()
{
	Blob* b;
	float x,y;
	//countN = countG = countA = 0;
	for(int i=0; i<blobsCount; i++)
	{
		b = &blobs[i];
		x = b->hand.x;
		y = b->hand.y;
				
		ofNoFill();
		ofSetColor(b->color.r,b->color.g,b->color.b,128);
		ofEllipse(x, y, (KinectVision::global->maxSearchRad)*2, (KinectVision::global->maxSearchRad)*2);
		
		// draw label
		ofFill();
		ofSetColor(255, 255, 255,255);
		ofDrawBitmapString("#" + ofToString(b->id),x+(KinectVision::global->maxSearchRad)+10,y-(KinectVision::global->maxSearchRad)+4);	
		ofLine(x+8, y-8, x+(KinectVision::global->maxSearchRad),y-(KinectVision::global->maxSearchRad));
		ofLine(x+(KinectVision::global->maxSearchRad),y-(KinectVision::global->maxSearchRad),x+(KinectVision::global->maxSearchRad)+7,y-(KinectVision::global->maxSearchRad));
		ofLine(x-4, y-4, x+4, y+4);
		ofLine(x+4, y-4, x-4, y+4);
	}
}

void BlobManager::drawHandsInfo()
{
	string theState;
	string theBorder;
	Blob* b;
	float x,y;
	
	float lineHeight = 15;
	float listX = 20;
	float listY = 256 + lineHeight;
	
	for(int i=0; i<blobsCount; i++)
	{
		b = &blobs[i];
		x = b->hand.x;
		y = b->hand.y;
		
		switch (b->state) {
			case BLOB_NEW:
				theState = "new";
				break;
			case BLOB_STATIC:
				theState = "static";				
				break;
			case BLOB_MOVEMENT:
				theState = "movement";
				break;
			case BLOB_GHOST:
				theState = "ghost";
				break;
		}
		
		switch (b->hand.closest_border) {
			case BlobManager_BORDER_TOP:
				theBorder = "top";
				break;
			case BlobManager_BORDER_BOTTOM:
				theBorder = "bottom";
				break;
			default:
				break;
		}
		ofFill();
		//ofSetColor(b->color.r, b->color.g, b->color.b, b->color.a);
		
		ofPushMatrix();
		ofSetColor(255, 255, 255,255);
		ofTranslate(x+(KinectVision::global->maxSearchRad)+10,y-(KinectVision::global->maxSearchRad) + lineHeight,0);
				ofDrawBitmapString("x:" + ofToString(b->hand.x) + " y:"+ ofToString(b->hand.y), 0,0);
		ofDrawBitmapString("life  :" + ofToString(b->life),0,1*lineHeight);
		ofDrawBitmapString("ghost :" + ofToString(b->ghost),0,2*lineHeight);
		ofDrawBitmapString("state :" + theState,0,3*lineHeight);
		ofDrawBitmapString("border:" + theBorder,0,4*lineHeight);
		
		ofPopMatrix();
		
		
		
		ofDrawBitmapString("#" + ofToString(b->id), listX, listY);
		listY += lineHeight;
		ofDrawBitmapString("x:" + ofToString(b->hand.x) + " y:"+ ofToString(b->hand.y), listX, listY);
		listY += lineHeight;
		ofDrawBitmapString("life : " + ofToString(b->life),listX, listY);
		listY += lineHeight;
		ofDrawBitmapString("ghost: " + ofToString(b->ghost),listX, listY);
		listY += lineHeight;
		ofDrawBitmapString("border:" + theBorder,listX, listY);
		listY += lineHeight;
		listY += lineHeight;
		
	}
	
	ofSetColor(255, 255, 255);
}

void BlobManager::drawParameters()
{
	float blobSizeMin = sqrt(KinectVision::global->innerMin)/2.0;
	float blobSizeMax = sqrt(KinectVision::global->innerMax)/2.0;
	
	ofFill();
	ofSetColor(20,20,20,190);
	ofRect(-10,-7,80,20);
	ofRect(-10,14,80,10);
	
	ofFill();
	ofSetColor(255,255,255);
	ofDrawBitmapString("Area min " + ofToString(KinectVision::global->innerMin) ,0,0);
	ofDrawBitmapString("     max " + ofToString(KinectVision::global->innerMax) ,0,10);
	ofDrawBitmapString("Movement " + ofToString(KinectVision::global->minMovement) + "px", 0,21);
	
	ofNoFill();
	ofSetColor(150,150,150,30);
	ofFill();
	ofCircle(0, 0, blobSizeMax);
	ofSetColor(0,0,0,30);
	ofCircle(0, 0, blobSizeMin);
	
	ofSetColor(255, 255, 255);
	ofFill();
	
}