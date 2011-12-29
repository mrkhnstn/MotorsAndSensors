/*
 *  TweenHelper.cpp
 *  TestBed_Mark
 *
 *  Created by TAQA_CH_0 on 28/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TweenHelper.h"

ofxEasing* TweenHelper::easingObject[11] = {&ofxTween::easingback,&ofxTween::easingbounce,&ofxTween::easingcirc,
	&ofxTween::easingcubic,&ofxTween::easingelastic,&ofxTween::easingexpo,&ofxTween::easinglinear,
	&ofxTween::easingquad,&ofxTween::easingquart,&ofxTween::easingquint,&ofxTween::easingsine};

string TweenHelper::easingObjectName[11] = {"back","bounce","circle","cubic","elastic","expo","linear","quad","quart","quint","sine"};
ofxTween::ofxEasingType TweenHelper::easingType[3] = {ofxTween::easeIn, ofxTween::easeOut, ofxTween::easeInOut};
string TweenHelper::easingTypeNames[3] = {"ease in","ease out","ease in/out"};