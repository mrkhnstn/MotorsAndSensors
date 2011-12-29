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

#include "UniformPropertiesEffector.h"
#include "PerlinForceField2f.h"
#include "ForceFieldSet2f.h"
#include "ForceEffector.h"
#include "PointSpriteParticleDrawer.h"
#include "ParticleGridDrawer.h"
#include "ParticleEffectorList.h"

/**
 test class for particle mesh
 */
class ParticleMeshTestBed : public ofxBaseObject {
	
public:
	
	// TODO: put all global parameters in to a struct
	
	Grid grid;
	int selectedCol;
	int selectedRow;
	bool drawGrid;
	
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
	float anchorLength; // set by gui
	float orthoStiffness; //< horizontal && vertical, set by gui
	float diagonalStiffness; //< set in gui
	float anchorStiffness; //< set in gui
	
	bool horizontalEnabled;
	bool verticalEnabled;
	bool lbToRtEnabled;
	bool ltToRbEnabled;
	bool anchorEnabled;
	
	bool horizontalDrawEnabled;
	bool verticalDrawEnabled;
	bool lbToRtDrawEnabled;
	bool ltToRbDrawEnabled;
	bool anchorDrawEnabled;
	bool pointsDrawEnabled;
	bool imageDrawEnabled;
	
	UniformPropertiesEffector	meshParticlePropertiesEffector;
	ParticleEffectorList		externalEffectors; //< add all external effetors to this  one
	
	PointSpriteParticleDrawer	pointSpriteDrawer;
	ParticleGridDrawer			particleGridDrawer;
	
	// parameters below to be external
	
	PerlinForceField2f			perlinForceField;
	ForceFieldSet2f				forceFieldSet;
	ForceEffector				forceFieldEffector;
	
	ofImage						testImage;
	
	
	/////////////////////////////////////////////////////////////////
	
	void setup(){
		particleSystem.pools.push_back(&meshPool);
		// not adding anchor pool as supposed to be static
		
		// external stuff
		perlinForceField.enableUpdate();
		forceFieldSet.addField(perlinForceField);
		forceFieldEffector.field = &forceFieldSet;
		
		externalEffectors.addEffector(forceFieldEffector);
		
		particleSystem.updateEffectors.push_back(&meshParticlePropertiesEffector);
		
		particleSystem.updateEffectors.push_back(&externalEffectors);
		
		pointSpriteDrawer.setup(128 * 128);
		
		particleSystem.drawers.push_back(&pointSpriteDrawer);
		
		
		testImage.loadImage("images/factoidTest.jpg");
		particleGridDrawer.setTexture(testImage.getTextureReference());
		particleSystem.drawers.push_back(&particleGridDrawer);
		
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
		
	}
	
	void setupGUI(){
		gui.page(1).addPageShortcut(gui.addPage("Particle Mesh Test Bed"));
		
		gui.addButton("reset mesh", this, &ParticleMeshTestBed::reset);
		
		gui.addTitle("grid");
		gui.addSlider("x", grid.x, 0, ofGetWidth());
		gui.addSlider("y", grid.y, 0, ofGetHeight());
		gui.addSlider("width", grid.width, 0, ofGetWidth());
		gui.addSlider("height", grid.height, 0, ofGetHeight());
		gui.addSlider("rows", grid.rows, 1, 32);
		gui.addSlider("cols", grid.cols, 1, 32);
		
		/*
		gui.addTitle("gridPoint");
		gui.addSlider("selected col", selectedCol, 0, 128);
		gui.addSlider("selected row", selectedRow, 0, 128);
		*/
		
		gui.addTitle("particle properties");
		meshParticlePropertiesEffector.setupGUI();

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
		gui.addToggle("anchor grid", drawGrid);
		gui.addToggle("anchor connections", anchorDrawEnabled);
		gui.addToggle("image", imageDrawEnabled);
		gui.addToggle("points", pointsDrawEnabled);
		gui.addToggle("horizontal lines", horizontalDrawEnabled);
		gui.addToggle("vertical lines", verticalDrawEnabled);
		gui.addToggle("LB to RT lines", lbToRtDrawEnabled);
		gui.addToggle("LT to RB lines", ltToRbDrawEnabled);
		
		gui.addTitle("image drawer");
		particleGridDrawer.setupGUI();
		
		gui.addTitle("point drawer");
		pointSpriteDrawer.setupGUI();
		
		// externalize

		
	}
	
	void postGUI(){
		reset();
	}
	
	void update(){
		// update cached constraint properties
		horizontalProperty.stiffness = orthoStiffness;
		verticalProperty.stiffness = orthoStiffness;
		lbToRtProperty.stiffness = diagonalStiffness;
		ltToRbProperty.stiffness = diagonalStiffness;
		anchorProperty.stiffness = anchorStiffness;
		
		float hSpan = grid.getColSpan();
		float vSpan = grid.getRowSpan();
		float dSpan = sqrt(hSpan * hSpan + vSpan * vSpan);
		horizontalProperty.length = hSpan;
		verticalProperty.length = vSpan;
		lbToRtProperty.length = dSpan;
		ltToRbProperty.length = dSpan;
		anchorProperty.length = anchorLength;
		
		if(horizontalEnabled){
			applyConstraints(horizontalConstraints, horizontalProperty);
		}
		
		if(verticalEnabled){
			applyConstraints(verticalConstraints, verticalProperty);
		}
		
		if(lbToRtEnabled){
			applyConstraints(lbToRtConstraints, lbToRtProperty);
		}
		
		if(ltToRbEnabled){
			applyConstraints(ltToRbConstraints, ltToRbProperty);
		}
		
		if(anchorEnabled){
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
		
		if(drawGrid){
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
		if(anchorDrawEnabled){
			drawConstraints(anchorConstraints);
		}
		
		// draw image / points
		particleSystem.drawers.clear();
		if (imageDrawEnabled) {
			particleSystem.drawers.push_back(&particleGridDrawer);
		}
		if (pointsDrawEnabled) {
			particleSystem.drawers.push_back(&pointSpriteDrawer);
		}
		particleSystem.draw();
		
		// draw constraints
		
		if(horizontalDrawEnabled){
			drawConstraints(horizontalConstraints);
		}
		
		if(verticalDrawEnabled){
			drawConstraints(verticalConstraints);
		}
		
		if(lbToRtDrawEnabled){
			drawConstraints(lbToRtConstraints);
		}
		
		if(ltToRbDrawEnabled){
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

		
};

