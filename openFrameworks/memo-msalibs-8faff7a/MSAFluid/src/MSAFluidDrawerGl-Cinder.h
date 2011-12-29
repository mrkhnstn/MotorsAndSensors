/***********************************************************************
 
 This class is for drawing a fluidsolver using the OpenFrameworks texture
 
 /***********************************************************************
 
 Copyright (c) 2008, 2009, 2010, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 

#pragma once

#include "MSAFluidDrawerBase.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"

namespace MSA {
	
	class FluidDrawerGl : public FluidDrawerBase {
	public:
		
		float getWidth() {
			return tex.getWidth();
		}
		
		float getHeight() {
			return tex.getHeight();
		}
		
	protected:	
		ci::Surface8u		surface;
		ci::gl::Texture		tex;

		void createTexture() {
			int texWidth = _fluidSolver->getWidth()-2;
			int texHeight =_fluidSolver->getHeight()-2;
			
			surface = ci::Surface8u( _pixels, texWidth, texHeight, false, ci::SurfaceChannelOrder::RGB );
			
			ci::gl::Texture::Format format;
			format.setInternalFormat( _glType );
			tex = ci::gl::Texture( texWidth, texHeight, format );
		}
		
		
		void updateTexture() {
			tex.update( surface );
		}
		
		void deleteTexture() {
			tex.reset();
		}
		
		void drawTexture(float x, float y, float w, float h) {
			ci::gl::draw( tex, ci::Rectf( x, y, x + w, y + h ) );
		}
	};
}
