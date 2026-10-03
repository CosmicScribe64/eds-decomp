#include "global.h"

/* A card word as stored in the duel lists (u32 container: tests compile to lsl + sign compare). */
struct DuelCard {
    u32 id:12;          /* card ID (index into gCardStats) */
    u32 unk12:5;
    u32 flag17:1;       /* bit 17 */
    u32 flag18:1;       /* bit 18 */
    u32 unk19:13;
};
union DuelCardWord {
    u32 w;
    struct DuelCard c;
};

/* Per-player duel state (0xD64 bytes), two of them at 0x020192E4 (see code_08007994). */
struct DuelPlayer {
    u16 lifePoints;     /* +0x000 */
    u8 handCount;       /* +0x002 */
    u8 deckCount;       /* +0x003 */
    u8 graveCount;      /* +0x004 */
    u8 fusionCount;     /* +0x005 */
    u8 banishCount;     /* +0x006 */
    u8 filler7[0x684 - 0x7];
    union DuelCardWord hand[80];    /* +0x684 */
    u32 deck[80];       /* +0x7C4 */
    u32 grave[80];      /* +0x904 */
    u32 fusion[80];     /* +0xA44 */
    u32 banish[80];     /* +0xB84 */
    u8 fillerCC4[0xD64 - 0xCC4];
};
struct DuelState {
    u32 unk0;
    struct DuelPlayer players[2];   /* +0x004 (= 0x020192E4) */
    u8 filler1ACC[0x1B14 - 0x1ACC];
    u16 unk1B14_0:2;
    u16 unk1B14_2:7;    /* +0x1B14 bits 2-8 */
    u16 unk1B14_9:7;
    u8 filler1B16[0x1B43 - 0x1B16];
    u8 unk1B43;
    u8 unk1B44;
    u8 filler1B45[0x1B50 - 0x1B45];
    /* +0x1B50: pending duel message (sent to the link partner, see DuelPrompt_Start) */
    u8 msgSent:1;       /* +0x1B50 bit 0: already forwarded over link (hypothesis) */
    u8 msgPending:1;    /* +0x1B50 bit 1 */
    u8 msgPlayer:1;     /* +0x1B50 bit 2 */
    u8 msgUnk3:1;
    u16 msgKind:6;      /* +0x1B50 bits 4-9: message kind */
    u16 msgUnkA:6;
    u16 msgArg;         /* +0x1B52 */
    u16 msgValue;       /* +0x1B54 */
    u16 msgRest[6];     /* +0x1B56 */
    u8 step;            /* +0x1B62: step of the prompt handlers below */
    u8 unk1B63;
    u16 result;         /* +0x1B64: prompt result */
};

extern struct DuelState gDuel;

extern struct DuelPlayer gDuelPlayers[2];
/* Same array through a constant address: GCC reloads it at every use instead of hoisting/CSEing it. */
extern u32 gDuelHands[];    /* = gDuelPlayers[0].hand */
/* hand[i] of a player, with the offsets added in the order the ROM uses. */
#define HAND_CARD(p, i) (*(u32 *)((u8 *)gDuelHands + (((p) & 1) * 0xD64 + (i) * 4)))
#define PLAYERS ((struct DuelPlayer *)0x020192E4)

/* Duel control at 0x02015EE8 (gDuelCtrl, hypothesis). */
struct DuelCtrl {
    u8 phase;           /* +0: index into the phase table 0x08198F80 */
    u8 link:1;          /* +1 bit 0: link duel (hypothesis) */
    u8 unk1_1:7;
};
extern struct DuelCtrl gDuelCtrl;

/* Link send buffer / state at 0x02017FB0 (fields used here). */
struct LinkState {
    u8 filler0[0x204];
    u16 msgId;          /* +0x204: message id (0xF0xx) */
    u16 msgArg;         /* +0x206 */
    u32 cards[0x3F];    /* +0x208: payload */
    u32 unk304:8;
    u32 dirtyHand:1;    /* +0x305 bit 0: list needs resending (hypothesis) */
    u32 dirtyDeck:1;    /* +0x305 bit 1 */
    u32 dirtyGrave:1;   /* +0x305 bit 2 */
    u32 dirtyFusion:1;  /* +0x305 bit 3 */
    u32 dirtyBanish:1;  /* +0x305 bit 4 */
    u32 unk305_5:3;
    u32 unk306_0:6;
    u32 unk306_6:1;     /* +0x306 bit 6 */
    u32 unk306_7:1;
    u32 unk307_0:7;
    u32 unk307_7:1;     /* +0x307 bit 7 */
};
extern struct LinkState gLinkState;
extern u32 gLinkTxCards[];     /* = gLinkState.cards */

void DuelPrompt_Start(void);
void FormatStr(char *dst, const char *fmt, ...);
void TextBoxOpen(u32 a, u32 b, u32 c, const char *text);
void TextBoxSetMenu(u32 a, u32 b, u32 c);
u32 DuelCursor_PickTarget(u32 keys);
void PlaySE(u32 a);
void sub_080197C0(int player, u16 id);
u32 DuelPrompt_Discard(int player, u16 arg, u16 a, u16 b);
u32 DuelPrompt_DiscardCost(int player, u16 arg, u16 a, u16 b);
u32 DuelPrompt_DiscardRandom(int player, u16 arg, u16 value);
u32 DuelPrompt_BanishRandom(int player, u16 arg, u16 value);
u32 DuelPrompt_BanishRandomFaceDown(int player, u16 arg, u16 value);
u32 DuelPrompt_PickOpponentHandCard(int player);
u32 DuelPrompt_Tribute(int player);
u32 DuelPrompt_SelectType(void);
u32 DuelPrompt_SelectAttribute(void);
u32 DuelPrompt_SelectTwoAttributes(void);
u32 DuelPrompt_PickOneOfTwoAttributes(void);
u32 DuelPrompt_PickOneOfFiveCards(u16 arg, u16 value);
u32 DuelPrompt_SetMonsterFromHand(int player);
u32 DuelPrompt_SelectGraveyardMonster(int player, u16 arg, u16 value);
u32 DuelPrompt_ConfirmCardEffect(int player, u16 number);
u32 DuelPrompt_OfferDiscardMagic(int player);
u32 DuelPrompt_ConfirmSpecialSummon(int unused, u16 card);
u32 DuelPrompt_ConfirmGraveyardSummon(int unused);
u32 DuelPrompt_SelectOwnReplacementTarget(int unused, u32 value);
u32 DuelPrompt_SelectOpponentReplacementTarget(int unused, u32 value);
void DiscardHandCard(int player, int idx, int a, int b);
extern const char gStrPromptDiscardMagic[];
extern const char gStrPromptSelectMagicToDiscard[];
extern const char gStrPromptPayToViewHand[];
extern const char gStrPromptChangeOpponentPosition[];
extern const u16 gCardNumberToId[];   /* card number to card id */
/* Through a constant address, as the ROM's register allocation needs. */
#define CARD_ID_TABLE ((const u16 *)0x08623DF4)

/* Card number (0..1999, 2000+ = alternate art) to card id; 0xFFFF maps to 0 (same inline as code_080044E4). */
static inline u16 CardNumberToId(u16 n)
{
    if (n == 0xFFFF)
        return 0;
    if (n < 2000)
        return *(CARD_ID_TABLE + (n & 0x7FF));
    return *(CARD_ID_TABLE + ((n - 2000) & 0x7FF)) + 1;
}
u16 GetCardIconObjTile(u16 a);
void AddAffineSprite(u32 yx, u16 shapeSize, u16 attr2, u32 affine);

extern const u16 gPulseScaleCurve[];   /* pulse scale, 16 entries */

/* gMain (0x03000040): only the fields used here. */
struct Main {
    u8 filler0[6];
    u16 newKeys;        /* +0x006 */
    u8 filler8[0x485E - 8];
    u16 frameCounter;   /* +0x485E */
};
extern struct Main gMain;
#define gMain gMain
u32 CanSummonFromHand(int player, u32 id);
u32 IsSpecialSummonOnly(u32 id);

extern const u32 gCardStats[];   /* card stats, indexed by card ID */
#define CSTATS ((const u32 *)0x08621DE0)
#define CARD_STATS(id) (CSTATS[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_LEVEL(id) ((CARD_STATS(id) & 0x1E000000) >> 25)

extern const char gStrPromptSpecialSummonFmt[];
extern const char gStrPromptOpponentSpecialSummonedFmt[];
extern const char gStrPromptSelectOwnReplacementTarget[];
extern const char gStrPromptSelectOpponentReplacementTarget[];
extern const char gCardNames[][0x40];   /* card name table */
extern u16 gUnk_086249D4;

struct Unk0201AE60 {
    u8 filler0[0x14];
    u16 unk14;
};
extern struct Unk0201AE60 gTextBox;

struct Unk0201CFB0 {
    u8 filler0[0x82C];
    u32 unk82C;
};
extern struct Unk0201CFB0 gDuelScreen;
void DuelPrompt_Post(int player, int kind, u16 arg, u16 value);
void MemCopy16(void *dst, const void *src, u32 size);
u32 LinkQueueMessage(void *buf, int len);
u16 DuelLink_SendMessageData(u16 head, const void *src, int size);
void CopyDuelCard(u32 *dst, const u32 *src);

u32 sub_08021CC8(void)
{
    return 0;
}

void sub_08021CCC(void)
{
    gDuel.unk1B43 = 0;
    gDuel.unk1B44 = 0;
}
void sub_08021CEC(u16 a, u16 b, u16 pulse)
{
    u16 tile;

    tile = GetCardIconObjTile(a) | 0x1000;
    AddAffineSprite(0x00400050, 0x80, tile,
                 pulse == 0 ? gPulseScaleCurve[(gMain.frameCounter & 0x1E) >> 1] << 16 : 0x01000000);
    if (b)
        tile = GetCardIconObjTile(a) | 0x1000;
    else
        tile = 0x40;
    AddAffineSprite(0x004000A0, 0x80, tile,
                 pulse ? (gPulseScaleCurve[(gMain.frameCounter & 0x1E) >> 1] << 16) | 0x20 : 0x01000020);
}
u32 DuelPrompt_ConfirmCardEffect(int player, u16 number)
{
    
    switch (gDuel.step) {
    case 0:
        sub_080197C0(player, CardNumberToId(number));
        gDuel.step++;
        return 0;
    case 1:
        switch (number) {
        case 0x489:
            TextBoxOpen(0x206, 0x712, 11, gStrPromptPayToViewHand);
            break;
        case 0x5ED:
        case 0x5EF:
            TextBoxOpen(0x206, 0x712, 11, gStrPromptChangeOpponentPosition);
            break;
        }
        TextBoxSetMenu(1, 0, 0);
        gDuel.step++;
        return 0;
    default:
        gDuel.result = gTextBox.unk14;
        return 1;
    }
}
#define EC_ID(w) (((w) << 20) >> 20)
/* The hand counts are read as an array behind a pointer to gDuel.players: agbcc then loads the
 * 0x020192E4 base before computing the index (the loop-bottom tests), unlike gDuel.players[k]. */
struct EC_Players { struct DuelPlayer p[2]; };
#define EC_PLAYERS (((struct EC_Players *)gDuel.players)->p)
u32 DuelPrompt_OfferDiscardMagic(int player)
{
    int i;

    switch (gDuel.step) {
    case 0:
        gDuel.result = 0;
        for (i = 0; i < EC_PLAYERS[player & 1].handCount; i++) {
            union DuelCardWord *c = &(gDuel.players + (player & 1))->hand[i];
            if (((CSTATS[c->w << 21 >> 21] & 0x1F00000) >> 20) == 22 && !c->c.flag18) {
                if (player) {
                    /* FAKEMATCH: makes player & 1 loop-variant so it is recomputed every pass, as in the ROM */
                    asm("" : "+r"(player));
                    gDuel.result = 1;
                    i = 0;
                    /* FAKEMATCH: lengthens i's live range by one insn, so the 0xD64 constant is allocated first (r3) and i gets r4 */
                    asm("");
                    for (; i < EC_PLAYERS[1].handCount; i++) {
                        if (((CSTATS[EC_ID((&gDuel.players[1].hand[i])->w) & 0x7FF] & 0x1F00000) >> 20) == 22) {
                            DiscardHandCard(1, i, 1, 1);
                            return 1;
                        }
                    }
                    gDuel.result = 0;
                    return 1;
                }
                TextBoxOpen(0x206, 0x712, 11, gStrPromptDiscardMagic);
                TextBoxSetMenu(1, 0, 0);
                gDuel.step++;
                return 0;
            }
        }
        return 1;
    case 1:
        if (gTextBox.unk14 == 0) {
            gDuel.result = 0;
            return 1;
        }
        TextBoxOpen(0x206, 0x712, 11, gStrPromptSelectMagicToDiscard);
        gDuel.step++;
        return 0;
    default:
        if (gMain.newKeys & 2) {
            gDuel.step = 0;
            return 0;
        }
        if (DuelCursor_PickTarget(1)) {
            u32 idx = gDuelScreen.unk82C;
            union DuelCardWord *c = &gDuel.players[0].hand[idx];
            if (((CSTATS[c->w << 21 >> 21] & 0x1F00000) >> 20) == 22) {
                int ok = 1;
                if (c->c.flag17)
                    ok = 0;
                if (c->c.flag18)
                    ok = 0;
                if (ok) {
                    gDuel.result = 1;
                    DiscardHandCard(0, idx, 1, 1);
                    return 1;
                }
            }
            PlaySE(3);
        }
        return 0;
    }
}
u32 DuelPrompt_ConfirmSpecialSummon(int unused, u16 card)
{
    char buf[0x80];

    if (gDuel.step == 0) {
        gDuel.result = 0;
        if (card == 0)
            return 1;
        FormatStr(buf, gStrPromptSpecialSummonFmt, gCardNames[card]);
        TextBoxOpen(0x206, 0x713, 11, buf);
        TextBoxSetMenu(1, 0, 0);
        gDuel.step++;
        return 0;
    }
    gDuel.result = gTextBox.unk14;
    return 1;
}
u32 DuelPrompt_ConfirmGraveyardSummon(int unused)
{
    char buf[0x100];

    if (gDuel.step != 0) {
        gDuel.result = gTextBox.unk14;
        return 1;
    }
    gDuel.result = 0;
    FormatStr(buf, gStrPromptOpponentSpecialSummonedFmt, gCardNames[gUnk_086249D4]);
    TextBoxOpen(0x206, 0x713, 11, gStrPromptOpponentSpecialSummonedFmt); /* passes the format, not buf */
    TextBoxSetMenu(1, 0, 0);
    gDuel.step++;
    return 0;
}
u32 DuelPrompt_SelectOwnReplacementTarget(int unused, u32 value)
{
    switch (gDuel.step) {
    case 0:
        gDuel.result = 0;
        TextBoxOpen(0x206, 0x713, 11, gStrPromptSelectOwnReplacementTarget);
        gDuel.step++;
        break;
    case 1:
        if (DuelCursor_PickTarget(0xF0)) {
            if (gDuelScreen.unk82C != value) {
                gDuel.result = gDuelScreen.unk82C;
                gDuel.step++;
            }
            PlaySE(3);
        }
        break;
    default:
        return 1;
    }
    return 0;
}
u32 DuelPrompt_SelectOpponentReplacementTarget(int unused, u32 value)
{
    switch (gDuel.step) {
    case 0:
        gDuel.result = 0;
        TextBoxOpen(0x206, 0x713, 11, gStrPromptSelectOpponentReplacementTarget);
        gDuel.step++;
        break;
    case 1:
        if (DuelCursor_PickTarget(0xF00000)) {
            if (gDuelScreen.unk82C != value) {
                gDuel.result = gDuelScreen.unk82C;
                gDuel.step++;
            }
            PlaySE(3);
        }
        break;
    default:
        return 1;
    }
    return 0;
}
/* Run the handler of the pending duel message; returns 1 while it is still running. */
u32 DuelPrompt_Run(void)
{
    u16 done;

    if (!gDuel.msgPending)
        return 0;
    if (gDuel.msgSent && gDuel.msgPlayer) {
        done = gLinkState.unk307_7;
    } else {
        switch (gDuel.msgKind) {
        case 1:
            done = DuelPrompt_Discard(gDuel.msgPlayer, gDuel.msgArg, gDuel.msgValue & 1, gDuel.msgValue & 2);
            break;
        case 2:
            done = DuelPrompt_DiscardCost(gDuel.msgPlayer, gDuel.msgArg, gDuel.msgValue & 1, gDuel.msgValue & 2);
            break;
        case 3:
            done = DuelPrompt_DiscardRandom(gDuel.msgPlayer, gDuel.msgArg, gDuel.msgValue);
            break;
        case 4:
            done = DuelPrompt_BanishRandom(gDuel.msgPlayer, gDuel.msgArg, gDuel.msgValue);
            break;
        case 5:
            done = DuelPrompt_BanishRandomFaceDown(gDuel.msgPlayer, gDuel.msgArg, gDuel.msgValue);
            break;
        case 6:
            done = DuelPrompt_PickOpponentHandCard(gDuel.msgPlayer);
            break;
        case 7:
            done = DuelPrompt_Tribute(gDuel.msgPlayer);
            break;
        case 8:
            done = DuelPrompt_SelectType();
            break;
        case 9:
            done = DuelPrompt_SelectAttribute();
            break;
        case 10:
            done = DuelPrompt_SelectTwoAttributes();
            break;
        case 11:
            done = DuelPrompt_PickOneOfTwoAttributes();
            break;
        case 12:
            done = DuelPrompt_PickOneOfFiveCards(gDuel.msgArg, gDuel.msgValue);
            break;
        case 13:
            done = DuelPrompt_SetMonsterFromHand(gDuel.msgPlayer);
            break;
        case 14:
            done = DuelPrompt_SelectGraveyardMonster(gDuel.msgPlayer, gDuel.msgArg, gDuel.msgValue);
            break;
        case 15:
            done = DuelPrompt_ConfirmCardEffect(gDuel.msgPlayer, gDuel.msgArg);
            break;
        case 16:
            done = DuelPrompt_SelectOpponentReplacementTarget(gDuel.msgPlayer, gDuel.msgArg);
            break;
        case 17:
            done = DuelPrompt_OfferDiscardMagic(gDuel.msgPlayer);
            break;
        case 18:
            done = DuelPrompt_ConfirmSpecialSummon(gDuel.msgPlayer, gDuel.msgArg);
            break;
        case 19:
            done = DuelPrompt_ConfirmGraveyardSummon(gDuel.msgPlayer);
            break;
        case 20:
            done = DuelPrompt_SelectOwnReplacementTarget(gDuel.msgPlayer, gDuel.msgArg);
            break;
        default:
            return 0;
        }
    }
    if (done) {
        if (gDuel.msgSent && !gDuel.msgPlayer)
            DuelLink_SendMessageData(0xF0A2, &gDuel.result, 0x10);
        gDuel.msgPending = 0;
        return 0;
    }
    return 1;
}
void DuelPrompt_Start(void)
{
    u16 m[9];

    gDuel.msgPending = 1;
    gDuel.step = 0;
    gDuel.unk1B63 = 0;
    gDuel.msgSent = 0;
    gDuel.msgUnk3 = 0;
    if (gDuel.msgPlayer && gDuelCtrl.link) {
        u16 kind = gDuel.msgKind;
        if (kind == 3)
            return;
        m[0] = kind;
        MemCopy16(&m[1], &gDuel.msgArg, 0x10);
        DuelLink_SendMessageData(0xF0A1, m, 0x12);
        gLinkState.unk307_7 = 0;
        gDuel.msgSent = 1;
    }
}
void DuelPrompt_Post(int player, int kind, u16 arg, u16 value)
{
    gDuel.msgPlayer = player;
    gDuel.msgKind = kind;
    gDuel.msgArg = arg;
    gDuel.msgValue = value;
    DuelPrompt_Start();
}
void DuelPrompt_PostData(int player, int kind, const u16 *src, int n)
{
    if (n > 8)
        n = 8;
    gDuel.msgPlayer = player;
    gDuel.msgKind = kind;
    MemCopy16(&gDuel.msgArg, src, n * 2);
    DuelPrompt_Start();
}
void DuelPrompt_PostDiscard(int player, int arg, u16 x, u16 y)
{
    u16 flags = x != 0;
    if (y)
        flags |= 2;
    DuelPrompt_Post(player, 1, arg, flags);
}
void DuelPrompt_PostDiscardCost(int player, int arg, u16 x, u16 y)
{
    u16 flags = x != 0;
    if (y)
        flags |= 2;
    DuelPrompt_Post(player, 2, arg, flags);
}
void DuelPrompt_PostRandomDiscard(int player, u16 arg, int value)
{
    if (gDuel.msgPending && gDuel.msgKind == 3)
        gDuel.msgValue += value;
    else
        DuelPrompt_Post(player, 3, arg, value);
}
void DuelPrompt_PostRandomBanish(int player, int value)
{
    if (gDuel.msgPending && gDuel.msgKind == 4)
        gDuel.msgValue += value;
    else
        DuelPrompt_Post(player, 4, 0, value);
}

void DuelPrompt_PostRandomBanishFaceDown(int player)
{
    DuelPrompt_Post(player, 5, 0, 1);
}

void DuelPrompt_PostTribute(int player)
{
    DuelPrompt_Post(player, 7, 0, 0);
}
/* Monster level: 0 for types 21-23, 10 for type 24 (Divine), else stats bits 25-28. */
static inline u32 GetCardLevel(u16 id)
{
    int type = CARD_TYPE(id);

    switch (type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 10;
    default:
        return CARD_LEVEL(id);
    }
}

u32 DuelPrompt_TryPostSetMonster(int player)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u16 id = HAND_CARD(player, i) << 20 >> 20;
        if (CanSummonFromHand(player, id) && !IsSpecialSummonOnly(id) && GetCardLevel(id) <= 4) {
            DuelPrompt_Post(player, 13, 0, 0);
            return 1;
        }
    }
    return 0;
}
void sub_08022914(void)
{
    if (!gLinkState.unk306_6 && gDuel.unk1B14_2 == 0) {
        gLinkState.unk306_6 = 1;
        gDuel.unk1B14_2 = 0;
    }
}
u32 sub_0802295C(void)
{
    gDuel.unk1B14_2 = 0;
    return 0;
}

struct Msg8 {
    u32 a:16;
    u32 b:16;
    u32 c:16;
    u32 d:16;
};

u16 DuelLink_SendMessage(u16 a, u16 b, u16 c, u16 d)
{
    struct Msg8 m;
    m.a = a;
    m.b = b;
    m.c = c;
    m.d = d;
    return LinkQueueMessage(&m, 8);
}

u16 DuelLink_SendMessageData(u16 head, const void *src, int size)
{
    u16 buf[0x80];
    buf[0] = head;
    if (size > 0)
        MemCopy16(&buf[1], src, size);
    return LinkQueueMessage(buf, size + 2);
}
/* Send a card list to the link partner: {id, (player << 8) | count, cards[count]}; the do/while
 * form keeps agbcc from strength-reducing &buf[i], as in the ROM. */
void DuelLink_SendHand(int player)
{
    int i;

    gLinkState.msgId = 0xF021;
    gLinkState.msgArg = gDuelPlayers[player & 1].handCount | ((u8)player << 8);
    i = 0;
    if (i < gDuelPlayers[player & 1].handCount)
        do
            CopyDuelCard(&gLinkTxCards[i], &gDuelPlayers[player & 1].hand[i].w);
        while (++i < gDuelPlayers[player & 1].handCount);
    LinkQueueMessage(&gLinkState.msgId, gDuelPlayers[player & 1].handCount * 4 + 4);
    gLinkState.dirtyHand = 0;
}
void DuelLink_SendDeck(int player)
{
    int i;

    gLinkState.msgId = 0xF022;
    gLinkState.msgArg = gDuelPlayers[player & 1].deckCount | ((u8)player << 8);
    i = 0;
    if (i < gDuelPlayers[player & 1].deckCount)
        do
            CopyDuelCard(&gLinkTxCards[i], &gDuelPlayers[player & 1].deck[i]);
        while (++i < gDuelPlayers[player & 1].deckCount);
    LinkQueueMessage(&gLinkState.msgId, gDuelPlayers[player & 1].deckCount * 4 + 4);
    gLinkState.dirtyDeck = 0;
}
void DuelLink_SendGraveyard(int player)
{
    int i;

    gLinkState.msgId = 0xF023;
    gLinkState.msgArg = gDuelPlayers[player & 1].graveCount | ((u8)player << 8);
    i = 0;
    if (i < gDuelPlayers[player & 1].graveCount)
        do
            CopyDuelCard(&gLinkTxCards[i], &gDuelPlayers[player & 1].grave[i]);
        while (++i < gDuelPlayers[player & 1].graveCount);
    LinkQueueMessage(&gLinkState.msgId, gDuelPlayers[player & 1].graveCount * 4 + 4);
    gLinkState.dirtyGrave = 0;
}
void DuelLink_SendFusionDeck(int player)
{
    int i;

    gLinkState.msgId = 0xF024;
    gLinkState.msgArg = gDuelPlayers[player & 1].fusionCount | ((u8)player << 8);
    i = 0;
    if (i < gDuelPlayers[player & 1].fusionCount)
        do
            CopyDuelCard(&gLinkTxCards[i], &gDuelPlayers[player & 1].fusion[i]);
        while (++i < gDuelPlayers[player & 1].fusionCount);
    LinkQueueMessage(&gLinkState.msgId, gDuelPlayers[player & 1].fusionCount * 4 + 4);
    gLinkState.dirtyFusion = 0;
}
void DuelLink_SendBanished(int player)
{
    int i;

    gLinkState.msgId = 0xF025;
    gLinkState.msgArg = gDuelPlayers[player & 1].banishCount | ((u8)player << 8);
    i = 0;
    if (i < gDuelPlayers[player & 1].banishCount)
        do
            CopyDuelCard(&gLinkTxCards[i], &gDuelPlayers[player & 1].banish[i]);
        while (++i < gDuelPlayers[player & 1].banishCount);
    LinkQueueMessage(&gLinkState.msgId, gDuelPlayers[player & 1].banishCount * 4 + 4);
    gLinkState.dirtyBanish = 0;
}
