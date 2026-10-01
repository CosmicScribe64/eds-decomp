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

#if 0 /* NONMATCHING: typed SE decoder. The candidate is 0x4AC bytes against 0x4A4 in the ROM. */
void sub_0807D6B4(s32 arg0, struct SoundChannelParams *arg1) {
    struct SoundDriver *driver = &gUnk_03005210;
    s32 temp_r3_3;
    s32 temp_r3_5;
    s32 var_r0_4;
    s32 var_r0_5;
    s32 var_r0_6;
    s32 var_r2_2;
    s32 var_r2_4;
    s32 var_r3_3;
    s32 var_r3_4;
    s8 var_r0_2;
    s8 var_r0_3;
    s32 var_r3;
    struct SoundSeTrack *temp_r6;
    struct SoundSeTrack *var_r0;
    struct SoundSeTrack *var_r1;
    u32 temp_r0_2;
    u16 temp_r3;
    s32 var_r3_2;
    u32 temp_r0_3;
    u8 temp_r0_4;
    u8 temp_r0_5;
    u8 temp_r1;
    u8 temp_r1_2;
    u8 temp_r1_3;
    u8 temp_r2;
    u8 temp_r2_2;
    u8 temp_r2_3;
    u8 temp_r3_2;
    u8 temp_r3_4;
    u8 temp_r4;
    s32 var_r2;
    s32 var_r2_3;
    s32 var_r2_5;
    const u8 *temp_r0;
    const u8 *var_r5;
    const u8 *var_r5_2;

    /* Flowgraph is not reducible, falling back to gotos-only mode. */
    temp_r6 = (struct SoundSeTrack *)&driver->seTracks[arg0];
    if (0x80 & temp_r6->flags) {
        goto block_2;
    }
    return;
block_2:
    *(u16 *)&arg1->dirty = 0;
    var_r5 = temp_r6->data;
    temp_r2 = temp_r6->flags;
    if (!(8 & temp_r2)) {
        goto block_9;
    }
    if ((s32) (s8) temp_r6->linkedTracks >= 0) {
        goto block_5;
    }
    return;
block_5:
    var_r3 = (s8) temp_r6->linkedTracks;
    if (var_r3 == 0) {
        goto block_8;
    }
    var_r0 = &temp_r6[var_r3];
loop_7:
    var_r0->linkedTracks = 0;
    var_r0--;
    var_r3 -= 1;
    if (var_r3 != 0) {
        goto loop_7;
    }
block_8:
    temp_r6->delay = 1;
    var_r5 = 0;
    temp_r6->flags = 0;
    goto block_31;
block_9:
    if (!(4 & driver->flags)) {
        goto block_11;
    }
    temp_r6->flags = 8 | temp_r2;
    goto block_5;
block_11:
    temp_r1 = 0x40 & temp_r2;
    if (temp_r1 != 0) {
        goto block_16;
    }
    temp_r6->flags = 0x40 | temp_r2;
    temp_r6->linkedTracks = 0;
    temp_r6->delay = (u16) temp_r1;
    temp_r6->channel.bytes.volume = 0;
    temp_r6->returnData = 0;
    goto block_31;
block_13:
    driver->seTracks[temp_r2_2 & 0xF].unk12 = 1;
    temp_r6->data = var_r5;
    goto block_121;
block_14:
    temp_r3 = ((temp_r2_2 & 0xF) << 8) | var_r5[0];
    var_r5 += 1;
    arg1->pitch = temp_r3;
    temp_r6->basePitch = temp_r3;
    var_r0_2 = 2;
    goto block_120;
block_15:
    var_r5 = temp_r0;
    temp_r6->returnData = 0;
    goto loop_25;
block_16:
    arg1->volume = (u8) temp_r6->channel.bytes.volume;
    temp_r2_3 = temp_r6->flags;
    if (!(1 & temp_r2_3)) {
        goto block_21;
    }
    if (arg0 <= 1) {
        goto block_20;
    }
    if (!(0x80 & gUnk_030053AC[gUnk_081A79E8[arg0]].flags)) {
        goto block_20;
    }
    goto block_124;
block_20:
    temp_r6->flags = 0xFE & temp_r2_3;
    arg1->command = 0x40;
    temp_r6->delay = 1;
block_21:
    temp_r0_2 = temp_r6->delay - 1;
    temp_r6->delay = temp_r0_2;
    if ((temp_r0_2 << 0x10) == 0) {
        goto block_23;
    }
    return;
block_23:
    temp_r1_2 = temp_r6->flags;
    if (!(2 & temp_r1_2)) {
        goto loop_25;
    }
    temp_r6->flags = 0xFD & temp_r1_2;
    arg1->command = 0x40;
loop_25:
    if (!(0x10 & temp_r6->flags)) {
        goto block_27;
    }
    temp_r6->delay = 1;
block_27:
    var_r2 = *var_r5++;
    if ((s32) var_r2 <= 0xEF) {
        goto block_29;
    }
    var_r2 = ((0xF & var_r2) << 8) + *var_r5++;
block_29:
    temp_r6->delay = (u16) var_r2;
    temp_r2_2 = *var_r5++;
    goto loop_111;
block_30:
    if (temp_r2_2 != 0xFD) {
        goto block_32;
    }
block_31:
    arg1->pitch = 0U;
    *(u16 *)&arg1->envelope = 0U;
    var_r0_3 = 0x40;
    goto block_77;
block_32:
    if (temp_r2_2 != 0xFC) {
        goto block_41;
    }
    temp_r3_2 = var_r5[0];
    var_r5 += 1;
    if (temp_r3_2 != 0) {
        goto block_36;
    }
    if (!(4 & temp_r6->flags)) {
        goto block_44;
    }
    var_r0_4 = 0xFB;
    goto block_39;
block_36:
    temp_r1_3 = temp_r6->flags;
    if (!(0x20 & temp_r1_3)) {
        goto block_40;
    }
    temp_r0_3 = temp_r6->loopCounter - 1;
    temp_r6->loopCounter = temp_r0_3;
    if ((temp_r0_3 << 0x18) != 0) {
        goto block_44;
    }
    var_r0_4 = 0xDF;
block_39:
    temp_r6->flags &= var_r0_4;
    var_r5 += 4;
    goto loop_25;
block_40:
    temp_r6->loopCounter = temp_r3_2;
    temp_r6->flags = temp_r1_3 | 0x20;
    goto block_44;
block_41:
    if (temp_r2_2 == 0xFB) {
        goto block_44;
    }
    if ((s32) temp_r2_2 <= 0xF9) {
        goto block_45;
    }
    temp_r6->returnData = (const u8 *) (var_r5 + 4);
block_44:
    var_r5 = (const u8 *)(var_r5[0] + (var_r5[1] << 8) + (var_r5[2] << 16) + (var_r5[3] << 24));
    goto loop_25;
block_45:
    if (temp_r2_2 == 0xF9) {
        goto block_47;
    }
    goto block_121;
block_47:
    temp_r6->flags |= 1;
    goto block_123;
block_48:
    if ((s32) temp_r2_2 <= 0x9F) {
        goto block_73;
    }
    if ((s32) temp_r2_2 <= 0xBF) {
        goto block_56;
    }
    if ((s32) temp_r2_2 <= 0xDF) {
        goto block_52;
    }
    var_r2_2 = var_r5[0] + (var_r5[1] << 8);
    var_r0_5 = var_r2_2 >> 4;
    goto block_58;
block_52:
    if ((s32) temp_r2_2 <= 0xCF) {
        goto block_54;
    }
    arg1->pitch = (u16) ((s16) (var_r5[0] | (var_r5[1] << 8)) + temp_r6->basePitch);
    var_r2_3 = var_r5[2];
    var_r5 += 1;
    goto block_55;
block_54:
    var_r2_3 = var_r5[0] | (var_r5[1] << 8);
    arg1->pitch = (u16) (temp_r6->basePitch + 0xFFFFFC00 + ((s32) var_r2_3 >> 5));
block_55:
    var_r0_6 = (var_r2_3 & 0x1F) + temp_r6->channel.bytes.volume;
    goto block_60;
block_56:
    if ((s32) temp_r2_2 <= 0xAF) {
        goto block_59;
    }
    var_r2_2 = var_r5[0] + (var_r5[1] << 8);
    var_r0_5 = (var_r2_2 >> 4) | 0x4000;
block_58:
    arg1->pitch = (u16) var_r0_5;
    var_r2_4 = var_r2_2 & 0xF;
    goto block_64;
block_59:
    temp_r3_3 = var_r5[0] | (var_r5[1] << 8);
    arg1->pitch = (u16) (((temp_r3_3 >> 5) + 0xFFFFFC00 + (s16) (0xFFFFBFFF & temp_r6->basePitch)) | 0x4000);
    var_r0_6 = (0x1F & temp_r3_3) + temp_r6->channel.bytes.volume;
block_60:
    var_r2_4 = var_r0_6 - 0x10;
    if (var_r2_4 >= 0) {
        goto block_62;
    }
    var_r2_4 = 0;
block_62:
    if (var_r2_4 <= 0xF) {
        goto block_64;
    }
    var_r2_4 = 0xF;
block_64:
    var_r3_2 = (3 & temp_r2_2) | (var_r2_4 << 8);
    if (8 & temp_r2_2) {
        goto block_68;
    }
    if (temp_r6->channel.packed == var_r3_2) {
        goto block_67;
    }
    temp_r6->channel.packed = var_r3_2;
    arg1->dirty = 2;
block_67:
    arg1->command = 2;
    *(u16 *)&arg1->envelope = var_r3_2;
block_68:
    if (temp_r2_2 & 4) {
        goto block_72;
    }
    temp_r6->basePitch = (u16) arg1->pitch;
    if (temp_r6->channel.packed == var_r3_2) {
        goto block_71;
    }
    var_r3_2 |= 8;
block_71:
    temp_r6->channel.packed = var_r3_2;
block_72:
    var_r5 += 2;
    goto block_121;
block_73:
    if ((s32) temp_r2_2 <= 0x7F) {
        goto block_78;
    }
    temp_r0_4 = temp_r2_2 & 0xF;
    arg1->volume = temp_r0_4;
    temp_r6->channel.bytes.volume = temp_r0_4;
    temp_r3_4 = var_r5[0];
    temp_r6->channel.bytes.flags = temp_r3_4;
    var_r5 += 1;
    if ((s32) temp_r2_2 > 0x8F) {
        goto block_76;
    }
    goto block_121;
block_76:
    arg1->pitch = (u16) (temp_r3_4 | 0xFFFF8000);
    var_r0_3 = 2;
block_77:
    arg1->command = var_r0_3;
    arg1->dirty = var_r0_3;
    goto block_121;
block_78:
    if ((s32) temp_r2_2 <= 0x6F) {
        goto block_80;
    }
    goto block_13;
block_80:
    if ((s32) temp_r2_2 <= 0x5F) {
        goto block_90;
    }
    arg1->sampleId = (s16) ((var_r5[0] + (var_r5[1] << 8)) | 0xFFFF8000);
    arg1->volume = (u8) var_r5[2];
    temp_r0_5 = var_r5[2];
    temp_r6->channel.bytes.volume = temp_r0_5;
    var_r5 += 3;
    temp_r6->basePitch = 0U;
    if (!(4 & temp_r2_2)) {
        goto block_83;
    }
    temp_r6->basePitch = (u16) (var_r5[0] | (var_r5[1] << 8));
    var_r5 += 2;
block_83:
    arg1->pitch = (u16) temp_r6->basePitch;
    arg1->command = 0x80;
    arg1->dirty = 2;
    var_r3_3 = (s32) temp_r0_5 >> 4;
    temp_r6->linkedTracks = (u8) var_r3_3;
    if (var_r3_3 == 0) {
        goto block_86;
    }
    var_r1 = &temp_r6[var_r3_3];
loop_85:
    var_r1->flags |= 0x88;
    var_r1->linkedTracks |= 0xFF;
    var_r1->data = 0;
    var_r1--;
    var_r3_3 -= 1;
    if (var_r3_3 != 0) {
        goto loop_85;
    }
block_86:
    if (!(temp_r2_2 & 8)) {
        goto block_121;
    }
    if (temp_r6->delay != 0) {
        goto block_89;
    }
    goto block_47;
block_89:
    temp_r6->flags |= 2;
    goto block_121;
block_90:
    if ((s32) temp_r2_2 <= 0x4F) {
        goto block_94;
    }
    var_r2_5 = temp_r2_2 & 0xF;
block_92:
    temp_r6->channel.bytes.volume = var_r2_5;
block_93:
    arg1->volume = var_r2_5;
    arg1->dirty = 2;
    goto block_121;
block_94:
    if ((s32) temp_r2_2 <= 0x3F) {
        goto block_101;
    }
    var_r2_5 = (s8) var_r5[0] + temp_r6->channel.bytes.volume;
    if ((s32) var_r2_5 >= 0) {
        goto block_97;
    }
    var_r2_5 = 0;
block_97:
    if ((s32) var_r2_5 <= 0x3F) {
        goto block_99;
    }
    var_r2_5 = 0x3F;
block_99:
    var_r5 += 1;
    if (1 & temp_r2_2) {
        goto block_93;
    }
    goto block_92;
block_101:
    if ((s32) temp_r2_2 <= 0x2F) {
        goto block_103;
    }
    goto block_14;
block_103:
    if ((s32) temp_r2_2 <= 0x1F) {
        goto block_107;
    }
    var_r3_4 = (((7 & temp_r2_2) << 8) | var_r5[0]) + 0xFFFFFC00;
    if (!(temp_r2_2 & 8)) {
        goto block_106;
    }
    temp_r6->basePitch = (u16) (temp_r6->basePitch + var_r3_4);
    var_r3_4 = 0;
block_106:
    arg1->pitch = (u16) (temp_r6->basePitch + var_r3_4);
    var_r0_2 = 2;
    goto block_120;
block_107:
    if ((s32) temp_r2_2 <= 0xF) {
        goto loop_111;
    }
    temp_r3_5 = 7 & temp_r2_2;
    temp_r4 = 0xEF & driver->seTracks[temp_r3_5].flags;
    driver->seTracks[temp_r3_5].flags = temp_r4;
    if (!(8 & temp_r2_2)) {
        goto block_110;
    }
    driver->seTracks[temp_r3_5].flags = temp_r4 | 0x10;
block_110:
    temp_r6->data = var_r5;
loop_111:
    if ((s32) temp_r2_2 > 0xEF) {
        goto block_113;
    }
    goto block_48;
block_113:
    if (temp_r2_2 != 0xFF) {
        goto block_117;
    }
    temp_r0 = temp_r6->returnData;
    if (temp_r0 == NULL) {
        goto block_116;
    }
    goto block_15;
block_116:
    temp_r6->flags |= 8;
    goto block_31;
block_117:
    if (temp_r2_2 == 0xFE) {
        goto block_119;
    }
    goto block_30;
block_119:
    var_r0_2 = 0x40;
block_120:
    arg1->command = var_r0_2;
block_121:
    if (temp_r6->delay != 0) {
        goto block_123;
    }
    goto loop_25;
block_123:
    temp_r6->data = var_r5;
block_124:
    arg1->volume = (u8) ((s32) (temp_r6->volume * arg1->volume) >> 4);
    return;
}

#endif
INCLUDE_ASM("asm/nonmatching/sound_driver", sub_0807D6B4);

#if 0 /* NONMATCHING: typed main sequencer. The candidate is 0x7DC bytes with a 0x70-byte stack
       * frame, and the target is 0x7CC bytes. */
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
/* Preserve the two fields aliased by PCM command F0 at output[9] + 0xB/0xC.
 * The compiler supplies the remaining 4-byte cursor spill in the 0x70 frame. */
struct SoundTickFrame {
    struct SoundChannelParams output[10];
    const u8 *songData;
    struct SoundBgmTrack *lastTrack;
    struct SoundBgmTrack *firstTrack;
    u8 *route1;
    struct SoundChannelParams *lastOutput;
    u8 *route2;
    u8 *route3;
};
extern const u16 gUnk_08139F50[];
extern const u16 gUnk_081AA20C[];
extern const s16 gUnk_081ABC4C[];
void sub_0807D6B4(s32 index, struct SoundChannelParams *output);
void sub_0807E918(struct SoundPcmVoice *voice, s32 id, s32 volume, s32 note);
typedef char bgm_track_size_check[sizeof(struct SoundBgmTrack) == 0x18 ? 1 : -1];
typedef char tick_frame_size_check[sizeof(struct SoundTickFrame) == 0x6C ? 1 : -1];
typedef char tick_song_offset_check[(u32)&((struct SoundTickFrame *)0)->songData == 0x50 ? 1 : -1];
typedef char tick_cursor_offset_check[(u32)&((struct SoundTickFrame *)0)->lastTrack == 0x54 ? 1 : -1];

void sub_0807DB58(struct SoundDriver *p) {
    u16 *flags;
    struct SoundTickFrame frame;
    struct SoundPcmVoice *var_r4_7;
    struct SoundChannelParams *var_r7;
    struct SoundChannelParams *var_r7_2;
    s16 temp_r4_6;
    s16 temp_r5_5;
    s16 temp_r5_6;
    const struct SoundSample *const *var_r0_2;
    s32 temp_r4;
    s32 temp_r5;
    s32 var_r3;
    s32 var_r4_2;
    s32 var_r4_4;
    s32 var_r4_5;
    s32 var_r5;
    s32 var_r5_3;
    s32 var_r9;
    s32 var_r9_2;
    s32 var_r9_3;
    s32 var_r9_4;
    s32 var_r9_5;
    s32 var_r9_6;
    s8 *temp_r0;
    s8 temp_r1_6;
    struct SoundTrack *var_r4_6;
    struct SoundBgmTrack *var_r6;
    struct SoundBgmTrack *var_r6_2;
    u32 temp_r0_2;
    u16 temp_r0_3;
    u16 temp_r0_6;
    u16 temp_r1;
    u16 temp_r1_7;
    s32 temp_r3;
    u16 temp_r5_4;
    u16 temp_r5_7;
    u16 var_r0;
    u16 var_r1;
    u32 var_r3_2;
    u16 var_r5_2;
    u16 var_r5_4;
    const u8 *temp_r2_2;
    const u8 *temp_r2_3;
    u8 *temp_r4_2;
    u8 *temp_r4_3;
    u8 *temp_r4_4;
    struct SoundBgmTrack *var_r6_3;
    u8 temp_r0_4;
    u32 temp_r0_5;
    u32 temp_r0_7;
    u8 temp_r1_2;
    u8 temp_r1_3;
    u8 temp_r1_4;
    u8 temp_r1_5;
    u8 temp_r2;
    u8 temp_r2_4;
    u8 temp_r4_5;
    u8 temp_r5_2;
    u8 temp_r5_3;
    u8 temp_r5_8;
    u8 temp_r6;
    u8 var_r4;
    u32 var_r4_3;
    struct SoundChannelParams *var_r7_3;
    struct SoundChannelParams *var_r7_4;

    /* Flowgraph is not reducible, falling back to gotos-only mode. */
    temp_r4 = p->targetVolume << 8;
    temp_r5 = temp_r4 - (*(u16 *)&p->fadeTimer);
    flags = &p->flags;
    temp_r3 = 0xFBF7 & *flags;
    *flags = temp_r3;
    temp_r6 = p->volume;
    if (temp_r5 == 0) {
        goto block_8;
    }
    if (temp_r5 <= 0) {
        goto block_4;
    }
    var_r5 = temp_r5 - (p->fadeSpeed * 0x10);
    if (var_r5 >= 0) {
        goto block_6;
    }
    goto block_5;
block_4:
    var_r5 = temp_r5 + (p->fadeSpeed * 0x10);
    if (var_r5 <= 0) {
        goto block_6;
    }
block_5:
    var_r5 = 0;
block_6:
    (*(u16 *)&p->fadeTimer) = (u16) (temp_r4 - var_r5);
    if (temp_r6 == p->volume) {
        goto block_11;
    }
    var_r1 = *flags;
    var_r0 = 8;
    goto block_10;
block_8:
    if (!(0xF & temp_r6)) {
        goto block_11;
    }
    var_r1 = 0x400;
    var_r0 = temp_r3;
block_10:
    *flags = var_r0 | var_r1;
block_11:
    if (!(0x100 & *flags)) {
        goto block_16;
    }
    if (temp_r6 != 0) {
        goto block_16;
    }
    var_r7 = &frame.output[9];
    var_r9 = 9;
loop_14:
    *(u32 *)var_r7 = 0;
    *(u32 *)&var_r7->dirty = 0;
    var_r7--;
    var_r9 -= 1;
    if (var_r9 >= 0) {
        goto loop_14;
    }
    goto block_101;
block_16:
    temp_r1 = p->flags;
    frame.songData = p->songData;
    var_r6 = ((struct SoundBgmTrack *)p->bgmTracks);
    frame.firstTrack = var_r6;
    if (!(1 & temp_r1)) {
        goto block_20;
    }
    var_r7_2 = &frame.output[0];
    var_r9_2 = 9;
loop_18:
    var_r6->flags = 0;
    var_r6++;
    *(u32 *)var_r7_2 = 0;
    var_r7_2->dirty = 1;
    var_r7_2->command = 0x40;
    var_r7_2++;
    var_r9_2 -= 1;
    if (var_r9_2 >= 0) {
        goto loop_18;
    }
    p->flags = temp_r1 & 0xFFFFBF7E;
    p->status = -1U;
    goto block_101;
block_20:
    if (0xC0 & temp_r1) {
        goto block_22;
    }
    return;
block_22:
    frame.lastOutput = &frame.output[9];
    temp_r4_2 = &((struct SoundBgmTrack *)p->bgmTracks)[1].routing;
    frame.route1 = temp_r4_2;
    temp_r4_3 = temp_r4_2 + 0x18;
    frame.route2 = temp_r4_3;
    temp_r4_4 = temp_r4_3 + 0x18;
    frame.route3 = temp_r4_4;
    frame.lastTrack = (struct SoundBgmTrack *)(temp_r4_4 + 0x7C);
    if (temp_r1 & 0x4000) {
        goto block_29;
    }
    p->flags = temp_r1 | 0x4000;
block_24:
    p->status = 0;
    var_r6_2 = frame.firstTrack;
    var_r6_2->routing = 0x11;
    *frame.route1 = 0x22;
    *frame.route2 = 0x44;
    *frame.route3 = 0x88;
    frame.lastTrack->routing = 0x33;
    ((struct SoundBgmTrack *)p->bgmTracks)[7].routing = 0x33;
    temp_r0 = (s8 *)&((struct SoundBgmTrack *)p->bgmTracks)[7] - 0x1C;
    *temp_r0 = 0x33;
    ((struct SoundBgmTrack *)p->bgmTracks)[8].routing = 0x33;
    ((struct SoundBgmTrack *)p->bgmTracks)[6].routing = 0x33;
    *(temp_r0 - 0x18) = 0x33;
    *(vu8 *)0x04000081 = 0xFFU;
    *(vu16 *)0x04000082 = 0x330E;
    var_r9_3 = 9;
loop_25:
    temp_r2 = 0xFE & var_r6_2->flags;
    var_r6_2->flags = temp_r2;
    if (!(0x40 & temp_r2)) {
        goto block_27;
    }
    var_r6_2->flags = temp_r2 | 0x80;
block_27:
    var_r6_2->flags &= 0xC0;
    var_r6_2->position = 0;
    var_r6_2->returnPosition = 0;
    *(u16 *)&var_r6_2->instrument = 0;
    *(u16 *)&var_r6_2->vibratoPhase = 0;
    var_r6_2++;
    var_r9_3 -= 1;
    if (var_r9_3 >= 0) {
        goto loop_25;
    }
    ((struct SoundBgmTrack *)p->bgmTracks)[1].instrument = 0x80;
    ((struct SoundBgmTrack *)p->bgmTracks)[0].instrument = 0x80;
    *(vu16 *)0x04000072 = 0;
    sub_0807D518(p, 0U, 0U);
block_29:
    p->status += 1;
    var_r7_3 = frame.lastOutput;
    var_r6_3 = frame.lastTrack;
    var_r9_4 = 9;
loop_30:
    *(u32 *)var_r7_3 = 0;
    *(u32 *)&var_r7_3->dirty = 0;
    temp_r1_2 = var_r6_3->flags;
    if (0x80 & temp_r1_2) {
        goto block_32;
    }
    goto block_84;
block_32:
    if (1 & temp_r1_2) {
        goto block_34;
    }
    var_r6_3->flags = (u8) (temp_r1_2 | 1);
    var_r3 = 0;
    goto block_58;
block_34:
    temp_r0_2 = var_r6_3->delay - 1;
    var_r6_3->delay = temp_r0_2;
    if ((temp_r0_2 << 0x10) == 0) {
        goto block_36;
    }
    goto block_84;
block_36:
    var_r3_2 = var_r6_3->position;
block_37:
    temp_r2_2 = &(&frame.songData[var_r6_3->songOffset])[var_r3_2];
    var_r3 = var_r3_2 + 1;
    temp_r4_5 = temp_r2_2[0];
    if ((s32) temp_r4_5 <= 0xFC) {
        goto block_45;
    }
    if (temp_r4_5 != 0xFF) {
        goto block_40;
    }
    p->flags |= 1;
    goto block_44;
block_40:
    if (temp_r4_5 != 0xFE) {
        goto block_42;
    }
    goto block_24;
block_42:
    var_r6_3->flags = (u8) (0x40 & var_r6_3->flags);
    if ((*(u16 *)&var_r7_3->dirty) == 0) {
        goto block_44;
    }
    goto block_98;
block_44:
    var_r6_3->channelVolume = 0U;
    *(u16 *)&var_r7_3->envelope = 0;
    var_r7_3->pitch = (u16) var_r6_3->pitch;
    var_r7_3->command = 0x40U;
    var_r7_3->dirty = 0x40;
    goto block_98;
block_45:
    if ((s32) temp_r4_5 <= 0xEF) {
        goto block_57;
    }
    if (temp_r4_5 != 0xF3) {
        goto block_48;
    }
    var_r6_3->songOffset = (u16) (var_r6_3->songOffset + var_r3);
    var_r3 = 0;
    goto block_77;
block_48:
    if (temp_r4_5 != 0xF2) {
        goto block_50;
    }
    var_r3 += 1;
    var_r7_3->sampleId = (s16) var_r6_3->instrument;
    var_r7_3->pitch = (s16) (((s16) var_r6_3->pitch - 0x40) + temp_r2_2[1]);
    var_r7_3->command = 1U;
    goto block_77;
block_50:
    if ((s32) temp_r4_5 <= 0xF0) {
        goto block_52;
    }
    var_r3 += 1;
    var_r6_3->vibratoDepth = (u8) ((u8) temp_r2_2[1] >> 1);
    var_r6_3->flags = (u8) (var_r6_3->flags | 0x20);
    goto block_77;
block_52:
    if (temp_r4_5 == 0xF0) {
        goto block_54;
    }
    goto block_77;
block_54:
    var_r3 += 1;
    temp_r5_2 = temp_r2_2[1];
    if (var_r9_4 <= 3) {
        goto block_56;
    }
    var_r4 = temp_r5_2 & 0xF;
    *((u8 *)var_r7_3 + 0xB) = (s8) ((s32) temp_r5_2 >> 4);
    *((u8 *)var_r7_3 + 0xC) = 1;
    goto block_59;
block_56:
    var_r6_3->routing = temp_r5_2;
    goto block_77;
block_57:
    if ((s32) temp_r4_5 <= 0xDF) {
        goto block_60;
    }
block_58:
    var_r4 = 0;
    var_r7_3->pitch = (u16) var_r6_3->pitch;
    var_r7_3->command = 0x40U;
block_59:
    var_r6_3->channelVolume = var_r4;
    var_r7_3->envelope = (u8) var_r6_3->instrument;
    var_r7_3->dirty = 1;
    goto block_77;
block_60:
    if ((s32) temp_r4_5 <= 0xCF) {
        goto block_63;
    }
    var_r4 = temp_r4_5 & 0xF;
    var_r3 += 1;
    temp_r0_3 = temp_r2_2[1] << 5;
    var_r7_3->pitch = temp_r0_3;
    var_r6_3->pitch = temp_r0_3;
    var_r7_3->command = 1U;
    if (var_r4 == var_r6_3->channelVolume) {
        goto block_77;
    }
    goto block_59;
block_63:
    if ((s32) temp_r4_5 <= 0xBF) {
        goto block_65;
    }
    var_r4 = temp_r4_5 & 0xF;
    var_r7_3->pitch = (u16) var_r6_3->pitch;
    var_r7_3->command = 1U;
    goto block_59;
block_65:
    if ((s32) temp_r4_5 <= 0x9F) {
        goto block_69;
    }
    var_r6_3->channelVolume = (u8) (temp_r4_5 & 0xF);
    temp_r0_4 = temp_r2_2[1];
    var_r7_3->sampleId = (s16) temp_r0_4;
    var_r6_3->instrument = temp_r0_4;
    var_r3 += 1;
    var_r5_2 = 0;
    if ((s32) temp_r4_5 <= 0xAF) {
        goto block_68;
    }
    var_r5_2 = (s8)temp_r2_2[2] << 5;
    var_r3 += 1;
block_68:
    var_r6_3->pitch = var_r5_2;
    var_r7_3->pitch = var_r5_2;
    *(u16 *)&var_r7_3->envelope = *(u16 *)&var_r6_3->instrument;
    var_r7_3->command = 0x80U;
    goto block_77;
block_69:
    if ((s32) temp_r4_5 <= 0x8F) {
        goto block_72;
    }
    var_r6_3->returnSongOffset = var_r6_3->songOffset;
    var_r6_3->returnPosition = (u16) (var_r3 + 3);
    var_r6_3->songOffset = (u16) gUnk_080E09D0[p->currentBgm].offset[var_r9_4];
    temp_r1_3 = temp_r2_2[3];
    var_r6_3->loopCounter = temp_r1_3;
    var_r3 = temp_r2_2[1] | ((u8) temp_r2_2[2] << 8);
    var_r4_2 = temp_r4_5 & 0xF;
    if (var_r4_2 == 0xF) {
        goto block_80;
    }
    var_r6_3->loopCounter = (u8) (temp_r1_3 + 1);
    goto block_74;
block_72:
    if ((s32) temp_r4_5 <= 0x7F) {
        goto block_77;
    }
    var_r4_2 = temp_r4_5 & 0xF;
block_74:
    if (var_r4_2 <= 3) {
        goto block_76;
    }
    var_r4_2 -= 4;
    sub_0807D518(p, (u32) var_r4_2, (u32) var_r6_3->channelVolume);
block_76:
    var_r7_3->envelope = (s8) var_r4_2;
    var_r6_3->instrument = (u8) var_r4_2;
block_77:
    if (var_r6_3->returnPosition == 0) {
        goto block_80;
    }
    temp_r0_5 = var_r6_3->loopCounter - 1;
    var_r6_3->loopCounter = temp_r0_5;
    if ((temp_r0_5 << 0x18) != 0) {
        goto block_80;
    }
    var_r6_3->songOffset = (u16) var_r6_3->returnSongOffset;
    var_r3 = (s32) var_r6_3->returnPosition;
block_80:
    temp_r2_3 = &(&frame.songData[var_r6_3->songOffset])[var_r3];
    var_r4_3 = temp_r2_3[0];
    var_r3_2 = var_r3 + 1;
    if ((s32) var_r4_3 <= 0xEF) {
        goto block_82;
    }
    var_r4_3 = ((0xF & var_r4_3) << 8) + temp_r2_3[1];
    var_r3_2 += 1;
block_82:
    var_r6_3->position = var_r3_2;
    var_r6_3->delay = (u16) var_r4_3;
    if ((var_r4_3 << 0x10) != 0) {
        goto block_84;
    }
    goto block_37;
block_84:
    temp_r5_3 = var_r6_3->flags;
    if (!(4 & temp_r5_3)) {
        goto block_88;
    }
    var_r4_4 = var_r6_3->fadeCounter - 1;
    if (var_r4_4 > 0) {
        goto block_87;
    }
    var_r4_4 = 0;
    var_r6_3->flags = (u8) (0xFB & temp_r5_3);
    var_r7_3->command = 0x40U;
block_87:
    var_r6_3->fadeCounter = (s8) var_r4_4;
    var_r6_3->channelVolume = (u8) ((s32) (var_r6_3->fadeVolume * var_r4_4) >> 2);
    var_r7_3->pitch = (u16) var_r6_3->pitch;
    *(u16 *)&var_r7_3->envelope = *(u16 *)&var_r6_3->instrument;
    var_r7_3->dirty = 1;
block_88:
    if (!(temp_r5_3 & 0x20)) {
        goto block_93;
    }
    temp_r2_4 = var_r6_3->vibratoDepth;
    if (temp_r2_4 != 0) {
        goto block_91;
    }
    var_r6_3->flags = (u8) (0xDF & var_r6_3->flags);
    var_r6_3->vibratoPhase = temp_r2_4;
    var_r4_5 = 0;
    goto block_92;
block_91:
    var_r6_3->vibratoPhase = (u8) (var_r6_3->vibratoPhase + 0x18);
    var_r4_5 = (s32) (var_r6_3->vibratoDepth * gUnk_081ABC4C[var_r6_3->vibratoPhase]) >> 0xC;
block_92:
    var_r7_3->pitch = (s16) (var_r6_3->pitch + var_r4_5);
    *(u16 *)&var_r7_3->envelope = *(u16 *)&var_r6_3->instrument;
    var_r7_3->sampleId = (s16) var_r6_3->instrument;
    var_r7_3->command = 1U;
block_93:
    var_r7_3->volume = (s8) ((s32) (p->volume * var_r6_3->channelVolume) >> 4);
    if (!(0x408 & p->flags)) {
        goto block_98;
    }
    if (var_r6_3->channelVolume == 0) {
        goto block_98;
    }
    if (var_r7_3->command != 0) {
        goto block_97;
    }
    var_r7_3->pitch = (u16) var_r6_3->pitch;
block_97:
    var_r7_3->envelope = (u8) var_r6_3->instrument;
    var_r7_3->dirty = 1;
block_98:
    var_r9_4 -= 1;
    var_r6_3--;
    var_r7_3--;
    if (var_r9_4 < 0) {
        goto block_100;
    }
    goto loop_30;
block_100:
    (*((u8 *)p + 0x198)) = (u8) (frame.firstTrack->routing | *frame.route1 | *frame.route2 | *frame.route3);
block_101:
    temp_r0_6 = 0x40 & p->flags;
    if (temp_r0_6 == 0) {
        goto block_108;
    }
    temp_r0_7 = p->sePriority - 1;
    p->sePriority = temp_r0_7;
    if ((s32) (temp_r0_7 << 0x18) >= 0) {
        goto block_104;
    }
    p->sePriority = 0;
block_104:
    var_r4_6 = p->seTracks;
    sub_0807D6B4(0, &frame.output[1]);
    sub_0807D6B4(1, &frame.output[3]);
    sub_0807D6B4(2, &frame.output[9]);
    sub_0807D6B4(3, &frame.output[8]);
    sub_0807D6B4(4, &frame.output[7]);
    sub_0807D6B4(5, &frame.output[6]);
    var_r5_3 = 0;
    var_r9_5 = 5;
loop_105:
    var_r5_3 |= var_r4_6->flags;
    var_r4_6++;
    var_r9_5 -= 1;
    if (var_r9_5 >= 0) {
        goto loop_105;
    }
    temp_r1_4 = 0x80 & var_r5_3;
    if (temp_r1_4 != 0) {
        goto block_109;
    }
    p->sePriority = temp_r1_4;
    p->flags &= 0xFFBF;
    goto block_109;
block_108:
    p->sePriority = (u8) temp_r0_6;
block_109:
    *(vu8 *)0x04000081 = (u8) (*((u8 *)p + 0x198));
    if ((*(u16 *)&frame.output[0].dirty) == 0) {
        goto block_113;
    }
    temp_r5_4 = gUnk_081AA20C[(s16)frame.output[0].pitch];
    if ((u8) (*(u16 *)&frame.output[0].dirty) == 0) {
        goto block_112;
    }
    *(vu16 *)0x04000062 = (frame.output[0].volume << 0xC) | frame.output[0].envelope;
    *(vu16 *)0x04000064 = temp_r5_4;
    goto block_113;
block_112:
    *(vu16 *)0x04000064 = (u16) (temp_r5_4 & 0x7FF);
block_113:
    if ((*(u16 *)&frame.output[1].dirty) == 0) {
        goto block_121;
    }
    temp_r5_5 = frame.output[1].pitch;
    if ((s32) temp_r5_5 < 0) {
        goto block_116;
    }
    if (0x4000 & temp_r5_5) {
        goto block_117;
    }
block_116:
    var_r5_4 = gUnk_081AA20C[(s16)temp_r5_5];
    goto block_118;
block_117:
    var_r5_4 = (temp_r5_5 & 0xFFFFBFFF) | 0x8000;
block_118:
    if ((u8) (*(u16 *)&frame.output[1].dirty) == 0) {
        goto block_120;
    }
    *(vu16 *)0x04000068 = (frame.output[1].volume << 0xC) | frame.output[1].envelope;
    *(vu16 *)0x0400006C = var_r5_4;
    goto block_121;
block_120:
    *(vu16 *)0x0400006C = (u16) (var_r5_4 & 0x7FF);
block_121:
    if ((*(u16 *)&frame.output[2].dirty) == 0) {
        goto block_126;
    }
    temp_r5_6 = 0x7FF & gUnk_081AA20C[(s16)frame.output[2].pitch];
    temp_r1_5 = frame.output[2].volume;
    if (temp_r1_5 != 0) {
        goto block_124;
    }
    *(vu16 *)0x04000072 = (s16) temp_r1_5;
    goto block_125;
block_124:
    sub_0807D518(p, (u32) frame.output[2].envelope, (u32) frame.output[2].volume);
    *(vu16 *)0x04000072 = 0x2000;
block_125:
    *(vu16 *)0x04000074 = temp_r5_6;
block_126:
    temp_r5_7 = (*(u16 *)&frame.output[3].dirty);
    if (temp_r5_7 == 0) {
        goto block_130;
    }
    temp_r4_6 = frame.output[3].volume << 0xC;
    if (temp_r5_7 & 0x202) {
        goto block_129;
    }
    *(vu16 *)0x04000078 = temp_r4_6;
    *(vu16 *)0x0400007C = gUnk_08139F50[(s16)frame.output[3].pitch];
    goto block_130;
block_129:
    *(vu16 *)0x04000078 = temp_r4_6;
    *(vu16 *)0x0400007C = (u16) frame.output[3].pitch;
block_130:
    var_r7_4 = &frame.output[9];
    var_r4_7 = &gUnk_030053AC[5];
    var_r9_6 = 5;
loop_131:
    temp_r5_8 = var_r7_4->command;
    if (temp_r5_8 == 0) {
        goto block_140;
    }
    temp_r1_6 = 0x80 & temp_r5_8;
    if (temp_r1_6 == 0) {
        goto block_134;
    }
    sub_0807E918(var_r4_7, (u16)var_r7_4->sampleId, var_r7_4->volume, (s16)var_r7_4->pitch);
    goto block_140;
block_134:
    if (!(temp_r5_8 & 0x40)) {
        goto block_136;
    }
    var_r4_7->flags = temp_r1_6;
    goto block_140;
block_136:
    temp_r1_7 = var_r7_4->sampleId;
    if (!(0x8000 & temp_r1_7)) {
        goto block_138;
    }
    var_r0_2 = &gUnk_08088A20[0x3FFF & temp_r1_7];
    goto block_139;
block_138:
    var_r0_2 = &gUnk_0811B420[(u16)var_r7_4->sampleId];
block_139:
    *(u16 *)&var_r4_7->stepAndFraction = (s16) ((s32) ((*var_r0_2)->rate * gUnk_081A960C[(s16)var_r7_4->pitch]) >> 0xC);
block_140:
    if (var_r7_4->dirty == 0) {
        goto block_142;
    }
    var_r4_7->volume = (u8) var_r7_4->volume;
block_142:
    var_r4_7--;
    var_r7_4--;
    var_r9_6 -= 1;
    if (var_r9_6 >= 0) {
        goto loop_131;
    }
    return;
}
#endif
INCLUDE_ASM("asm/nonmatching/sound_driver", sub_0807DB58);

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
