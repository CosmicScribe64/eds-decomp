#ifndef GUARD_SOUND_H
#define GUARD_SOUND_H

/*
 * Sound: the Konami sound driver (src/sound_driver.c, compiled with agbcc; the ARM mixer in
 * src/sound_mixer_arm.s) and the game-side API (src/collection.c).
 *
 * Game code calls the API: PlaySE / PlayBGM / StopBGM / FadeOutBGM check the Options flags in
 * gSaveData.options, allow one sound effect per frame and remember the current song in gMain.currentBgm.
 * The API only posts requests (gSoundDriver.pendingBgm / pendingSe). SoundMain starts them from the main
 * loop; SoundVBlank runs the sequencer (SoundSequencerTick) and the ARM mixer every frame.
 *
 * Channels: 10 BGM tracks (square 1, square 2, wave, noise, PCM 0-5) and 6 SE tracks that take over the
 * outputs of square 2, noise and PCM 5-2 while they play. PCM is mixed in software into two DMA-fed FIFO
 * rings (gSoundPcmBuffer): PCM channels 0-2 to FIFO A, 3-5 to FIFO B.
 *
 * Sound ids are in constants/sound.h. Bytecode formats: wiki data/sound-sequence-format.md; driver:
 * wiki game/sound-engine.md, functions/sound-driver.md, functions/sound-api.md, functions/sound-mixer.md.
 */

#include "global.h"

/* --- Tracks ------------------------------------------------------------------------------------------- */

/* Index of a BGM track in gSoundDriver.bgmTracks[] and of its output in SoundSequencerTick. */
enum SoundBgmChannel {
    SOUND_CH_SQ1 = 0,
    SOUND_CH_SQ2 = 1,
    SOUND_CH_WAVE = 2,
    SOUND_CH_NOISE = 3,
    SOUND_CH_PCM0 = 4,
    SOUND_CH_PCM1 = 5,
    SOUND_CH_PCM2 = 6,
    SOUND_CH_PCM3 = 7,
    SOUND_CH_PCM4 = 8,
    SOUND_CH_PCM5 = 9,
};

/* SoundBgmTrack.flags. */
enum SoundBgmTrackFlags {
    BGM_TRACK_STARTED = 0x1,
    BGM_TRACK_FADE = 0x4,           /* per-track fade (fadeCounter/fadeVolume); nothing sets it */
    BGM_TRACK_VIBRATO = 0x20,
    BGM_TRACK_ENABLED = 0x40,
    BGM_TRACK_PLAYING = 0x80,
};

/* One BGM track (0x18 bytes), gSoundDriver.bgmTracks[enum SoundBgmChannel]. Positions are offsets into
 * the song data (gSoundDriver.songData). */
struct SoundBgmTrack {
    s16 pitch;                  /* +0x00: current pitch in 1/32 semitones */
    u8 instrument;              /* +0x02: PSG duty/length byte, wave pattern (wave track) or bank-0 sample
                                 *        id (PCM tracks) */
    u8 noteVolume;              /* +0x03: track volume 0-15 */
    u16 songOffset;             /* +0x04: track base in songData (moved by the F3 loop-start command) */
    u16 position;               /* +0x06: read position, relative to songOffset */
    u16 returnPosition;         /* +0x08: return position of a 0x9X call, 0 = none */
    u16 returnSongOffset;       /* +0x0A: songOffset to restore after the call */
    u16 delay;                  /* +0x0C: ticks until the next event */
    u8 vibratoPhase;            /* +0x0E: index into gVibratoSineTable */
    u8 vibratoDepth;            /* +0x0F: F1 operand >> 1 */
    u8 flags;                   /* +0x10: enum SoundBgmTrackFlags */
    s8 fadeCounter;             /* +0x11: per-track fade (BGM_TRACK_FADE) */
    u8 fadeVolume;              /* +0x12: per-track fade start volume */
    u8 callCounter;             /* +0x13: events left in a 0x9X call */
    u8 routing;                 /* +0x14: NR51 bits of a PSG track (0x11/0x22/0x44/0x88 at start; F0 pan) */
    u8 unk15[3];                /* +0x15: unused */
};

/* SoundSeTrack.flags (names: medium confidence). */
enum SoundSeTrackFlags {
    SE_TRACK_WAIT_PCM = 0x1,            /* waiting for the PCM sample to end (cleared by a release) */
    SE_TRACK_KEYOFF_AFTER_DELAY = 0x2,
    SE_TRACK_RELEASE = 0x4,             /* leave the FC 00 sustain loop and play to the end (SoundReleaseSE) */
    SE_TRACK_STOP = 0x8,
    SE_TRACK_UNK_10 = 0x10,
    SE_TRACK_LOOPING = 0x20,
    SE_TRACK_STARTED = 0x40,
    SE_TRACK_ACTIVE = 0x80,
};

/* Output slots an SE claims (SoundSeDef.slotMask / SoundSeTrack.slotMask). */
enum SoundSeSlot {
    SE_SLOT_PCM2 = 0x1,
    SE_SLOT_PCM3 = 0x2,
    SE_SLOT_PCM4 = 0x4,
    SE_SLOT_PCM5 = 0x8,
    SE_SLOT_NOISE = 0x10,
    SE_SLOT_SQ2 = 0x20,
};

/* One SE track (0x18 bytes), gSoundDriver.seTracks[i] (= gSoundSeTracks[i]). Track i plays on square 2,
 * noise, PCM 5, PCM 4, PCM 3, PCM 2 for i = 0..5 and runs its own bytecode pointer. */
struct SoundSeTrack {
    const u8 *data;             /* +0x00: bytecode cursor */
    const u8 *returnData;       /* +0x04: return pointer of an FA call */
    u16 basePitch;              /* +0x08: base pitch/frequency for the relative tone commands */
    u8 instrument;              /* +0x0A: low byte of SOUNDxCNT_H (envelope/duty/length) */
    u8 noteVolume;              /* +0x0B: current note volume. SoundSeTrackTick also accesses +0x0A..+0x0B as
                                 *        one halfword; *(u16 *)&track->instrument compiles to the same code
                                 *        as the packed union sound_driver.c uses today */
    u16 delay;                  /* +0x0C: ticks until the next event */
    s16 soundId;                /* +0x0E: SE id (request & 0xFFF) */
    u8 priority;                /* +0x10: gSeTable[].priority */
    u8 slotMask;                /* +0x11: enum SoundSeSlot bits the SE occupies */
    u8 loopCounter;             /* +0x12: FC loop count; set to 1 by another track's 0x7X break-loop */
    u8 flags;                   /* +0x13: enum SoundSeTrackFlags */
    u8 volume;                  /* +0x14: SE volume (seVolume at start); scales the output, >> 4 */
    s8 linkedTracks;            /* +0x15: following tracks held by a PCM link; -1 on a held track */
    u8 unk16[2];                /* +0x16: unused */
};

/* --- Driver state ----------------------------------------------------------------------------------------- */

/* gSoundDriver.flags. */
enum SoundDriverFlags {
    SOUND_FLAG_STOP_BGM = 0x1,          /* request: the next tick stops every BGM track (SoundStopBGM) */
    SOUND_FLAG_STOP_ALL_SE = 0x4,       /* request: every SE track ends on its next tick (SoundStopAllSE) */
    SOUND_FLAG_VOLUME_CHANGED = 0x8,
    SOUND_FLAG_SE_ACTIVE = 0x40,
    SOUND_FLAG_BGM_ACTIVE = 0x80,
    SOUND_FLAG_BGM_PAUSE = 0x100,       /* SoundPauseBGM: freeze the BGM once its volume reaches 0 */
    SOUND_FLAG_WAVE_BANK = 0x200,       /* wave RAM bank in use (SoundLoadWaveRam toggles it) */
    SOUND_FLAG_VOLUME_REFRESH = 0x400,
    SOUND_FLAG_TICK_PAUSED = 0x2000,    /* SoundVBlank skips the sequencer (set during SoundSeekBGM) */
    SOUND_FLAG_BGM_INITIALIZED = 0x4000,
};

/* The driver, gSoundDriver (0x03005210, 0x19C bytes). Volumes are 0..0x10 (0x10 = full). */
struct SoundDriver {
    s32 bgmTick;                        /* +0x000: ticks since the song started or looped; -1 after init/stop */
    const u8 *songData;                 /* +0x004: current song's data (gSongTable[id]) */
    struct SoundBgmTrack bgmTracks[10]; /* +0x008: enum SoundBgmChannel order */
    struct SoundSeTrack seTracks[6];    /* +0x0F8: square 2, noise, PCM 5, PCM 4, PCM 3, PCM 2 */
    u16 flags;                          /* +0x188: enum SoundDriverFlags */
    s16 pendingBgm;                     /* +0x18A: requested song, -1 = none */
    s16 pendingSe;                      /* +0x18C: requested SE, -1 = none (bit 15 = do not restart if
                                         *         already playing; broken, never used) */
    s16 currentBgm;                     /* +0x18E: song loaded by SoundMain */
    u8 volumeFrac;                      /* +0x190: fraction byte of the 8.8 BGM volume; the driver reads and
                                         *         writes volumeFrac/volume as one u16 */
    u8 volume;                          /* +0x191: BGM master volume (scales BGM tracks only) */
    u8 fadeSpeed;                       /* +0x192: volume moves fadeSpeed * 16 / 256 per tick toward
                                         *         targetVolume */
    u8 targetVolume;                    /* +0x193: fade target */
    u8 bgmFadeInSpeed;                  /* +0x194: fade-in speed for the pending song, 0 = start at full */
    u8 seLockTimer;                     /* +0x195: lockTicks of the last SE started, decremented every
                                         *         tick; SEs with lockTicks are refused until it is 0 */
    u8 seVolume;                        /* +0x196: volume for the pending SE (0x10), copied to its tracks */
    u8 seVariant;                       /* +0x197: 0-3, shifts the pending SE's PCM tracks
                                         *         (gSeVariantTrackMap row) */
    u8 psgRouting;                      /* +0x198: NR51 shadow, OR of the PSG tracks' routing bytes */
    u8 pad199[3];                       /* +0x199: up to gSoundPcmChannels (0x030053AC) */
};

typedef char sound_h_check_bgm_track_size[sizeof(struct SoundBgmTrack) == 0x18 ? 1 : -1];
typedef char sound_h_check_se_track_size[sizeof(struct SoundSeTrack) == 0x18 ? 1 : -1];
typedef char sound_h_check_se_instrument[(u32)&((struct SoundSeTrack *)0)->instrument == 0xA ? 1 : -1];
typedef char sound_h_check_se_delay[(u32)&((struct SoundSeTrack *)0)->delay == 0xC ? 1 : -1];
typedef char sound_h_check_se_flags[(u32)&((struct SoundSeTrack *)0)->flags == 0x13 ? 1 : -1];
typedef char sound_h_check_driver_size[sizeof(struct SoundDriver) == 0x19C ? 1 : -1];
typedef char sound_h_check_driver_se[(u32)&((struct SoundDriver *)0)->seTracks == 0xF8 ? 1 : -1];
typedef char sound_h_check_driver_flags[(u32)&((struct SoundDriver *)0)->flags == 0x188 ? 1 : -1];
typedef char sound_h_check_driver_volume[(u32)&((struct SoundDriver *)0)->volumeFrac == 0x190 ? 1 : -1];
typedef char sound_h_check_driver_variant[(u32)&((struct SoundDriver *)0)->seVariant == 0x197 ? 1 : -1];
typedef char sound_h_check_driver_routing[(u32)&((struct SoundDriver *)0)->psgRouting == 0x198 ? 1 : -1];

/* SoundChannelOutput.command (names: medium confidence). */
enum SoundChannelCommand {
    CHAN_CMD_NONE = 0,
    CHAN_CMD_UPDATE = 1,            /* set by a BGM track */
    CHAN_CMD_UPDATE_SE = 2,         /* set by an SE track */
    CHAN_CMD_KEY_OFF = 0x40,
    CHAN_CMD_KEY_ON = 0x80,
};

/* What a track asks of its hardware channel or PCM channel for this tick (8 bytes). SoundSequencerTick
 * fills one per BGM track (out[enum SoundBgmChannel]); SoundSeTrackTick overwrites the slot an SE plays on. */
struct SoundChannelOutput {
    s16 pitch;                  /* +0: 1/32-semitone pitch; bit 14 = raw frequency (square 2 / SE), raw
                                 *     NR43 value for SE noise */
    u8 instrument;              /* +2: SOUNDxCNT_H low byte (squares) or wave pattern (wave) */
    u8 volume;                  /* +3: output volume */
    u8 volumeDirty;             /* +4: non-zero: rewrite volume / restart (1 BGM, 2 SE, 0x40 off) */
    u8 command;                 /* +5: enum SoundChannelCommand */
    u16 sampleId;               /* +6: PCM sample id (bit 15 = bank 1) */
};

/* --- PCM channels and the FIFO rings ---------------------------------------------------------------------- */

/* SoundPcmChannel.flags. */
enum SoundPcmFlags {
    PCM_FLAG_LOOP = 0x40,
    PCM_FLAG_ACTIVE = 0x80,
};

/* One software PCM channel (0x10 bytes), gSoundPcmChannels[6]; read by the ARM mixer (src/sound_mixer_arm.s
 * names the same offsets PCM_DATA..PCM_VOLUME). */
struct SoundPcmChannel {
    const s8 *data;             /* +0x0: current sample pointer */
    u32 remaining;              /* +0x4: samples left before the end / loop point */
    u16 step;                   /* +0x8: pitch step, 0x1000 = one sample per output byte (SoundPcmStart
                                 *       stores step and frac as one u32) */
    u16 frac;                   /* +0xA: 12-bit fractional position, owned by the mixer */
    s16 sampleId;               /* +0xC: bit 15 selects bank 1 */
    u8 flags;                   /* +0xE: enum SoundPcmFlags */
    u8 volume;                  /* +0xF: 0 = silent; the mixer uses volume + 1 */
};

/* FIFO ring positions, gSoundDmaPos: byte offsets into the 0x2C0 used bytes of each gSoundPcmBuffer ring. */
struct SoundFifoPos {
    u16 dmaPosA;                /* +0: FIFO A DMA read position */
    u16 mixPosA;                /* +2: FIFO A mixed up to here */
    u16 dmaPosB;                /* +4: FIFO B DMA read position */
    u16 mixPosB;                /* +6: FIFO B mixed up to here */
};

typedef char sound_h_check_output_size[sizeof(struct SoundChannelOutput) == 8 ? 1 : -1];
typedef char sound_h_check_pcm_size[sizeof(struct SoundPcmChannel) == 0x10 ? 1 : -1];
typedef char sound_h_check_pcm_id[(u32)&((struct SoundPcmChannel *)0)->sampleId == 0xC ? 1 : -1];
typedef char sound_h_check_fifo_size[sizeof(struct SoundFifoPos) == 8 ? 1 : -1];

/* --- ROM data formats --------------------------------------------------------------------------------------- */

/* A song header (0x18 bytes), gSongTable[songId] (58 songs). The data pointer is stored as two halfwords
 * because SoundMain reads it that way. */
struct SoundSong {
    u16 songDataLo;             /* +0x0: low half of the song data pointer */
    u16 songDataHi;             /* +0x2: high half */
    u16 trackOffset[10];        /* +0x4: per track (enum SoundBgmChannel), offset from the song data */
};

/* A sound effect (0x1C bytes), gSeTable[enum SoundEffect]. Named SoundSeDef because enum SoundEffect (the
 * ids) takes the tag. */
struct SoundSeDef {
    const u8 *tracks[6];        /* +0x00: bytecode per SE track (square 2, noise, PCM 5-2); NULL = unused */
    u8 priority;                /* +0x18: refused while a needed slot plays a higher priority */
    u8 slotMask;                /* +0x19: enum SoundSeSlot bits */
    u16 lockTicks;              /* +0x1A: loaded into seLockTimer; other SEs with lockTicks are refused while
                                 *        it runs */
};

/* A PCM sample, entries of gPcmSampleTable (bank 0, BGM instruments) and gPcmSampleTable2 (bank 1). */
struct SoundSample {
    s32 rate;                   /* +0x0: step multiplier (2048 for every sample in the game) */
    u32 length;                 /* +0x4: sample count */
    s32 loopStart;              /* +0x8: loop point, -1 = one-shot */
    s8 data[0];                 /* +0xC: `length` signed 8-bit samples (agbcc has no C99 []) */
};

typedef char sound_h_check_song_size[sizeof(struct SoundSong) == 0x18 ? 1 : -1];
typedef char sound_h_check_se_def_size[sizeof(struct SoundSeDef) == 0x1C ? 1 : -1];
typedef char sound_h_check_sample_data[(u32)&((struct SoundSample *)0)->data == 0xC ? 1 : -1];

/* --- RAM ------------------------------------------------------------------------------------------------------ */

/* The driver state. */
extern struct SoundDriver gSoundDriver;
/* Alias of gSoundDriver.seTracks (0x03005308), used by SoundReleaseSE / SoundReleaseAllSE; keep the
 * separate symbol where a unit uses it (matching choice). */
extern struct SoundSeTrack gSoundSeTracks[6];
/* The six software PCM channels (0x030053AC): 0-2 mix into FIFO A, 3-5 into FIFO B. */
extern struct SoundPcmChannel gSoundPcmChannels[6];
/* FIFO ring positions (0x0300540C). */
extern struct SoundFifoPos gSoundDmaPos;
/* FIFO A [0] and FIFO B [1] ring buffers (0x03005414); 0x2C0 bytes of each are used. */
extern s8 gSoundPcmBuffer[2][0x320];
/* IWRAM copy of the ARM inner mix loop SoundMixChannel (0x38 words), run there by SoundMixFifo. */
extern u32 gSoundMixCodeRam[0x38];

/* --- Game-side API (src/collection.c) ------------------------------------------------------------------------- */

/* gSaveData.options & OPTION_SE_ON. */
u16 IsSeEnabled(void);
/* gSaveData.options & OPTION_BGM_ON. */
u16 IsBgmEnabled(void);
/* Set or clear OPTION_SE_ON. */
void SetSeEnabled(u16 on);
/* Set or clear OPTION_BGM_ON. */
void SetBgmEnabled(u16 on);
/* Request sound effect seId (enum SoundEffect) if SEs are on; at most one per frame (gMain.lastSeFrame). */
void PlaySE(u32 seId);
/* Request song songId if music is on and it is not gMain.currentBgm; then currentBgm = songId. */
void PlayBGM(u32 songId);
/* Request song songId if music is on, without updating gMain.currentBgm. */
void PlayBGMNoTrack(u32 songId);
/* Like PlayBGM but gated on the SE option: the non-looping duel banner songs (callers wait with
 * SoundIsBGMPlaying). */
void PlayJingle(u32 songId);
/* Stop the song (if music is on); currentBgm = 0xFFFF. */
void StopBGM(void);
/* Fade the song out at speed 0x10 (if music is on); currentBgm = 0xFFFF. */
void FadeOutBGM(void);
/* Fade the song out at `speed` without checking the option; currentBgm = 0xFFFF. */
void FadeOutBGMAtSpeed(u32 speed);
/* Stop the song and every SE (each only if its option is on); currentBgm = 0xFFFF. */
void StopAllSound(void);

/* --- Driver: setup and per-frame entry points ------------------------------------------------------------------- */

/* Reset the PSG and the driver state, install SoundDma1Intr in *dma1IntrSlot (if not NULL), load the first
 * wave pattern and SoundDmaInit. */
void SoundInit(void (**dma1IntrSlot)(void));
/* Copy SoundMixChannel to gSoundMixCodeRam, clear the PCM channels and FIFO rings, start the FIFO DMAs and
 * Timer0. */
void SoundDmaInit(void);
/* Copy wave pattern gWaveRamPatterns[wave * 16 + level] into wave RAM (switching banks). */
void SoundLoadWaveRam(struct SoundDriver *drv, u32 wave, u32 level);
/* DMA1 IRQ (every 16 FIFO bytes): advance the DMA positions and restart both FIFO DMAs at the ring end. */
void SoundDma1Intr(void);
/* VBlank: SoundSequencerTick and the ARM mixer, unless SOUND_FLAG_TICK_PAUSED. */
void SoundVBlank(void);
/* Main loop: start the pending SE, then the pending song (fading out a different playing song first). */
void SoundMain(void);
/* One sequencer tick: master fade, BGM stop/pause/start, the 10 BGM and 6 SE tracks, PSG registers and
 * PCM channels. */
void SoundSequencerTick(struct SoundDriver *drv);
/* Advance SE track `track` (0-5) by one tick and write its channel request to *out. */
void SoundSeTrackTick(s32 track, struct SoundChannelOutput *out);
/* Start PCM sample sampleId on chan at volume and pitch (a gSoundPitchTable index). */
void SoundPcmStart(struct SoundPcmChannel *chan, s32 sampleId, s32 volume, s32 pitch);
/* Number of active PCM channels (PCM_FLAG_ACTIVE). */
int SoundCountActivePcm(void);

/* --- Driver: songs ---------------------------------------------------------------------------------------------------- */

/* pendingBgm = song, starting at full volume. */
void SoundRequestBGM(s32 song);
/* pendingBgm = song, fading in from 0 at fadeSpeed. */
void SoundRequestBGMFadeIn(s32 song, s32 fadeSpeed);
/* SoundRequestBGM(song) unless that song is already the active BGM. */
void SoundRequestBGMIfNotPlaying(s32 song);
/* 1 while `song` is the active BGM. */
int SoundIsBGMPlaying(s32 song);
/* Fade the active BGM to `volume` (speed 0x40); returns currentBgm, or -1 when no BGM is active. */
int SoundSetBGMVolume(s32 volume);
/* Fade the BGM volume to 0 at fadeSpeed; the song keeps running silently. */
void SoundFadeOutBGM(s32 fadeSpeed);
/* Fade the BGM out at fadeSpeed and freeze its tracks at volume 0 (SEs keep playing). */
void SoundPauseBGM(s32 fadeSpeed);
/* Undo SoundPauseBGM: fade back to full volume at fadeSpeed. */
void SoundResumeBGM(s32 fadeSpeed);
/* 1 when the BGM volume has reached its fade target. */
int SoundIsBGMFadeDone(void);
/* gSoundDriver.bgmTick. */
u32 SoundGetBGMTick(void);
/* Fast-forward: start `song` now (if song >= 0) and run `ticks` sequencer ticks synchronously. */
void SoundSeekBGM(s32 song, s32 ticks);
/* SoundSeekBGM, then fade in from 0 at fadeSpeed. */
void SoundSeekBGMFadeIn(s32 song, s32 ticks, s32 fadeSpeed);
/* Request SOUND_FLAG_STOP_BGM. */
void SoundStopBGM(void);
/* Request SOUND_FLAG_STOP_BGM | SOUND_FLAG_STOP_ALL_SE. */
void SoundStopAll(void);

/* --- Driver: sound effects ------------------------------------------------------------------------------------------- */

/* pendingSe = se at volume 0x10, variant 0. */
void SoundRequestSE(s32 se);
/* SoundRequestSE(se) with seVariant = variant & 3, started immediately. */
void SoundStartSEVariant(s32 se, s32 variant);
/* Start the pending SE if its slots are free (priority) and the SE lock allows it. */
void SoundStartPendingSE(void);
/* Release every track playing SE `se`: it leaves its sustain loop and plays to its end. */
void SoundReleaseSE(s32 se);
/* SoundReleaseSE for every active SE track. */
void SoundReleaseAllSE(void);
/* Request SOUND_FLAG_STOP_ALL_SE: every SE track ends on its next tick. */
void SoundStopAllSE(void);

/* --- ARM mixer (src/sound_mixer_arm.s) ---------------------------------------------------------------------------------
 * Hand-written ARM code with a register interface, not callable from C as declared; the prototypes only
 * give the symbols a type (SoundVBlank reaches SoundMixAll through a linker veneer; SoundDmaInit copies
 * SoundMixChannel to gSoundMixCodeRam). */

/* Mix PCM channels 0-2 into FIFO A and 3-5 into FIFO B. */
void SoundMixAll(void);
/* Clear and mix one span of a FIFO ring. */
void SoundMixFifo(void);
/* Inner mix loop; runs from its IWRAM copy. */
void SoundMixChannel(void);

#endif /* GUARD_SOUND_H */
