#pragma once
/*
 *  BlendSetting.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 26/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSimpleGuiToo.h"
#include "ofxShader.h"
#include "Singleton.h"



///////////////////////////////////////////////////////////////
/**
 multiply rgb of texture by alpha of texture and then multiply the resulting color with vertex/gl colour
 */
class PreMultiplyAlphaShader : public ofxShader{

public:
	
	PreMultiplyAlphaShader() : ofxShader() {
		loadShader("shaders/PreMultiplyAlpha.frag","shaders/PreMultiplyAlpha.vert");
	}
	
};

///////////////////////////////////////////////////////////////

/**
 not in use at the moment
*/
class PreMultiplyColourShader : public ofxShader{
	
public:
	
	PreMultiplyColourShader() : ofxShader() {
		loadShader("shaders/PreMultiplyColor.frag","shaders/PreMultiplyColor.vert");
	}
	
};

///////////////////////////////////////////////////////////////
/*
 use this shader where the source is a grayscale png and you want to use the image as alpha channel
 - gl_FragColor = vec4(gl_Color.r*color0.r,gl_Color.g*color0.r,gl_Color.b*color0.r,color0.r);
 - uses red as the grayscale channel
 - rgb become gl_Color multiplied by grayscale channel
 - a becomes grayscale channel
 */
class GrayscaleAlphaShader : public ofxShader{
	
public:
	
	GrayscaleAlphaShader() : ofxShader() {
		loadShader("shaders/GrayscaleAlpha.frag","shaders/GrayscaleAlpha.vert");
	}
	
};

///////////////////////////////////////////////////////////////

#define BlendSetting_NONE				0
#define BlendSetting_NORMAL				1
#define BlendSetting_ADDITIVE			2
#define BlendSetting_MULTIPLY			3
#define BlendSetting_SUBTRACT			4
#define BlendSetting_COUNT				5

#define BlendSetting_PRE_NONE			0
#define BlendSetting_PRE_ALPHA			1
#define BlendSetting_PRE_GRAYSCALE		2
#define BlendSetting_PRE_COUNT			3

/**
 Helper object to set OpenGL blend modes via gui
 */
class BlendSetting {

public:
	
	int setting;
	int preMultiplySetting;
	
	string name;
	

	
	BlendSetting(){
		setting = BlendSetting_NONE;
		preMultiplySetting= BlendSetting_PRE_NONE;
	}
	
	void setupGUI(){
		string choiceTitles[BlendSetting_COUNT];
		choiceTitles[BlendSetting_NONE] = "none";
		choiceTitles[BlendSetting_NORMAL] = "normal";
		choiceTitles[BlendSetting_ADDITIVE] = "additive";
		choiceTitles[BlendSetting_MULTIPLY] = "multiply";
		choiceTitles[BlendSetting_SUBTRACT] = "subtract";
		gui.addComboBox("blend setting", setting, BlendSetting_COUNT, choiceTitles);
		
		string preMultiplyTitles[BlendSetting_PRE_COUNT];
		preMultiplyTitles[BlendSetting_PRE_NONE] = "none";
		preMultiplyTitles[BlendSetting_PRE_ALPHA] = "alpha";
		preMultiplyTitles[BlendSetting_PRE_GRAYSCALE] = "grayscale";
		gui.addComboBox("pre multiply", preMultiplySetting, BlendSetting_PRE_COUNT, preMultiplyTitles);
	}
	
	void begin(){
		switch (setting) {
			case BlendSetting_NONE:
				glDisable(GL_BLEND);
				break;
			case BlendSetting_NORMAL:
				glEnable(GL_BLEND);
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
				break;
			case BlendSetting_ADDITIVE:
				glEnable(GL_BLEND);
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE);
				break;
			case BlendSetting_MULTIPLY:
				glEnable(GL_BLEND);
				glBlendFuncSeparate(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
				break;
			case BlendSetting_SUBTRACT:
				glEnable(GL_BLEND);
				glBlendEquationSeparate(GL_FUNC_REVERSE_SUBTRACT, GL_FUNC_ADD);
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE); // dest_color - src_alpha * src_color
				break;
			default:
				break;
		}
		
		switch (preMultiplySetting) {
			case BlendSetting_PRE_ALPHA:
				Singleton<PreMultiplyAlphaShader>::instance()->setShaderActive(true);
				break;
			case BlendSetting_PRE_GRAYSCALE:
				Singleton<GrayscaleAlphaShader>::instance()->setShaderActive(true);
				break;
		}
	}
	
	/**
	 - use this if you use another shader
	 - does not enable pre multiplying via shaders
	 - pre multiplying needs to be taken care of by external shader
	 */
	void beginNoShader(){
		switch (setting) {
			case BlendSetting_NONE:
				glDisable(GL_BLEND);
				break;
			case BlendSetting_NORMAL:
				glEnable(GL_BLEND);
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
				break;
			case BlendSetting_ADDITIVE:
				glEnable(GL_BLEND);
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE);
				break;
			case BlendSetting_MULTIPLY:
				glEnable(GL_BLEND);
				glBlendFuncSeparate(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
				break;
			case BlendSetting_SUBTRACT:
				glEnable(GL_BLEND);
				glBlendEquationSeparate(GL_FUNC_REVERSE_SUBTRACT, GL_FUNC_ADD);
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE); // dest_color - src_alpha * src_color
				break;
			default:
				break;
		}
		
	}
	
	void end(){
		switch (preMultiplySetting) {
			case BlendSetting_PRE_ALPHA:
				Singleton<PreMultiplyAlphaShader>::instance()->setShaderActive(false);
				break;
			case BlendSetting_PRE_GRAYSCALE:
				Singleton<GrayscaleAlphaShader>::instance()->setShaderActive(false);
				break;
		}
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		
	}
	
	void endNoShader(){
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	}
	
};