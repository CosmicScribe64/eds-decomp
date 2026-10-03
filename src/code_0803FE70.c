#include "global.h"

/*
 * Target-selection routines ("choose a card/zone" prompts), int f(struct CardRef *ref), returning 1 when done and 0 while waiting.
 * Step counter at 0x02017A40+0x3E5 (0 = prompt, >0 = wait for input). See wiki/functions/code-0803fe70.md.
 */
struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 unk4_0 : 2;
    u8 skip4 : 1;
    u8 unk4_3 : 5;
    u8 unk5;
    u16 pos;
    u16 unk8;
    u8 numTargets : 3;  /* +0x0A bits 0-2 */
    u8 unkA_3 : 5;
    u8 unkB;
    u16 targets[3];     /* +0x0C */
};

void PlaySE(int a);
void TextBoxOpen(u32 a, u32 b, u32 c, const void *d);
void DuelPrompt_Post(int a, int b, int c, int d);
void AddEffectTarget(struct CardRef *ref, u16 v);
u16 TryAddEffectTarget(struct CardRef *ref, int a, int z);
int CanCardTargetZone(u16 id, int a, int z);
u32 DuelCursor_PickTarget(u32 keys);
extern u8 gChain[];
#define SEL_STEP gChain[0x3E5]
struct MainView { u8 unk0[6]; u16 h6; };
extern struct MainView gMain;
struct DuelScreenView { u8 unk0[0x824]; u32 w824; u32 w828; u32 w82C; };
extern struct DuelScreenView gDuelScreen;
struct DuelGlobal {
    u8 unk0[0x1B64];
    u16 w1B64;          /* prompt result */
    u16 w1B66;
};
extern struct DuelGlobal gDuel;
extern const u8 gStrDesignateOpponentSpellTrapToDestroy[];
void FormatStr(char *dst, const char *fmt, const char *arg);
int EffectOwnSkullOrThunderCheck(struct CardRef *ref, u16 v);
int GetZoneCardType(int player, int zone);
extern const u16 gUnk_08623E1E[];
extern const char gCardNames[][0x40];
extern const char gStrDesignateFaceUpMonsterOfTwoFmt[], gStrThunderType[], gStrSelectOpponentMonsterToControlFmt[], gStrMachineType[];
extern const u8 gStrDesignateOpponentMonsterToTribute[];
extern const u8 gStrDesignateEquipToSwitch[];
extern const char gStrDesignateMonsterToSwitchEquipFmt[];
int EffectTailorOfTheFickleCheck(struct CardRef *ref, u16 pos);
int IsValidEquipTarget(int a, int b, int c, int d);
int FindMonsterLinkedToCard(int a, int b);
void DuelCmd_Push(u16 msg, u16 arg1, u16 arg2, u16 arg3);
extern const u8 gStrDesignateSpellTrapToDestroy[];
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
extern const u8 gStrDesignateMonsterToHalveAtk[];
int EffectRiryokuPrepare(struct CardRef *ref, int a, int b);
extern const u8 gStrDesignateOpponentMonsterToDestroy[], gStrDesignateMonsterToGiveControl[];
extern const u8 gStrDesignateNewTargetFmt[];
int CanRedirectEffectToZone(u16 num, struct CardRef *other, int p, int zn);
extern const u8 gStrDesignateMonsterToIncreaseAtk[], gStrDesignateMonsterToIncreaseDef[], gStrDesignateMonsterToDecreaseDef[];
extern const u8 gStrDesignateMonsterToSetFaceDown[];
extern const u16 gCardIdToNumber[];
extern const u8 gStrDesignateMonsterForDefensePosition[];
int DuelScreen_FadeOutStep(void);
void ResetVideo(void);
int ProhibitCardSelect_Run(void);
void DuelScreen_Init(void);
int DuelScreen_FadeInStep(void);
void sub_08019820(int player, int id);
extern const u8 gStrDesignateCardToProhibit[];
struct DuelCard { u32 id : 12; u32 unk12 : 20; };
struct DuelZone {
    struct DuelCard card;   /* +0x00 */
    u8 unk4;
    u8 unk5;
    u8 flags6;
    u8 unk7[0x94 - 7];
};
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gDuelZones[2];
#define ZA(o) ((struct DuelZone *)((o) + (0xD64 * ((1 - ref->player) & 1)) + (u32)gDuelZones))
#define ZO(p, o) ((struct DuelZone *)((p) * 0xD64 + (o) + (u32)gDuelZones))
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))
extern const u8 gStrDesignateFaceDownMonsterToBanish[];
extern const u8 gStrDesignateFaceDownSpellTrapToBanish[];

int EffectStatModifierTargetChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    if (es[0x3E5] == 0) {
        ref->numTargets = 0;
        switch (((const u16 *)0x08622AB4)[ref->id & 0x7FF]) {
        case 0x433:
        case 0x3F7:
        case 0x5FE:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToIncreaseAtk);
            break;
        case 0x3F8:
        case 0x434:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToIncreaseDef);
            break;
        case 0x43A:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToDecreaseDef);
            break;
        default:
            return 1;
        }
        gChain[0x3E5]++;
    } else if (DuelCursor_PickTarget(0xE000E0) != 0) {
        if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0)
            return 1;
        PlaySE(3);
    }
    return 0;
}
int EffectBlockAttackChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterForDefensePosition);
        ref->numTargets = 0;
        (*st)++;
    } else if (gMain.h6 & 2) {
        *st = z;
        return z;
    } else if (DuelCursor_PickTarget(0x700000) != 0) {
        u32 p = gDuelScreen.w824;
        int zn = gDuelScreen.w828 + gDuelScreen.w82C;
        if (CanCardTargetZone(ref->id, p, zn) != 0) {
            TryAddEffectTarget(ref, p, zn);
            return 1;
        }
        PlaySE(3);
    }
    return 0;
}
int EffectDarknessApproachesChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToSetFaceDown);
        (*st)++;
    } else if (DuelCursor_PickTarget(0xE000E0) != 0) {
        u32 p = gDuelScreen.w824;
        int zn = gDuelScreen.w828 + gDuelScreen.w82C;
        int pl = 1 & p;
        u16 n = ((const u16 *)0x08622AB4)[(*(u32 *)ZB2(pl, zn) << 21) >> 21];
        if ((u16)(n - 0x780) > 0x4F) {
            if (TryAddEffectTarget(ref, p, zn) != 0)
                return 1;
        }
        PlaySE(3);
    }
    return 0;
}
int EffectRedirectTargetChainB(struct CardRef *ref, struct CardRef *other)
{
    char buf[0x80];
    if (((const u16 *)0x08622AB4)[ref->id & 0x7FF] == 0x525 && (((u8 *)ref)[3] & 0xFC) == 0x40) {
        u8 *es = gChain;
        u8 *st = es + 0x3E5;
        if (*st == 0) {
            ref->numTargets = 0;
            DuelPrompt_Post(ref->player, 0x14, ref->unk8 >> 8, 0);
            (*st)++;
            return 0;
        }
        TryAddEffectTarget(ref, ref->player, gDuel.w1B64);
        return 1;
    } else {
        u8 *es = gChain;
        u8 *st = es + 0x3E5;
        if (*st == 0) {
            FormatStr(buf, gStrDesignateNewTargetFmt, gCardNames[other->id]);
            TextBoxOpen(0x206, 0x712, 0xB, buf);
            {
                int mask = ~7;
                register u8 fields __asm__("r1");

                /* FAKEMATCH: keep this bitfield-update scratch in r1. */
                fields = ((u8 *)ref)[0xA];
                __asm__("" : : "r"(fields));
                ((u8 *)ref)[0xA] = mask & fields;
            }
            (*st)++;
            return 0;
        } else {
            /* Effect-table entries 216/327 select this routine for 0x431/0x525.
             * ROM assigns keys only for those numbers; its default path
             * passes the existing r2 value unchanged. Preserve that behavior. */
            int keys;
            switch (((const u16 *)0x08622AB4)[ref->id & 0x7FF]) {
            case 0x431:
                keys = 0xF000F0;
                break;
            case 0x525:
                keys = 0xF0;
                break;
            }
            if (DuelCursor_PickTarget(keys) != 0) {
                u32 p = gDuelScreen.w824;
                int zn = gDuelScreen.w828 + gDuelScreen.w82C;
                if (CanRedirectEffectToZone(((const u16 *)0x08622AB4)[ref->id & 0x7FF], other, p, zn) != 0) {
                    u32 a = (1 - p) << 24;
                    u32 b = (u32)zn << 24;
                    if (other->targets[0] != (u16)((a >> 8 | b) >> 16)) {
                        TryAddEffectTarget(ref, p, zn);
                        return 1;
                    }
                }
                PlaySE(3);
            }
        }
    }
    return 0;
}

int EffectTailorOfTheFickleChainB(struct CardRef *ref)
{
    char buf[0x80];
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    int v = *st;
    switch (v) {
    case 0:
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateEquipToSwitch);
        ref->numTargets = 0;
        (*st)++;
        return 0;
    case 1:
        if (DuelCursor_PickTarget(0xC000C) == 0)
            return 0;
        {
            u32 p = gDuelScreen.w824;
            int sum = gDuelScreen.w828 + gDuelScreen.w82C;
            u16 w = (u8)p | (u8)sum << 8;
            if (EffectTailorOfTheFickleCheck(ref, w) != 0) {
                u16 msg;
                PlaySE(1);
                msg = (v & ((u8 *)ref)[2]) ? 0x8008 : 8;
                DuelCmd_Push(msg, gDuelScreen.w824, (u8)gDuelScreen.w828 | (u8)gDuelScreen.w82C << 8, 0);
                AddEffectTarget(ref, w);
                (*st)++;
                return 0;
            }
            PlaySE(3);
        }
        return 0;
    case 2: {
        const char *fmt = gStrDesignateMonsterToSwitchEquipFmt;
        int pl = 1 & ref->targets[0];
        struct DuelZone *zp = ZB(pl, ref->targets[0] >> 8);
        u32 off = (*(u32 *)zp << 20) >> 14;
        /* FAKEMATCH: the ROM loads the name-table address into r3 for the add. */
        register const char *names asm("r3") = (const char *)gCardNames;
        FormatStr(buf, fmt, (const char *)(off + (u32)names));
        TextBoxOpen(0x206, 0x712, 0xB, buf);
        {
            /* FAKEMATCH: keep r2/r3 busy across the increment so reload picks r4
             * for the step pointer, as in the ROM; this also sets the reload
             * order that case 3 depends on. Emits no code. */
            register int k2 asm("r2");
            register int k3 asm("r3");
            (*st)++;
            asm("" : : "r"(k2), "r"(k3));
        }
        return 0;
    }
    case 3:
        if (DuelCursor_PickTarget(0xE000E0) != 0) {
            u32 p = gDuelScreen.w824;
            int zn = gDuelScreen.w828 + gDuelScreen.w82C;
            u8 tp = ref->targets[0];
            int tz = ref->targets[0] >> 8;
            if (IsValidEquipTarget(tp, tz, p, zn) != 0) {
                int r = FindMonsterLinkedToCard(tp, tz);
                u32 a = p << 24;
                u32 b = (u32)zn << 24;
                if (r != (u16)((a >> 8 | b) >> 16)) {
                    TryAddEffectTarget(ref, p, zn);
                    (*st)++;
                    goto out3; /* skip the failure sound; case 3's tail then merges into case 0's */
                }
            }
            PlaySE(3);
        }
    out3:
        return 0;
    default:
        return 1;
    }
}
/* AI side: per player, zones 5-10, first a card with flags6 bit 1 and type 0x16, then any card without that flag; human side: prompt + cursor. */
int EffectSpellTrapTargetChainB(struct CardRef *ref)
{
    int pl = 1 & ((u8 *)ref)[2];
    u8 *es;
    u8 *st;
    if (pl) {
        int i;
        ref->numTargets = 0;
        for (i = 0; i <= 1; i++) {
            int j;
            for (j = 5; j <= 10; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                u16 id = (*(u32 *)z << 20) >> 20;
                /* FAKEMATCH: the empty use raises z's allocation priority above id's, so z gets r1 and id r3 as in the ROM */
                asm("" : : "r"(z));
                if (id != 0 && (z->flags6 & 2) && CARD_TYPE(id) == 0x16) {
                    TryAddEffectTarget(ref, i, j);
                    return 1;
                }
            }
            for (j = 5; j <= 10; j++) {
                struct DuelZone *z = ZB2(1 & i, j);
                if ((*(u32 *)z << 20) != 0 && !(z->flags6 & 2)) {
                    TryAddEffectTarget(ref, i, j);
                    return 1;
                }
            }
        }
        return 1;
    }
    es = gChain;
    st = es + 0x3E5;
    if (*st == 0) {
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateSpellTrapToDestroy);
        ref->numTargets = 0;
        (*st)++;
        return 0;
    } else if (gMain.h6 & 2) {
        *st = pl;
        return 0;
    } else if (DuelCursor_PickTarget(0xE000E) != 0) {
        TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C);
        return 1;
    }
    return 0;
}
int EffectDustTornadoChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOpponentSpellTrapToDestroy);
        ref->numTargets = 0;
        (*st)++;
        return z;
    } else if (gMain.h6 & 2) {
        *st = z;
        return z;
    } else if (DuelCursor_PickTarget(0xE0000) != 0) {
        TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C);
        return 1;
    }
    return 0;
}
int EffectEarthshakerChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    u16 value; /* FAKEMATCH: shared narrow temporary matches the return paths. */

    switch (value = *st) {
    case 0:
        ref->numTargets = 0;
        DuelPrompt_Post(ref->player, 0xA, 0, 0);
        (*st)++;
        return 0;
    case 1:
    case 2:
    case 3:
    case 4:
        (*st)++;
        return 0;
    case 5:
        DuelPrompt_Post(1 - ref->player, 0xB, gDuel.w1B64, gDuel.w1B66);
        (*st)++;
        return 0;
    default:
        AddEffectTarget(ref, (value = gDuel.w1B64) + 1);
        return 1;
    }
} /* 0x0804067C size 0x98 */
int EffectDeclareTypeChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        DuelPrompt_Post(ref->player, 8, 0, 0);
        break;
    case 1:
        AddEffectTarget(ref, gDuel.w1B64 + 1);
        break;
    default:
        return 1;
    }
    (*st)++;
    return 0;
}
int EffectNoblemanOfCrossoutChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateFaceDownMonsterToBanish);
        ref->numTargets = 0;
        (*st)++;
        return z;
    } else if (gMain.h6 & 2) {
        *st = z;
        return z;
    } else if (DuelCursor_PickTarget(0xD000D0) != 0) {
        TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C);
        return 1;
    }
    return 0;
}
int EffectNoblemanOfExterminationChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateFaceDownSpellTrapToBanish);
        ref->numTargets = 0;
        (*st)++;
        return z;
    } else if (gMain.h6 & 2) {
        *st = z;
        return z;
    } else if (DuelCursor_PickTarget(0x20002) != 0) {
        TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C);
        return 1;
    }
    return 0;
}
/* Text-entry style prompt: steps 0..4; the result u16 at 0x03004872 becomes the target (hypothesis). */
int EffectProhibitionChainB(struct CardRef *ref)
{
    switch (SEL_STEP) {
    case 0:
        ref->numTargets = 0;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateCardToProhibit);
        SEL_STEP++;
        return 0;
    case 1:
        if (DuelScreen_FadeOutStep() != 0) {
            ResetVideo();
            gChain[0x3E6] = 0;
            gChain[0x3E7] = 0;
            SEL_STEP++;
        }
        return 0;
    case 2:
        if (ProhibitCardSelect_Run() != 0) {
            DuelScreen_Init();
            SEL_STEP++;
        }
        return 0;
    case 3:
        if (DuelScreen_FadeInStep() != 0) {
            SEL_STEP++;
        }
        return 0;
    case 4: {
        u8 *mv;
        u16 *r;
        int pl;
        ref->numTargets = 0;
        pl = ref->player;
        mv = (u8 *)&gMain;
        r = (u16 *)(mv + 0x4872);
        sub_08019820(pl, *r);
        AddEffectTarget(ref, *r);
        SEL_STEP++;
        return 1;
    }
    default:
        return 1;
    }
}
int EffectRiryokuChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        if (EffectRiryokuPrepare(ref, 0, 0) == 0)
            return 1;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToHalveAtk);
        (*st)++;
        return 0;
    case 1:
        if (gMain.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (DuelCursor_PickTarget(0xE000E0) == 0)
            return 0;
        TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C);
        (*st)++;
        return 0;
    case 2:
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToIncreaseAtk);
        (*st)++;
        return 0;
    case 3:
        if (gMain.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (DuelCursor_PickTarget(0xE000E0) == 0)
            return 0;
        {
            u32 p = gDuelScreen.w824;
            int zn = gDuelScreen.w828 + gDuelScreen.w82C;
            u16 w = (u8)p | (u8)zn << 8;
            if (ref->targets[0] == w) {
                PlaySE(3);
                return 0;
            }
            TryAddEffectTarget(ref, p, zn);
        }
        return 1;
    default:
        return 1;
    }
}
int EffectDestroyAndGiveMonsterChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOpponentMonsterToDestroy);
        (*st)++;
        return 0;
    case 1:
        if (gMain.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (DuelCursor_PickTarget(0xF00000) == 0)
            return 0;
        TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C);
        (*st)++;
        return 0;
    case 2:
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToGiveControl);
        (*st)++;
        return 0;
    case 3:
        if (gMain.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (DuelCursor_PickTarget(0xF0) == 0)
            return 0;
        TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C);
        (*st)++;
        return 0;
    default:
        return 1;
    }
}
int EffectTributeOpponentMonsterChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        ref->numTargets = 0;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOpponentMonsterToTribute);
        (*st)++;
        return z;
    } else if (gMain.h6 & 2) {
        *st = z;
        return z;
    } else if (DuelCursor_PickTarget(0xF00000) != 0) {
        TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C);
        return 1;
    }
    return 0;
}
int EffectOwnSkullOrThunderChainB(struct CardRef *ref)
{
    char buf[0x100];
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        {
            register const char *fmt __asm__("r1") = gStrDesignateFaceUpMonsterOfTwoFmt;
            u16 id = gUnk_08623E1E[0];
            register u32 off __asm__("r2");
            register const char *names __asm__("r3");

            off = id << 6;
            names = (const char *)gCardNames;
            FormatStr(buf, fmt, (const char *)(off + (u32)names));
        }
        gDuelScreen.w828 += 0;
        FormatStr(buf, buf, gStrThunderType);
        TextBoxOpen(0x206, 0x712, 0xB, buf);
        ref->numTargets = 0;
        (*st)++;
    } else if (gMain.h6 & 2) {
        *st = z;
        return z;
    } else if (DuelCursor_PickTarget(0xE0) != 0) {
        u32 p = gDuelScreen.w824;
        int zn = gDuelScreen.w828 + gDuelScreen.w82C;
        if (EffectOwnSkullOrThunderCheck(ref, (u8)p | (u8)zn << 8) != 0) {
            TryAddEffectTarget(ref, p, zn);
            return 1;
        }
        PlaySE(3);
    }
    return 0;
}
static inline struct DuelZone *ZoneFromOpponentOffset(struct CardRef *ref, int o)
{
    int product = ((1 - ref->player) & 1) * 0xD64;

    /* FAKEMATCH: preserve the offset-first address sum without emitting code. */
    __asm__("" : : "r"(product));
    return (struct DuelZone *)(o + product + (u32)gDuelZones);
}

int EffectTakeControlOfMachineChainB(struct CardRef *ref)
{
    char buf[0x100];
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        int i;
        ref->numTargets = 0;
        for (i = 0; i <= 4; i++) {
            u32 shifted = (u32)((u8 *)ref)[2] << 31;
            int o;

            /* FAKEMATCH: load the player bit before multiplying the zone index. */
            __asm__("" : : "r"(shifted));
            o = i * 0x94;
            if ((ZoneFromOpponentOffset(ref, o)->flags6 & 2) && (*(u32 *)ZoneFromOpponentOffset(ref, o) << 20) != 0
                && GetZoneCardType(1 - ref->player, i) == 7) {
                FormatStr(buf, gStrSelectOpponentMonsterToControlFmt, gStrMachineType);
                TextBoxOpen(0x206, 0x712, 0xB, buf);
                SEL_STEP++;
                return 0;
            }
        }
        return 1;
    } else if (gMain.h6 & 2) {
        int zero = 0;
        *st = zero;
        return zero;
    } else if (DuelCursor_PickTarget(0xE00000) != 0) {
        u32 p = gDuelScreen.w824;
        int zn = gDuelScreen.w828 + gDuelScreen.w82C;
        if (GetZoneCardType(p, zn) == 7) {
            TryAddEffectTarget(ref, p, zn);
            return 1;
        }
        PlaySE(3);
    }
    return 0;
}
