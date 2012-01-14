/*
 *  SceneCtrl.cpp
 *  mercedes
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SceneCtrl.h"

void SceneCtrl::setup(){
	
	//add scenes
	scenes.push_back(new IdleScene());
	scenes.push_back(new PalindromeScene());
	scenes.push_back(new StepScene());
	scenes.push_back(new SimpleInteractiveScene());
	//scenes.push_back(new TestScene1());
	
	for(int i=0; i<scenes.size(); i++)
		scenes[i]->setup();
	
	currentSceneId = 0;
	playlistIndex = 0;
	
}

void SceneCtrl::setupGUI(){
	gui.page(1).addPageShortcut(gui.addPage("Scenes"));
	
	// add a combobox for manual scene selection
	string names[scenes.size()];
	for(int i=0; i<scenes.size(); i++){
		names[i] = scenes[i]->name;
	}
	
	gui.addComboBox("Current_Scene", currentSceneId, scenes.size(), names);
	
	gui.addTitle("Playlist");
	string playlistChoices[scenes.size()+1];
	playlistChoices[0] = "EMPTY";
	for(int i=0; i<scenes.size(); i++){
		playlistChoices[i+1] = scenes[i]->name;
	}
	for (int i=0; i<NUM_PLAYLIST_ITEMS; i++) {
		gui.addComboBox("Scene_"+ofToString(i), playlist[i], scenes.size()+1, playlistChoices);
	}
	
	gui.addTitle("Scenes");
	
	for(int i=0; i<scenes.size(); i++)
		scenes[i]->setupGUI();
}

void SceneCtrl::postGUI(){
	for(int i=0; i<scenes.size(); i++)
		scenes[i]->postGUI();
	
	mode = SCENE_CTRL_MANUAL; // TODO: change this to SCENE_CTRL_PLAYLIST
}

void SceneCtrl::update(){
	if (mode == SCENE_CTRL_IDLE) return;
	
	switch (mode) {
		case SCENE_CTRL_PLAYLIST:
			if (!getCurrentScene().enabled) { // if current scene is not playing (not enabled)
				// find next scene to playback
				playlistIndex = (playlistIndex + 1) % NUM_PLAYLIST_ITEMS;
				int whileCounter = 0; // to prevent endless loop if all play items empty
				while (playlist[playlistIndex] != 0 && whileCounter < NUM_PLAYLIST_ITEMS) {
					playlistIndex = (playlistIndex + 1) % NUM_PLAYLIST_ITEMS;
				}
				
				if(playlist[playlistIndex] > 0) // if a non empty slot was found
					setCurrentScene(playlist[playlistIndex]-1); // -1 because 0 corresponds to empty slot so scene ids are offset by 1
			}
			break;
		case SCENE_CTRL_INTERACTIVE:
			if (!getCurrentScene().enabled) {
				//
			}			
		default:
			break;
	}
	
	getCurrentScene().update();
}

void SceneCtrl::draw(){
	if (mode == SCENE_CTRL_IDLE) return;
	getCurrentScene().draw();
}

void SceneCtrl::exit(){
	for (int i=0; i<scenes.size(); i++) {
		delete scenes[i];
	}
	scenes.clear();
}

Scene& SceneCtrl::getCurrentScene(){
	return *scenes[currentSceneId];
}

void SceneCtrl::setCurrentScene(int sceneId){
	//if (currentSceneId != sceneId) {
	//	scenes[currentSceneId]->stop();
		currentSceneId = sceneId;
		scenes[currentSceneId]->start();
	//}
}