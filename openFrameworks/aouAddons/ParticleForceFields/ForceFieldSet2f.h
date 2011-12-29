#pragma once
/*
 *  ForceFieldSet2f.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 23/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ForceField2f.h"
#include <set>

//////////////////////////////////////////////////////////////

class ForceFieldSet2f: public ForceField2f{

public:
	set<ForceField2f*> fields;
	
	virtual ofxVec2f getForce(ofxVec2f& pos_){
		ofxVec2f force;
		for(set<ForceField2f*>::iterator it = fields.begin(); it != fields.end(); ++it){
			ForceField2f* field = *it;
			force += field->getForce(pos_);
		}
		return force;
	}
	
	void addField(ForceField2f& field){
		fields.insert(&field);
	}
	
	void removeField(ForceField2f& field){
		set<ForceField2f*>::iterator it = fields.find(&field);
		if(it != fields.end())
			fields.erase(it);
	}
};

//////////////////////////////////////////////////////////////

