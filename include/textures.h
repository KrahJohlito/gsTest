#ifndef __TEXTURES_H
#define __TEXTURES_H

enum INTERNAL_TEXTURE {
	BACKGROUND = 0,
	CROSS,
	CIRCLE,
	FONT,

	TEXTURES_COUNT
};

int texPngLoad(GSTEXTURE *texture, int texID);

#endif
