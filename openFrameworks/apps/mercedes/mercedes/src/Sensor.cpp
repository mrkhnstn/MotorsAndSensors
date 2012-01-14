/*
 *  Sensor.cpp
 *  mercedes
 *
 *  Created by Mark Hauenstein on 29/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sensor.h"

bool Sensor::doDraw = true;
bool Sensor::doDrawRays = true;
bool Sensor::doDrawHitPoints = true;
bool Sensor::doDrawLabels = false;
bool Sensor::doDrawSensorThreshold = false;
float Sensor::userInProximityDistance = 230;

bool Sensor::doStaticInit = true;
ofTrueTypeFont Sensor::font;