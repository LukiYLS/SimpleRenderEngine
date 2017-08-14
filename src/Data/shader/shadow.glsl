
//create depth shader

//cpu code
/*
	glGenTextures(1,&depth_fbo);
	glBindTexture(GL_TEXURE_2D,depth_fbo);
	glTexImage2D();
	
	
	glGenFramebuffers(1,&depth_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER,depth_fbo);
	
	glFrambufferTexture();
	glDrawBuffer(GL_NONE);
*/

#version 330
