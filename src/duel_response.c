#include "global.h"

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
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
struct DuelFlags { u8 bit0 : 1; u8 bit1 : 1; u8 rest : 6; u8 filler1[4]; };
#define DUEL_FLAGS (*(struct DuelFlags *)((u8 *)0x020192E4 + 0x1B0E))
struct DuelPlayerB { u8 unk0[2]; u8 handCount; u8 unk3[4]; u8 b7; u8 pad[0xD64 - 8]; };
extern struct CardRef gEventResponseEntry;
extern u8 gDuel[];
extern struct DuelPlayerB gDuelPlayers[2];
struct DuelHand { u32 h[0x1B4 / 4]; u8 pad[0xD64 - 0x1B4]; };
extern struct DuelHand gDuelHands[2];
int CanPlaceSpellTrapCard(int player, u16 id);
int GetCardSpellSpeed(u16 id);
int CanActivateEffect(struct CardRef *ref, int a, int b);
extern const u8 gStrEventYouSummoned[], gStrEventOpponentSummoned[], gStrEventYouFlipSummoned[], gStrEventOpponentFlipSummoned[], gStrEventYouSpecialSummoned[], gStrEventOpponentSpecialSummoned[], gStrEventYouSet[], gStrEventOpponentSet[], gStrEventAttackTargetFmt[];
extern const u8 gStrEventPositionChanged[], gStrEventFlippedFaceUp[], gStrEventControlSwitched[], gStrEventBattleFlipEffect[], gStrEventYouDeclaredBattle[], gStrEventOpponentDeclaredBattle[], gStrEventBattleDestroyed[], gStrEventYouTookBattleDamage[], gStrEventYouDealtBattleDamage[];
extern const u8 gStrEventYouTookDeflectedDamage[], gStrEventOpponentTookDeflectedDamage[], gStrEventYouTookDamage[], gStrEventYouDealtDamage[], gStrEventMagicDestroyed[], gStrEventTrapDestroyed[], gStrEventContinuousTrapPlayed[], gStrEventContinuousMagicPlayed[], gStrEventFieldMagicPlayed[];
extern const u8 gStrEventEquipped[], gStrEventCardDrawn[], gStrEventMonsterReturnedToHand[], gStrEventDeckToGraveyard[], gStrEventYouDiscarded[], gStrEventOpponentDiscarded[], gStrEventMonsterSentToGraveyard[], gStrEventSeparator[], gStrAskActivateQuickPlayOrTrap[];
extern const u8 gAlias_080851E8[];
extern const char gCardNames[][0x40];
void StrCopy(u8 *dst, const u8 *src);
void StrCat(u8 *dst, const u8 *src);
void FormatStr(char *dst, const char *fmt, ...);
void FormatInt(char *dst, char *a, char *b);
void GetZoneCardStats(u32 player, u32 slot, void *out);
extern u8 gChain[];
int EventResponse_Run();
struct F491 { u8 lo : 4; u8 b4 : 1; u8 b5 : 1; u8 f6 : 1; u8 f7 : 1; };
struct G5EE8 { u8 unk0; u8 b1; };
extern struct G5EE8 gDuelCtrl;
void DuelLink_SendMessageData(u32 a, void *b, int c);
int CanActivateFieldCard(struct CardRef *ref, int player, int idx);
int CanActivateHandCard(struct CardRef *ref, int player, int idx);


/* Takes (ref, player, hand index). Can this hand card be used as the effect's
 * card? Fills ref->id. */
int CanActivateHandCard(struct CardRef *ref, int player, int idx)
{
    u16 id = (*(u32 *)((u8 *)gDuelHands + idx * 4 + (1 & player) * 0xD64) << 20) >> 20;
    u32 n = 0x7FF & id;
    if (CanPlaceSpellTrapCard(player, id) != 0 && (((((const u32 *)0x08621DE0)[n] & 0x1F00000) >> 20) == 0x16)) {
        if (GetCardSpellSpeed(id) > 1)
            goto store;
        {
            u32 f = *((u8 *)gDuelHands + 0x148A);
            u32 m = (f << 27) >> 29;
            if (m == 2 || m == 4) {
                if (((f << 30) >> 31) == player)
                    goto store;
            }
        }
    }
fail:
    return 0;
store:
    {
        int t;
        ref->id = id;
        t = CARD_TYPE(id);
        switch (t) {
        case 0x15:
        case 0x16:
            if (gDuelPlayers[1 & player].b7 >> 6 != 0)
                goto fail;
        }
        return (u16)CanActivateEffect(ref, 0, 1);
    }
}
/* Takes (ref, player, kind 5 | 0xB, index). Returns 0x41 when the hand/field
 * card is usable for the effect, else 1. */
int EventResponse_GetCommands(struct CardRef *ref, int player, int kind, int idx)
{
    int r = 1;
    switch (kind) {
    case 0xB:
        if ((u16)CanActivateHandCard(ref, player, idx))
            r = 0x41;
        break;
    case 5:
        if ((u16)CanActivateFieldCard(ref, player, idx + 5))
            r = 0x41;
        break;
    }
    return r;
}
/* The player array at gDuel + 4, wrapped so the hand-loop test loads
 * the constant base first (an ARRAY_REF, not pointer arithmetic). */
struct PL2_080420A4 { struct DuelPlayerB p[2]; };
/* Is any spell/trap zone (5-9) or hand card of the player usable right now? */
int EventResponse_CanPlayerRespond(int player)
{
    int i;
    u8 *e;
    for (i = 5; i <= 9; i++) {
        u32 id = (*(u32 *)ZB2(1 & player, i) << 20) >> 20;
        if (id != 0) {
            int n = GetCardSpellSpeed(id);
            int ok = (u16)CanActivateFieldCard(&gEventResponseEntry, player, i);
            if (n > 1 && ok != 0)
                goto found;
        }
    }
    e = gDuel;
    if (((struct DuelFlags *)(e + 0x1B12))->bit1 == player)
        goto hand;
    return 0;
found:
    return 1;
hand:
    for (i = 0; i < ((struct PL2_080420A4 *)(gDuel + 4))->p[1 & player].handCount; i++) {
        u32 id = (*(u32 *)((u8 *)gDuelHands + i * 4 + (1 & player) * 0xD64) << 20) >> 20;
        if (id != 0) {
            int n = GetCardSpellSpeed(id);
            int ok = (u16)CanActivateHandCard(&gEventResponseEntry, player, i);
            if (n > 1 && ok != 0)
                goto found;
        }
    }
    return 0;
}
/* Build the description text of a queued effect into buf, chosen by ref->kind (5-30) and its small arguments. */
void EventResponse_BuildPromptText(struct CardRef *ref, u8 *buf)
{
    char *t[3];
    char tmp[0x100];
    const u8 *txt;
    switch (ref->kind) {
    case 5:
        switch ((u8)ref->pos) {
        case 0:
            txt = gStrEventYouSummoned;
            break;
        case 1:
            txt = gStrEventOpponentSummoned;
            break;
        default:
            goto out;
        }
        goto call;
    case 6:
        switch ((u8)ref->pos) {
        case 0:
            txt = gStrEventYouFlipSummoned;
            break;
        case 1:
            txt = gStrEventOpponentFlipSummoned;
            break;
        default:
            goto out;
        }
        goto call;
    case 7:
        switch ((u8)ref->pos) {
        case 0:
            txt = gStrEventYouSpecialSummoned;
            break;
        case 1:
            txt = gStrEventOpponentSpecialSummoned;
            break;
        default:
            goto out;
        }
        goto call;
    case 8:
        switch ((u8)ref->pos) {
        case 0:
            txt = gStrEventYouSet;
            break;
        case 1:
            txt = gStrEventOpponentSet;
            break;
        default:
            goto out;
        }
        goto call;
    case 17: {
        u8 pb;
        u16 w;
        int zz;
        int p;
        struct DuelZone *zn;
        u32 id;
        *(u16 *)buf = 0;
        pb = (u8)ref->unk8;
        w = ref->unk8;
        zz = w >> 8;
        p = 1 & pb;
        zn = ZB(p, zz);
        id = (*(u32 *)zn << 20) >> 20;
        if (id != 0 && (zn->flags6 & 2)) {
            GetZoneCardStats(pb, zz, t);
            FormatStr((char *)buf, (const char *)gStrEventAttackTargetFmt, gCardNames[id]);
            FormatInt(tmp, (char *)buf, t[1]);
            FormatInt((char *)buf, tmp, t[2]);
        }
        goto out;
    }
    case 10:
        txt = gStrEventPositionChanged;
        goto call;
    case 11:
        txt = gStrEventFlippedFaceUp;
        goto call;
    case 12:
        txt = gStrEventControlSwitched;
        goto call;
    case 18:
        txt = gStrEventBattleFlipEffect;
        goto call;
    case 16:
        switch ((u8)ref->pos) {
        case 0:
            txt = gStrEventYouDeclaredBattle;
            break;
        case 1:
            txt = gStrEventOpponentDeclaredBattle;
            break;
        default:
            goto out;
        }
        goto call;
    case 19:
        txt = gStrEventBattleDestroyed;
        goto call;
    case 13:
        switch (((u8)ref->unk8 & 0xF)) {
        case 0:
            txt = gStrEventYouTookBattleDamage;
            break;
        case 1:
            txt = gStrEventYouDealtBattleDamage;
            break;
        default:
            goto out;
        }
        goto call;
    case 14:
        switch (((u8)ref->unk8 & 0xF)) {
        case 0:
            txt = gStrEventYouTookDeflectedDamage;
            break;
        case 1:
            txt = gStrEventOpponentTookDeflectedDamage;
            break;
        default:
            goto out;
        }
        goto call;
    case 15:
        switch (ref->unk8) {
        case 0:
            txt = gStrEventYouTookDamage;
            break;
        case 1:
            txt = gStrEventYouDealtDamage;
            break;
        default:
            goto out;
        }
        goto call;
    case 20:
        txt = gStrEventMagicDestroyed;
        goto call;
    case 21:
        txt = gStrEventTrapDestroyed;
        goto call;
    case 22:
        txt = gStrEventContinuousTrapPlayed;
        goto call;
    case 23:
        txt = gStrEventContinuousMagicPlayed;
        goto call;
    case 24:
        txt = gStrEventFieldMagicPlayed;
        goto call;
    case 25:
        txt = gStrEventEquipped;
        goto call;
    case 26:
        switch (ref->unk8) {
        case 0:
            txt = gStrEventCardDrawn;
            break;
        case 1:
            txt = gAlias_080851E8;
            break;
        default:
            goto out;
        }
        goto call;
    case 27:
        txt = gStrEventMonsterReturnedToHand;
        goto call;
    case 28:
        txt = gStrEventDeckToGraveyard;
        goto call;
    case 29:
        switch (ref->unk8) {
        case 0:
            txt = gStrEventYouDiscarded;
            break;
        case 1:
            txt = gStrEventOpponentDiscarded;
            break;
        default:
            goto out;
        }
        goto call;
    case 30:
        goto last;
    }
    goto out;
call:
    StrCopy(buf, txt);
    goto out;
last:
    StrCopy(buf, gStrEventMonsterSentToGraveyard);
out:
    StrCat(buf, gStrEventSeparator);
    StrCat(buf, gStrAskActivateQuickPlayOrTrap);
}
/* Widths below follow the ROM's ldrb/ldrh/ldr accesses. These views
 * deliberately overlap because the selection flags are also updated as a u32. */
typedef int (*EffectCallback)(struct CardRef *, u8 *);
extern u8 gLinkState[], gTextBox[], gDuelScreen[], gMain[];
extern const u8 gStrLinkChainPromptEffect[], gStrLinkChainPromptCard[], gStrSelectSpellTrapForChain[], gStrSelectSpellTrapToActivate[];
extern const u32 gCardEffects[], gCardStats[];
void CardMenu_Update(void);
void DuelCmd_Push(u16 cmd, u16 a, u16 b, u16 c);
void Chain_AddPending(u32 a, u32 b);
u32 Chain_GetResponseCommands(void *entry, int player, int kind, int index);
u16 DuelLink_SendMessage(u16 cmd, u16 a, u16 b, u16 c);
void CardListView_Open(int player, int area, int a, int b);
int EventResponse_CanPlayerRespond(int player);
void CardMenu_PlaySpellTrapFromHand(int a, int b, struct CardRef *ref);
int FindCardEffect(u32 card);
int DuelCursor_PickTarget(u32 keys);
int AiShouldActivateSetCard(struct CardRef *ref, int a);
u32 DuelCursor_GetCardId(void);
void TextBoxOpen(u16 a, u16 b, u16 c, const u8 *text);
void TextBoxSetMenu(u16 mode, void (*cb1)(void), u16 (*cb2)(void));
void PlaySE(u32 sound);

/* Local aliases retain the addresses and explicit access widths. */
union EffectCardRef {
    struct CardRef ref;
    struct { u16 id; union { u16 word; u8 byte[2]; } __attribute__((packed, aligned(2))) flags; u8 rest[0x10]; } raw;
};
union EffectHalf { u16 word; u8 byte[2]; } __attribute__((packed, aligned(2)));
struct EffectStateView {
    u8 unk0[0x3E4];
    u8 targetPhase, targetStep;
    u8 unk3E6[0x480 - 0x3E6];
    EffectCallback phase4, phase5;
    u8 unk488[2];
    union EffectHalf requestedZone;
    u32 requestedArg;
    u8 requestStep, requestFlags, selectionPhase, selectionMode;
    u8 unk494[0x4A8 - 0x494];
    union EffectCardRef queuedRef, selectedRef;
    u16 activeCard;
};
struct DuelSelectionView {
    u8 unk0[0x1B12];
    u8 turnFlags;
    u8 unk1B13[0x1B28 - 0x1B13];
    u16 cardId;
    u8 unk1B2A[2];
    union { u32 word; u8 byte[4]; } flags;
    u8 auxiliaryFlags;
    u8 unk1B31[2];
    u8 playerAndArea;
    union EffectHalf index;
};
struct EffectWaitView { u8 unk0[0x14]; u16 pending; };
struct EffectCursorView { u8 unk0[0x824]; s32 player, area, index; };
struct EffectKeysView { u8 unk0[6]; u16 pressed; };
extern struct EffectStateView gEffectState asm("gChain");
extern struct DuelSelectionView gDuelSelection asm("gDuel");
extern struct EffectWaitView gEffectWait asm("gTextBox");
extern struct EffectCursorView gEffectCursor asm("gDuelScreen");
extern struct EffectKeysView gEffectKeys asm("gMain");
typedef char effect_ref_size_check[sizeof(union EffectCardRef) == 0x14 ? 1 : -1];
typedef char effect_ref_offset_check[(u32)&((struct EffectStateView *)0)->selectedRef == 0x4BC ? 1 : -1];
typedef char effect_cursor_flags_check[(u32)&((struct DuelSelectionView *)0)->flags == 0x1B2C ? 1 : -1];


struct Sel44C {
    u16 flag0 : 1;
    u16 active : 1;
    u16 cursor : 4;
    u16 rows : 4;
    u32 mask : 16;
    u32 state : 8;
    u32 unk34 : 8;
    u32 unk42 : 8;
    u16 timer : 7;
    u16 player : 1;
    u32 zone : 7;
    u16 index : 8;
    u16 unk73 : 7;
};
struct D44C {
    u8 unk0[0x1B12];
    u8 turnFlags;
    u8 unk1B13[0x1B28 - 0x1B13];
    u16 cardId;
    u8 unk1B2A[2];
    struct Sel44C sel;
};
extern struct D44C gAliasB_020192E0;
#define gD44C gAliasB_020192E0
#define SEL44 gD44C.sel
struct Z44C { u32 card; u8 unk4, unk5, flags6; u8 unk7[0x94 - 7]; };
#define ZN44(p, z) ((struct Z44C *)((p) * 0xD64 + (z) * 0x94 + (u32)((u8 *)&gD44C + 0x2C)))
struct E44C {
    u8 unk0[0x48A];
    u16 zone;           /* 0x48A */
    u32 arg;            /* 0x48C */
    u8 step;            /* 0x490 */
    u8 lo : 4;          /* 0x491 */
    u8 b4 : 1;
    u8 b5 : 1;
    u8 f6 : 1;
    u8 f7 : 1;
    u8 unk492[0x4A8 - 0x492];
    struct CardRef ref; /* 0x4A8 */
};
extern struct E44C gAliasB_02017A40;
#define gE44C gAliasB_02017A40
struct W44C { u8 unk0[0x14]; u16 h14; };
extern struct W44C gAliasB_0201AE60;
struct K44C { u8 unk0[6]; u16 h6; };
extern struct K44C gAliasB_03000040;
struct C44C { u8 unk0[0x824]; u32 w824, w828, w82C; };
extern struct C44C gAliasB_0201CFB0;
struct F44C { u8 unk0[0x307]; u8 lo3 : 3; u8 b3 : 1; u8 hi : 4; };
extern struct F44C gAliasB_02017FB0;
void CardMenu_Update(void);
void DuelCmd_Push(u16 cmd, u16 a, u16 b, u16 c);
void Chain_AddPending(u32 a, u32 b);
void CardListView_Open(int player, int area, int a, int b);
void CardMenu_PlaySpellTrapFromHand(int a, int b, struct CardRef *ref);
int DuelCursor_PickTarget(u32 keys);
int AiShouldActivateSetCard(struct CardRef *ref, int a);
u32 DuelCursor_GetCardId(void);
void TextBoxOpen(u16 a, u16 b, u16 c, const u8 *text);
void TextBoxSetMenu(u16 mode, void (*cb1)(void), u16 (*cb2)(void));
void PlaySE(u32 sound);

/* Effect-request step machine on 0x02017A40+0x490: queue the request (step 0), describe it and
 * wait for the link reply (1/2), let the player pick a zone or hand card (10/11, widget at
 * 0x020192E0+0x1B2C), or search spell/trap zones 5-9 for a usable card (200/201); 100/101 send
 * and await the link message, 240 flips the player bit. Returns 1 when the request is finished. */
int EventResponse_Run(void)
{
    u8 text[0x100];
    int slot;
    int st = gE44C.step;

    switch ((u8)st) {
    case 0:
        gE44C.f7 = 0;
        gE44C.ref.id = 0;
        gE44C.ref.player = gE44C.b4;
        gE44C.ref.kind = gE44C.zone;
        gE44C.ref.pos = gE44C.arg;
        gE44C.ref.unk8 = gE44C.arg >> 16;
        if ((u16)EventResponse_CanPlayerRespond(gE44C.b4) == 0) {
            gE44C.step = 0xF0;
            return 0;
        }
        gE44C.step++;
    case 1:
        if (gE44C.b4) {
            u8 s;
            if (gDuelCtrl.b1 & 1)
                s = 0x64;
            else
                s = 0xC8;
            gE44C.step = s;
        } else {
            EventResponse_BuildPromptText(&gE44C.ref, text);
            TextBoxOpen(0x204, 0x916, 0xB, text);
            TextBoxSetMenu(1, NULL, NULL);
            gE44C.step++;
        }
        return 0;
    case 2:
        if (gAliasB_0201AE60.h14 == 0) {
            gE44C.step = 0xF0;
            return 0;
        }
        gE44C.step = 0xA;
        SEL44.flag0 = 0;
        SEL44.active = 0;
        return 0;
    case 0xA:
        if (SEL44.flag0) {
            CardMenu_Update();
            return 0;
        }
        if (SEL44.active) {
            gE44C.step++;
            return 0;
        }
        if (gAliasB_03000040.h6 & 2) {
            gE44C.step = 1;
            return 0;
        }
        {
            u32 keys = 0xE;
            if (!(gD44C.turnFlags & 2))
                keys = 0xF;
            if (DuelCursor_PickTarget(keys) == 0)
                return 0;
        }
        {
            u32 p = gAliasB_0201CFB0.w824;
            u32 area = gAliasB_0201CFB0.w828;
            u32 idx = gAliasB_0201CFB0.w82C;
            u16 choice = DuelCursor_GetCardId();
            switch (area) {
            case 0:
            case 5:
            case 10:
            case 11:
                if (choice != 0) {
                    SEL44.flag0 = 1;
                    SEL44.state = 0;
                    SEL44.mask = (u16)EventResponse_GetCommands(&gEventResponseEntry, p, area, idx);
                    return 0;
                }
                PlaySE(3);
                return 0;
            case 13:
                PlaySE(3);
                return 0;
            case 12:
            case 14:
            case 15:
                CardListView_Open(p, area, 0, 0);
                PlaySE(1);
                return 0;
            }
        }
        return 0;
    case 0xB:
        switch (SEL44.zone) {
        case 0xB:
            CardMenu_PlaySpellTrapFromHand(1, 0, &gE44C.ref);
            if (SEL44.active)
                return 0;
            break;
        case 5:
            SEL44.active = 0;
            {
                u32 p = 1 & SEL44.player;
                if (!(ZN44(p, SEL44.index + SEL44.zone)->flags6 & 2)) {
                    u16 msg;
                    if (SEL44.player)
                        msg = 0x807F;
                    else
                        msg = 0x7F;
                    DuelCmd_Push(msg, SEL44.index + SEL44.zone, 0, 0);
                }
            }
            {
                /* hi/z temporaries stop fold from moving the 0x200000 constant out of the zone term */
                u32 hi = ((1 & SEL44.player) << 31) | (gE44C.ref.kind << 25);
                u32 z = (((SEL44.index + SEL44.zone) & 0x1F) << 16) | 0x200000;
                Chain_AddPending(hi | z | gD44C.cardId, gE44C.ref.pos | (gE44C.ref.unk8 << 16));
            }
            break;
        }
        gE44C.f7 = 1;
        gE44C.f6 = 0;
        return 1;
    case 0x64:
        DuelLink_SendMessageData(0xF051, &gE44C.ref, 0x14);
        gE44C.f7 = 0;
        gAliasB_02017FB0.b3 = 0;
        gE44C.step++;
        return 0;
    case 0x65:
        /* FAKEMATCH: "& 1" keeps fold from turning the bitfield test into a mask test (ROM tests via lsl #28) */
        if (!(gAliasB_02017FB0.b3 & 1))
            return 0;
        if (!gE44C.f7) {
            gE44C.step = 0xF0;
            return 0;
        }
        gE44C.f6 = 0;
        return 1;
    case 0xC8:
        for (slot = 5; slot <= 9; slot++) {
            u32 id = ZN44(1 & gE44C.b4, slot)->card << 20 >> 20;
            if (id != 0) {
                int n = GetCardSpellSpeed(id);
                int ok = (u16)CanActivateFieldCard(&gE44C.ref, gE44C.b4, slot);
                if (n > 1 && ok != 0 && AiShouldActivateSetCard(&gE44C.ref, 0) != 0)
                    goto found;
            }
        }
        gE44C.step = 0xF0;
        return 0;
    case 0xC9:
        if (!(ZN44(gE44C.b4, gE44C.lo)->flags6 & 2)) {
            u16 msg;
            if (gE44C.b4)
                msg = 0x807F;
            else
                msg = 0x7F;
            DuelCmd_Push(msg, gE44C.lo, 0, 0);
        }
        {
            u32 hi = (gE44C.b4 << 31) | (gE44C.ref.kind << 25);
            u32 z = (gE44C.lo << 16) | 0x200000;
            Chain_AddPending(hi | z | (ZN44(gE44C.b4, gE44C.lo)->card << 20 >> 20),
                         gE44C.ref.pos | (gE44C.ref.unk8 << 16));
        }
        gE44C.f7 = 1;
        gE44C.f6 = 0;
        return 1;
    case 0xF0:
        {
            /* FAKEMATCH: the int temporary keeps combine from turning (1 - b4) & 1 into an eor */
            int t = 1 - gE44C.b4;
            gE44C.b4 = t;
        }
        if (gE44C.b4 != gE44C.b5) {
            gE44C.step = 0;
            return 0;
        }
        return 1;
    found:
        gE44C.lo = slot;
        gE44C.step++;
        return 0;
    default:
        gE44C.f6 = 0;
        return 1;
    }
}
/* Queue a request (player, zone, 32-bit arg): sent as a message when the link flags allow, else stored in the 0x02017A40 request block. */
void EventResponse_Request(int player, u16 zone, u32 arg)
{
    if (1 & gDuelCtrl.b1) {
        u8 *e = gDuel;
        if (2 & *(e + 0x1B12)) {
            u16 buf[4];
            buf[0] = 1 - player;
            buf[1] = zone;
            buf[2] = arg;
            buf[3] = arg >> 16;
            DuelLink_SendMessageData(0xF059, buf, 0xA);
            return;
        }
    }
    {
        u8 *es = gChain;
        struct F491 *f;
        u8 z;
        u16 *hp = (u16 *)(es + 0x48A);
        z = 0;
        *hp = zone;
        *(u32 *)(es + 0x48C) = arg;
        f = (struct F491 *)(es + 0x491);
        f->b4 = 1 & player;
        f->b5 = 1 & player;
        es[0x490] = z;
        f->lo = 0;
        f->f6 = 1;
        f->f7 = 0;
    }
}

/* If the pending flag (bit 6 of 0x02017A40+0x491) is set, run EventResponse_Run and clear it once that finishes; returns 1 while the flag was set. */
int EventResponse_Update(void)
{
    u8 *es = gChain;
    u8 *p = es + 0x491;
    if (*p & 0x40) {
        if ((u16)EventResponse_Run() != 0)
            ((struct F491 *)p)->f6 = 0;
        return 1;
    }
    return 0;
}
struct RefEnt { u8 player : 1; u8 unk1 : 3; u16 zone : 6; u16 kind : 6; u8 pad[0x10]; };

/* Is there a queued reference with the given player and zone in the list at `list`? (count at +0x140, 0x14-byte entries from +2) */
int IsZoneInChainList(u8 *list, int player, int zone)
{
    int i;
    for (i = 0; i < *(u16 *)(list + 0x140); i++) {
        struct RefEnt *e = (struct RefEnt *)(list + 2 + i * 0x14);
        if (e->player == player && e->zone == zone)
            return 1;
    }
    return 0;
}
typedef u16 (*Cb42BE0)(struct CardRef *, u16 *);
struct E42BE0 {
    u8 unk0[0x3E4];
    u8 b3E4;
    u8 b3E5;
    u8 unk3E6[0x480 - 0x3E6];
    Cb42BE0 cb4;          /* 0x480 */
    Cb42BE0 cb5;          /* 0x484 */
    u8 unk488[0x492 - 0x488];
    u8 b0 : 1;            /* 0x492 */
    u8 phase : 7;
    u8 mode : 1;          /* 0x493 */
    u8 modeRest : 7;
    u8 unk494[0x4BC - 0x494];
    struct CardRef ref;   /* 0x4BC */
    u16 activeCard;       /* 0x4D0 */
};
extern struct E42BE0 gAlias_02017A40;
#define gE42BE0 gAlias_02017A40
/* Selection widget at 0x020192E0+0x1B2C (same layout as card_menu_input). */
struct Sel42BE0 {
    u16 flag0 : 1;
    u16 active : 1;
    u16 cursor : 4;
    u16 rows : 4;
    u32 mask : 16;
    u32 state : 8;
    u32 unk34 : 8;
    u32 unk42 : 8;
    u16 timer : 7;
    u16 player : 1;
    u32 zone : 7;
    u16 index : 8;
    u16 unk73 : 7;
};
struct D42BE0 { u8 unk0[0x1B2C]; struct Sel42BE0 sel; };
extern struct D42BE0 gAlias_020192E0;
#define gD42BE0 gAlias_020192E0
#define SEL42 gD42BE0.sel
struct Z42BE0 { u32 card; u8 unk4, unk5, flags6; u8 unk7[0x94 - 7]; };
#define ZN42(p, z) ((struct Z42BE0 *)((p) * 0xD64 + (z) * 0x94 + (u32)((u8 *)&gD42BE0 + 0x2C)))
struct Ent42BE0 { u8 pad[0x10]; Cb42BE0 a; Cb42BE0 b; };
extern struct Ent42BE0 gAlias_0819A9D4[];
#define gEnt42BE0 gAlias_0819A9D4
struct W42BE0 { u8 unk0[0x14]; u16 h14; };
extern struct W42BE0 gAlias_0201AE60;
#define gW42BE0 gAlias_0201AE60
struct K42BE0 { u8 unk0[6]; u16 h6; };
extern struct K42BE0 gAlias_03000040;
#define gK42BE0 gAlias_03000040
struct C42BE0 { u8 unk0[0x824]; u32 w824, w828, w82C; };
extern struct C42BE0 gAlias_0201CFB0;
#define gC42BE0 gAlias_0201CFB0

/* Selection/callback state machine on the phase bits of 0x02017A40+0x492: describe the
 * active card or effect, wait for the cursor selection, fill the reference at +0x4BC and look
 * up its two effect callbacks (0x18-byte entries at 0x0819A9D4, +0x10/+0x14), then run them
 * until each returns nonzero. Returns 1 when the link message was sent.
 * Matching notes: every early exit is an explicit `return 0` (cross-jumping keeps the copy after
 * CardMenu_Update); the zone base must use the same symbol as the selection struct so CSE turns it
 * into base+0x2C; `index` is a u16:8 field (its extraction is recomputed from the shifted halfword);
 * msg is an if/else (jump.c hoists the 0x7F set before the compare). */
int DuelLink_AnswerActivateQuery(void) {
    u8 text[0x100];

    switch (gE42BE0.phase) {
    case 0:
        if (gE42BE0.mode) {
            u16 id = gE42BE0.activeCard;
            if (((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20) <= 0x14)
                FormatStr((char *)text, (const char *)gStrLinkChainPromptEffect, ((const char (*)[0x40])0x0822C720)[id]);
            else
                FormatStr((char *)text, (const char *)gStrLinkChainPromptCard, ((const char (*)[0x40])0x0822C720)[id]);
        } else {
            EventResponse_BuildPromptText(&gE42BE0.ref, text);
        }
        TextBoxOpen(0x204, 0x916, 0xB, text);
        TextBoxSetMenu(1, NULL, NULL);
        gE42BE0.phase++;
        return 0;
    case 1:
        if (gW42BE0.h14 == 0) {
            DuelLink_SendMessage(0xF055, 0, 0, 0);
            return 1;
        }
        if (gE42BE0.mode)
            TextBoxOpen(0x206, 0x712, 0xB, gStrSelectSpellTrapForChain);
        else
            TextBoxOpen(0x206, 0x712, 0xB, gStrSelectSpellTrapToActivate);
        gE42BE0.phase++;
        SEL42.flag0 = 0;
        SEL42.active = 0;
        return 0;
    case 2:
        if (SEL42.flag0) {
            CardMenu_Update();
            return 0;
        }
        if (SEL42.active) {
            gE42BE0.phase++;
            return 0;
        }
        if (gK42BE0.h6 & 2) {
            gE42BE0.phase = 0;
            return 0;
        }
        if (DuelCursor_PickTarget(0xEE) == 0)
            return 0;
        {
            u32 p = gC42BE0.w824;
            u32 area = gC42BE0.w828;
            u32 idx = gC42BE0.w82C;
            u16 choice = DuelCursor_GetCardId();
            switch (area) {
            case 0:
            case 5:
                if (choice != 0) {
                    SEL42.flag0 = 1;
                    SEL42.state = 0;
                    if (gE42BE0.mode)
                        SEL42.mask = (u16)Chain_GetResponseCommands(&gE42BE0.activeCard, p, area, idx);
                    else
                        SEL42.mask = (u16)EventResponse_GetCommands(&gE42BE0.ref, p, area, idx);
                    return 0;
                }
                PlaySE(3);
                return 0;
            case 10:
            case 11:
                if (choice != 0) {
                    SEL42.flag0 = 1;
                    SEL42.state = 0;
                    SEL42.mask = 1;
                    return 0;
                }
                PlaySE(3);
                return 0;
            case 13:
                PlaySE(3);
                return 0;
            case 12:
            case 14:
            case 15:
                CardListView_Open(p, area, 0, 0);
                PlaySE(1);
                return 0;
            }
        }
        return 0;
    case 3:
        SEL42.active = 0;
        if (gE42BE0.mode)
            DuelCmd_Push(7, 0, 0, 0);
        {
            u32 p = 1 & SEL42.player;
            if (!(ZN42(p, SEL42.index + SEL42.zone)->flags6 & 2)) {
                u16 msg;
                if (SEL42.player)
                    msg = 0x807F;
                else
                    msg = 0x7F;
                DuelCmd_Push(msg, SEL42.index + SEL42.zone, 0, 0);
            }
        }
        gE42BE0.ref.zone = SEL42.zone + SEL42.index;
        gE42BE0.ref.id = (ZN42(1 & SEL42.player, gE42BE0.ref.zone)->card << 20) >> 20;
        gE42BE0.ref.player = 0;
        {
            int n = FindCardEffect(gE42BE0.ref.id);
            if (n == -1) {
                gE42BE0.cb4 = NULL;
                gE42BE0.cb5 = NULL;
            } else {
                gE42BE0.cb4 = gEnt42BE0[n].a;
                gE42BE0.cb5 = gEnt42BE0[n].b;
            }
        }
        gE42BE0.b3E4 = 0;
        gE42BE0.b3E5 = 0;
        gE42BE0.phase++;
        return 0;
    case 4:
        if (gE42BE0.cb4 != NULL) {
            if (gE42BE0.cb4(&gE42BE0.ref, &gE42BE0.activeCard) != 0)
                gE42BE0.phase++;
        } else {
            gE42BE0.phase++;
        }
        return 0;
    case 5:
        if (gE42BE0.cb5 != NULL) {
            if (gE42BE0.cb5(&gE42BE0.ref, &gE42BE0.activeCard) != 0)
                gE42BE0.phase++;
        } else {
            gE42BE0.phase++;
        }
        return 0;
    default:
        if (gE42BE0.mode)
            DuelLink_SendMessageData(0xF056, &gE42BE0.ref, 0x14);
        else
            DuelLink_SendMessageData(0xF053, &gE42BE0.ref, 0x14);
        return 1;
    }
}
