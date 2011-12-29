#include "VerletConstraint.h"
#include "ofMain.h"

VerletConstraint::VerletConstraint(){
	a = NULL;
	b = NULL;
	property.length = 1;
	property.maxLength = 1;
	property.stiffness = 1;
	property.broken = false;
}

VerletConstraint::VerletConstraint(VerletPoint* _a, VerletPoint* _b, float _length, float _maxLength, float _stiffness) {
	a = _a;
	b = _b;
	property.length = _length;
	property.maxLength = _maxLength;
	property.stiffness = _stiffness;
	property.broken = false;
}

VerletConstraint::VerletConstraint(VerletPoint* _a, VerletPoint* _b, VerletConstraintProperty _property){
	a = _a;
	b = _b;
	property = _property;
}

/*
VerletConstraint::VerletConstraint(VerletPoint* _a, VerletPoint* _b, VerletConstraintProperty& _property) {
	a = _a;
	b = _b;
	property = _property;
}
*/

void VerletConstraint::step(){
	if(!property.broken){
		ofxVec2f& pa = a->p;
		ofxVec2f& pb = b->p;
		float dx = pa.x - pb.x;
		float dy = pa.y - pb.y;

		float d = sqrtf(dx*dx+dy*dy); //< TODO: replace with fast inverse square root
		
		if(d != 0){
			dx /= d;
			dy /= d;
		}
		
		if(d > property.maxLength){ //< should this be in a different constraint?
			d = property.maxLength;
		}
		//if((maxLength != 0) && (d > maxLength)){
		//	broken = true;
		//}
		d = property.length - d;
		d = property.stiffness*d;
		
		dx = dx*d;
		dy = dy*d;
		
		a->force(dx, dy);
		b->force(-dx, -dy);
	}
}

void VerletConstraint::draw(){
	ofLine(a->p.x, a->p.y, b->p.x, b->p.y);
}