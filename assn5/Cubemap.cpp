#include "Cubemap.hpp"
#include "Image.hpp"
#include <stdio.h>

#ifdef _WIN32
// don't complain if we use standard IO functions instead of windows-only
#pragma warning( disable: 4996 )
#endif

static GLenum format[] = {
	GL_NONE,					// no 0-channel textures
	GL_LUMINANCE,				// RGB=value0, A=1
	GL_LUMINANCE_ALPHA,			// RGB=value0, A=value1
	GL_RGB,						// R=value0, G=value1, B=value2, A=1
	GL_RGBA						// R=value0, G=value1, B=value2, A=value3
};

// translate number of channels to storage format
static GLenum storage[] = {
	GL_NONE,					// no 0-channel textures
	GL_LUMINANCE8,				// 8-bit per channel luminance texture
	GL_LUMINANCE8_ALPHA8,		// 8-bit per channel luminance-alpha texture
	GL_RGB8,					// 8-bit per channel RGB texture
	GL_RGBA8					// 8-bit per channel RGBA texture
};

Cubemap::Cubemap(const char* files_fmt, int order[6], int texture) {
	glActiveTexture(GL_TEXTURE0 + texture);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	char file[128];
	for (int i = 0; i < 6; i++) {
		Image im;
		sprintf(file, files_fmt, order[i]);
		printf("Loading cubemap texture: %s\n", file);
		FILE *fp = PAMopen(file, im);
		PAMread(fp, im);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format[im.depth], im.width, im.height, 0, format[im.depth], GL_UNSIGNED_BYTE, im.data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Cubemap::~Cubemap() {
	glDeleteTextures(1, &id);
}
