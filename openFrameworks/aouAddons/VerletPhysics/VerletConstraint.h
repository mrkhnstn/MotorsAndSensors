#pragma once

#include "VerletPoint.h"

struct VerletConstraintProperty {
	bool broken; //< not required at the moment
	float length; //< length of spring
	float maxLength; //< used to cap the calculated distance between point a and point b, not max length of spring!
	float stiffness; //< stiffness of spring
};

class VerletConstraint {	
	
public:
	
	VerletPoint* a;
	VerletPoint* b;
	VerletConstraintProperty property;
	
	VerletConstraint();
	
    VerletConstraint(VerletPoint* _a, VerletPoint* _b, float _length, float _maxLength, float _stiffness);
	
    VerletConstraint(VerletPoint* _a, VerletPoint* _b, VerletConstraintProperty _property);
	
	//VerletConstraint(VerletPoint *_a, <#VerletPoint *_b#>, VerletConstraintProperty& _property);

	void step();
	
	void draw();
	
};