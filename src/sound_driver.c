#include "global.h"
#include "sound.h"

/* Konami sound driver, compiled with agbcc -O2 -fprologue-bugfix.
 * The sequencer and mixer retain their assembly until they match in C. */
#define REG_SOUND3CNT_L (*(vu16 *)0x04000070)
#define REG_WAVE_RAM0 (*(vu32 *)0x04000090)
#define REG_WAVE_RAM1 (*(vu32 *)0x04000094)
#define REG_WAVE_RAM2 (*(vu32 *)0x04000098)
#define REG_WAVE_RAM3 (*(vu32 *)0x0400009C)
extern const u32 gUnk_08139550[][4];
void sub_0807D3D0(void);
void sub_0807E324(void);
void sub_0807DB58(struct SoundDriver *p);
void __sub_0807EAD0_from_thumb(void);
void sub_0807E3D8(void);
void sub_0807E554(void);
void sub_0807E674(s32 id);
void sub_0807E814(s32 id);
void sub_0807E9C8(s32 id, s32 ticks);

struct SoundSample {
    s32 rate;
    u32 length;
    s32 loopStart;
    s8 data[1];
};
extern const struct SoundSample *const gUnk_08088A20[];
extern const struct SoundSample *const gUnk_0811B420[];
extern const u16 gUnk_081A960C[];
struct SoundDmaState {
    u16 position;
    u16 unk2;
    u16 previousPosition;
    u16 unk6;
};
extern struct SoundDmaState gUnk_0300540C;
extern s8 gUnk_03005414[0x640];
struct SoundDmaRegs {
    volatile u32 source;
    volatile u32 destination;
    u16 count;
    volatile u16 flags;
};
struct SoundSong {
    u16 dataLo;
    u16 dataHi;
    u16 offset[10];
};
extern const struct SoundSong gUnk_080E09D0[];
extern u32 gUnk_03005A54[];
void sub_0807EC1C(void);
struct SoundEffect {
    const u8 *tracks[6];
    u8 priority;
    u8 slots;
    u16 lock;
};
extern const struct SoundEffect gUnk_08087FD0[];
extern const u8 gUnk_081A79F9[];

void sub_0807D3D0(void)
{
  struct SoundPcmVoice *voice;
  struct SoundDmaState *state;
  s8 *buffer;
  int i;
  u32 zero;
  *((vu16 *) 0x04000200) &= 0xF9F7;
  {
    vu16 *dma1 = (vu16 *) 0x040000BC;
    vu16 *dma2;
    dma1[5] &= 0xC5FF;
    dma1[5] &= 0x7FFF;
    dma1[5];
    dma2 = (vu16 *) 0x040000C8;
    dma2[5] &= 0xC5FF;
    dma2[5] &= 0x7FFF;
    dma2[5];
  }
  *((vu32 *) 0x040000C4) = 0;
  *((vu32 *) 0x040000D0) = 0;
  {
    vu32 *dma = (vu32 *) 0x040000D4;
    dma[0] = (u32) sub_0807EC1C;
    dma[1] = (u32) gUnk_03005A54;
    dma[2] = 0x84000038;
    dma[2];
  }
  {
    vu32 *dma = (vu32 *) 0x040000D4;
    while (dma[2] & 0x80000000)
    {
      ;
    }

  }
  voice = gUnk_030053AC;
  state = &gUnk_0300540C;
  buffer = gUnk_03005414;
  {
    struct SoundPcmVoice *iter = voice;
    i = 6;
    do
    {
      iter->flags = 0;
      iter->sampleId = 0;
      iter++;
    }
    while ((--i) != 0);
  }
  /* Keep the initial voice base live through the reset loop. This empty
   * constraint emits no instructions and preserves all values. */
  asm volatile ("" : : "r"(voice));
  *((u32 *) state) = 0;
  *((u32 *) (&state->previousPosition)) = 0;
  {
    vu32 *dma = (vu32 *) 0x040000D4;
    zero = 0;
    dma[0] = (u32) (&zero);
    dma[1] = (u32) buffer;
    dma[2] = 0x85000190;
    dma[2];
  }
  {
    vu32 *dma = (vu32 *) 0x040000D4;
    while (dma[2] & 0x80000000)
    {
      ;
    }

  }
  *((vu8 *) 0x04000083) = 0xBB;
  i = 8;
  do
  {
    *((vu32 *) 0x040000A0) = 0;
    *((vu32 *) 0x040000A4) = 0;
  }
  while ((--i) != 0);
  {
    vu32 *dma = (vu32 *) 0x040000BC;
    do {
      dma[0] = (u32) buffer;
      dma[1] = 0x040000A0;
      dma[2] = 0xF6000004;
      dma[2];
      dma += 3;
    } while (0);
    dma[0] = (u32) (buffer + 0x320);
    dma[1] = 0x040000A4;
    dma[2] = 0xF6000004;
    dma[2];
  }
  *((vu16 *) 0x04000200) |= 0x208;
  *((vu32 *) 0x04000100) = 0x0080FCB9;
}
void sub_0807D518(struct SoundDriver *p, u32 a, u32 b)
{
    const u32 *src = gUnk_08139550[a * 16 + b];
    u16 bank;
    REG_WAVE_RAM0 = src[0];
    REG_WAVE_RAM1 = src[1];
    REG_WAVE_RAM2 = src[2];
    REG_WAVE_RAM3 = src[3];
    bank = 0;
    if (!(p->flags & 0x200))
        bank = 0x40;
    p->flags ^= 0x200;
    REG_SOUND3CNT_L = bank | 0x80;
}

void sub_0807D578(void (**dma1Slot)(void))
{
    struct SoundDriver *p;
    struct SoundTrack *track;
    int i;

    *(vu16 *)0x04000084 = 0x80;
    *(vu16 *)0x04000060 = 0;
    *(vu16 *)0x04000062 = 0;
    *(vu16 *)0x04000064 = 0x8000;
    *(vu16 *)0x04000068 = 0;
    *(vu16 *)0x0400006C = 0x8000;
    *(vu16 *)0x04000070 = 0;
    *(vu16 *)0x04000072 = 0x2000;
    *(vu16 *)0x04000074 = 0;
    *(vu16 *)0x04000078 = 0;
    *(vu16 *)0x0400007C = 0;
    *(vu16 *)0x04000200 &= 0xF9F7;
    *(vu16 *)0x04000080 = 0xFF77;
    *(vu16 *)0x04000082 = 0xE;
    *(vu16 *)0x04000088 = (*(vu16 *)0x04000088 & 0x3FFF) | 0x4000;
    if (dma1Slot != 0)
        *dma1Slot = sub_0807E324;
    p = &gUnk_03005210;
    p->flags = 0;
    p->pendingBgm = 0xFFFF;
    p->pendingSe = -1;
    p->fadeTimer = 0;
    p->volume = 0x10;
    p->targetVolume = 0x10;
    p->fadeSpeed = 0x10;
    p->status = -1;
    track = p->bgmTracks;
    i = 10;
    do {
        track->priority = 0;
        i--;
        track++;
    } while (i != 0);
    p->sePriority = 0;
    i = 5;
    track = &p->seTracks[5];
    do {
        track->flags = 0;
        track->data = 0;
        track--;
    } while (--i >= 0);
    sub_0807D518(p, 0, 0);
    *(vu16 *)0x04000074 = 0x8000;
    sub_0807D3D0();
}

union SoundSeChannelState {
    u16 packed;
    struct {
        u8 flags;
        u8 volume;
    } __attribute__((packed)) bytes;
} __attribute__((packed, aligned(2)));

/* SE-specific view of the shared 0x18-byte track. */
struct SoundSeTrack {
    const u8 *data;
    const u8 *returnData;
    u16 basePitch;
    union SoundSeChannelState channel;
    u16 delay;
    s16 soundId;
    u8 priority;
    u8 slotMask;
    u8 loopCounter;
    u8 flags;
    u8 volume;
    s8 linkedTracks;
    u8 unk16[2];
};

struct SoundChannelParams {
    u16 pitch;
    u8 envelope;
    u8 volume;
    u8 dirty;
    u8 command;
    s16 sampleId;
};
extern const u8 gUnk_081A79E8[];
typedef char se_track_size_check[sizeof(struct SoundSeTrack) == 0x18 ? 1 : -1];
typedef char channel_params_size_check[sizeof(struct SoundChannelParams) == 8 ? 1 : -1];
typedef char se_channel_state_size_check[sizeof(union SoundSeChannelState) == 2 ? 1 : -1];
typedef char se_channel_offset_check[(u32)&((struct SoundSeTrack *)0)->channel == 0xA ? 1 : -1];
typedef char se_delay_offset_check[(u32)&((struct SoundSeTrack *)0)->delay == 0xC ? 1 : -1];

/* Advances SE track `idx` by one tick and decodes its bytecode into `out`.
 * The bytecode cursor `p` and the shared stream value `data` follow the
 * original register use; commands 0x00-0x1F re-dispatch the same command. */
void sub_0807D6B4(s32 idx, struct SoundChannelParams *out) {
    struct SoundDriver *driver = &gUnk_03005210;
    struct SoundSeTrack *track = (struct SoundSeTrack *)&driver->seTracks[idx];
    const u8 *p;
    s32 data;
    s32 tmp;

    if (!(track->flags & 0x80))
        return;
    *(u16 *)&out->dirty = 0;
    p = track->data;
    if (track->flags & 8) {
        if (track->linkedTracks < 0)
            return;
    clear_linked:
        for (tmp = track->linkedTracks; tmp != 0; tmp--)
            track[tmp].linkedTracks = 0;
        track->delay = 1;
        p = NULL;
        track->flags = 0;
        goto stop;
    }
    if (driver->flags & 4) {
        track->flags |= 8;
        goto clear_linked;
    }
    if (!(track->flags & 0x40)) {
        track->flags |= 0x40;
        track->linkedTracks = 0;
        track->delay = 0;
        track->channel.bytes.volume = 0;
        track->returnData = NULL;
        goto stop;
    }
    out->volume = track->channel.bytes.volume;
    if (track->flags & 1) {
        if (idx > 1) {
            idx = gUnk_081A79E8[idx];
            if (gUnk_030053AC[idx].flags & 0x80)
                goto scale;
        }
        track->flags &= 0xFE;
        out->command = 0x40;
        track->delay = 1;
    }
    if (--track->delay != 0)
        return;
    if (track->flags & 2) {
        track->flags &= 0xFD;
        out->command = 0x40;
    }
top:
    if (track->flags & 0x10)
        track->delay = 1;
    data = *p++;
    if (data > 0xEF) {
        data = (data & 0xF) << 8;
        data += *p++;
    }
    track->delay = data;
    data = *p++;
    for (;;) {
        if (data > 0xEF) {
            if (data == 0xFF) {
                if (track->returnData != NULL) {
                    p = track->returnData;
                    track->returnData = NULL;
                    goto top;
                }
                track->flags |= 8;
                goto stop;
            } else if (data == 0xFE) {
                out->command = 0x40;
                break;
            } else if (data == 0xFD) {
            stop:
                out->pitch = 0;
                *(u16 *)&out->envelope = 0;
                out->command = 0x40;
                out->dirty = 0x40;
                goto next;
            } else if (data == 0xFC) {
                tmp = *p++;
                if (tmp == 0) {
                    if (track->flags & 4) {
                        track->flags &= 0xFB;
                        p += 4;
                        goto top;
                    }
                } else if (track->flags & 0x20) {
                    if (--track->loopCounter == 0) {
                        track->flags &= 0xDF;
                        p += 4;
                        goto top;
                    }
                } else {
                    track->loopCounter = tmp;
                    track->flags |= 0x20;
                }
                /* A separate copy of the pointer read (cross-jumped into the
                 * FA/FB copy) gives `p` the reference weight that puts it in r5. */
                p = (const u8 *)(p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24));
                goto top;
            } else if (data == 0xFB) {
                goto jump;
            } else if (data > 0xF9) {
                track->returnData = p + 4;
            jump:
                p = (const u8 *)(p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24));
                goto top;
            } else if (data == 0xF9) {
            finish:
                track->flags |= 1;
                goto end;
            }
            goto next;
        } else if (data > 0x9F) {
            s32 c = data;
            s32 x;
            if (data > 0xBF) {
                if (data > 0xDF) {
                    data = p[0] + (p[1] << 8);
                    out->pitch = data >> 4;
                    data &= 0xF;
                    goto set_state;
                } else if (data > 0xCF) {
                    s32 y = (s16)(p[0] | (p[1] << 8)) + track->basePitch;
                    out->pitch = y;
                    data = p[2];
                    p++;
                } else {
                    data = p[0] | (p[1] << 8);
                    out->pitch = (s16)track->basePitch - 0x400 + (data >> 5);
                }
                data &= 0x1F;
                x = data + track->channel.bytes.volume;
            } else if (data > 0xAF) {
                data = p[0] + (p[1] << 8);
                out->pitch = (data >> 4) | 0x4000;
                data &= 0xF;
                goto set_state;
            } else {
                tmp = p[0] | (p[1] << 8);
                data = (tmp >> 5) - 0x400;
                data += (s16)((s16)track->basePitch & ~0x4000);
                out->pitch = data | 0x4000;
                x = (tmp & 0x1F) + track->channel.bytes.volume;
            }
            data = x - 0x10;
            if (data < 0)
                data = 0;
            if (data > 0xF)
                data = 0xF;
        set_state:
            tmp = (c & 3) | (data << 8);
            if (!(c & 8)) {
                if (track->channel.packed != tmp) {
                    track->channel.packed = tmp;
                    out->dirty = 2;
                }
                out->command = 2;
                *(u16 *)&out->envelope = tmp;
            }
            if (!(c & 4)) {
                track->basePitch = out->pitch;
                if (track->channel.packed != tmp)
                    tmp |= 8;
                track->channel.packed = tmp;
            }
            p += 2;
            goto next;
        } else if (data > 0x7F) {
            track->channel.bytes.volume = out->volume = data & 0xF;
            tmp = *p;
            track->channel.bytes.flags = tmp;
            p++;
            if (data > 0x8F) {
                out->pitch = tmp | 0x8000;
                out->command = 2;
                out->dirty = 2;
            }
            goto next;
        } else if (data > 0x6F) {
            data &= 0xF;
            driver->seTracks[data].unk12 = 1;
            track->data = p;
            goto next;
        } else if (data > 0x5F) {
            tmp = p[0] + (p[1] << 8);
            out->sampleId = tmp | 0x8000;
            out->volume = p[2];
            {
                u8 v = p[2];
                track->channel.bytes.volume = v;
                tmp = v;
            }
            p += 3;
            track->basePitch = 0;
            if (data & 4) {
                track->basePitch = p[0] | (p[1] << 8);
                p += 2;
            }
            out->pitch = track->basePitch;
            out->command = 0x80;
            out->dirty = 2;
            tmp >>= 4;
            track->linkedTracks = tmp;
            for (; tmp != 0; tmp--) {
                s32 t; /* FAKEMATCH: an int temporary keeps the OR; |= 0xFF folds to a store */
                track[tmp].flags |= 0x88;
                t = (u8)track[tmp].linkedTracks;
                t |= 0xFF;
                track[tmp].linkedTracks = t;
                track[tmp].data = NULL;
            }
            if (data & 8) {
                if (track->delay == 0)
                    goto finish;
                track->flags |= 2;
            }
            goto next;
        } else if (data > 0x4F) {
            data &= 0xF;
        set_vol:
            track->channel.bytes.volume = data;
        set_out:
            out->volume = data;
            out->dirty = 2;
            goto next;
        } else if (data > 0x3F) {
            tmp = data;
            data = (s8)p[0] + track->channel.bytes.volume;
            if (data < 0)
                data = 0;
            if (data > 0x3F)
                data = 0x3F;
            p++;
            if (tmp & 1)
                goto set_out;
            goto set_vol;
        } else if (data > 0x2F) {
            tmp = ((data & 0xF) << 8) | *p;
            p++;
            out->pitch = tmp;
            track->basePitch = tmp;
            out->command = 2;
            goto next;
        } else if (data > 0x1F) {
            tmp = (((data & 7) << 8) | p[0]) + -0x400;
            if (data & 8) {
                track->basePitch += tmp;
                tmp = 0;
            }
            out->pitch = track->basePitch + tmp;
            out->command = 2;
            goto next;
        } else if (data > 0xF) {
            tmp = data & 7;
            driver->seTracks[tmp].flags &= 0xEF;
            if (data & 8)
                driver->seTracks[tmp].flags |= 0x10;
            track->data = p;
        }
    }
next:
    if (track->delay == 0)
        goto top;
end:
    track->data = p;
scale:
    out->volume = (out->volume * track->volume) >> 4;
}

#if 0 /* NONMATCHING (score 26): NONMATCHING (score 26): structured rewrite. Generic s32 temps a (r4) and b (r5)
       * reused across the whole function are essential. Remaining: reset loop (for i=0..9, reversed by loop.c) hoists a
       * dead QI zero (r7) and the 0xFE constant, and the post-loop 0x04000072 zero lands in r1 instead of r7. int temps
       * avoid HImode/QImode narrowing of masks and MMIO values; chained t[4]=t[6]=t[8]=t[5]=t[7]=t[9]=0x33 routing;
       * array-decl vs pointer-arith table forms steer constant-pool load placement. */
struct SoundBgmTrack {
    u16 pitch;
    u8 instrument;
    u8 channelVolume;
    u16 songOffset;
    u16 position;
    u16 returnPosition;
    u16 returnSongOffset;
    u16 delay;
    u8 vibratoPhase;
    u8 vibratoDepth;
    u8 flags;
    s8 fadeCounter;
    u8 fadeVolume;
    u8 loopCounter;
    u8 routing;
    u8 unk15[3];
};
struct SoundTickOut {
    s16 pitch;
    u8 envelope;
    u8 volume;
    u8 dirty;
    u8 command;
    u16 sampleId;
};
extern const u16 gUnk_08139F50[];
extern const u16 gUnk_081AA20C[];
extern const s16 gUnk_081ABC4C[];
void sub_0807D6B4(s32 index, struct SoundChannelParams *output);
void sub_0807E918(struct SoundPcmVoice *voice, s32 id, s32 volume, s32 note);
struct SoundDriverTick {
    struct SoundDriver base;
    u8 routing;
};
typedef char bgm_track_size_check[sizeof(struct SoundBgmTrack) == 0x18 ? 1 : -1];

void sub_0807DB58(struct SoundDriver *p)
{
    struct SoundTickOut out[10];
    struct SoundTickOut *o;
    struct SoundBgmTrack *track;
    const u8 *songData;
    const u8 *cmdp;
    s32 i;
    s32 a;
    s32 b;
    u16 flags;
    u8 oldVolume;
    u32 pos;

    a = p->targetVolume << 8;
    b = a - *(u16 *)&p->fadeTimer;
    p->flags &= 0xFBF7;
    oldVolume = p->volume;
    if (b != 0) {
        if (b > 0) {
            b -= p->fadeSpeed << 4;
            if (b < 0)
                b = 0;
        } else {
            b += p->fadeSpeed << 4;
            if (b > 0)
                b = 0;
        }
        *(u16 *)&p->fadeTimer = a - b;
        if (oldVolume != p->volume)
            p->flags |= 8;
    } else if (oldVolume & 0xF) {
        p->flags |= 0x400;
    }

    if ((p->flags & 0x100) && oldVolume == 0) {
        o = &out[9];
        for (i = 9; i >= 0; i--) {
            ((u32 *)o)[0] = 0;
            ((u32 *)o)[1] = 0;
            o--;
        }
    } else {
        flags = p->flags;
        songData = p->songData;
        track = (struct SoundBgmTrack *)p->bgmTracks;
        if (flags & 1) {
            o = out;
            for (i = 9; i >= 0; i--) {
                track->flags = 0;
                track++;
                ((u32 *)o)[0] = 0;
                o->dirty = 1;
                o->command = 0x40;
                o++;
            }
            {
                s32 t = flags & ~0x4081;
                p->flags = t;
            }
            p->status = -1;
        } else {
            if (!(flags & 0xC0))
                return;
            if (!(flags & 0x4000)) {
                p->flags = flags | 0x4000;
            reset:
                p->status = 0;
                track = (struct SoundBgmTrack *)p->bgmTracks;
                track[0].routing = 0x11;
                track[1].routing = 0x22;
                track[2].routing = 0x44;
                track[3].routing = 0x88;
                track[4].routing = track[6].routing = track[8].routing = track[5].routing = track[7].routing = track[9].routing = 0x33;
                *(vu8 *)0x04000081 = 0xFF;
                *(vu16 *)0x04000082 = 0x330E;
                for (i = 0; i < 10; i++) {
                    track->flags &= 0xFE;
                    if (track->flags & 0x40)
                        track->flags |= 0x80;
                    track->flags &= 0xC0;
                    track->position = 0;
                    track->returnPosition = 0;
                    *(u16 *)&track->instrument = 0;
                    *(u16 *)&track->vibratoPhase = 0;
                    track++;
                }
                ((struct SoundBgmTrack *)p->bgmTracks)[1].instrument = 0x80;
                ((struct SoundBgmTrack *)p->bgmTracks)[0].instrument = 0x80;
                *(vu16 *)0x04000072 = 0;
                sub_0807D518(p, 0, 0);
            }
            p->status++;
            o = &out[9];
            track = &((struct SoundBgmTrack *)p->bgmTracks)[9];
            for (i = 9; i >= 0; i--, track--, o--) {
                ((u32 *)o)[0] = 0;
                ((u32 *)o)[1] = 0;
                if (track->flags & 0x80) {
                    if (!(track->flags & 1)) {
                        track->flags |= 1;
                        pos = 0;
                        goto note_off;
                    }
                    if (--track->delay == 0) {
                        pos = track->position;
                        loop:
                            cmdp = songData + track->songOffset + pos;
                            pos++;
                            a = cmdp[0];
                            if (a > 0xFC) {
                                if (a == 0xFF) {
                                    p->flags |= 1;
                                } else if (a == 0xFE) {
                                    goto reset;
                                } else {
                                    track->flags &= 0x40;
                                    if (*(u16 *)&o->dirty != 0)
                                        goto next;
                                }
                                track->channelVolume = 0;
                                *(u16 *)&o->envelope = 0;
                                o->pitch = track->pitch;
                                o->command = 0x40;
                                o->dirty = 0x40;
                                goto next;
                            } else if (a > 0xEF) {
                                if (a == 0xF3) {
                                    track->songOffset += pos;
                                    pos = 0;
                                } else if (a == 0xF2) {
                                    pos++;
                                    b = cmdp[1] - 0x40 + (s16)track->pitch;
                                    o->sampleId = track->instrument;
                                    o->pitch = b;
                                    o->command = 1;
                                } else if (a > 0xF0) {
                                    pos++;
                                    track->vibratoDepth = cmdp[1] >> 1;
                                    track->flags |= 0x20;
                                } else if (a == 0xF0) {
                                    pos++;
                                    b = cmdp[1];
                                    if (i > 3) {
                                        a = b & 0xF;
                                        o[1].volume = b >> 4;
                                        o[1].dirty = 1;
                                        goto set_volume;
                                    }
                                    track->routing = b;
                                }
                            } else if (a > 0xDF) {
                            note_off:
                                a = 0;
                                o->pitch = track->pitch;
                                o->command = 0x40;
                            set_volume:
                                track->channelVolume = a;
                                o->envelope = track->instrument;
                                o->dirty = 1;
                            } else if (a > 0xCF) {
                                a = a & 0xF;
                                pos++;
                                track->pitch = o->pitch = cmdp[1] << 5;
                                o->command = 1;
                                if (a == track->channelVolume)
                                    goto check_loop;
                                goto set_volume;
                            } else if (a > 0xBF) {
                                a = a & 0xF;
                                o->pitch = track->pitch;
                                o->command = 1;
                                goto set_volume;
                            } else if (a > 0x9F) {
                                u8 ins;
                                track->channelVolume = a & 0xF;
                                ins = cmdp[1];
                                o->sampleId = ins;
                                track->instrument = ins;
                                pos++;
                                b = 0;
                                if (a > 0xAF) {
                                    b = (s8)cmdp[2] << 5;
                                    pos++;
                                }
                                track->pitch = b;
                                o->pitch = b;
                                *(u16 *)&o->envelope = *(u16 *)&track->instrument;
                                o->command = 0x80;
                            } else if (a > 0x8F) {
                                u8 count;
                                track->returnSongOffset = track->songOffset;
                                track->returnPosition = pos + 3;
                                b = p->currentBgm * 12 + i + 2;
                                {
                                    const u16 *tbl = (const u16 *)gUnk_080E09D0;
                                    track->songOffset = tbl[b];
                                }
                                count = cmdp[3];
                                track->loopCounter = count;
                                pos = cmdp[1] | (cmdp[2] << 8);
                                a = a & 0xF;
                                if (a == 0xF)
                                    goto read_delay;
                                track->loopCounter = count + 1;
                                goto wave;
                            } else if (a > 0x7F) {
                                a = a & 0xF;
                            wave:
                                if (a > 3) {
                                    a -= 4;
                                    sub_0807D518(p, a, track->channelVolume);
                                }
                                o->envelope = a;
                                track->instrument = a;
                            }
                        check_loop:
                            if (track->returnPosition != 0) {
                                if (--track->loopCounter == 0) {
                                    track->songOffset = track->returnSongOffset;
                                    pos = track->returnPosition;
                                }
                            }
                        read_delay:
                            cmdp = songData + track->songOffset + pos;
                            a = cmdp[0];
                            pos++;
                            if (a > 0xEF) {
                                a = (a & 0xF) << 8;
                                a += cmdp[1];
                                pos++;
                            }
                            track->position = pos;
                            track->delay = a;
                            if ((u16)a == 0)
                                goto loop;
                    }
                }
                {
                    b = track->flags;
                    if (b & 4) {
                        a = track->fadeCounter - 1;
                        if (a <= 0) {
                            a = 0;
                            {
                                s32 t = b & 0xFB;
                                track->flags = t;
                            }
                            o->command = 0x40;
                        }
                        track->fadeCounter = a;
                        track->channelVolume = (track->fadeVolume * a) >> 2;
                        o->pitch = track->pitch;
                        *(u16 *)&o->envelope = *(u16 *)&track->instrument;
                        o->dirty = 1;
                    }
                    if (b & 0x20) {
                        if (track->vibratoDepth == 0) {
                            track->flags &= 0xDF;
                            track->vibratoPhase = 0;
                            a = 0;
                        } else {
                            track->vibratoPhase += 0x18;
                            a = (gUnk_081ABC4C[track->vibratoPhase] * track->vibratoDepth) >> 12;
                        }
                        o->pitch = track->pitch + a;
                        *(u16 *)&o->envelope = *(u16 *)&track->instrument;
                        o->sampleId = track->instrument;
                        o->command = 1;
                    }
                }
                o->volume = (track->channelVolume * p->volume) >> 4;
                if ((p->flags & 0x408) && track->channelVolume != 0) {
                    if (o->command == 0)
                        o->pitch = track->pitch;
                    o->envelope = track->instrument;
                    o->dirty = 1;
                }
            next:;
            }
            track = (struct SoundBgmTrack *)p->bgmTracks;
            ((struct SoundDriverTick *)p)->routing = track[0].routing | track[1].routing | track[2].routing | track[3].routing;
        }
    }

    if (p->flags & 0x40) {
        struct SoundTrack *se;
        if ((s8)--p->sePriority < 0)
            p->sePriority = 0;
        se = p->seTracks;
        sub_0807D6B4(0, (struct SoundChannelParams *)&out[1]);
        sub_0807D6B4(1, (struct SoundChannelParams *)&out[3]);
        sub_0807D6B4(2, (struct SoundChannelParams *)&out[9]);
        sub_0807D6B4(3, (struct SoundChannelParams *)&out[8]);
        sub_0807D6B4(4, (struct SoundChannelParams *)&out[7]);
        sub_0807D6B4(5, (struct SoundChannelParams *)&out[6]);
        b = 0;
        for (i = 5; i >= 0; i--) {
            b |= se->flags;
            se++;
        }
        if (!(b & 0x80)) {
            p->sePriority = 0;
            p->flags &= 0xFFBF;
        }
    } else {
        p->sePriority = 0;
    }
    *(vu8 *)0x04000081 = ((struct SoundDriverTick *)p)->routing;

    o = out;
    if (*(u16 *)&o[0].dirty != 0) {
        b = gUnk_081AA20C[o[0].pitch];
        if (o[0].dirty != 0) {
            {
                s32 t = (o[0].volume << 12) | o[0].envelope;
                *(vu16 *)0x04000062 = t;
            }
            *(vu16 *)0x04000064 = b;
        } else {
            *(vu16 *)0x04000064 = b & 0x7FF;
        }
    }
    if ((b = *(u16 *)&o[1].dirty) != 0) {
        b = o[1].pitch;
        if (b < 0 || !(b & 0x4000))
            b = gUnk_081AA20C[b];
        else
            b = (b & ~0x4000) | 0x8000;
        if (o[1].dirty != 0) {
            {
                s32 t = (o[1].volume << 12) | o[1].envelope;
                *(vu16 *)0x04000068 = t;
            }
            *(vu16 *)0x0400006C = b;
        } else {
            *(vu16 *)0x0400006C = b & 0x7FF;
        }
    }
    if (*(u16 *)&o[2].dirty != 0) {
        b = gUnk_081AA20C[o[2].pitch] & 0x7FF;
        if (o[2].volume == 0) {
            *(vu16 *)0x04000072 = 0;
        } else {
            sub_0807D518(p, o[2].envelope, o[2].volume);
            *(vu16 *)0x04000072 = 0x2000;
        }
        *(vu16 *)0x04000074 = b;
    }
    if ((b = *(u16 *)&o[3].dirty) != 0) {
        a = o[3].volume << 12;
        if (!(b & 0x202)) {
            *(vu16 *)0x04000078 = a;
            *(vu16 *)0x0400007C = gUnk_08139F50[o[3].pitch];
        } else {
            *(vu16 *)0x04000078 = a;
            *(vu16 *)0x0400007C = o[3].pitch;
        }
    }

    {
        struct SoundPcmVoice *voice;
        o += 9;
        voice = &gUnk_030053AC[5];
        for (i = 5; i >= 0; i--) {
            b = o->command;
            if (b != 0) {
                if (b & 0x80) {
                    sub_0807E918(voice, o->sampleId, o->volume, o->pitch);
                } else if (b & 0x40) {
                    voice->flags = 0;
                } else {
                    const struct SoundSample *s;
                    if (o->sampleId & 0x8000)
                        s = gUnk_08088A20[o->sampleId & 0x3FFF];
                    else
                        s = gUnk_0811B420[o->sampleId];
                    *(u16 *)&voice->stepAndFraction = (*(gUnk_081A960C + o->pitch) * s->rate) >> 12;
                }
            }
            if (o->dirty != 0)
                voice->volume = o->volume;
            voice--;
            o--;
        }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/sound_driver", sub_0807DB58); /* 0x0807DB58 size 0x7CC */

void sub_0807E324(void)
{
    struct SoundDmaState *state = &gUnk_0300540C;
    int position = state->position + 0x10;
    if (position > 0x2BF) {
        struct SoundDmaRegs *dma1 = (struct SoundDmaRegs *)0x040000BC;
        struct SoundDmaRegs *dma2;
        *(vu16 *)((u8 *)dma1 + 10) &= 0xC5FF;
        *(vu16 *)((u8 *)dma1 + 10) &= 0x7FFF;
        *(vu16 *)((u8 *)dma1 + 10);
        dma2 = (struct SoundDmaRegs *)0x040000C8;
        *(vu16 *)((u8 *)dma2 + 10) &= 0xC5FF;
        *(vu16 *)((u8 *)dma2 + 10) &= 0x7FFF;
        *(vu16 *)((u8 *)dma2 + 10);
        dma1->source = (u32)gUnk_03005414;
        dma1->destination = 0x040000A0;
        *(vu32 *)&dma1->count = 0xF6000004;
        *(vu32 *)&dma1->count;
        dma2->source = (u32)(gUnk_03005414 + 0x320);
        dma2->destination = 0x040000A4;
        *(vu32 *)&dma2->count = 0xF6000004;
        *(vu32 *)&dma2->count;
        position = 0;
    }
    state->previousPosition = position;
    state->position = position;
}

void sub_0807E3B0(void)
{
    struct SoundDriver *p = &gUnk_03005210;
    if (!(p->flags & 0x2000)) {
        sub_0807DB58(p);
        __sub_0807EAD0_from_thumb();
    }
}

void sub_0807E3D8(void)
{
    struct SoundDriver *p = &gUnk_03005210;
    const struct SoundEffect *effect;
    s32 metadata;
    int priority;
    int slots;
    int variant;
    const u8 *map;
    int overlap;
    int bits;
    register struct SoundTrack *track asm("r3");
    int i;
    int id;
    int request;
    u8 *volume;
    request = p->pendingSe;
    if (request < 0)
        return;
    effect = &gUnk_08087FD0[request & 0xFFF];
    metadata = *(const s32 *)&effect->priority;
    /* These empty constraints emit no instructions and preserve initialized
     * values. Keep the first request through metadata loading, the variant
     * through cursor setup, and the second signed request through allocation
     * and final flag updates to retain the ROM register lifetimes. */
    asm volatile ("" : : "r"(effect), "r"(request));
    priority = metadata & 0xFF;
    if ((metadata >> 16) != 0 && (s8)p->sePriority != 0)
        goto done;
    slots = (metadata >> 8) & 0xFF;
    variant = p->seVariant;
    map = &gUnk_081A79F9[variant * 6];
    if (variant != 0) {
        bits = slots >> variant;
        slots &= 0x30;
        slots |= bits;
    }
    overlap = 0;
    bits = slots;
    track = &p->seTracks[5];
    asm volatile ("" : "+r"(variant));
    i = 5;
    do {
        if ((bits & 1) && (track->flags & 0x80)) {
            if (track->priority > priority)
                goto done;
            overlap |= track->slotMask;
        }
        bits >>= 1;
        track--;
    } while (--i >= 0);
    overlap &= ~slots;
    p->sePriority = effect->lock;
    id = p->pendingSe;
    asm volatile ("" : : "r"(id));
    id &= 0xFFF;
    bits = slots;
    i = 5;
    track = &p->seTracks[5];
    volume = &p->seVolume;
    do {
        if (bits & 1) {
            track->soundId = id;
            track->priority = priority;
            track->slotMask = slots;
            track->data = effect->tracks[*map];
            track->volume = *volume;
            track->flags = -0x80;
        }
        map--;
        asm volatile ("" : : "r"(id));
        track--;
        bits >>= 1;
    } while (--i >= 0);
    track = &p->seTracks[5];
    if (overlap != 0) {
        do {
            /* The ROM repeatedly marks slot 5 without advancing track. */
            if (overlap & 1)
                track->flags |= 8;
            overlap >>= 1;
        } while (overlap != 0);
    }
    p->flags = (p->flags & 0xFFFB) | 0x40;
    asm volatile ("" : : "r"(id));
done:
    p->pendingSe = 0xFFFF;
}

void sub_0807E554(void)
{
    struct SoundDriver *p;
    int song;
    sub_0807E3D8();
    p = &gUnk_03005210;
    song = p->pendingBgm;
    if (song >= 0) {
        struct SoundTrack *track;
        const u16 *header;
        /* Re-read the request for the compare to retain the ROM's register lifetimes. */
        if ((p->flags & 0x80) && p->currentBgm != p->pendingBgm) {
            if (p->volume != 0) {
                p->fadeSpeed = 0x10;
                p->targetVolume = 0;
                return;
            }
            p->fadeSpeed = 0x10;
        }
        if (p->bgmFadeSpeed == 0) {
            p->volume = 0x10;
            p->targetVolume = 0x10;
        } else {
            p->fadeSpeed = p->bgmFadeSpeed;
            p->targetVolume = 0x10;
        }
        p->fadeTimer = 0;
        p->currentBgm = p->pendingBgm;
        p->pendingBgm = 0xFFFF;
        track = p->bgmTracks;
        header = (const u16 *)&gUnk_080E09D0[song];
        p->songData = (const u8 *)(header[0] | header[1] << 16);
        header += 2;
        song = 10;
        do {
            track->priority = 0xC0; /* BGM tracks use +0x10 as flags. */
            *(u16 *)&track->offset = *header;
            song--;
            header++;
            track++;
        } while (song != 0);
        p->pendingBgm = 0xFFFF;
        p->flags = (p->flags & 0xBFFF) | 0x80;
    }
}

void sub_0807E674(s32 id)
{
    struct SoundDriver *p = &gUnk_03005210;
    p->pendingBgm = id;
    p->bgmFadeSpeed = 0;
}

void sub_0807E690(s32 id, s32 b)
{
    struct SoundDriver *p = &gUnk_03005210;
    p->pendingBgm = id;
    p->bgmFadeSpeed = b;
}

int sub_0807E6B0(s32 id)
{
    struct SoundDriver *p = &gUnk_03005210;
    if (p->flags & 0x80)
        return p->currentBgm == id;
    return 0;
}

void sub_0807E6E8(s32 id)
{
    struct SoundDriver *p = &gUnk_03005210;
    if (!(p->flags & 0x80) || p->currentBgm != id) {
        p->pendingBgm = id;
        p->bgmFadeSpeed = 0;
    }
}

int sub_0807E724(s32 volume)
{
    struct SoundDriver *p = &gUnk_03005210;
    if (p->flags & 0x80) {
        p->targetVolume = volume;
        p->fadeSpeed = 0x40;
        return p->currentBgm;
    } else {
        return -1;
    }
}

void sub_0807E764(s32 vol)
{
    struct SoundDriver *p = &gUnk_03005210;
    p->targetVolume = 0;
    p->fadeSpeed = vol;
}

void sub_0807E780(s32 vol)
{
    struct SoundDriver *p = &gUnk_03005210;
    p->flags |= 0x100;
    p->targetVolume = 0;
    p->fadeSpeed = vol;
}

void sub_0807E7B8(s32 vol)
{
    struct SoundDriver *p = &gUnk_03005210;
    p->flags &= ~0x100;
    p->targetVolume = 0x10;
    p->fadeSpeed = vol;
}

int sub_0807E7E8(void)
{
    struct SoundDriver *p = &gUnk_03005210;
    return p->volume == p->targetVolume;
}

void sub_0807E814(s32 id)
{
    struct SoundDriver *p = &gUnk_03005210;
    if (id & 0x8000) {
        struct SoundTrack *track = p->seTracks;
        int i = 6;
        do {
            if ((track->flags & 0x80) && track->soundId == id)
                return;
            track++;
        } while (--i != 0);
    }
    p->pendingSe = id;
    p->seVolume = 0x10;
    p->seVariant = 0;
}

void sub_0807E870(s32 id, s32 variant)
{
    sub_0807E814(id);
    gUnk_03005210.seVariant = variant & 3;
    sub_0807E3D8();
}

void sub_0807E898(s32 id)
{
    struct SoundTrack *track = gUnk_03005308;
    int i = 6;
    do {
        if ((track->flags & 0x80) && track->soundId == id) {
            track->flags |= 4;
            if (track->flags & 1) {
                track->delay = 1;
                track->flags &= 0xFE;
            }
        }
        track++;
    } while (--i != 0);
}

void sub_0807E8DC(void)
{
    struct SoundTrack *track = gUnk_03005308;
    int i = 6;
    do {
        if (track->flags & 0x80) {
            track->flags |= 4;
            if (track->flags & 1) {
                track->delay = 1;
                track->flags &= 0xFE;
            }
        }
        track++;
    } while (--i != 0);
}

void sub_0807E918(struct SoundPcmVoice *voice, s32 id, s32 volume, s32 note)
{
    const struct SoundSample *sample;
    const u16 *pitch;
    id &= 0xCFFF;
    if (id & 0x8000)
        sample = gUnk_08088A20[id & 0x3FFF];
    else
        sample = gUnk_0811B420[id];
    pitch = &gUnk_081A960C[note];
    note = *pitch * sample->rate >> 12;
    voice->stepAndFraction = note;
    voice->remaining = sample->length;
    voice->data = sample->data;
    voice->sampleId = id & ~0x7000;
    voice->volume = volume;
    voice->flags = 0x80;
    if (sample->loopStart >= 0)
        voice->flags = 0xC0;
}

int sub_0807E990(void)
{
    struct SoundPcmVoice *voice = gUnk_030053AC;
    int count = 0;
    int i;
    for (i = 0; i < 6; i++) {
        if (voice->flags & 0x80)
            count++;
        voice++;
    }
    return count;
}

u32 sub_0807E9BC(void)
{
    return gUnk_03005210.status;
}

void sub_0807E9C8(s32 a, s32 b)
{
    struct SoundDriver *p = &gUnk_03005210;
    u8 v;
    p->flags |= 0x2000;
    v = p->targetVolume;
    if (a >= 0) {
        sub_0807E674(a);
        sub_0807E554();
        p->targetVolume = 0;
        *(u16 *)&p->fadeTimer = 0;
        v = 0x10;
    }
    while (--b >= 0)
        sub_0807DB58(p);
    p->targetVolume = v;
    p->flags &= ~0x2000;
}

void sub_0807EA4C(s32 a, s32 b, s32 c)
{
    struct SoundDriver *p = &gUnk_03005210;
    sub_0807E9C8(a, b);
    p->fadeSpeed = c;
    p->targetVolume = 0x10;
    p->volume = 0;
    p->fadeTimer = 0;
}

void sub_0807EA88(void)
{
    gUnk_03005210.flags |= 4;
}

void sub_0807EAA0(void)
{
    gUnk_03005210.flags |= 1;
}

void sub_0807EAB8(void)
{
    gUnk_03005210.flags |= 5;
}
