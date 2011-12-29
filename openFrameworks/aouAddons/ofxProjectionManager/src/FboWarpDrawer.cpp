/*
 *  FboWarpDrawer.cpp
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 24/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "FboWarpDrawer.h"
#include "FboWarpOutput.h"

FboWarpDrawer::FboWarpDrawer(){
	perspectiveCorrectionPrecision = 1;
	maxRecursion = 4;
	bgColor.r = 0;
	bgColor.g = 0;
	bgColor.b = 0;
	bgColor.a = 1;
	fboWidth = 1024;
	fboHeight = 768;
	drawCount = 0;
	shader = NULL;
	parentName = "Projection Manager";
	
	/*
	highlightRadius = 10;
	highlightPointSize = 1;
	highlightCrossHair = true;
	highlightCircle = true;
	*/
};

void FboWarpDrawer::setup(int fboWidth, int fboHeight, ofxShader& shader_){
	this->fboWidth = fboWidth;
	this->fboHeight = fboHeight;
	shader = &shader_;
	fboTexture.allocate(fboWidth, fboHeight, GL_RGBA, 1, true);
	//mask.loadImage("edgeblend_masks/test.png");
}

void FboWarpDrawer::setupGUI(string name){
	//gui.page(parentName).addPageShortcut(gui.addPage(name+" Drawer"));
	/*
	gui.addTitle("colors");
	gui.addColorPicker("outer grid",&outerGridColor.r,true,true);
	gui.addColorPicker("inner grid",&innerGridColor.r,true,true);
	gui.addColorPicker("blank",&blankColor.r,true,true);
	*/
	/*
	gui.addTitle("highlights");
	gui.addColorPicker("highlight color",&highlightColor.r,true,true);
	gui.addSlider("highlight point size", highlightPointSize,1,4);
	gui.addSlider("highlight radius", highlightRadius,1,64);
	gui.addToggle("highlight circle", highlightCircle);
	gui.addToggle("highlight  ", highlightCrossHair);
	*/
}

void FboWarpDrawer::swapIn(bool clear){
	fboTexture.begin();
	if(clear){
		fboTexture.clear(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	}
}

void FboWarpDrawer::swapOut(){
	fboTexture.end();
}

/*
void FboWarpDrawer::draw(){
	for(int i = 0; i < quadRenderManagers.size(); ++i){
		drawSub(i);
	}
}
*/

void FboWarpDrawer::drawSub(int i, FboWarpOutput* output, bool blending, ofTexture* texture){
	if (i<0 || i>=quadRenderManagers.size()) {
		return;
	}
	QuadRenderManager2f* quadRenderManager = quadRenderManagers[i];
	ofImage* mask = masks[i];
	FboWarpGrid* grid = grids[i];
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	// enable alpha blending using the alpha channel of texture too
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO);
	
	glActiveTextureARB(GL_TEXTURE0_ARB);
	
	if (texture == NULL) {
		fboTexture.bind();
	} else {
		texture->bind();
	}
	
	if(blending){
		glActiveTextureARB(GL_TEXTURE1_ARB);
		mask->getTextureReference().bind();
		shader->setShaderActive(true);
		shader->setUniformVariable1i("tex0", 0);
		shader->setUniformVariable1i("tex1", 1);
		shader->setUniformVariable2f("offset", -grid->src.x, grid->src.y + grid->src.height - grid->window.height);
		/*
		shader->setUniformVariable3f("inputMin", 0, 0, 0);
		shader->setUniformVariable3f("inputMax", 1, 1, 1);
		shader->setUniformVariable3f("outputMin", 0, 0, 0);
		shader->setUniformVariable3f("outputMax", 1, 1, 1);
		shader->setUniformVariable3f("gammaVal", 1, 1, 1);
		*/
		Levels& levels = output->levels0;
		shader->setUniformVariable3f("inputMin", levels.minInput, levels.minInput, levels.minInput);
		shader->setUniformVariable3f("inputMax", levels.maxInput, levels.maxInput, levels.maxInput);
		shader->setUniformVariable3f("outputMin", levels.minOutput, levels.minOutput, levels.minOutput);
		shader->setUniformVariable3f("outputMax", levels.maxOutput, levels.maxOutput, levels.maxOutput);
		if (levels.useRGB) {
			shader->setUniformVariable3f("gammaVal", levels.gammaRGB.r, levels.gammaRGB.g, levels.gammaRGB.b);
		} else {
			shader->setUniformVariable3f("gammaVal", levels.gamma, levels.gamma, levels.gamma);
		}
		Levels& levels2 = output->levels2;
		shader->setUniformVariable3f("inputMin2", levels2.minInput, levels2.minInput, levels2.minInput);
		shader->setUniformVariable3f("inputMax2", levels2.maxInput, levels2.maxInput, levels2.maxInput);
		shader->setUniformVariable3f("outputMin2", levels2.minOutput, levels2.minOutput, levels2.minOutput);
		shader->setUniformVariable3f("outputMax2", levels2.maxOutput, levels2.maxOutput, levels2.maxOutput);
		if (levels2.useRGB) {
			shader->setUniformVariable3f("gammaVal2", levels2.gammaRGB.r, levels2.gammaRGB.g, levels2.gammaRGB.b);
		} else {
			shader->setUniformVariable3f("gammaVal2", levels2.gamma, levels2.gamma, levels2.gamma);
		}
		
	}
	
	quadRenderManager->draw(true, false);
	
	if(blending){
		shader->setShaderActive(false);
		glActiveTextureARB(GL_TEXTURE1_ARB);
		mask->getTextureReference().unbind();
	}
	
	glActiveTextureARB(GL_TEXTURE0_ARB);
	if (texture == NULL) {
		fboTexture.unbind();
	} else {
		texture->unbind();
	}
	glDisable(GL_BLEND);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	
}

void FboWarpDrawer::registerGrid(FboWarpGrid& _grid, ofImage* mask){
	grids.push_back(&_grid);
	masks.push_back(mask);
	QuadRenderManager2f* renderManager = new QuadRenderManager2f();
	quadRenderManagers.push_back(renderManager);
}

void FboWarpDrawer::buildGrids(){
	for(int i = 0; i < grids.size(); ++i){
		FboWarpGrid* grid = grids[i];
		reset();
		buildGrid(*grid, i);
	}
	
	//int quadCount = texCoordVector.size()/8;
	
	
}

void FboWarpDrawer::buildGrid(FboWarpGrid& _grid, int g){
	
	_grid.buildRadialGrid();
	
	float hmid = 0.5;
	float vmid = 0.5;
	float depth = maxRecursion;
	
	for(int j = 1; j <= _grid.rows; ++j){
		for(int i = 1; i <= _grid.cols; ++i){
			
			float indexa = i-1 + (j-1)*(_grid.cols+1.0);
			float indexb = i + (j-1)*(_grid.cols+1.0);
			float indexc = i + (j)*(_grid.cols+1.0);
			float indexd = i-1 + (j)*(_grid.cols+1.0);
			
			ofxVec2f& originala = _grid.originalGrid[indexa];
			ofxVec2f& originalb = _grid.originalGrid[indexb];
			ofxVec2f& originalc = _grid.originalGrid[indexc];
			ofxVec2f& originald = _grid.originalGrid[indexd];
			
			ofxVec2f& warpeda = _grid.warpedLensedGrid[indexa];
			ofxVec2f& warpedb = _grid.warpedLensedGrid[indexb];
			ofxVec2f& warpedc = _grid.warpedLensedGrid[indexc];
			ofxVec2f& warpedd = _grid.warpedLensedGrid[indexd];
			
			drawQuad(warpeda.x, warpeda.y, warpedb.x, warpedb.y, warpedc.x, warpedc.y, warpedd.x, warpedd.y,
					 originala.x, originala.y, originalb.x, originalb.y, originalc.x, originalc.y, originald.x, originald.y,
					 depth, hmid, vmid);
		}
	}
	
	QuadRenderManager2f* quadRenderManager = quadRenderManagers[g];
	quadRenderManager->setup(drawCount);
	
	int i;
	for(i = 0; i < texCoordVector.size(); ++i){
		quadRenderManager->texCoordBuffer[i] = texCoordVector[i];
	}
	for(i = 0; i < vertexVector.size(); ++i){
		quadRenderManager->vertexBuffer[i] = vertexVector[i];
	}
	
	int colorSize = drawCount*16;
	for(i = 0; i < colorSize; ++i){
		quadRenderManager->colorBuffer[i] = 1;
	}
	
}

void FboWarpDrawer::drawHighlight(FboWarpOutput* output, FboWarpGrid& grid, int index, bool inside){
	//int index = x+y*(grid.cols+1);
	if(grid.isValidPoint(index)){
		
		ofxVec2f point = inside ? grid.originalGrid[index] : grid.warpedLensedGrid[index];
		
		if (inside) {
			point.y = fboHeight - point.y;
		}
				
		ofPushStyle();
		
		glColor4f(output->highlightColor.r, output->highlightColor.g, output->highlightColor.b, output->highlightColor.a);
		glPointSize(output->highlightPointSize);
		glBegin(GL_POINTS);
		glVertex2f(point.x, point.y);
		glEnd();
		glPointSize(1);
		
		ofSetLineWidth(output->highlightPointSize);
		ofNoFill();
		
		if (output->highlightCrossHair) {
			ofPushMatrix();
			ofTranslate(point.x, point.y, 0);
			ofRotate(45, 0, 0, 1);
			ofLine(-output->highlightRadius, 0, -0.25 * output->highlightRadius, 0);
			ofLine(output->highlightRadius, 0, 0.25 * output->highlightRadius, 0);
			ofLine(0,-output->highlightRadius, 0, -0.25 * output->highlightRadius);
			ofLine(0,output->highlightRadius, 0, 0.25 * output->highlightRadius);
			ofPopMatrix();
		}
		
		if (output->highlightCircle) {
			ofCircle(point.x, point.y, output->highlightRadius);
		}
		
		ofPopStyle();
	}
}

void FboWarpDrawer::drawGrid(FboWarpOutput* output, FboWarpGrid& grid, int g, bool outer, bool inner, bool blank, bool drawAsPoints){
	float linewidth = 1;
	float shift = floor(linewidth*0.5);
	QuadRenderManager2f* quadRenderManager = quadRenderManagers[g];
	
	if (blank) {
		//ofSetColor(0, 0, 0);
		ofPushStyle();
		ofEnableAlphaBlending();
		setColor(output->blankColor);
		ofRect(grid.src.x, grid.src.y, grid.src.width, grid.src.height);
		
		ofPopStyle();
	}
	
	if(inner){
		//ofSetColor(255, 0, 0);
		setColor(output->innerGridColor);
		ofFill();
		ofSetLineWidth(0);
		
		for(int i = 0; i < quadRenderManager->quadCount; ++i){
			int j = i*8;
			float x1 = quadRenderManager->texCoordBuffer[j];
			float y1 = (float)fboHeight - quadRenderManager->texCoordBuffer[j+1];
			float x2 = quadRenderManager->texCoordBuffer[j+2];
			float y2 = (float)fboHeight - quadRenderManager->texCoordBuffer[j+3];
			float x3 = quadRenderManager->texCoordBuffer[j+4];
			float y3 = (float)fboHeight - quadRenderManager->texCoordBuffer[j+5];
			float x4 = quadRenderManager->texCoordBuffer[j+6];
			float y4 = (float)fboHeight - quadRenderManager->texCoordBuffer[j+7];
			
			ofRect(x1 - shift, y1 - shift, x2 - x1 + linewidth, linewidth);
			ofRect(x4 - shift, y4 - shift, x3 - x4 + linewidth, linewidth);
			ofRect(x1 - shift, y1 - shift, linewidth, y4 - y1 + linewidth);
			ofRect(x2 - shift, y2 - shift, linewidth, y3 - y2 + linewidth);
			
		}
	}
	
	if(outer){
		
		ofPushStyle();
		setColor(output->outerGridColor);
		ofNoFill();
		ofSetLineWidth(1);
		
		if (drawAsPoints) {
			
			
			glBegin(GL_POINTS);
			for(int j = 0; j <= grid.rows; ++j){
				for(int i = 0; i <= grid.cols; ++i){
					ofxVec2f& originala = grid.originalGrid[i + j * (grid.cols + 1)];
					glVertex2f(originala.x, originala.y);
				}
			}
			glEnd();
			
		} else {
			
			for(int j = 1; j <= grid.rows; ++j){
				for(int i = 1; i <= grid.cols; ++i){
					
					float indexa = i-1 + (j-1)*(grid.cols+1);
					float indexb = i + (j-1)*(grid.cols+1);
					float indexc = i + (j)*(grid.cols+1);
					float indexd = i-1 + (j)*(grid.cols+1);
					
					ofxVec2f& originala = grid.originalGrid[indexa];
					ofxVec2f& originalb = grid.originalGrid[indexb];
					ofxVec2f& originalc = grid.originalGrid[indexc];
					ofxVec2f& originald = grid.originalGrid[indexd];
					
					float x1 = originala.x;
					float y1 = (float)fboHeight - originala.y;
					float x2 = originalb.x;
					float y2 = (float)fboHeight - originalb.y;
					float x3 = originalc.x;
					float y3 = (float)fboHeight - originalc.y;
					float x4 = originald.x;
					float y4 = (float)fboHeight - originald.y;
					
					ofLine(x1, y1, x2, y2);
					ofLine(x2, y2, x3, y3);
					ofLine(x3, y3, x4, y4);
					ofLine(x4, y4, x1, y1);
					
					/*
					ofRect(x1 - shift, y1 - shift, x2 - x1 + linewidth, linewidth);
					ofRect(x4 - shift, y4 - shift, x3 - x4 + linewidth, linewidth);
					ofRect(x1 - shift, y1 - shift, linewidth, y4 - y1 + linewidth);
					ofRect(x2 - shift, y2 - shift, linewidth, y3 - y2 + linewidth);
					*/
					
				}
			}
		}
		
		ofPopStyle();
	}
	ofSetColor(255, 255, 255);
}

void FboWarpDrawer::drawOuterGrid(FboWarpOutput* output,FboWarpGrid& grid, bool drawAsPoints){
	
	
	ofPushStyle();
	setColor(output->outerGridColor);
	ofNoFill();
	ofSetLineWidth(1);
	ofEnableAlphaBlending();
	
	if (drawAsPoints) {
		
		
		glBegin(GL_POINTS);
		for(int j = 0; j <= grid.rows; ++j){
			for(int i = 0; i <= grid.cols; ++i){
				ofxVec2f& originala = grid.warpedLensedGrid[i + j * (grid.cols + 1)];
				glVertex2f(originala.x, originala.y);
			}
		}
		glEnd();
		
	} else {
	
		for(int j = 1; j <= grid.rows; ++j){
			for(int i = 1; i <= grid.cols; ++i){
				
				float indexa = i-1 + (j-1)*(grid.cols+1);
				float indexb = i + (j-1)*(grid.cols+1);
				float indexc = i + (j)*(grid.cols+1);
				float indexd = i-1 + (j)*(grid.cols+1);
				
				ofxVec2f& originala = grid.warpedLensedGrid[indexa];
				ofxVec2f& originalb = grid.warpedLensedGrid[indexb];
				ofxVec2f& originalc = grid.warpedLensedGrid[indexc];
				ofxVec2f& originald = grid.warpedLensedGrid[indexd];
				
				float x1 = originala.x;
				float y1 = originala.y;
				float x2 = originalb.x;
				float y2 = originalb.y;
				float x3 = originalc.x;
				float y3 = originalc.y;
				float x4 = originald.x;
				float y4 = originald.y;
				
				ofLine(x1, y1, x2, y2);
				ofLine(x2, y2, x3, y3);
				ofLine(x3, y3, x4, y4);
				ofLine(x4, y4, x1, y1);
				//ofRect(x1 - shift, y1 - shift, x2 - x1 + linewidth, linewidth);
				//ofRect(x4 - shift, y4 - shift, x3 - x4 + linewidth, linewidth);
				//ofRect(x1 - shift, y1 - shift, linewidth, y4 - y1 + linewidth);
				//ofRect(x2 - shift, y2 - shift, linewidth, y3 - y2 + linewidth);
				
			}
		}
		
	}
	ofPopStyle();
}

void FboWarpDrawer::drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
			  float a1, float b1, float a2, float b2, float a3, float b3, float a4, float b4,
			  int depth, float hmid, float vmid
			  ){
	//if(drawCount >= maxCount) return;
	
	
	float _hmid = hmid;
	float _hmidi = 1 - hmid;
	float _vmid = vmid;
	float _vmidi = 1 - vmid;
	
	float x12 = x1*_hmid+x2*_hmidi;
	float x34 = x3*_hmid+x4*_hmidi;
	float xm = x12*_vmid+x34*_vmidi;
	
	float y12 = y1*_hmid+y2*_hmidi;
	float y34 = y3*_hmid+y4*_hmidi;
	float ym = y12*_vmid+y34*_vmidi;
	
	float xrm = (x2+x4)*0.5;
	float yrm = (y2+y4)*0.5;
	
	float dx = xm - xrm;
	float dy = ym - yrm;
	float d2 = dx*dx+dy*dy;
	float d = sqrt(d2);
	
	if(d < perspectiveCorrectionPrecision || depth <= 0){
		//we have reached the maximum depth, or the precision is good enough so lets draw a quad
		vertexVector.push_back(x1);
		vertexVector.push_back(y1);
		vertexVector.push_back(x2);
		vertexVector.push_back(y2);
		vertexVector.push_back(x3);
		vertexVector.push_back(y3);
		vertexVector.push_back(x4);
		vertexVector.push_back(y4);
		
		texCoordVector.push_back(a1);
		texCoordVector.push_back(b1);
		texCoordVector.push_back(a2);
		texCoordVector.push_back(b2);
		texCoordVector.push_back(a3);
		texCoordVector.push_back(b3);
		texCoordVector.push_back(a4);
		texCoordVector.push_back(b4);
		
		++drawCount;
	} else {
		//recurse!
		
		float x12 = x1*_hmid+x2*_hmidi;
		float x34 = x4*_hmid+x3*_hmidi;
		float xm = x12*_vmid+x34*_vmidi;
		
		float y12 = y1*_hmid+y2*_hmidi;
		float y34 = y4*_hmid+y3*_hmidi;
		float ym = y12*_vmid+y34*_vmidi;
		
		float x23 = x2*_vmid+x3*_vmidi;
		float x41 = x1*_vmid+x4*_vmidi;
		
		float y23 = y2*_vmid+y3*_vmidi;
		float y41 = y1*_vmid+y4*_vmidi;
		
		float a12 = (a1+a2)*0.5;
		float a23 = (a2+a3)*0.5;
		float a34 = (a3+a4)*0.5;
		float a41 = (a4+a1)*0.5;
		float am = (a12+a34)*0.5;
		
		float b12 = (b1+b2)*0.5;
		float b23 = (b2+b3)*0.5;
		float b34 = (b3+b4)*0.5;
		float b41 = (b4+b1)*0.5;
		float bm = (b12+b34)*0.5;
		
		--depth;
		hmid = (hmid+0.5)*0.5;
		vmid = (vmid+0.5)*0.5;
		
		drawQuad(x1, y1, x12, y12, xm, ym, x41, y41,
				 a1, b1, a12, b12, am, bm, a41, b41,
				 depth, hmid, vmid);
		
		drawQuad(x12, y12, x2, y2, x23, y23, xm, ym,
				 a12, b12, a2, b2, a23, b23, am, bm,
				 depth, hmid, vmid);
		
		drawQuad(xm, ym, x23, y23, x3, y3, x34, y34,
				 am, bm, a23, b23, a3, b3, a34, b34,
				 depth, hmid, vmid);
		
		drawQuad(x41, y41, xm, ym, x34, y34, x4, y4,
				 a41, b41, am, bm, a34, b34, a4, b4,
				 depth, hmid, vmid);
	}
};

void FboWarpDrawer::reset(){
	texCoordVector.clear();
	vertexVector.clear();
	drawCount = 0;
}

void FboWarpDrawer::setColor(ofColor& c){
	glColor4f(c.r,c.g,c.b,c.a);
}

ofxFBOTexture& FboWarpDrawer::getTexture(){
	return fboTexture;
}