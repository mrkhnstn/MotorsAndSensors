#pragma once
/*
 *  ParticleProperty.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 01/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

/**
 used by particle system in KacareUserManager
 */

#include "ofMain.h"

// properties common to all custom properties
// all custom properties need to implement the fields of this property before their custom fields
struct BaseProperty { 
	char id;
	float radius; //< target radius usually
	float r; //< target colour usually
	float g;
	float b;
	float a;
	int maxLife;
};

struct UserProperty {
	char id;
	float radius; //< target radius usually
	float r; //< target colour usually
	float g;
	float b;
	float a;
	int	 handId;
	//bool isConnected;
	int lastUpdate; //
	// link to factoid particle / constraint
	int ghost;
	int state;
	int life;
	bool isMouse;
	int closestBorder;
	float originX;
	float originY;
};

#define EGG_STATE_NORMAL	0
#define EGG_STATE_PREOUTRO	1
#define EGG_STATE_SELECTED	2

struct EggProperty {
	char id;
	float radius; //< target radius usually
	float r; //< target colour usually
	float g;
	float b;
	float a;
	int maxLife;
	int modelId; //< use to retrieve model from factoid pool
	float pulse; //< used to tween the egg ring
	bool flip;
	int isOpen; //< increments when factoid open
	float tween; //< 0 .. 1, used when open << set from FactoidViewContainer
	float contentTween;
	int state; //< 0: normal, 1: pre_outro, 2: selected
};

struct FactoidProperty {
	char id;
	float radius; //< target radius usually
	float r; //< target colour usually
	float g;
	float b;
	float a;
	float factoidX;
	float factoidY;
	bool flip;
};

struct SolarProperty {
	char id;
	float massSeed;
	float radiusSeed;
	float radius; //< target radius usually
	float colourSeed;
	float r; //< target colour usually
	float g;
	float b;
	float a;
	bool isInUser;
	bool isInFactoid;
	bool isInEgg;
	float charge;
	float burnout;
	float burnoutSeed;
};

struct PhysicsProperty {
	char id;
	float radius; //< target radius usually
	float r; //< target colour usually
	float g;
	float b;
	float a;
	int maxLife;
	float e;
	float e_latency;
	float e_level;
	int  level;
	int  life;
	int  release;
	int   parent_x;
	int   parent_y;
	float t_bond;
	int   burnoutTime;
	int	  idleTime;
	float t_burnoutTime;
	int   mode;
	float cr,cg,cb;
	float dir_x;
	float dir_y;
	float dir_z;
	float x,y,z;
};

struct WindProperty {
	char id;
	float radius; //< target radius usually
	float r; //< target colour usually
	float g;
	float b;
	float a;
	int maxLife;
	bool disableFade;
};

struct AtomProperty {
	char id;
	float radius; //< target radius usually
	float r; //< target colour usually
	float g;
	float b;
	float a;
	int maxLife;
	float rotationSpeed;
	float rotationSeed;
};

struct YotaProperty {
	char id;
	float radius; //< target radius usually
	float r; //< target colour usually
	float g;
	float b;
	float a;
	int maxLife;
	float z;
	float noiseX;
	float noiseSpeed;
};

union ParticleProperty {
	BaseProperty	base;
	UserProperty	user;
	EggProperty		egg;
	SolarProperty	solar;
	WindProperty	wind;
	PhysicsProperty physics;
	AtomProperty	atom;
	YotaProperty    yota;
};
