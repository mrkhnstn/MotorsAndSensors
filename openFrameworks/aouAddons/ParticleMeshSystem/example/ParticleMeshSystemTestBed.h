#pragma once
/*
 *  ParticleMeshSystemTestBed.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 28/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "ParticleMeshSystem.h"
#include "PerlinForceField2f.h"
#include "ForceFieldSet2f.h"
#include "ForceEffector.h"
#include "Singleton.h"
#include "GravityForceField2f.h"

class ParticleMeshSystemTestBed : public ofxBaseObject {
	
public:
	
	
	vector<ParticleMeshSystem*>	meshes;
	
	ofImage				testImage;
	
	PerlinForceField2f	perlinForceField;
	ForceFieldSet2f		forceFieldSet;
	ForceEffector		forceFieldEffector;
	GravityForceField2f gravityForceField;
	
	ParticleMeshSystemProperty property2;
	
	
	////////////////////////////////////////////////////////////
	
	void setup(){
		
		enableMouseEvents();
		
		// create overall force field
		perlinForceField.enableUpdate();
		forceFieldSet.addField(perlinForceField);
		forceFieldSet.addField(gravityForceField);
		forceFieldEffector.field = &forceFieldSet;
		
		// create meshes
		int meshCount = 3;
		for(int i=0; i<meshCount; i++){
			meshes.push_back(new ParticleMeshSystem());
		}
		
		for(int i=0; i < meshes.size(); i++){
			ParticleMeshSystem& mesh = *meshes[i];
			
			mesh.setup();
			
			if (i>0) { // first mesh uses singleton property others property2
				mesh.setProperty(property2);
			}
			
			// set image to be drawn by mesh
			testImage.loadImage("images/factoidTest.jpg");
			mesh.setTexture(testImage.getTextureReference());
			
			// apply force field effectors to mesh
			mesh.addEffector(forceFieldEffector);
			
		}
	}
	
	void setupGUI(){
		gui.page(1).addPageShortcut(gui.addPage("Mesh Properties"));
		Singleton<ParticleMeshSystemProperty>::instance()->setupGUI();
		
		gui.page(1).addPageShortcut(gui.addPage("Mesh Properties 2"));
		property2.setupGUI();
		
		for(int i=0; i < meshes.size(); i++){
			ParticleMeshSystem& mesh = *meshes[i];
			gui.page(1).addPageShortcut(gui.addPage("particle mesh system " + ofToString(i)));
			mesh.setupGUI();
			
		}
		
		gui.page(1).addPageShortcut(gui.addPage("perlin force field"));
		perlinForceField.setupGUI();
		gui.addTitle("mouse gravity force");
		gravityForceField.setupGUI();
	}
	
	void postGUI(){
		for(vector<ParticleMeshSystem*>::iterator it = meshes.begin(); it != meshes.end(); ++it){
			ParticleMeshSystem& mesh = **it;
			mesh.postGUI();
		}
	}
	
	void update(){		
		gravityForceField.center.set(getMouseX(),getMouseY());
		
		for(vector<ParticleMeshSystem*>::iterator it = meshes.begin(); it != meshes.end(); ++it){
			ParticleMeshSystem& mesh = **it;
			mesh.update();
		}
	}
	
	void draw(){
		for(vector<ParticleMeshSystem*>::iterator it = meshes.begin(); it != meshes.end(); ++it){
			ParticleMeshSystem& mesh = **it;
			mesh.draw();
		}
	}
	
};
