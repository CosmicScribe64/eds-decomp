/*
 * sound_driver (0x0807D3D0-0x0807EACF): the Konami sound driver (wiki/functions/sound-driver.md).
 *
 * PSG/PCM playback for BGM and sound effects, driven once per VBlank by SoundVBlank: SoundSequencerTick
 * advances the ten BGM tracks and the six SE tracks of gSoundDriver, programs the PSG registers and
 * starts PCM voices; the ARM mixer (asm/sound_mixer_arm.s, copied to IWRAM by SoundDmaInit) renders the
 * PCM voices into the FIFO ring buffers. The game-side wrappers (PlayBGM/PlaySE,
 * wiki/functions/sound-api.md) call the request functions at the bottom of this file; everything here
 * is the driver itself. All 30 functions are byte-matching C.
 */
#include "global.h" /* u8/s8/u16/s16/u32/s32, vu8/vu16/vu32 */
#include "sound.h"  /* struct SoundDriver, SoundBgmTrack, SoundSeTrack, SoundChannelOutput, SoundPcmChannel, SoundFifoPos, SoundSong, SoundSeDef, SoundSample, enum SoundDriverFlags, the driver prototypes, gSoundDriver, gSoundSeTracks, gSoundPcmChannels, gSoundDmaPos, gSoundPcmBuffer, gSoundMixCodeRam */
#include "gba.h"    /* REG_IE */

extern const u32 gWaveRamPatterns[][4]; /* 0x08139550: 16-byte PSG ch3 wave patterns */
void __sub_0807EAD0_from_thumb(void); /* linker veneer to the ARM mixer entry in IWRAM */

extern const struct SoundSample *const gPcmSampleTable2[]; /* 0x08088A20: sample ids with bit 15 set */
extern const struct SoundSample *const gPcmSampleTable[];  /* 0x0811B420: sample ids with bit 15 clear */
extern const u16 gSoundPitchTable[];                       /* 0x081A960C: pitch step per note */
/* One DMA channel's register block (0x040000BC = DMA1, 0x040000C8 = DMA2). */
struct SoundDmaRegs {
    volatile u32 source;      /* +0x0 SAD */
    volatile u32 destination; /* +0x4 DAD */
    u16 count;                /* +0x8 CNT_L */
    volatile u16 flags;       /* +0xA CNT_H */
};

/* gSongTable entries: SoundMain walks each entry as u16s (header[0] | header[1] << 16), so the data
 * pointer stays split in struct SoundSong. */
extern const struct SoundSong gSongTable[]; /* 0x080E09D0: 58 songs */
extern const struct SoundSeDef gSeTable[];  /* 0x08087FD0 */
extern const u8 gSeVariantTrackMap[];       /* 0x081A79F9: slot -> track map per SE variant */

/* SoundDmaInit (wiki): configures Timer0, the FIFO DMAs and their interrupts, copies the ARM mixer
 * to IWRAM and clears the PCM voices and ring buffers. */
void SoundDmaInit(void)
{
  struct SoundPcmChannel *voice;
  struct SoundFifoPos *state;
  s8 *buffer;
  int i;
  u32 zero;
  REG_IE &= 0xF9F7; /* mask the Timer0/DMA1/DMA2 interrupts while setting up */
  {
    /* DMA1/DMA2 CNT_H (base + 0xA): disable both FIFO DMAs. The read/modify/write stays in this
     * vu16* form; a volatile struct-field access makes agbcc emit an extra load (wiki). */
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
  *((vu32 *) 0x040000C4) = 0; /* DMA1CNT */
  *((vu32 *) 0x040000D0) = 0; /* DMA2CNT */
  {
    vu32 *dma = (vu32 *) 0x040000D4; /* DMA3: copy the mixer code to IWRAM */
    dma[0] = (u32) SoundMixChannel;
    dma[1] = (u32) gSoundMixCodeRam;
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
  voice = gSoundPcmChannels;
  state = &gSoundDmaPos;
  buffer = gSoundPcmBuffer[0];
  {
    struct SoundPcmChannel *iter = voice;
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
  *((u32 *) (&state->dmaPosB)) = 0;
  {
    vu32 *dma = (vu32 *) 0x040000D4; /* DMA3: zero the PCM ring buffers */
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
  *((vu8 *) 0x04000083) = 0xBB; /* SOUNDCNT_H high byte: FIFO A/B setup and reset */
  i = 8;
  do
  {
    REG_FIFO_A = 0; /* drain both FIFOs */
    REG_FIFO_B = 0;
  }
  while ((--i) != 0);
  {
    /* DMA1/DMA2: stream the two ring halves into FIFO_A/FIFO_B. Matching: the do-while(0) scope
     * preserves the final pointer arithmetic and allocation (wiki). */
    vu32 *dma = (vu32 *) 0x040000BC;
    do {
      dma[0] = (u32) buffer;
      dma[1] = 0x040000A0; /* FIFO_A */
      dma[2] = 0xF6000004;
      dma[2];
      dma += 3;
    } while (0);
    dma[0] = (u32) (buffer + 0x320);
    dma[1] = 0x040000A4; /* FIFO_B */
    dma[2] = 0xF6000004;
    dma[2];
  }
  REG_IE |= 0x208; /* enable the Timer0 and DMA1 interrupts */
  *((vu32 *) 0x04000100) = 0x0080FCB9; /* TM0CNT_L/H in one write: reload 0xFCB9, timer on */
}
/* SoundLoadWaveRam (wiki): loads the four wave-RAM words of pattern (wave, variant), toggles
 * SOUND_FLAG_WAVE_BANK and selects the next wave bank in REG_SOUND3CNT_L. */
void SoundLoadWaveRam(struct SoundDriver *p, u32 wave, u32 variant)
{
    const u32 *src = gWaveRamPatterns[wave * 16 + variant];
    u16 bank;
    REG_WAVE_RAM0 = src[0];
    REG_WAVE_RAM1 = src[1];
    REG_WAVE_RAM2 = src[2];
    REG_WAVE_RAM3 = src[3];
    bank = 0;
    if (!(p->flags & SOUND_FLAG_WAVE_BANK))
        bank = 0x40;
    p->flags ^= SOUND_FLAG_WAVE_BANK;
    REG_SOUND3CNT_L = bank | 0x80;
}

/* SoundInit (wiki): initializes the sound hardware and driver state, optionally installs the DMA1
 * handler through dma1Slot, clears the tracks, and calls the DMA setup. */
void SoundInit(void (**dma1Slot)(void))
{
    struct SoundDriver *p;
    struct SoundBgmTrack *track;
    struct SoundSeTrack *seTrack;
    int i;

    REG_SOUNDCNT_X = 0x80; /* master sound enable */
    REG_SOUND1CNT_L = 0;
    REG_SOUND1CNT_H = 0;
    REG_SOUND1CNT_X = 0x8000;
    REG_SOUND2CNT_L = 0;
    REG_SOUND2CNT_H = 0x8000;
    REG_SOUND3CNT_L = 0;
    REG_SOUND3CNT_H = 0x2000;
    REG_SOUND3CNT_X = 0;
    REG_SOUND4CNT_L = 0;
    REG_SOUND4CNT_H = 0;
    REG_IE &= 0xF9F7;
    REG_SOUNDCNT_L = 0xFF77;
    REG_SOUNDCNT_H = 0xE;
    *(vu16 *)0x04000088 = (*(vu16 *)0x04000088 & 0x3FFF) | 0x4000; /* BIAS: set amplitude resolution */
    if (dma1Slot != 0)
        *dma1Slot = SoundDma1Intr;
    p = &gSoundDriver;
    p->flags = 0;
    p->pendingBgm = 0xFFFF;
    p->pendingSe = -1;
    p->volumeFrac = 0;
    p->volume = 0x10;
    p->targetVolume = 0x10;
    p->fadeSpeed = 0x10;
    p->bgmTick = -1;
    track = p->bgmTracks;
    i = 10;
    do {
        track->flags = 0;
        i--;
        track++;
    } while (i != 0);
    p->seLockTimer = 0;
    i = 5;
    seTrack = &p->seTracks[5];
    do {
        seTrack->flags = 0;
        seTrack->data = 0;
        seTrack--;
    } while (--i >= 0);
    SoundLoadWaveRam(p, 0, 0);
    REG_SOUND3CNT_X = 0x8000; /* start channel 3 */
    SoundDmaInit();
}

extern const u8 gSeTrackPcmChannel[]; /* 0x081A79E8: SE track -> PCM channel map */

/* Advances SE track `idx` by one tick and decodes its bytecode into `out`.
 * The bytecode cursor `p` and the shared stream value `data` follow the
 * original register use; commands 0x00-0x1F re-dispatch the same command. */
void SoundSeTrackTick(s32 idx, struct SoundChannelOutput *out) {
    struct SoundDriver *driver = &gSoundDriver;
    struct SoundSeTrack *track = &driver->seTracks[idx];
    const u8 *p;
    s32 data;
    s32 tmp;

    if (!(track->flags & 0x80))
        return;
    *(u16 *)&out->volumeDirty = 0;
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
    if (driver->flags & SOUND_FLAG_STOP_ALL_SE) {
        track->flags |= 8;
        goto clear_linked;
    }
    if (!(track->flags & 0x40)) {
        track->flags |= 0x40;
        track->linkedTracks = 0;
        track->delay = 0;
        track->noteVolume = 0;
        track->returnData = NULL;
        goto stop;
    }
    out->volume = track->noteVolume;
    if (track->flags & 1) {
        if (idx > 1) {
            idx = gSeTrackPcmChannel[idx];
            if (gSoundPcmChannels[idx].flags & 0x80)
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
                *(u16 *)&out->instrument = 0;
                out->command = 0x40;
                out->volumeDirty = 0x40;
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
                    /* FAKEMATCH: a halfword store keeps the base-pitch sum folded in the ROM order (s16 pitch reassociates it) */
                    *(u16 *)&out->pitch = (s16)track->basePitch - 0x400 + (data >> 5);
                }
                data &= 0x1F;
                x = data + track->noteVolume;
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
                x = (tmp & 0x1F) + track->noteVolume;
            }
            data = x - 0x10;
            if (data < 0)
                data = 0;
            if (data > 0xF)
                data = 0xF;
        set_state:
            tmp = (c & 3) | (data << 8);
            if (!(c & 8)) {
                if (*(u16 *)&track->instrument != tmp) {
                    *(u16 *)&track->instrument = tmp;
                    out->volumeDirty = 2;
                }
                out->command = 2;
                *(u16 *)&out->instrument = tmp;
            }
            if (!(c & 4)) {
                track->basePitch = out->pitch;
                if (*(u16 *)&track->instrument != tmp)
                    tmp |= 8;
                *(u16 *)&track->instrument = tmp;
            }
            p += 2;
            goto next;
        } else if (data > 0x7F) {
            track->noteVolume = out->volume = data & 0xF;
            tmp = *p;
            track->instrument = tmp;
            p++;
            if (data > 0x8F) {
                out->pitch = tmp | 0x8000;
                out->command = 2;
                out->volumeDirty = 2;
            }
            goto next;
        } else if (data > 0x6F) {
            data &= 0xF;
            driver->seTracks[data].loopCounter = 1;
            track->data = p;
            goto next;
        } else if (data > 0x5F) {
            tmp = p[0] + (p[1] << 8);
            out->sampleId = tmp | 0x8000;
            out->volume = p[2];
            {
                u8 v = p[2];
                track->noteVolume = v;
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
            out->volumeDirty = 2;
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
            track->noteVolume = data;
        set_out:
            out->volume = data;
            out->volumeDirty = 2;
            goto next;
        } else if (data > 0x3F) {
            tmp = data;
            data = (s8)p[0] + track->noteVolume;
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

extern const u16 gNoiseTable[];      /* 0x08139F50: ch4 noise frequencies */
extern const u16 gPsgFreqTable[];    /* 0x081AA20C: PSG frequency register values per pitch */
extern const s16 gVibratoSineTable[]; /* 0x081ABC4C */
/* Per-VBlank sequencer: fades the master volume, advances the ten BGM tracks,
 * merges the six SE tracks, then programs the PSG registers and PCM voices.
 * a and b are generic int temporaries reused throughout (r4/r5 in the ROM). */

void SoundSequencerTick(struct SoundDriver *p)
{
    struct SoundChannelOutput out[10];
    struct SoundChannelOutput *o;
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
    b = a - *(u16 *)&p->volumeFrac;
    p->flags &= 0xFBF7; /* clear SOUND_FLAG_VOLUME_CHANGED | SOUND_FLAG_VOLUME_REFRESH (pool constant) */
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
        *(u16 *)&p->volumeFrac = a - b;
        if (oldVolume != p->volume)
            p->flags |= SOUND_FLAG_VOLUME_CHANGED;
    } else if (oldVolume & 0xF) {
        p->flags |= SOUND_FLAG_VOLUME_REFRESH;
    }

    if ((p->flags & SOUND_FLAG_BGM_PAUSE) && oldVolume == 0) {
        o = &out[9];
        for (i = 9; i >= 0; i--) {
            ((u32 *)o)[0] = 0;
            ((u32 *)o)[1] = 0;
            o--;
        }
    } else {
        flags = p->flags;
        songData = p->songData;
        track = p->bgmTracks;
        if (flags & SOUND_FLAG_STOP_BGM) {
            o = out;
            for (i = 9; i >= 0; i--) {
                track->flags = 0;
                track++;
                ((u32 *)o)[0] = 0;
                o->volumeDirty = 1;
                o->command = 0x40;
                o++;
            }
            {
                /* FAKEMATCH: int temporary keeps the AND in SImode (0xFFFFBF7E pool constant);
                 * clears SOUND_FLAG_BGM_INITIALIZED | SOUND_FLAG_BGM_ACTIVE | SOUND_FLAG_STOP_BGM */
                s32 t = flags & ~0x4081;
                p->flags = t;
            }
            p->bgmTick = -1;
        } else {
            if (!(flags & (SOUND_FLAG_BGM_ACTIVE | SOUND_FLAG_SE_ACTIVE)))
                return;
            if (!(flags & SOUND_FLAG_BGM_INITIALIZED)) {
                p->flags = flags | SOUND_FLAG_BGM_INITIALIZED;
            reset:
                p->bgmTick = 0;
                track = p->bgmTracks;
                track[0].routing = 0x11;
                track[1].routing = 0x22;
                track[2].routing = 0x44;
                track[3].routing = 0x88;
                /* the chained order reproduces the ROM's store order (9, 7, 5, 8, 6, 4) */
                track[4].routing = track[6].routing = track[8].routing = track[5].routing = track[7].routing = track[9].routing = 0x33;
                *(vu8 *)0x04000081 = 0xFF; /* SOUNDCNT_L high byte: route every channel to both outputs */
                REG_SOUNDCNT_H = 0x330E;
                for (i = 0; i < 10; i++) {
                    /* FAKEMATCH: a plain byte store here avoids the field-store zero that loop.c hoists */
                    *(u8 *)&track->flags &= 0xFE;
                    if (track->flags & 0x40)
                        track->flags |= 0x80;
                    track->flags &= 0xC0;
                    track->position = 0;
                    track->returnPosition = 0;
                    *(u16 *)&track->instrument = 0;
                    *(u16 *)&track->vibratoPhase = 0;
                    track++;
                }
                /* FAKEMATCH: the SOUND3CNT_H zero comes from the r7 variable (o) in the ROM */
                o = 0;
                (p->bgmTracks)[1].instrument = 0x80;
                (p->bgmTracks)[0].instrument = 0x80;
                REG_SOUND3CNT_H = (u32)o;
                SoundLoadWaveRam(p, 0, 0);
            }
            p->bgmTick++;
            o = &out[9];
            track = &(p->bgmTracks)[9];
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
                                    p->flags |= SOUND_FLAG_STOP_BGM;
                                } else if (a == 0xFE) {
                                    goto reset;
                                } else {
                                    track->flags &= 0x40;
                                    if (*(u16 *)&o->volumeDirty != 0)
                                        goto next;
                                }
                                track->noteVolume = 0;
                                *(u16 *)&o->instrument = 0;
                                o->pitch = track->pitch;
                                o->command = 0x40;
                                o->volumeDirty = 0x40;
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
                                        o[1].volumeDirty = 1;
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
                                track->noteVolume = a;
                                o->instrument = track->instrument;
                                o->volumeDirty = 1;
                            } else if (a > 0xCF) {
                                a = a & 0xF;
                                pos++;
                                track->pitch = o->pitch = cmdp[1] << 5;
                                o->command = 1;
                                if (a == track->noteVolume)
                                    goto check_loop;
                                goto set_volume;
                            } else if (a > 0xBF) {
                                a = a & 0xF;
                                o->pitch = track->pitch;
                                o->command = 1;
                                goto set_volume;
                            } else if (a > 0x9F) {
                                u8 ins;
                                track->noteVolume = a & 0xF;
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
                                *(u16 *)&o->instrument = *(u16 *)&track->instrument;
                                o->command = 0x80;
                            } else if (a > 0x8F) {
                                u8 count;
                                track->returnSongOffset = track->songOffset;
                                track->returnPosition = pos + 3;
                                b = p->currentBgm * 12 + i + 2;
                                {
                                    /* FAKEMATCH: pointer local loads the table base before scaling b */
                                    const u16 *tbl = (const u16 *)gSongTable;
                                    track->songOffset = tbl[b];
                                }
                                count = cmdp[3];
                                track->callCounter = count;
                                pos = cmdp[1] | (cmdp[2] << 8);
                                a = a & 0xF;
                                if (a == 0xF)
                                    goto read_delay;
                                track->callCounter = count + 1;
                                goto wave;
                            } else if (a > 0x7F) {
                                a = a & 0xF;
                            wave:
                                if (a > 3) {
                                    a -= 4;
                                    SoundLoadWaveRam(p, a, track->noteVolume);
                                }
                                o->instrument = a;
                                track->instrument = a;
                            }
                        check_loop:
                            if (track->returnPosition != 0) {
                                if (--track->callCounter == 0) {
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
                                /* FAKEMATCH: int temporary avoids a QImode AND with -5 */
                                s32 t = b & 0xFB;
                                track->flags = t;
                            }
                            o->command = 0x40;
                        }
                        track->fadeCounter = a;
                        track->noteVolume = (track->fadeVolume * a) >> 2;
                        o->pitch = track->pitch;
                        *(u16 *)&o->instrument = *(u16 *)&track->instrument;
                        o->volumeDirty = 1;
                    }
                    if (b & 0x20) {
                        if (track->vibratoDepth == 0) {
                            track->flags &= 0xDF;
                            track->vibratoPhase = 0;
                            a = 0;
                        } else {
                            track->vibratoPhase += 0x18;
                            a = (gVibratoSineTable[track->vibratoPhase] * track->vibratoDepth) >> 12;
                        }
                        o->pitch = track->pitch + a;
                        *(u16 *)&o->instrument = *(u16 *)&track->instrument;
                        o->sampleId = track->instrument;
                        o->command = 1;
                    }
                }
                o->volume = (track->noteVolume * p->volume) >> 4;
                if ((p->flags & 0x408) && track->noteVolume != 0) { /* VOLUME_DIRTY | PARTIAL_MUTE */
                    if (o->command == 0)
                        o->pitch = track->pitch;
                    o->instrument = track->instrument;
                    o->volumeDirty = 1;
                }
            next:;
            }
            track = p->bgmTracks;
            p->psgRouting = track[0].routing | track[1].routing | track[2].routing | track[3].routing;
        }
    }

    if (p->flags & SOUND_FLAG_SE_ACTIVE) {
        struct SoundSeTrack *se;
        if ((s8)--p->seLockTimer < 0)
            p->seLockTimer = 0;
        se = p->seTracks;
        SoundSeTrackTick(0, &out[1]);
        SoundSeTrackTick(1, &out[3]);
        SoundSeTrackTick(2, &out[9]);
        SoundSeTrackTick(3, &out[8]);
        SoundSeTrackTick(4, &out[7]);
        SoundSeTrackTick(5, &out[6]);
        b = 0;
        for (i = 5; i >= 0; i--) {
            b |= se->flags;
            se++;
        }
        if (!(b & 0x80)) {
            p->seLockTimer = 0;
            p->flags &= 0xFFBF; /* clear SOUND_FLAG_SE_ACTIVE (pool constant) */
        }
    } else {
        p->seLockTimer = 0;
    }
    *(vu8 *)0x04000081 = p->psgRouting; /* SOUNDCNT_L high byte: channel routing */

    o = out;
    if (*(u16 *)&o[0].volumeDirty != 0) {
        b = gPsgFreqTable[o[0].pitch];
        if (o[0].volumeDirty != 0) {
            {
                /* FAKEMATCH: value computed before the MMIO address is loaded */
                s32 t = (o[0].volume << 12) | o[0].instrument;
                REG_SOUND1CNT_H = t;
            }
            REG_SOUND1CNT_X = b;
        } else {
            REG_SOUND1CNT_X = b & 0x7FF;
        }
    }
    if ((b = *(u16 *)&o[1].volumeDirty) != 0) {
        b = o[1].pitch;
        if (b < 0 || !(b & 0x4000))
            b = gPsgFreqTable[b];
        else
            b = (b & ~0x4000) | 0x8000;
        if (o[1].volumeDirty != 0) {
            {
                s32 t = (o[1].volume << 12) | o[1].instrument;
                REG_SOUND2CNT_L = t;
            }
            REG_SOUND2CNT_H = b;
        } else {
            REG_SOUND2CNT_H = b & 0x7FF;
        }
    }
    if (*(u16 *)&o[2].volumeDirty != 0) {
        b = gPsgFreqTable[o[2].pitch] & 0x7FF;
        if (o[2].volume == 0) {
            REG_SOUND3CNT_H = 0;
        } else {
            SoundLoadWaveRam(p, o[2].instrument, o[2].volume);
            REG_SOUND3CNT_H = 0x2000;
        }
        REG_SOUND3CNT_X = b;
    }
    if ((b = *(u16 *)&o[3].volumeDirty) != 0) {
        a = o[3].volume << 12;
        if (!(b & 0x202)) {
            REG_SOUND4CNT_L = a;
            REG_SOUND4CNT_H = gNoiseTable[o[3].pitch];
        } else {
            REG_SOUND4CNT_L = a;
            REG_SOUND4CNT_H = o[3].pitch;
        }
    }

    {
        struct SoundPcmChannel *voice;
        o += 9;
        voice = &gSoundPcmChannels[5];
        for (i = 5; i >= 0; i--) {
            b = o->command;
            if (b != 0) {
                if (b & 0x80) {
                    SoundPcmStart(voice, o->sampleId, o->volume, o->pitch);
                } else if (b & 0x40) {
                    voice->flags = 0;
                } else {
                    const struct SoundSample *s;
                    if (o->sampleId & 0x8000)
                        s = gPcmSampleTable2[o->sampleId & 0x3FFF];
                    else
                        s = gPcmSampleTable[o->sampleId];
                    voice->step = (*(gSoundPitchTable + o->pitch) * s->rate) >> 12;
                }
            }
            if (o->volumeDirty != 0)
                voice->volume = o->volume;
            voice--;
            o--;
        }
    }
}

/* SoundDma1Intr (wiki): DMA1 IRQ handler. Advances the read position by 16 and, at the 0x2C0-byte
 * wrap, restarts both FIFO DMAs at the two ring buffer halves. */
void SoundDma1Intr(void)
{
    struct SoundFifoPos *state = &gSoundDmaPos;
    int position = state->dmaPosA + 0x10;
    if (position > 0x2BF) {
        struct SoundDmaRegs *dma1 = (struct SoundDmaRegs *)0x040000BC;
        struct SoundDmaRegs *dma2;
        /* Matching: the CNT_H read/modify/writes go through (u8 *)dma + 10 vu16 accesses, not the
         * struct's volatile flags field (the field form emits an extra load; see the wiki notes). */
        *(vu16 *)((u8 *)dma1 + 10) &= 0xC5FF;
        *(vu16 *)((u8 *)dma1 + 10) &= 0x7FFF;
        *(vu16 *)((u8 *)dma1 + 10);
        dma2 = (struct SoundDmaRegs *)0x040000C8;
        *(vu16 *)((u8 *)dma2 + 10) &= 0xC5FF;
        *(vu16 *)((u8 *)dma2 + 10) &= 0x7FFF;
        *(vu16 *)((u8 *)dma2 + 10);
        dma1->source = (u32)gSoundPcmBuffer[0];
        dma1->destination = 0x040000A0; /* FIFO_A */
        *(vu32 *)&dma1->count = 0xF6000004;
        *(vu32 *)&dma1->count;
        dma2->source = (u32)gSoundPcmBuffer[1];
        dma2->destination = 0x040000A4; /* FIFO_B */
        *(vu32 *)&dma2->count = 0xF6000004;
        *(vu32 *)&dma2->count;
        position = 0;
    }
    state->dmaPosB = position;
    state->dmaPosA = position;
}

/* SoundVBlank (wiki): the per-frame entry. Runs the sequencer and the ARM mixer unless
 * SOUND_FLAG_TICK_PAUSED (a SoundSeekBGM fast-forward) suppresses them. */
void SoundVBlank(void)
{
    struct SoundDriver *p = &gSoundDriver;
    if (!(p->flags & SOUND_FLAG_TICK_PAUSED)) {
        SoundSequencerTick(p);
        __sub_0807EAD0_from_thumb();
    }
}

/* SoundStartPendingSE (wiki): starts the pending SE, arbitrating slot masks and priorities against
 * the running SE tracks; overlapping lower-priority tracks are marked stopping. */
void SoundStartPendingSE(void)
{
    struct SoundDriver *p = &gSoundDriver;
    const struct SoundSeDef *effect;
    s32 metadata;
    int priority;
    int slots;
    int variant;
    const u8 *map;
    int overlap;
    int bits;
    /* FAKEMATCH: pins the SE track cursor in r3 as in the ROM. */
    register struct SoundSeTrack *track asm("r3");
    int i;
    int id;
    int request;
    u8 *volume;
    request = p->pendingSe;
    if (request < 0)
        return;
    effect = &gSeTable[request & 0xFFF];
    metadata = *(const s32 *)&effect->priority;
    /* These empty constraints emit no instructions and preserve initialized
     * values. Keep the first request through metadata loading, the variant
     * through cursor setup, and the second signed request through allocation
     * and final flag updates to retain the ROM register lifetimes. */
    asm volatile ("" : : "r"(effect), "r"(request));
    priority = metadata & 0xFF;
    if ((metadata >> 16) != 0 && (s8)p->seLockTimer != 0)
        goto done;
    slots = (metadata >> 8) & 0xFF;
    variant = p->seVariant;
    map = &gSeVariantTrackMap[variant * 6];
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
    p->seLockTimer = effect->lockTicks;
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
    p->flags = (p->flags & 0xFFFB) | 0x40; /* clear SOUND_FLAG_STOP_ALL_SE, set SOUND_FLAG_SE_ACTIVE */
    asm volatile ("" : : "r"(id));
done:
    p->pendingSe = 0xFFFF;
}

/* SoundMain (wiki): processes the pending SE, handles BGM fade-out, loads a requested song into
 * gSoundDriver and initializes its ten tracks. */
void SoundMain(void)
{
    struct SoundDriver *p;
    int song;
    SoundStartPendingSE();
    p = &gSoundDriver;
    song = p->pendingBgm;
    if (song >= 0) {
        struct SoundBgmTrack *track;
        const u16 *header;
        /* Re-read the request for the compare to retain the ROM's register lifetimes. */
        if ((p->flags & SOUND_FLAG_BGM_ACTIVE) && p->currentBgm != p->pendingBgm) {
            if (p->volume != 0) {
                p->fadeSpeed = 0x10;
                p->targetVolume = 0;
                return;
            }
            p->fadeSpeed = 0x10;
        }
        if (p->bgmFadeInSpeed == 0) {
            p->volume = 0x10;
            p->targetVolume = 0x10;
        } else {
            p->fadeSpeed = p->bgmFadeInSpeed;
            p->targetVolume = 0x10;
        }
        p->volumeFrac = 0;
        p->currentBgm = p->pendingBgm;
        p->pendingBgm = 0xFFFF;
        track = p->bgmTracks;
        header = (const u16 *)&gSongTable[song];
        p->songData = (const u8 *)(header[0] | header[1] << 16);
        header += 2;
        song = 10;
        do {
            track->flags = 0xC0;
            track->songOffset = *header;
            song--;
            header++;
            track++;
        } while (song != 0);
        p->pendingBgm = 0xFFFF;
        p->flags = (p->flags & 0xBFFF) | 0x80; /* clear SOUND_FLAG_BGM_INITIALIZED, set SOUND_FLAG_BGM_ACTIVE */
    }
}

/* SoundRequestBGM (wiki): sets a BGM request and clears its requested fade speed. */
void SoundRequestBGM(s32 id)
{
    struct SoundDriver *p = &gSoundDriver;
    p->pendingBgm = id;
    p->bgmFadeInSpeed = 0;
}

/* SoundRequestBGMFadeIn (wiki): sets a BGM request and the supplied fade speed. */
void SoundRequestBGMFadeIn(s32 id, s32 fadeSpeed)
{
    struct SoundDriver *p = &gSoundDriver;
    p->pendingBgm = id;
    p->bgmFadeInSpeed = fadeSpeed;
}

/* SoundIsBGMPlaying (wiki): whether SOUND_FLAG_BGM_ACTIVE is set and the current song is `id`. */
int SoundIsBGMPlaying(s32 id)
{
    struct SoundDriver *p = &gSoundDriver;
    if (p->flags & SOUND_FLAG_BGM_ACTIVE)
        return p->currentBgm == id;
    return 0;
}

/* SoundRequestBGMIfNotPlaying (wiki): requests a song if it is not already playing. */
void SoundRequestBGMIfNotPlaying(s32 id)
{
    struct SoundDriver *p = &gSoundDriver;
    if (!(p->flags & SOUND_FLAG_BGM_ACTIVE) || p->currentBgm != id) {
        p->pendingBgm = id;
        p->bgmFadeInSpeed = 0;
    }
}

/* SoundSetBGMVolume (wiki): sets the playing BGM's target volume and fade speed 0x40;
 * returns the song id, or -1 if no BGM is active. */
int SoundSetBGMVolume(s32 volume)
{
    struct SoundDriver *p = &gSoundDriver;
    if (p->flags & SOUND_FLAG_BGM_ACTIVE) {
        p->targetVolume = volume;
        p->fadeSpeed = 0x40;
        return p->currentBgm;
    } else {
        return -1;
    }
}

/* SoundFadeOutBGM (wiki): sets the target volume to zero with the supplied fade speed. */
void SoundFadeOutBGM(s32 fadeSpeed)
{
    struct SoundDriver *p = &gSoundDriver;
    p->targetVolume = 0;
    p->fadeSpeed = fadeSpeed;
}

/* SoundPauseBGM (wiki): sets SOUND_FLAG_BGM_PAUSE, target volume zero, and the fade speed. */
void SoundPauseBGM(s32 fadeSpeed)
{
    struct SoundDriver *p = &gSoundDriver;
    p->flags |= SOUND_FLAG_BGM_PAUSE;
    p->targetVolume = 0;
    p->fadeSpeed = fadeSpeed;
}

/* SoundResumeBGM (wiki): clears SOUND_FLAG_BGM_PAUSE and fades back toward volume 0x10. */
void SoundResumeBGM(s32 fadeSpeed)
{
    struct SoundDriver *p = &gSoundDriver;
    p->flags &= ~SOUND_FLAG_BGM_PAUSE;
    p->targetVolume = 0x10;
    p->fadeSpeed = fadeSpeed;
}

/* SoundIsBGMFadeDone (wiki): whether current and target volume are equal. */
int SoundIsBGMFadeDone(void)
{
    struct SoundDriver *p = &gSoundDriver;
    return p->volume == p->targetVolume;
}

/* SoundRequestSE (wiki): queues an SE with full volume and variant zero; an id with bit 15 set is
 * suppressed when an identical SE is already active. */
void SoundRequestSE(s32 id)
{
    struct SoundDriver *p = &gSoundDriver;
    if (id & 0x8000) {
        struct SoundSeTrack *track = p->seTracks;
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

/* SoundStartSEVariant (wiki): requests an SE, sets its variant (& 3) and starts it immediately. */
void SoundStartSEVariant(s32 id, s32 variant)
{
    SoundRequestSE(id);
    gSoundDriver.seVariant = variant & 3;
    SoundStartPendingSE();
}

/* SoundReleaseSE (wiki): marks active SE tracks with this id for stopping; delayed tracks are
 * woken by clearing flag 1 and setting delay 1. */
void SoundReleaseSE(s32 id)
{
    struct SoundSeTrack *track = gSoundSeTracks;
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

/* SoundReleaseAllSE (wiki): the SoundReleaseSE stop operation applied to every active SE track. */
void SoundReleaseAllSE(void)
{
    struct SoundSeTrack *track = gSoundSeTracks;
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

/* SoundPcmStart (wiki): starts a PCM voice, calculating its pitch step and loop flags from the
 * sample header. `note` is reused for the calculated step; the operand order is load-bearing. */
void SoundPcmStart(struct SoundPcmChannel *voice, s32 id, s32 volume, s32 note)
{
    const struct SoundSample *sample;
    const u16 *pitch;
    id &= 0xCFFF;
    if (id & 0x8000)
        sample = gPcmSampleTable2[id & 0x3FFF];
    else
        sample = gPcmSampleTable[id];
    pitch = &gSoundPitchTable[note];
    note = *pitch * sample->rate >> 12;
    *(u32 *)&voice->step = note;
    voice->remaining = sample->length;
    voice->data = sample->data;
    voice->sampleId = id & ~0x7000;
    voice->volume = volume;
    voice->flags = 0x80; /* active */
    if (sample->loopStart >= 0)
        voice->flags = 0xC0; /* active + looping */
}

/* SoundCountActivePcm (wiki): counts the six PCM voices whose active flag is set. */
int SoundCountActivePcm(void)
{
    struct SoundPcmChannel *voice = gSoundPcmChannels;
    int count = 0;
    int i;
    for (i = 0; i < 6; i++) {
        if (voice->flags & 0x80)
            count++;
        voice++;
    }
    return count;
}

/* SoundGetBGMTick (wiki): returns the driver status word (sequencer tick counter). */
u32 SoundGetBGMTick(void)
{
    return gSoundDriver.bgmTick;
}

/* SoundSeekBGM (wiki): pauses normal ticks, optionally starts a song, advances the requested
 * number of sequencer ticks by hand, and restores the target volume. */
void SoundSeekBGM(s32 id, s32 ticks)
{
    struct SoundDriver *p = &gSoundDriver;
    u8 v;
    p->flags |= SOUND_FLAG_TICK_PAUSED;
    v = p->targetVolume;
    if (id >= 0) {
        SoundRequestBGM(id);
        SoundMain();
        p->targetVolume = 0;
        *(u16 *)&p->volumeFrac = 0;
        v = 0x10;
    }
    while (--ticks >= 0)
        SoundSequencerTick(p);
    p->targetVolume = v;
    p->flags &= ~SOUND_FLAG_TICK_PAUSED;
}

/* SoundSeekBGMFadeIn (wiki): seeks with SoundSeekBGM, then fades in from volume zero. */
void SoundSeekBGMFadeIn(s32 id, s32 ticks, s32 fadeSpeed)
{
    struct SoundDriver *p = &gSoundDriver;
    SoundSeekBGM(id, ticks);
    p->fadeSpeed = fadeSpeed;
    p->targetVolume = 0x10;
    p->volume = 0;
    p->volumeFrac = 0;
}

/* SoundStopAllSE (wiki): sets SOUND_FLAG_STOP_ALL_SE; the sequencer stops the SE tracks next tick. */
void SoundStopAllSE(void)
{
    gSoundDriver.flags |= SOUND_FLAG_STOP_ALL_SE;
}

/* SoundStopBGM (wiki): sets SOUND_FLAG_STOP_BGM; the sequencer stops the BGM tracks next tick. */
void SoundStopBGM(void)
{
    gSoundDriver.flags |= SOUND_FLAG_STOP_BGM;
}

/* SoundStopAll (wiki): sets both stop flags. */
void SoundStopAll(void)
{
    gSoundDriver.flags |= SOUND_FLAG_STOP_BGM | SOUND_FLAG_STOP_ALL_SE;
}
