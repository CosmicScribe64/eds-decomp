#ifndef GUARD_SOUND_H
#define GUARD_SOUND_H

#include "global.h"

/* Konami driver state. Offsets are verified against the driver disassembly. */
struct SoundTrack {
    const u8 *data;
    u32 offset;
    u8 unk8[4];
    u16 delay;
    s16 soundId;
    u8 priority;
    u8 slotMask;
    u8 unk12;
    u8 flags;
    u8 volume;
    s8 linkedTracks;
    u8 unk16[2];
};

struct SoundDriver {
    u32 status;
    const u8 *songData;
    struct SoundTrack bgmTracks[10];
    struct SoundTrack seTracks[6];
    u16 flags;                     /* +0x188 */
    s16 pendingBgm;
    s16 pendingSe;
    s16 currentBgm;
    u8 fadeTimer;
    u8 volume;
    u8 fadeSpeed;
    u8 targetVolume;
    u8 bgmFadeSpeed;
    u8 sePriority;
    u8 seVolume;
    u8 seVariant;
};

struct SoundPcmVoice {
    const s8 *data;
    u32 remaining;
    u32 stepAndFraction;           /* +8: low u16 step, high u16 mixer fraction */
    s16 sampleId;
    u8 flags;
    u8 volume;
};

extern struct SoundDriver gUnk_03005210;
extern struct SoundTrack gUnk_03005308[6];
extern struct SoundPcmVoice gUnk_030053AC[6];

typedef char sound_track_size_check[sizeof(struct SoundTrack) == 0x18 ? 1 : -1];
typedef char sound_flags_offset_check[(u32)&((struct SoundDriver *)0)->flags == 0x188 ? 1 : -1];
typedef char sound_variant_offset_check[(u32)&((struct SoundDriver *)0)->seVariant == 0x197 ? 1 : -1];
typedef char sound_voice_size_check[sizeof(struct SoundPcmVoice) == 0x10 ? 1 : -1];

#endif
