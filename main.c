#include <iopcontrol.h>
#include <iopheap.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <sbv_patches.h>

#include <gsKit.h>
#include <dmaKit.h>
#include <gsToolkit.h>

#include <stdio.h>
#include "include/gui.h"
#include "include/textures.h"
#include "include/sound.h"

extern unsigned char libsd_irx[];
extern unsigned int size_libsd_irx;

extern unsigned char audsrv_irx[];
extern unsigned int size_audsrv_irx;

extern void pad_init();
extern void readPad(void);

void init(GSGLOBAL *gsGlobal, GSFONTM *gsFontM)
{
	u64 Black = GS_SETREG_RGBAQ(0x00,0x00,0x00,0x00,0x00);

	// Reboot IOP
	SifInitRpc(0);
	while(!SifIopReset("", 0)){};
	while(!SifIopSync()){};

	// Initialize SIF services
	SifInitRpc(0);
	SifLoadFileInit();
	SifInitIopHeap();
	sbv_patch_enable_lmb();

	// Load Modules
	SifLoadModule("rom0:SIO2MAN", 0, NULL);
	SifLoadModule("rom0:PADMAN", 0, NULL);

	SifExecModuleBuffer(libsd_irx, size_libsd_irx, 0, NULL, NULL);
	SifExecModuleBuffer(audsrv_irx, size_audsrv_irx, 0, NULL, NULL);

	dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
			D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

	// Initialize the DMAC
	dmaKit_chan_init(DMA_CHANNEL_GIF);

	gsGlobal->DoubleBuffering = GS_SETTING_OFF;
	gsGlobal->ZBuffering = GS_SETTING_OFF;

	gsKit_init_screen(gsGlobal);
	gsKit_mode_switch(gsGlobal, GS_PERSISTENT);

	gsKit_fontm_upload(gsGlobal, gsFontM);
	gsFontM->Spacing = 0.75f;

	gsKit_clear(gsGlobal, Black);

	// Clear static loaded textures, use texture manager instead
	gsKit_vram_clear(gsGlobal);

	gsKit_set_primalpha(gsGlobal, GS_SETREG_ALPHA(0, 1, 0, 1, 128), 0);
	gsGlobal->PrimAlphaEnable = GS_SETTING_ON;

	pad_init();
	sfxInit();
}

int main(int argc, char *argv[])
{
	GSGLOBAL *gsGlobal = gsKit_init_global();
	GSFONTM *gsFontM = gsKit_init_fontm();
	GSTEXTURE gsTexture[TEXTURES_COUNT];

	init(gsGlobal, gsFontM);

	// Load all textures
	int i;
	for (i = 0; i < TEXTURES_COUNT; i++)
	{
		gsTexture[i].Delayed = 1;
		texPngLoad(&gsTexture[i], i);
		gsTexture[i].Filter = GS_FILTER_LINEAR;
	}

	drawBackground(gsGlobal, &gsTexture[BACKGROUND]);

	drawTexture(gsGlobal, &gsTexture[CROSS], 20, 50);
	drawFont(gsGlobal, &gsTexture[FONT], gsFontM, 55, 25, 0.7f, "Play Confirm ADPCM\n");

	drawTexture(gsGlobal, &gsTexture[CIRCLE], 20, 100);
	drawFont(gsGlobal, &gsTexture[FONT], gsFontM, 55, 75, 0.7f, "Play Cancel ADPCM\n");

	while(1)
	{
		gsKit_queue_exec(gsGlobal);
		gsKit_sync_flip(gsGlobal);
		gsKit_TexManager_nextFrame(gsGlobal);

		readPad();
	}

	// Exit (which I'm not doing...)
	sfxEnd();

	SifExitIopHeap();
	SifExitRpc();

	return 0;
}
