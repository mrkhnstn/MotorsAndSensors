#pragma once
/*
 *  ParticleMeshTestBed.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 26/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "VerletConstraint.h"
#include "Grid.h"
#include "ParticleSystem.h"
#include "ParticleGridPool.h"
#include "Utils.h"
#include "Singleton.h"

#include "UniformPropertiesEffector.h"
#include "PointSpriteParticleDrawer.h"
#include "ParticleGridDrawer.h"
#include "ParticleEffectorList.h"

#include "ofxValueObserverT.h"


class ParticleMeshSystemProperty {
	
public:
	
	float anchorLength; // set by gui
	float orthoStiffness; //< horizontal && vertical, set by gui
	float diagonalStiffness; //< set in gui
	float anchorStiffness; //< set in gui
	
	bool horizontalEnabled;
	bool verticalEnabled;
	bool lbToRtEnabled;
	bool ltToRbEnabled;
	bool anchorEnabled;
	
	bool gridDrawEnabled;
	bool horizontalDrawEnabled;
	bool verticalDrawEnabled;
	bool lbToRtDrawEnabled;
	bool ltToRbDrawEnabled;
	bool anchorDrawEnabled;
	bool pointsDrawEnabled;
	bool imageDrawEnabled;
	
	UniformPropertiesEffector	particlePropertiesEffector;
	BlendSetting blendSetting;
	ofColor blendColor;

	
	ParticleMeshSystemProperty(){
		// set default values
		anchorLength = 0; // set by gui
		orthoStiffness = 0.05; //< horizontal && vertical, set by gui
		diagonalStiffness = 0.05; //< set in gui
		anchorStiffness = 0.05; //< set in gui
		
		horizontalEnabled = false;
		verticalEnabled = false;
		lbToRtEnabled = true;
		ltToRbEnabled = true;
		anchorEnabled = true;
		
		gridDrawEnabled = false;
		horizontalDrawEnabled = false;
		verticalDrawEnabled = false;
		lbToRtDrawEnabled = false;
		ltToRbDrawEnabled = false;
		anchorDrawEnabled = false;
		pointsDrawEnabled = false;
		imageDrawEnabled = true;
		
		blendColor.r = 1;
		blendColor.g = 1;
		blendColor.b = 1;
		blendColor.a = 1;
	}
	
	void setupGUI(){
		gui.addTitle("constraints properties");
		gui.addSlider("ortho stiffness", orthoStiffness, 0, .1);
		gui.addSlider("diagonal stiffness", diagonalStiffness, 0, .1);
		gui.addSlider("anchor stiffness", anchorStiffness, 0, .1);
		gui.addSlider("anchor length", anchorLength, 0, 128);
		
		gui.addTitle("constraints enabled");
		gui.addToggle("horizontal enabled", horizontalEnabled);
		gui.addToggle("vertical enabled", verticalEnabled);
		gui.addToggle("LB to RT enabled", lbToRtEnabled);
		gui.addToggle("LT to RB enabled", ltToRbEnabled);
		gui.addToggle("anchor enabled", anchorEnabled);
		
		gui.addTitle("draw");
		gui.addToggle("anchor grid", gridDrawEnabled);
		gui.addToggle("anchor connections", anchorDrawEnabled);
		gui.addToggle("image", imageDrawEnabled);
		//gui.addToggle("points", pointsDrawEnabled);
		gui.addToggle("horizontal lines", horizontalDrawEnabled);
		gui.addToggle("vertical lines", verticalDrawEnabled);
		gui.addToggle("LB to RT lines", lbToRtDrawEnabled);
		gui.addToggle("LT to RB lines", ltToRbDrawEnabled);
		
		blendSetting.setupGUI();
		gui.addColorPicker("blend color", &blendColor.r, true, true);
		
		gui.addTitle("particle properties");
		particlePropertiesEffector.setupGUI();
	}
	
};

////////////////////////////////////////////////////////////////////

/**
- class to control and draw a mesh of particles forming a grid
= combines a particle system with a modified particle pool (ParticleGridPool.h) and a modified particle drawer (ParticleGridDrawer.h)
= TODO: requires some performance optimization
*/
class ParticleMeshSystem : public ofxBaseObject {
	
public:
	
	Grid grid;
	ofxValueObserverT<int> gridColObserver; //< used to detect changes in grid
	ofxValueObserverT<int> gridRowObserver; //< used to detect changes in grid
	
	ParticleSystem	particleSystem;
	ParticleGridPool	meshPool;
	ParticleGridPool	anchorPool;
	
	vector<VerletConstraint*>	allConstraints;
	vector<VerletConstraint*>	horizontalConstraints;
	vector<VerletConstraint*>	verticalConstraints;
	vector<VerletConstraint*>	lbToRtConstraints; //< left bottom to right top diagonals
	vector<VerletConstraint*>	ltToRbConstraints; //< left top to right bottom diagonals
	vector<VerletConstraint*>	anchorConstraints; //< connections between each intersection and a static counter part
	
	VerletConstraintProperty	horizontalProperty;
	VerletConstraintProperty	verticalProperty;
	VerletConstraintProperty	lbToRtProperty;
	VerletConstraintProperty	ltToRbProperty;
	VerletConstraintProperty	anchorProperty;
	
	float horizontalLength; // set by grid
	float verticalLength; // set by grid
	float diagonalLength; // set by grid
	
	
	
	ParticleEffectorList		externalEffectors; //< add all external effetors to this  one
	
	//PointSpriteParticleDrawer	pointSpriteDrawer;
	ParticleGridDrawer			drawer;
	
	/////////////////////////////////////////////////////////////////
	
	void setup(){
		property = Singleton<ParticleMeshSystemProperty>::instance(); //< use singleton property as default setup
		
		particleSystem.pools.push_back(&meshPool);
		// not adding anchor pool as supposed to be static
		
		particleSystem.updateEffectors.push_back(&property->particlePropertiesEffector);
		particleSystem.updateEffectors.push_back(&externalEffectors);
		
		//pointSpriteDrawer.setup(128 * 128);
		//particleSystem.drawers.push_back(&pointSpriteDrawer);
		
		particleSystem.drawers.push_back(&drawer);
		
		horizontalProperty.broken = false;
		verticalProperty.broken = false;
		lbToRtProperty.broken = false;
		ltToRbProperty.broken = false;
		anchorProperty.broken = false;
		
		horizontalProperty.maxLength = 1000;
		verticalProperty.maxLength = 1000;
		lbToRtProperty.maxLength = 1000;
		ltToRbProperty.maxLength = 1000;
		anchorProperty.maxLength = 1000;
		
		gridColObserver.setup(grid.cols,this,&ParticleMeshSystem::onGridChanged);
		gridRowObserver.setup(grid.rows,this,&ParticleMeshSystem::onGridChanged);
		gridChanged = false; //< if set to true then reset on update
		
		reset();
		
	}
	
	void setProperty(ParticleMeshSystemProperty& property){
		this->property = &property;
		
		// reinitialize effector pipeline with new particlePropertiesEffector 
		particleSystem.updateEffectors.clear();
		particleSystem.updateEffectors.push_back(&property.particlePropertiesEffector);
		particleSystem.updateEffectors.push_back(&externalEffectors);
		
		drawer.meshProperty = &property;

	}
	
	ParticleMeshSystemProperty& getProperty(){
		return *property;
	}
	
	void setupGUI(){
		
		gui.addButton("reset mesh", this, &ParticleMeshSystem::reset);
		
		gui.addTitle("grid");
		gui.addSlider("x", grid.x, 0, ofGetWidth());
		gui.addSlider("y", grid.y, 0, ofGetHeight());
		gui.addSlider("width", grid.width, 0, ofGetWidth());
		gui.addSlider("height", grid.height, 0, ofGetHeight());
		gui.addSlider("rows", grid.rows, 1, 64);
		gui.addSlider("cols", grid.cols, 1, 64);
		
		//gui.addTitle("particle properties");
		//meshParticlePropertiesEffector.setupGUI();
	
		gui.addTitle("image drawer");
		drawer.setupGUI();
		
		//gui.addTitle("point drawer");
		//pointSpriteDrawer.setupGUI();
	}
	
	void postGUI(){
		reset();
	}
	
	void update(){
		
		if(gridChanged){
			reset();
			gridChanged = false;
		}
		
		// update cached constraint properties
		horizontalProperty.stiffness = property->orthoStiffness;
		verticalProperty.stiffness = property->orthoStiffness;
		lbToRtProperty.stiffness = property->diagonalStiffness;
		ltToRbProperty.stiffness = property->diagonalStiffness;
		anchorProperty.stiffness = property->anchorStiffness;
		
		float hSpan = grid.getColSpan();
		float vSpan = grid.getRowSpan();
		float dSpan = sqrt(hSpan * hSpan + vSpan * vSpan);
		horizontalProperty.length = hSpan;
		verticalProperty.length = vSpan;
		lbToRtProperty.length = dSpan;
		ltToRbProperty.length = dSpan;
		anchorProperty.length = property->anchorLength;
		
		if(property->horizontalEnabled){
			applyConstraints(horizontalConstraints, horizontalProperty);
		}
		
		if(property->verticalEnabled){
			applyConstraints(verticalConstraints, verticalProperty);
		}
		
		if(property->lbToRtEnabled){
			applyConstraints(lbToRtConstraints, lbToRtProperty);
		}
		
		if(property->ltToRbEnabled){
			applyConstraints(ltToRbConstraints, ltToRbProperty);
		}
		
		if(property->anchorEnabled){
			applyConstraints(anchorConstraints, anchorProperty);
		}
		
		particleSystem.update();
	}
	
	void applyConstraints(vector<VerletConstraint*>& constraints, VerletConstraintProperty& property){
		for(vector<VerletConstraint*>::iterator it = constraints.begin(); it != constraints.end(); ++it){
			VerletConstraint& constraint = **it;
			constraint.property = property;
			constraint.step();
		}
	}
	
	void draw(){
		
		if(property->gridDrawEnabled){
			//
			ofSetColor(255, 255, 255);
			ofNoFill();
			ofSetLineWidth(1);
			grid.draw();
		}
		
		// update anchor points to grid dimensions 
		// TODO: only do this if grid dimensions have changed
		// TODO: implement grid rotation
		for(int row = 0; row < anchorPool.getRows(); row++){
			for(int col = 0; col < anchorPool.getCols(); col++){
				// translate anchor particle
				Particle* particle;
				particle = anchorPool.getParticle(col, row);
				particle->jump(grid.getPoint(col, row));
			}
		}
		
		// draw anchor lines
		if(property->anchorDrawEnabled){
			drawConstraints(anchorConstraints);
		}
	
		/*
		// draw image / points
		particleSystem.drawers.clear();
		if (property->imageDrawEnabled) {
			particleSystem.drawers.push_back(&particleGridDrawer);
		}
		if (property->pointsDrawEnabled) {
			particleSystem.drawers.push_back(&pointSpriteDrawer);
		}
		*/
		if (property->imageDrawEnabled) {
			particleSystem.draw();
		}
		
		// draw constraints
		
		if(property->horizontalDrawEnabled){
			drawConstraints(horizontalConstraints);
		}
		
		if(property->verticalDrawEnabled){
			drawConstraints(verticalConstraints);
		}
		
		if(property->lbToRtDrawEnabled){
			drawConstraints(lbToRtConstraints);
		}
		
		if(property->ltToRbDrawEnabled){
			drawConstraints(ltToRbConstraints);
		}
		
	}
	
	void drawConstraints(vector<VerletConstraint*>& constraints){
		for(vector<VerletConstraint*>::iterator it = constraints.begin(); it != constraints.end(); ++it){
			VerletConstraint& constraint = **it;
			constraint.draw();
		}
	}
	
	void reset(ofEventArgs& e){
		reset();
	}
	
	void reset(){
		// clear and delete old constraints if available
		ClearPointerVector(allConstraints);
		horizontalConstraints.clear();
		verticalConstraints.clear();
		lbToRtConstraints.clear(); //< left bottom to right top diagonals
		ltToRbConstraints.clear(); //< left top to right bottom diagonals
		anchorConstraints.clear(); //< connections between each intersection and a static counter part
		
		// reinitialize 
		meshPool.setup(grid.cols, grid.rows);
		anchorPool.setup(grid.cols, grid.rows);
		
		// setup all mesh and anchor particles to correspond to grid position
		for(int row = 0; row < grid.rows; row++){
			for(int col = 0; col < grid.cols; col++){
				// setup 
				meshPool.getParticle(col, row)->jump(grid.getPoint(col, row));
				
				// setup anchor particle
				Particle* particle;
				particle = anchorPool.getParticle(col, row);
				particle->jump(grid.getPoint(col, row));
				particle->setMass(0); //<TODO: create a static point to particle constraint
			}
		}
		
		// create horizontal connections
		for(int row = 0; row < grid.rows; row++){
			for(int col = 0; col < grid.cols-1; col++){
				Particle* particleA = meshPool.getParticle(col, row);
				Particle* particleB = meshPool.getParticle(col+1, row);				
				VerletConstraint* constraint = new VerletConstraint(particleA, particleB, horizontalProperty);
				horizontalConstraints.push_back(constraint);
				allConstraints.push_back(constraint);
			}
		}
		
		// create vertical connections
		for(int row = 0; row < grid.rows - 1; row++){
			for(int col = 0; col < grid.cols; col++){
				Particle* particleA = meshPool.getParticle(col, row);
				Particle* particleB = meshPool.getParticle(col, row+1);				
				VerletConstraint* constraint = new VerletConstraint(particleA, particleB, verticalProperty);
				verticalConstraints.push_back(constraint);
				allConstraints.push_back(constraint);
			}
		}
		
		// create left bottom to right top diagonals
		for(int row = 0; row < grid.rows - 1; row++){
			for(int col = 0; col < grid.cols - 1; col++){
				Particle* particleA = meshPool.getParticle(col, row+1); // left bottom
				Particle* particleB = meshPool.getParticle(col+1, row); // right top				
				VerletConstraint* constraint = new VerletConstraint(particleA, particleB, lbToRtProperty);
				lbToRtConstraints.push_back(constraint);
				allConstraints.push_back(constraint);
			}
		}
		
		// create left top to right bottom diagonals
		for(int row = 0; row < grid.rows - 1; row++){
			for(int col = 0; col < grid.cols - 1; col++){
				Particle* particleA = meshPool.getParticle(col, row); // left top
				Particle* particleB = meshPool.getParticle(col+1, row+1);				
				VerletConstraint* constraint = new VerletConstraint(particleA, particleB, ltToRbProperty);
				ltToRbConstraints.push_back(constraint);	
				allConstraints.push_back(constraint);
			}
		}
		
		// create anchor connections
		for(int row = 0; row < grid.rows; row++){
			for(int col = 0; col < grid.cols; col++){
				Particle* particleA = anchorPool.getParticle(col, row); // left top
				Particle* particleB = meshPool.getParticle(col, row);				
				VerletConstraint* constraint = new VerletConstraint(particleA, particleB, anchorProperty);
				anchorConstraints.push_back(constraint);
				allConstraints.push_back(constraint);
			}
		}
		
	}
	
	void clearEffectors(){
		externalEffectors.clearEffectors();
	}
	
	void addEffector(ParticleEffector& effector){
		externalEffectors.addEffector(effector);
	}
	
	void removeEffector(ParticleEffector& effector){
		externalEffectors.removeEffector(effector);
	}
	
	void setTexture(ofTexture& texture){
		drawer.setTexture(texture);
	}
	
protected:
	
	ParticleMeshSystemProperty* property;
	UniformPropertiesEffector*	meshParticlePropertiesEffector; //< every time property changes this one has to be updated

	bool gridChanged;
	
	/**
	 gets triggered if number of columns or rows change
	 */
	void onGridChanged(ofEventArgs& e){
		gridChanged = true;
	}
	
	
	
};

