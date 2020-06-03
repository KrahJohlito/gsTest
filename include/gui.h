#ifndef __GUI_H
#define __GUI_H

void drawBackground(GSGLOBAL *gsGlobal, GSTEXTURE *gsTexture);
void drawTexture(GSGLOBAL *gsGlobal, GSTEXTURE *gsTexture, float fx, float fy);
void drawFont(GSGLOBAL *gsGlobal, GSTEXTURE *gsTexture, GSFONTM *gsFontM, float x, float y, float scale, char *text);

#endif
