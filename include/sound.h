#ifndef __SOUND_H
#define __SOUND_H

enum SFX {
    SFX_CANCEL,
    SFX_CONFIRM,

    SFX_COUNT
};

int sfxInit(void);
void sfxPlay(int id);
void sfxEnd(void);

#endif
