#include "global.h"
#include "main.h"
#include "duel.h"

#define gMain gUnk_03000040

/*
 * Canonical main.h splits +0x4888 as unk4888_0:4 / counter4888:2 / unk4888_6:2.
 * This unit reads bit 1 (skipScript) and bits 2-3 (unk4888_2) separately, so it keeps
 * a unit-specific view of that one byte at the same address.
 */
struct MainFlags4888 {
    u8 filler0[0x4888];
    u8 unk4888_0:1;
    u8 skipScript:1;                /* 0x4888 bit 1 */
    u8 unk4888_2:2;                 /* 0x4888 bits 2-3 */
    u8 rest4888_4:4;
};
#define gMainBits (*(struct MainFlags4888 *)&gUnk_03000040)

/* Save image (0x02011C20). */
struct SaveOpponent {
    u16 unk0_0:11;
    u16 unk0_11:5;
    u16 unk2;
};

struct SaveData {
    u8 filler0[0x20D0];
    struct SaveOpponent opponents[(0x215C - 0x20D0) / 4];  /* 0x20D0, indexed by opponent */
    u16 unk215C;
    u16 unk215E;                    /* 0x215E: counter, reset by some events */
    u16 unk2160;                    /* 0x2160 */
    u8 unk2162;                     /* 0x2162 */
    u8 unk2163;
    u16 unk2164;                    /* 0x2164: bit 0/1 set after a duel (see sub_0801BE90) */
};
extern struct SaveData gUnk_02011C20;
#define gSaveData gUnk_02011C20

struct Unk02015EE8 {
    u8 phase;                       /* 0x0: duel phase index (see program-flow) */
    u8 unk1_0:1;
    u8 unk1_1:7;
};
extern struct Unk02015EE8 gUnk_02015EE8;

extern const u16 gUnk_080819BE[];   /* 0x1C card IDs */

/* Per-opponent post-duel text IDs (0x10 bytes each). */
struct OpponentText {
    u16 win;        /* 0x0 */
    u16 lose;       /* 0x2 */
    u16 draw;       /* 0x4 */
    u16 unk6;       /* 0x6 */
    u16 unk8;       /* 0x8 */
    u16 winAlt4;    /* 0xA: if save state == 4 */
    u16 winAlt9;    /* 0xC: if save state == 9 */
    u16 unkE;
};
extern const struct OpponentText gUnk_080817FC[];
extern const u16 gUnk_08081AE4[];
extern const u16 gUnk_0808198C[];

u32 sub_08002FD0(void);
void sub_0801AE2C(void);
u32 sub_08001AE4(void);

u16 sub_0801BCFC(void)
{
    switch (gMain.step488A) {
    case 0:
        if (!gMainBits.skipScript) {
            if (sub_08002FD0()) {
                gMain.step488A++;
                gMain.seqIndex1 = 0;
                gMain.seqState1 = 0;
                gMain.seqState2 = 0;
            }
            return 0;
        }
        gMain.step488A++;
    case 1:
        sub_0801AE2C();
        gMain.step488A++;
    case 2:
        if (sub_08001AE4()) {
            gMain.step488A++;
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        return 0;
    }
    return 1;
}

u16 sub_08029D7C(void);
u16 sub_08029EC4(void);
u16 sub_08029E34(void);

u16 sub_0801BE0C(void)
{
    if (!gMain.counter4888)
        return sub_08029D7C();
    else if (gUnk_020192E0.result == 2)
        return sub_08029E34();
    else
        return sub_08029EC4();
}

void sub_0801F744(void);
void sub_0800817C(void);
void sub_08007E68(u32, u32);
void sub_08059258(u32);

u16 sub_0801BE58(void)
{
    sub_0801F744();
    gUnk_02015EE8.unk1_0 = 0;
    sub_0800817C();
    sub_08007E68(0, 8);
    sub_08059258(0);
    sub_08007E68(1, 8);
    return 1;
}

int sub_08063EA0(void);
u32 sub_08063EDC(u16);
void sub_08077948(u32);
void sub_08077998(u32);
void sub_080779E8(u32);

void sub_0801BE90(void)
{
    int before = sub_08063EA0();
    u32 i;
    int countBefore, countAfter;

    for (i = 0, countBefore = 0; i <= 0x1B; i++) {
        if (sub_08063EDC(gUnk_080819BE[i]))
            countBefore++;
    }
    switch (gUnk_020192E0.result) {
    case 1:
        sub_08077948(gMain.opponent);
        break;
    case 2:
        sub_08077998(gMain.opponent);
        break;
    case 3:
        sub_080779E8(gMain.opponent);
        break;
    }
    if (sub_08063EA0() > before)
        gSaveData.unk2164 |= 1;
    for (i = 0, countAfter = 0; i <= 0x1B; i++) {
        if (sub_08063EDC(gUnk_080819BE[i]))
            countAfter++;
    }
    if (countAfter > countBefore)
        gSaveData.unk2164 |= 2;
}

void sub_08001C10(u16 textId);
void sub_08077B24(u16 bgm);
u32 sub_0806EF74(void);
extern const u16 gUnk_08624CCE;
extern const u16 gUnk_08624CD0;
extern const u16 gUnk_08624CD2;

u32 sub_08063AF8(void);
u32 sub_08063B48(u16 pack);
u16 sub_0801B6D4(void);
void sub_0807761C(u16 card);
void sub_080754BC(void);
void sub_08077498(u16 card);
void sub_08077A28(void);
void sub_0800688C(u16 card, u16 b, u16 c);
u16 sub_08006D08(void);

#define RESET_SEQ()             \
    do {                        \
        gMain.seqIndex1 = 0;    \
        gMain.seqState1 = 0;    \
        gMain.seqState2 = 0;    \
    } while (0)

/* Campaign step after the post-duel text: rewards (cards / booster packs). */
u16 sub_0801BF80(void)
{
    switch (gMain.step488A) {
    case 0:
        sub_0801BE90();
        gMain.step488A = 1;
        return 0;
    case 1:
        switch (gUnk_020192E0.result) {
        case 1:
            RESET_SEQ();
            switch (gMain.events) {
            case 0x1000000:
                sub_08001C10(0xC8);
                gSaveData.unk215E++;
                gMain.rewardCard = gUnk_08624CD0;
                gMain.step488A = 0xA;
                return 0;
            case 0x2000000:
                sub_08001C10(0xCA);
                gSaveData.unk215E++;
                gMain.rewardCard = gUnk_08624CCE;
                gMain.step488A = 0xA;
                return 0;
            case 0x4000000:
                sub_08001C10(0xCC);
                gSaveData.unk215E++;
                gMain.rewardCard = gUnk_08624CD2;
                gMain.step488A = 0xA;
                return 0;
            case 0x8000000:
                sub_08001C10(0xCE);
                gSaveData.unk215E = 0;
                gSaveData.unk2162++;
                gMain.rewardCard = 0;
                gMain.step488A = 0xA;
                return 0;
            case 0x10000000:
                gSaveData.unk2160 = 1;
                gMain.rewardPack = 0x1F8;
                gMain.step488A = 0xF;
                return 0;
            case 0x20000000:
                sub_08001C10(0x2BF);
                gMain.rewardPack = 0x386;
                gMain.step488A = 0xE;
                return 0;
            case 0x800000:
                gMain.rewardPack = 0x1FA;
                gMain.step488A = 0xF;
                return 0;
            case 1:
            case 4:
            case 0x400:
            case 0x8000:
                gMain.rewardPack = 0x1FD;
                gMain.step488A = 0xF;
                return 0;
            case 2:
                gMain.rewardPack = 5;
                gMain.step488A = 0xF;
                return 0;
            case 8:
                gMain.rewardPack = 0x15;
                gMain.step488A = 0xF;
                return 0;
            case 0x10:
                gMain.rewardPack = 0xB;
                gMain.step488A = 0xF;
                return 0;
            case 0x20:
            case 0x200:
            case 0x1000:
                gMain.rewardPack = 0x1F9;
                gMain.step488A = 0xF;
                return 0;
            case 0x40:
                gMain.rewardPack = 0xC;
                gMain.step488A = 0xF;
                return 0;
            case 0x80:
                gMain.rewardPack = 0x16;
                gMain.step488A = 0xF;
                return 0;
            case 0x100:
                gMain.rewardPack = 0x17;
                gMain.step488A = 0xF;
                return 0;
            case 0x800:
                gMain.rewardPack = 4;
                gMain.step488A = 0xF;
                return 0;
            case 0x2000:
            case 0x4000:
            case 0x10000:
            case 0x20000:
                gMain.rewardPack = 0x1FA;
                gMain.step488A = 0xF;
                return 0;
            case 0x40000:
            case 0x80000:
                gMain.rewardPack = 0x1F8;
                gMain.step488A = 0xF;
                return 0;
            }
            gMain.step488A++;
            return 0;
        case 2:
            RESET_SEQ();
            switch (gMain.events) {
            case 0x800000:
                switch (gMain.opponent) {
                case 11: sub_08001C10(0x2AF9); break;
                case 12: sub_08001C10(0x2EE1); break;
                case 13: sub_08001C10(0x32C9); break;
                case 14: sub_08001C10(0x36B1); break;
                case 15: sub_08001C10(0x3A99); break;
                }
                gMain.step488A = 0x16;
                gMain.rewardCard = sub_0801B6D4();
                sub_0807761C(gMain.rewardCard);
                sub_080754BC();
                return 0;
            case 0x1000000:
            case 0x2000000:
            case 0x4000000:
            case 0x8000000:
                gSaveData.unk215E = 0;
            case 0x400000:
            case 0x10000000:
            case 0x20000000:
                sub_08001C10(0x12C);
                gMain.step488A = 0x14;
                return 0;
            }
            break;
        }
        return 1;
    case 2:
        {
            u16 flag = gSaveData.unk2164 & 2;
            if (flag) {
                gSaveData.unk2164 &= ~2;
                sub_08001C10(0x15F);
            } else {
                gMain.rewardPack = flag;
                RESET_SEQ();
                gMain.step488A++;
            }
        }
        gMain.step488A++;
        return 0;
    case 3:
        if (sub_08001AE4()) {
            gMain.step488A++;
            RESET_SEQ();
        }
        return 0;
    case 4:
        return sub_08063AF8();
    case 0xA:
        if (sub_08001AE4()) {
            if (gMain.rewardCard) {
                sub_08077498(gMain.rewardCard);
                sub_080754BC();
                gMain.step488A = 0xC;
                return 0;
            } else {
                sub_0807761C(gUnk_08624CCE);
                sub_0807761C(gUnk_08624CD0);
                sub_0807761C(gUnk_08624CD2);
                sub_08077A28();
                sub_080754BC();
                RESET_SEQ();
                gMain.step488A++;
            }
        }
        return 0;
    case 0xB:
        if (sub_08063B48(0x1FD)) {
            RESET_SEQ();
            gMain.step488A = 0x19;
            return 0;
        }
        return 0;
    case 0xC:
        sub_0800688C(gMain.rewardCard, 0, 0);
        gMain.step488A++;
    case 0xD:
        return sub_08006D08();
    case 0xE:
        if (sub_08001AE4()) {
            RESET_SEQ();
            gMain.step488A++;
        }
        return 0;
    case 0xF:
        return sub_08063B48(gMain.rewardPack);
    case 0x14:
        return sub_08001AE4();
    case 0x16:
        sub_0800688C(gMain.rewardCard, 0, 0);
        gMain.step488A++;
    case 0x17:
        if (sub_08006D08()) {
            RESET_SEQ();
            gMain.step488A++;
        }
        return 0;
    case 0x18:
        return sub_08001AE4();
    case 0x19:
        return 1;
    }
    return 1;
}


u16 sub_0801C938(void)
{
    s32 opp = gMain.opponent;
    u16 text;
    int done;

    switch (opp) {
    case 0:
    case 0x19:
    case 0x1F:
        return 1;
    }
    text = gUnk_08081AE4[opp];
    switch (gMain.step488A) {
    case 0:
        done = 0;
        if (gMainBits.unk4888_2 == 1) {
            done = 1;
        } else {
            switch (gUnk_020192E0.result) {
            case 1:
                gMain.score++;
                break;
            case 2:
                gMain.score--;
                break;
            case 3:
                break;
            }
            gMain.counter4888++;
            switch (gMain.counter4888) {
            case 3:
                if (gMain.score < 0)
                    gUnk_020192E0.result = 2;
                if (gMain.score > 0)
                    gUnk_020192E0.result = 1;
                if (gMain.score == 0)
                    gUnk_020192E0.result = 3;
                done = 1;
                break;
            case 2:
                switch (gMain.score) {
                case -2:
                    gUnk_020192E0.result = 2;
                    done = 1;
                    break;
                case 2:
                    gUnk_020192E0.result = 1;
                    done = 1;
                    break;
                }
                break;
            }
        }
        if (!done) {
            switch (gUnk_020192E0.result) {
            case 1:
                sub_08001C10(gUnk_080817FC[opp].unk6);
                break;
            case 2:
                sub_08001C10(gUnk_080817FC[opp].unk8);
                break;
            case 3:
                sub_08001C10(gUnk_080817FC[opp].unk8);
                break;
            }
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
            gMain.step488A++;
            gMain.step488A++;
            sub_08077B24(0x15);
            return 0;
        }
    show:
        switch (gUnk_020192E0.result) {
        case 1:
            switch (gSaveData.opponents[opp].unk0_0) {
            case 4:
                text = gUnk_080817FC[opp].winAlt4;
                break;
            case 9:
                text = gUnk_080817FC[opp].winAlt9;
                break;
            default:
                text = gUnk_080817FC[opp].win;
                break;
            }
            sub_08077B24(0x18);
            break;
        case 2:
            text = gUnk_080817FC[opp].lose;
            if (gMain.events == 0x800000)
                sub_08077B24(0x1C);
            else
                sub_08077B24(0x19);
            break;
        case 3:
            text = gUnk_080817FC[opp].draw;
            sub_08077B24(0x19);
            break;
        }
        if (gMain.events == 0x800000) {
            switch (gUnk_020192E0.result) {
            case 1:
                switch (opp) {
                case 11: text = 0x2AFB; break;
                case 12: text = 0x2EE3; break;
                case 13: text = 0x32CB; break;
                case 14: text = 0x36B3; break;
                case 15: text = 0x3A9B; break;
                }
                break;
            default:
                switch (opp) {
                case 11: text = 0x2AF8; break;
                case 12: text = 0x2EE0; break;
                case 13: text = 0x32C8; break;
                case 14: text = 0x36B0; break;
                case 15: text = 0x3A98; break;
                }
                break;
            }
        }
        sub_08001C10(text);
        gMain.seqIndex1 = 0;
        gMain.seqState1 = 0;
        gMain.seqState2 = 0;
        gMain.step488A++;
    case 1:
        if (sub_08001AE4())
            return 1;
        break;
    case 2:
        if (sub_08001AE4()) {
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
            gMainBits.skipScript = 1;
            gMain.step488A++;
        }
        break;
    case 3:
        if (sub_0806EF74()) {
            sub_08001C10(gUnk_0808198C[opp]);
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
            gMain.step488A++;
        }
        break;
    case 4:
        if (sub_08001AE4()) {
            gMain.seqIndexCampaign -= 3;
            gMain.step488A = 0;
            gMain.seqState0 = 0;
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        break;
    }
    return 0;
}

