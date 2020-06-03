#include <gsKit.h>
#include <gsToolkit.h>

#include "include/gui.h"

void drawBackground(GSGLOBAL *gsGlobal, GSTEXTURE *gsTexture)
{
	u64 texCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);

	gsKit_TexManager_bind(gsGlobal, gsTexture);

	gsKit_prim_sprite_texture(gsGlobal, gsTexture,
						0.0f,  // X1
						0.0f,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						gsTexture->Width, // X2
						gsTexture->Height, // Y2
						gsTexture->Width, // U2
						gsTexture->Height, // V2
						2,
						texCol);
}

void drawTexture(GSGLOBAL *gsGlobal, GSTEXTURE *gsTexture, float fx, float fy)
{
	u64 texCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);

	gsKit_TexManager_bind(gsGlobal, gsTexture);

	gsKit_prim_sprite_texture(gsGlobal, gsTexture,
						fx,  // X1
						fy - gsTexture->Height,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						fx + gsTexture->Width, // X2
						fy, // Y2
						gsTexture->Width, // U2
						gsTexture->Height, // V2
						2,
						texCol);
}

void drawFont(GSGLOBAL *gsGlobal, GSTEXTURE *gsTexture, GSFONTM *gsFontM, float x, float y, float scale, char *text)
{
	u64 WhiteFont = GS_SETREG_RGBAQ(0xFF,0xFF,0xFF,0x80,0x00);

	gsKit_TexManager_bind(gsGlobal, gsTexture);

	gsKit_fontm_print_scaled(gsGlobal, gsFontM, x, y, 3, scale, WhiteFont, text);
}
