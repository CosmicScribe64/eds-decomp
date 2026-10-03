#include "global.h"

struct DuelCard { u32 id:12; u32 flag12:1; u32 rest:19; };
struct ActRec {
    u32 player:1, zone5:5, zone8:8, f14:1, f15:1;
    u32 f16:3, f19:3, f22:3, f25:1, f26:1, f27:1, f28:1, f29:1, f30:1;
    u32 cardId:16, pad47:17;
    struct DuelCard card;
    u16 h0C;
    u32 active:1, ready:1, kind:3, step:7, phase:4;
    u32 aux0:3, aux3:8, aux11:8, pad19:13;
};
extern struct ActRec gSummonAction;
struct DuelPlayer { u8 pad0[8]; u8 f8lo:4, handAction:1, queued:1, f8hi:2; u8 rest[0xD64-9]; };
extern struct DuelPlayer gDuelPlayers[];
extern u8 gDuelCtrl[];
struct DuelWork { u8 pad[0x306]; u8 flags; };
extern struct DuelWork gLinkState;
void DuelLink_SendMessageData(int command, void *data, int size);
void DuelCmd_Push(int command, int a, int b, int c);
void DuelCursor_Select(int player, int zone, int index);
void Chain_AddPending(u32 action, u32 target);
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
void TributeMonster(int player, int zone);
struct Menu { u8 pad[0x14]; u16 selection; };
extern struct Menu gTextBox;
int AiShouldSetMonster(int id, int flag);
void TextBoxOpen(int a, int b, int c, const void *text);
void TextBoxSetMenu(int kind, void (*draw)(void), int (*keys)(void));
void SummonPositionMenu_Draw(void);
int SummonPositionMenu_HandleInput(void);
extern const u8 gStrSelectDisplayPosition[];
void CopyDuelCard(struct DuelCard *dst, struct DuelCard *src);
void DestroyFieldCard(int player, int zone, int flag);
int CountActiveCardsOnField(int player, u16 number);
void PayChainEnergyCost(int player);
extern u32 gDuelHands[];
void SummonAction_Start(void);
int IsToonMonster(int number);
u16 ExecuteSummonAction(void), ExecuteSummonActionAskPosition(void), SummonStep_Flip(void), SummonStep_SpecialFromHand(void);
void DrawCards(int player, int zone);
void ApplyPumpkingBoost(int player, int zone);
void DisableFaceUpTraps(void), ApplyKotodama(void);
void ApplyKotodamaToZone(int player, int zone);
void ApplyDragonCaptureJar(int player);
void EventResponse_Request(int player, int kind, int target);
void DuelLink_SendMessage(int command, int a, int b, int c);
struct Zone { u32 card; u8 pad[2]; u8 flag0:1, flag1:1, rest:6; u8 tail[0x94-7]; };
extern u8 gDuelZones[];
#define ZONE(p,z) ((struct Zone *)((z)*0x94 + (p)*0xD64 + (u32)gDuelZones))
extern const u16 gCardNumberToId[];
static inline int NumberId(int number)
{
    /* FAKEMATCH: the original table lookup keeps its initialized address in r0. */
    register u32 address __asm__("r0") = number * 2;
    address += (u32)gCardNumberToId;
    return *(const u16 *)address;
}
#define NUMBER_ID(n) NumberId(n)

int CanActivateEffectOfCard(int player, u16 id, u16 x);
u32 HasFlipEffect(u16 number, u16 flag);
void ShowCardEffect(int player, u16 id);
void TriggerMysteriousPuppeteer(int player);
void ChangeBattlePosition(int player, int zone, u16 a, u16 b);
/* Zone address terms are staged in the same order as the record loads. */
static inline struct Zone *ActionZone(int player, int zone)
{
    int off = zone * 0x94;
    off += player * 0xD64;
    off += (u32)gDuelZones;
    return (struct Zone *)off;
}

static inline struct Zone *RecordZone(struct ActRec *r)
{
    int player = r->player & 1;
    int zone = r->zone5;
    return ActionZone(player, zone);
}
#define ACTION_STATS(id) (((const u32 *)0x08621DE0)[(id)&0x7FF])
static inline u16 ActionNumberId(int number)
{
    /* FAKEMATCH: retain the initialized doubled table index in r1. */
    register u32 off asm("r1") = number * 2;
    off += (u32)gCardNumberToId;
    return *(const u16 *)off;
}

static inline u32 ActionCardLevel(u32 id)
{
    switch ((int)((ACTION_STATS(id) & 0x1F00000) >> 20)) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 10;
    default:
        return (ACTION_STATS(id) & 0x1E000000) >> 25;
    }
}

u16 SummonStep_Flip(void)
{
    struct ActRec *r = &gSummonAction;
    switch (r->step) {
    case 0: {
        int p = r->player;
        int side = p & 1;
        int z = r->zone5;
        if (ActionZone(side, z)->flag0) {
            u32 id;
            DuelCmd_Push(p ? 0x807E : 0x7E, z, 1, 0);
            id = ((RecordZone(r)->card << 20) >> 20);
            if (CARD_NUMBER(id) == 0x5E && CanActivateEffectOfCard(r->player, id, 0)) {
                int player = r->player & 1;
                u32 action = (u32)player << 31;
                int zone = r->zone5;
                u32 destination = zone << 16;
                destination |= 0x14400000;
                action |= destination;
                action |= ((ActionZone(player, zone)->card << 20) >> 20);
                Chain_AddPending(action, 0);
            }
        } else {
            DuelCmd_Push(p ? 0x807F : 0x7F, z, 1, 0);
        }
        gSummonAction.step++;
        return 0;
    }
    case 1:
        DuelCursor_Select(r->player, 0, r->zone5);
        DuelCmd_Push(0x71, r->cardId, 1, 0);
        r->step++;
        return 0;
    case 2:
        if (ActionCardLevel(r->cardId) <= 2) {
            int number = 0x2AE;
            if (CountActiveCardsOnField(0, number) > 0 || CountActiveCardsOnField(1, number) > 0) {
                ShowCardEffect(gSummonAction.player, ActionNumberId(number));
                DestroyFieldCard(gSummonAction.player, gSummonAction.zone5, 1);
                return 1;
            }
        }
        gSummonAction.step++;
        return 0;
    case 3:
        DuelCmd_Push(r->player ? 0x8090 : 0x90, r->zone5, r->h0C, 0);
        {
            /* FAKEMATCH: preserve the initialized record byte in its original scratch. */
            register u32 byte asm("r3") = *(u8 *)r;
            asm("" : : "r"(byte));
            TriggerMysteriousPuppeteer((byte << 31) >> 31);
        }
        switch (CARD_NUMBER(r->cardId)) {
        case 0x1F3:
        case 0x455:
        case 0x462:
        case 0x4D8:
        case 0x4DE:
        case 0x534:
            if (CanActivateEffectOfCard(gSummonAction.player, ((RecordZone(&gSummonAction)->card << 20) >> 20), 0)) {
                int player = gSummonAction.player;
                u32 action = (u32)(player & 1) << 31;
                int zone = gSummonAction.zone5;
                u32 destination = zone << 16;
                destination |= 0xC400000;
                action |= destination;
                action |= gSummonAction.cardId;
                Chain_AddPending(action, player | zone << 8);
            }
            break;
        case 0x31C:
            ChangeBattlePosition(r->player, r->zone5, 0, 0);
            break;
        }
        if (HasFlipEffect(CARD_NUMBER(gSummonAction.cardId), 0)) {
            if (CanActivateEffectOfCard(gSummonAction.player, ((RecordZone(&gSummonAction)->card << 20) >> 20), 0) &&
                !CountActiveCardsOnField(0, 0x5FA) && !CountActiveCardsOnField(1, 0x5FA)) {
                int player = gSummonAction.player;
                u32 action = (u32)(player & 1) << 31;
                int zone = gSummonAction.zone5;
                u32 destination = zone << 16;
                destination |= 0xC400000;
                action |= destination;
                action |= gSummonAction.cardId;
                Chain_AddPending(action, player | zone << 8);
            }
        }
        gSummonAction.step++;
        return 0;
    default:
        return 1;
    }
}

u16 SummonStep_Special(void)
{
    struct ActRec *r = &gSummonAction;
    switch (r->step) {
    case 0: {
        int command = r->player ? 0x8077 : 0x77;
        int zone = r->zone5;
        int flag = r->f14;
        int flags = r->f15 << 1;
        flags |= flag;
        DuelCmd_Push(command, zone | (flags << 8),
                    *(u16 *)&r->card, *((u16 *)&r->card + 1));
        r->step++;
        return 0;
    }
    case 1:
        DuelCursor_Select(r->player, 0, r->zone5);
        DuelCmd_Push(0x71, r->cardId, 1, 0);
        r->step++;
        return 0;
    case 2: {
        int command = r->player ? 0x8090 : 0x90;
        int id;
        DuelCmd_Push(command, r->zone5, r->h0C, 0);
        id = r->cardId;
        if (CARD_NUMBER(id) == 0x4DE) {
            int player = r->player;
            u32 action = player << 31;
            int zone = r->zone5;
            u32 destination = zone << 16;
            destination |= 0xC400000;
            action |= destination;
            action |= id;
            Chain_AddPending(action, player | (zone << 8));
        }
        r->step++;
        return 0;
    }
    default:
        return 1;
    }
}
u16 SummonStep_SpecialChoosePosition(void)
{
    struct ActRec *r = &gSummonAction;
    struct DuelCard copy;
    switch (r->step) {
    case 0:
        if (r->player) {
            gTextBox.selection = AiShouldSetMonster(r->cardId, r->f14);
        } else {
            TextBoxOpen(0x207, 0x30F, 11, gStrSelectDisplayPosition);
            TextBoxSetMenu(5, SummonPositionMenu_Draw, SummonPositionMenu_HandleInput);
        }
        r->step++;
        return 0;
    case 1: {
        int command, zone, flag, flags;
        u32 word;
        r->f15 = gTextBox.selection;
        if (!r->f15) r->f14 = 1;
        CopyDuelCard(&copy, &r->card);
        command = r->player ? 0x8077 : 0x77;
        zone = r->zone5;
        flag = r->f14;
        flags = r->f15 << 1;
        flags |= flag;
        DuelCmd_Push(command, zone | (flags << 8), (u16)*(u32 *)&copy, *(u32 *)&copy >> 16);
        r->step++;
        return 0;
    }
    case 2:
        DuelCursor_Select(r->player, 0, r->zone5);
        DuelCmd_Push(0x71, r->cardId, 1, 0);
        r->step++;
        return 0;
    case 3: {
        int command = r->player ? 0x8090 : 0x90;
        int id, number;
        DuelCmd_Push(command, r->zone5, r->h0C, 0);
        id = r->cardId;
        number = CARD_NUMBER(id);
        switch (number) {
        case 0x4DE: {
            int player = r->player;
            u32 action = player << 31;
            int zone = r->zone5;
            u32 destination = zone << 16;
            destination |= 0xC400000;
            action |= destination;
            action |= id;
            Chain_AddPending(action, player | (zone << 8));
        }
        break;
        case 0x5F6: {
            int i;
            for (i = 0; i < 5; i++) {
                if (i != gSummonAction.zone5)
                    DestroyFieldCard(gSummonAction.player, i, 1);
            }
        }
        break;
        }
        gSummonAction.step++;
        return 0;
    }
    default:
        return 1;
    }
}
u16 SummonStep_SpecialFromHand(void)
{
    struct ActRec *r = &gSummonAction;
    switch (r->step) {
    case 0: {
        u16 msg;
        int id;
        if (r->f25) TributeMonster(r->player, r->f16);
        if (r->f26) TributeMonster(r->player, r->f19);
        if (r->f27) TributeMonster(r->player, r->f22);
        msg = r->player ? 0x80C4 : 0xC4;
        id = r->cardId;
        {
            /* FAKEMATCH: the initialized shifted field stays in r0 until masked. */
            register u32 shifted asm("r0") = *(u16 *)r >> 6;
            u32 mask = 15;
            u32 packed = mask;
            packed &= shifted;
            packed <<= 4;
            mask &= r->zone5;
            packed |= mask;
            packed |= (r->f14 | r->f15 << 1) << 8;
            DuelCmd_Push(msg,id,packed,0);
        }
        r->step++;
        return 0;
    }
    case 1:
        DuelCursor_Select(r->player, 0, r->zone5);
        DuelCmd_Push(0x71, r->cardId, 1, 0);
        r->step++;
        return 0;
    case 2: {
        int command = r->player ? 0x8090 : 0x90;
        int id;
        DuelCmd_Push(command, r->zone5, r->h0C, 0);
        id = r->cardId;
        if (CARD_NUMBER(id) == 0x4DE) {
            int player = r->player;
            u32 action = player << 31;
            int zone = r->zone5;
            u32 destination = zone << 16;
            destination |= 0xA400000;
            action |= destination;
            action |= id;
            Chain_AddPending(action, player | (zone << 8));
        }
        r->step++;
        return 0;
    }
    default:
        return 1;
    }
}


/* FAKEMATCH: the initialized u16 copy below preserves the original r8-to-r2
 * transfer. Its input is a 12-bit card id, so narrowing is lossless. */
u16 SummonAction_Update(void)
{
    struct RawAct { u8 pad[14]; u8 flags; };
    int flags = ((struct RawAct *)&gSummonAction)->flags;
    if (flags & 1) {
        if (!(2 & flags) || !gSummonAction.player) {
            switch (((u32)flags << 27) >> 29) {
            case 1: if (!ExecuteSummonAction()) return 1; break;
            case 2: if (!ExecuteSummonActionAskPosition()) return 1; break;
            case 3: if (!SummonStep_Flip()) return 1; break;
            case 4: if (!SummonStep_Special()) return 1; break;
            case 5: if (!SummonStep_SpecialChoosePosition()) return 1; break;
            case 6: if (!SummonStep_SpecialFromHand()) return 1; break;
            }
        } else {
            if (!(gLinkState.flags >> 7)) return 1;
        }
        {
            struct ActRec *r = &gSummonAction;
            struct Zone *zone;
            int card;
            r->active = 0;
            {
                int player = r->player;
                int index = r->zone5;
                zone = ZONE(player, index);
            }
            card = (zone->card << 20) >> 20;
            if (card != 0) {
                int id = card;
                int wasFlagged = zone->flag1;
                DuelCursor_Select(r->player, 0, r->zone5);
                if (r->h0C & 0x20) {
                    int player = r->player;
                    int number = 0x595;
                    int index = CountActiveCardsOnField(player, number);
                    if (index > 0) {
                        DuelCmd_Push(r->player ? 0x8073 : 0x73, NUMBER_ID(number), 1, 0);
                        DrawCards(r->player, index);
                    }
                }
                if (wasFlagged) {
                    int number = 0x610;
                    if (CountActiveCardsOnField(0, number) || CountActiveCardsOnField(1, number)) {
                        DuelCmd_Push(gSummonAction.player ? 0x8073 : 0x73, NUMBER_ID(number), 1, 0);
                        DuelCmd_Push(gSummonAction.player ? 0x8096 : 0x96, gSummonAction.zone5, 1, 0);
                    }
                }
                {
                    u32 index = 0x7FF;
                    register u16 savedId __asm__("r2") = id;
                    int number;
                    index &= savedId;
                    number = ((const u16 *)0x08622AB4)[index];
                    switch (number) {
                    case 0x62:
                        ApplyPumpkingBoost(gSummonAction.player, gSummonAction.zone5);
                        break;
                    case 0x2EF:
                        if (wasFlagged) {
                            DuelCmd_Push(gSummonAction.player ? 0x8073 : 0x73, NUMBER_ID(number), 1, 0);
                            DisableFaceUpTraps();
                        }
                        break;
                    case 0x464:
                        if (wasFlagged) ApplyKotodama();
                        break;
                    }
                }
                if (wasFlagged)
                    ApplyKotodamaToZone(gSummonAction.player, gSummonAction.zone5);
                {
                    u32 byte = *(u8 *)&gSummonAction;
                    ApplyDragonCaptureJar((byte << 31) >> 31);
                }
                {
                    int kind;
                    u32 kindFlags = ((struct RawAct *)&gSummonAction)->flags;
                    switch ((int)((kindFlags << 27) >> 29)) {
                    case 1: case 2: { int choice = gSummonAction.f14 ? 5 : 8; kind = choice; break; }
                    case 3: kind = 6; break;
                    default: kind = 7; break;
                    }
                    EventResponse_Request(1 - gSummonAction.player, kind,
                                 gSummonAction.player | (gSummonAction.zone5 << 8));
                }
            }
        }
        if ((((struct RawAct *)&gSummonAction)->flags & 2) && !gSummonAction.player)
            DuelLink_SendMessage(0xF05B, 0, 0, 0);
    }
    return gSummonAction.active;
}

void SummonAction_Start(void)
{
    struct ActRec *r = &gSummonAction;
    r->step = 0;
    r->phase = 0;
    r->aux0 = 0;
    r->aux3 = 0;
    r->aux11 = 0;
    r->active = 1;
    r->ready = 0;
    gDuelPlayers[r->player & 1].queued = 1;
    if (r->player && (gDuelCtrl[1] & 1)) {
        r->ready = 1;
        DuelLink_SendMessageData(0xF05A, r, 0x14);
        gLinkState.flags &= 0x7F;
    }
}
void SummonAction_StartFromLink(void)
{
    struct ActRec *r = &gSummonAction;
    r->player = 0;
    r->step = 0;
    r->phase = 0;
    r->aux0 = 0;
    r->aux3 = 0;
    r->aux11 = 0;
    r->active = 1;
    r->ready = 1;
    {
        struct DuelCard *card = &r->card;
        card->flag12 = 1 - card->flag12;
    }
}
/* Word-valued callers are decoded by the original two halfword shifts. */
/* Queue a hand summon/set and decode the two packed tribute bytes.
 * FAKEMATCH: five initialized register bindings and two empty constraints
 * preserve the original packed-record loads and stores. Caller-saved bindings
 * die before calls. The card ID spans byte 3 and the low 15 bits of halfword 4;
 * stage that split explicitly to retain the original cached low bit. */
struct QueueBytes {
    u8 byte0, byte1;
};
void QueueNormalSummon(int player, int zone, int target, int tributeWord, int faceWord)
{
    u16 tribute = tributeWord;
    u16 faceUp = faceWord;
    register struct ActRec *r asm("r9");
    u32 mask;
    register u32 idmask asm("r8");
    const u16 *table;
    gSummonAction.player = player;
    gSummonAction.zone5 = target;
    gSummonAction.zone8 = zone;
    if (faceUp != 0) {
        gSummonAction.f14 = 1;
        gSummonAction.f15 = 0;
    }
    else {
        gSummonAction.f14 = 0;
        gSummonAction.f15 = 1;
    }
    if (CountActiveCardsOnField(0, 0x47F) || CountActiveCardsOnField(1, 0x47F)) gSummonAction.f14 = 1;
    if (tribute != 0) {
        u8 lo = tribute;
        u8 hi = tribute >> 8;
        gSummonAction.f16 = lo & 7;
        gSummonAction.f19 = hi & 7;
        gSummonAction.f25 = lo >> 7;
        gSummonAction.f26 = hi >> 7;
        gSummonAction.f28 = (lo >> 4) & 1;
        gSummonAction.f29 = (hi >> 4) & 1;
        r = &gSummonAction;
    }
    else {
        gSummonAction.f16 = 0;
        gSummonAction.f19 = 0;
        gSummonAction.f25 = 0;
        gSummonAction.f26 = 0;
        r = &gSummonAction;
    }
    {
        struct ActRec *first = r;
        u32 offset;
        u32 packed;
        {
            u32 side = player;
            u32 address;
            side &= 1;
            address = (u32)zone * 4;
            offset = side * 0xD64;
            address += offset;
            address += (u32)gDuelHands;
            address = *(u32 *)address;
            address <<= 20;
            {
                u32 low = address >> 20;
                low &= 1;
                low <<= 7;
                packed = *((u8 *)first + 3) & 0x7F;
                packed |= low;
                *((u8 *)first + 3) = packed;
            }
            mask = 0x7FFF;
            address >>= 21;
            {
                u32 clear = 0xFFFF8000;
                *(u16 *)((u8 *)first + 4) = (*(u16 *)((u8 *)first + 4) & clear) | address;
            }
        }
        first->kind = 1;
        first->h0C = 3;
        if (!IsToonMonster(({
            u32 value;
            packed >>= 7;
            value = mask;
            value &= *(u16 *)((u8 *)first + 4);
            value <<= 1;
            value |= packed;
            idmask = 0x7FF;
            value &= idmask;
            value <<= 1;
            table = (const u16 *)0x08622AB4;
            *(const u16 *)((u32)table + value);
        }
        ))) {
            first->h0C = 2;
            {
                u32 base = (u32)gDuelHands - 0x684;
                base = offset + base;
                {
                    u32 flags = *((u8 *)base + 8);
                    *((u8 *)base + 8) = flags | 0x10;
                }
            }
        }
    }
    if (IsToonMonster(({
        struct ActRec *rp = r;
        u32 low = ((u8 *)rp)[3];
        u32 bit = low >> 7;
        register struct ActRec *hp asm("r2") = r;
        u32 high = ((u16 *)hp)[2];
        u32 value;
        mask &= high;
        value = (mask << 1) | bit;
        {
            register u32 m asm("r3") = idmask;
            asm("" : : "r"(m));
            value &= m;
        }
        table[value];
    }
    ))) {
        u32 flags = 0x40;
        register struct QueueBytes *readp asm("r6") = (struct QueueBytes *)r;
        flags |= readp->byte1;
        {
            struct QueueBytes *dst = (struct QueueBytes *)r;
            /* Keep the store address in an ordinary low-register pseudo.
             * A direct r7 binding makes old_agbcc emit a separate ADD.
             * These scratch clobbers are dead here and emit no instructions. */
            asm("" : "+l"(dst) : "r"(flags) : "r1", "r2", "r3", "r4", "r5", "r6");
            dst->byte1 = flags;
        }
    }
    PayChainEnergyCost(player);
    SummonAction_Start();
}

void QueueNormalSummonChoosePosition(int player, int zone, int target, u16 tribute)
{
    struct ActRec *r;
    gSummonAction.player = player;
    gSummonAction.zone5 = target;
    gSummonAction.zone8 = zone;
    gSummonAction.f14 = 0;
    if (CountActiveCardsOnField(0, 0x47F) || CountActiveCardsOnField(1, 0x47F))
        gSummonAction.f14 = 1;
    if (tribute != 0) {
        u8 lo = tribute;
        u8 hi = tribute >> 8;
        r = &gSummonAction;
        r->f16 = lo & 7;
        r->f19 = hi & 7;
        r->f25 = lo >> 7;
        r->f26 = hi >> 7;
        r->f28 = (lo >> 4) & 1;
        r->f29 = (hi >> 4) & 1;
    } else {
        gSummonAction.f16 = 0;
        gSummonAction.f19 = 0;
        gSummonAction.f25 = 0;
        gSummonAction.f26 = 0;
        r = &gSummonAction;
    }
    r->cardId = ((*(u32 *)((player & 1) * 0xD64 + zone * 4 + (u32)gDuelHands)) << 20) >> 20;
    r->kind = 2;
    r->h0C = 3;
    PayChainEnergyCost(player);
    SummonAction_Start();
}
