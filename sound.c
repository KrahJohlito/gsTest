#include <stdio.h>
#include <audsrv.h>
#include "include/sound.h"

extern unsigned char cancel_adp[];
extern unsigned int size_cancel_adp;

extern unsigned char confirm_adp[];
extern unsigned int size_confirm_adp;

struct sfxEffect {
	const char *name;
	void *buffer;
	int size;
};

static struct sfxEffect sfx_files[SFX_COUNT] = {
	{"cancel.adp"},
	{"confirm.adp"},
};

static struct audsrv_adpcm_t sfx[SFX_COUNT];
static int sfx_initialized = 0;

static void sfxInitDefaults(void)
{
	sfx_files[SFX_CANCEL].buffer = cancel_adp;
	sfx_files[SFX_CANCEL].size = size_cancel_adp;
	sfx_files[SFX_CONFIRM].buffer = confirm_adp;
	sfx_files[SFX_CONFIRM].size = size_confirm_adp;
}

//Returns number of audio files successfully loaded, < 0 if an unrecoverable error occurred.
int sfxInit(void)
{
	int i, ret, loaded;

	if (!sfx_initialized)
	{
		if (audsrv_init() != 0) {
			printf("SFX: Failed to initialize audsrv\n");
			printf("SFX: Audsrv returned error string: %s\n", audsrv_get_error_string());
			return -1;
		}
		sfx_initialized = 1;
	}

	audsrv_adpcm_init();

	sfxInitDefaults();

	loaded = 0;
	for (i = 0; i < SFX_COUNT; i++)
	{
		audsrv_adpcm_set_volume(i, 100);
		ret = audsrv_load_adpcm(&sfx[i], sfx_files[i].buffer, sfx_files[i].size);
		if (ret == 0)
		{
			printf("SFX: Loaded %s, size=%d\n", sfx_files[i].name, sfx_files[i].size);
			loaded++;
		}
		else
		{
			printf("SFX: failed to load %s, error %d\n", sfx_files[i].name, ret);
		}
	}

	return loaded;
}

void sfxEnd(void)
{
	if (!sfx_initialized)
	{
		printf("SFX: %s: ERROR: not initialized!\n", __FUNCTION__);
	return;
	}

	audsrv_quit();
	sfx_initialized = 0;
}

void sfxPlay(int id)
{
	if (!sfx_initialized)
	{
		printf("SFX: %s: ERROR: not initialized!\n", __FUNCTION__);
		return;
	}

	audsrv_ch_play_adpcm(id, &sfx[id]);
}
