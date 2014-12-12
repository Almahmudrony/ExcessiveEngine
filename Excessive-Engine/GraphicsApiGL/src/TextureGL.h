#pragma once

#include "IGapi.h"
#include "ITexture.h"

#include "GL/glew.h"

#define MAX_TEXTURE_SLOTS 16

//static GLuint currently_bound_textures[MAX_TEXTURE_SLOTS];
static unsigned max_anisotropy = 4; //TODO we'd need a way to set this up...
extern GLenum texture_formats[69];
extern GLenum texture_types[69];
extern unsigned texture_sizes[69];
extern GLenum texture_internal_formats[69];

class TextureGL : public ITexture
{
public:
	GLuint ID;
	GLuint viewID;
	GLenum target;
	unsigned dim;
	rTexture desc;

	void destroy() override;

	const rTexture& getDesc() override;
	void getSubData(const rTextureUpdate& d) override; //the pointer should be updated
	void genMipChain() override;
};