#include "global.h"

/* gBattle (0x02018450) flag byte. */
struct BattleB {
    u8 attacker : 1;
    u8 direct : 1;
    u8 unk2 : 1;
    u8 f3 : 1;
    u8 f4 : 1;
    u8 f5 : 1;
    u8 unk6 : 2;
    u8 unk1_0 : 1;
    u8 defSlot : 3;
    u8 unk1_4 : 4;
    u8 pad[0x20];
};
extern struct BattleB gBattle;

struct BattleH {
    u16 unk0_0 : 6;
    u16 atkSlot : 3;
    u16 defSlot : 3;
    u16 unk0_12 : 4;
    u8 pad[0x20];
};
#define BTH (*(struct BattleH *)&gBattle)
struct BattleFull {
    u16 unk0_0 : 6;
    u16 atkSlot : 3;
    u16 defSlot : 3;
    u16 unk0_12 : 4;
    u8 pad[0x148 - 2];
    u16 lp[2];
    u16 unk4[2];
    u8 pad2[0x20];
};
#define BF (*(struct BattleFull *)&gBattle)
#define BT_U16(off) (*(u16 *)((u8 *)&gBattle + (off)))
struct DuelZone {
    u32 card;
    u16 unk4;
    u8 pad[0x94 - 6];
};
struct PlayerState {
    u8 unk0[0x28];
    struct DuelZone zones[11];
    u8 filler[0xD64 - 0x28 - 11 * 0x94];
};
extern struct PlayerState gDuelPlayers[2];
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)&gDuelPlayers[0].zones[0]))
struct DGCnt { u16 f0 : 1; u16 cnt : 8; u16 rest : 7; u8 pad[0x20]; };
struct DuelGlobal {
    u8 unk0[0x1B16];
    u16 f1B16_0 : 1;
    u16 cnt : 8;
    u16 f1B16_9 : 7;
};
extern struct DuelGlobal gDuel;
int CountMonsters(int player);
int FindFaceUpCardOnField(int player, u16 number, int a);
void Chain_AddPending(u32 card, u32 b);
int Battle_CheckReplay(int player);
void EventResponse_Request(int player, int a, u32 word);
extern const u16 gCardNumberToId[];
int CountActiveCardsOnField(int player, u16 number);
void ShowCardEffect(int player, u16 id);
void SendTopDeckCardsToGraveyard(int player, int n, int a);
void LoseLifePoints(int player, int lp);

/* Views relative to the zone base symbol 0x0201930C (as the ROM does in 0x0804B640). */
extern u8 gDuelZones[];
#define ZBB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))
#define ZBB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))
#define CNTB (*(struct DGCnt *)(gDuelZones + 0x1AEA))
struct DGWord {
    u32 unk0 : 9;
    u32 stage : 8;
    u32 unk17 : 15;
};
#define STAGEB (*(struct DGWord *)(gDuelZones + 0x1AE8))
struct Cursor { u8 unk0[0x824]; u32 w824; u32 w828; u32 w82C; };
extern struct Cursor gDuelScreen;
struct Unk0201AE60 { u8 unk0[0x14]; u16 h14; u8 pad[0x20]; };
extern struct Unk0201AE60 gTextBox;
extern const u16 gCardIdToNumber[];
#define TBL(id) (*(u16 *)((u8 *)gCardIdToNumber + (((id) & 0x7FF) << 1)))
extern const u8 gStrSelectTributeToAttack[], gStrCoinTossCall[];
int CanMonsterAttack(int player, int zone, u16 flag);
void DuelCmd_Push(u16 msg, u16 a, u16 b, u16 c);
void MarkMonsterAttacked(int player, int zone);
int AiPickTributeMonster(int a, int b);
int TributeMonster(int player, int column);
void TextBoxOpen(u32 a, u32 b, u32 c, const void *d);
void TextBoxSetMenu(u32 a, u32 b, u32 c);
u32 DuelCursor_PickTarget(u32 keys);
void PlaySE(u16 se);
int Random(void);
int HalveRoundDown(u16 v);

static inline u16 BattleCardNumber(u16 id)
{
    return ((const u16 *)0x08622AB4)[id & 0x7FF];
}
static inline u16 SnapshotZoneValue(int player, int zone)
{
    int p = player & 1;
    int off = zone * 0x94 + p * 0xD64;
    u8 *base = gDuelZones;
    return ((struct DuelZone *)(off + (u32)base))->unk4;
}
/* The stage crosses a halfword boundary, but its value has a u16 container. */
struct BattleDuelStage {
    u8 prefix[0x1B14];
    u32 lo : 9;
    u16 stage : 8;
    u32 hi : 15;
    u8 pad[8];
};
#define STAGEE (*(struct BattleDuelStage *)&gDuel)
int BattleStage_DeclareAttack(int player)
{
    u32 pl = player & 1;
    struct DGCnt *c;
    u16 id;
    id = ZBB2(pl, BTH.atkSlot)->card << 20 >> 20;
    c = &CNTB;
    switch (c->cnt) {
    case 0:
        if (*(u8 *)&gBattle & 8)
            return 1;
        if ((u16)CanMonsterAttack(player, BTH.atkSlot, 1) == 0) {
            DuelCmd_Push(player ? 0x8035 : 0x35, BTH.atkSlot, 1, 0);
            STAGEB.stage = 1;
            return 0;
        } else {
            int i;
            for (i = 0; i < 2; i++) {
                u32 slot;
                if (i == player)
                    slot = BTH.atkSlot;
                else
                    slot = BTH.defSlot;
                BF.lp[i] = CountMonsters(i);
                BF.unk4[i] = SnapshotZoneValue(i, slot);
            }
            gBattle.f3 = 1;
            switch (BattleCardNumber(id)) {
            case 0x2D6:
            case 0x2D7:
            case 0x2D8:
            case 0x2FE:
                DuelCmd_Push(player ? 0x8043 : 0x43, 500, 1, 0);
                return 1;
            case 0x226:
                DuelCmd_Push(player ? 0x8043 : 0x43, 1000, 1, 0);
                return 1;
            case 0x2E8:
            case 0x2F9:
                if (player != 0) {
                    int r = AiPickTributeMonster(BTH.atkSlot, 0);
                    if (r >= 0) {
                        TributeMonster(player, r);
                        return 1;
                    }
                    MarkMonsterAttacked(player, BTH.atkSlot);
                    STAGEE.stage = STAGEE.stage + 8;
                    gDuel.cnt = 0;
                    return 0;
                } else {
                    TextBoxOpen(0x204, 0x715, 0xB, gStrSelectTributeToAttack);
                    gDuel.cnt = 100;
                    return 0;
                }
            case 0x16E:
                TextBoxOpen(0x206, 0x613, 0xB, gStrCoinTossCall);
                TextBoxSetMenu(2, 0, 0);
                CNTB.cnt = 200;
                return 0;
            }
            return 1;
        }
    case 100:
        if (DuelCursor_PickTarget(0xF0) != 0) {
            if (BTH.atkSlot != gDuelScreen.w82C) {
                DuelCmd_Push(player ? 0x8008 : 8, player, (u8)gDuelScreen.w828 | (u8)gDuelScreen.w82C << 8, 0);
                TributeMonster(player, gDuelScreen.w82C);
                c->cnt++;
            } else {
                PlaySE(3);
            }
        }
        return 0;
    case 101:
        if ((u16)Battle_CheckReplay(player) == 0)
            return 1;
        if ((u16)CanMonsterAttack(player, BTH.atkSlot, 0) == 0)
            STAGEB.stage = 1;
        else
            STAGEB.stage = 2;
        return 0;
    case 200: {
        u16 x = Random() & 1;
        DuelCmd_Push(player ? 0x80E0 : 0xE0, gTextBox.h14, x, 0);
        DuelCmd_Push(player ? 0x8012 : 0x12, 0, 0, 0);
        if (x != gTextBox.h14)
            DuelCmd_Push(player ? 0x8043 : 0x43, HalveRoundDown(*(u16 *)(gDuelZones - 0x28 + pl * 0xD64)), 1, 0);
        return 1;
    }
    default:
        return 1;
    }
}
int BattleStage_PayAttackCosts(int player)
{
    if (!(*(u8 *)&gBattle & 0x10)) {
        u16 no;
        int n;
        gBattle.f4 = 1;
        n = CountActiveCardsOnField(1 - player, no = 0x427);
        if (n > 0) {
            ShowCardEffect(player, ((const u16 *)0x08623DF4)[no]);
            SendTopDeckCardsToGraveyard(player, n, 1);
        }
        no = 0x42A;
        n = CountActiveCardsOnField(0, no);
        n += CountActiveCardsOnField(1, no);
        if (n > 0) {
            ShowCardEffect(player, ((const u16 *)0x08623DF4)[no]);
            LoseLifePoints(player, n * 500);
        }
    }
    return 1;
}
static inline u32 BattleCardEvent(int player, int zone, u16 id, u32 kind)
{
    return ((u32)(player & 1) << 31) | (((zone & 0x1F) << 16) | (kind << 20)) | id;
}
int BattleStage_RespondToAttack(int player)
{
    switch (gDuel.cnt) {
    case 0: {
        int i;
        for (i = 0; i < 2; i++) {
            u32 slot;
            if (i == player)
                slot = BTH.atkSlot;
            else
                slot = BTH.defSlot;
            BF.lp[i] = CountMonsters(i);
            BF.unk4[i] = SnapshotZoneValue(i, slot);
        }
        CNTB.cnt++;
        return 0;
    }
    case 1: {
        int opp = 1 - player;
        u16 no = 0x590;
        if (CountActiveCardsOnField(opp, no) != 0) {
            int x = FindFaceUpCardOnField(opp, no, -1);
            Chain_AddPending(BattleCardEvent(opp, x, ((const u16 *)0x08623DF4)[no], 0x202), 0);
            gDuel.cnt = 4;
        } else {
            gDuel.cnt++;
        }
        return 0;
    }
    case 2:
        if ((u16)Battle_CheckReplay(player) != 0)
            return 0;
        gDuel.cnt++;
        return 0;
    case 3:
        EventResponse_Request(1 - player, 0x10,
                     ((u8)player | BTH.atkSlot << 8) | ((u8)(1 - player) | BTH.defSlot << 8) << 16);
        gDuel.cnt++;
        return 0;
    case 4:
        if ((u16)Battle_CheckReplay(player) != 0)
            return 0;
        return 1;
    default:
        return 1;
    }
}

/* Defender reveal / flip-effect battle step. The public argument remains int. */
struct DefenderBattleView {
    u8 flags0 : 2;
    u8 revealEffect : 1;
    u8 flags3 : 5;
    u8 bit8 : 1;
    u8 defender : 3;
    u8 flags12 : 4;
    u16 cardId;
    u8 pad4[4];
    u8 flags8lo : 3;
    u8 flags8bit3 : 1;
    u8 flags8hi : 4;
    u8 pad9[9];
    u16 damageA;
    u8 flags14lo : 3;
    u8 flags14bit3 : 1;
    u8 flags14hi : 4;
    u8 pad15[9];
    u16 damageB;
    u8 tail[4];
};
#define DB (*(struct DefenderBattleView *)&gBattle)
extern struct DGCnt gUnk_0201ADF6;
struct DefenderStageView {
    u8 prefix[0x1AE8];
    u32 lo : 9;
    u16 stage : 8;
    u32 hi : 15;
    u8 tail[4];
};
#define DEF_STAGE (*(struct DefenderStageView *)gDuelZones)
u32 HasFlipEffect(u16 number, u16 flag);
int FindFreeSpellTrapZone(int player);
int CanPlaceSpellTrapCard(int player, u16 id);
void FlipFieldCard(int player, int zone, u16 arg);
void ShowRevealedCard(int player, u16 id);
void TriggerMysteriousPuppeteer(int player);
void sub_080197C0(int player, u16 id);
void MoveFieldCard(int player, u16 from, u16 to);
void EquipCard(int player, u16 from, u16 to);
void CalcBattle(int player, u16 noAtk);
static inline u16 DefenderCardNumber(u16 id)
{
    return ((const u16 *)0x08622AB4)[id & 0x7FF];
}
static inline struct DuelZone *DefenderZone(int player)
{
    int p = player & 1;
    int zone = BTH.defSlot;
    int off = zone * 0x94 + p * 0xD64;
    u8 *base = gDuelZones;
    return (struct DuelZone *)(off + (u32)base);
}
#define DEF_ZONE_FLAGS(p) (((u8 *)DefenderZone(p))[6])
#define DEF_ZONE_ID(p) ((DefenderZone(p)->card << 20) >> 20)

int BattleStage_RevealDefender(int player)
{
    /* FAKEMATCH: keep the duel base in ip through the opening state dispatch. */
    register u8 *e asm("r12");
    if (*(u8 *)&gBattle & 2)
        return 1;
    e = (u8 *)&gDuel;
    switch (gUnk_0201ADF6.cnt) {
    case 0: {
        int opp = 1 - player;
        if (!(DEF_ZONE_FLAGS(opp) & 2)) {
            /* FAKEMATCH: retain the initialized base until the defender slot is spilled. */
            asm("" : : "r"(e));
            FlipFieldCard(opp, BTH.defSlot, 0);
            ShowRevealedCard(player, DEF_ZONE_ID(opp));
            TriggerMysteriousPuppeteer(opp);
        } else {
            DB.cardId = 0;
            gUnk_0201ADF6.cnt++;
        }
        gDuel.cnt++;
        return 0;
    }
    case 1: {
        u16 id;
        u16 number;
        int pl = (1 - player) & 1;
        int off = BTH.defSlot * 0x94 + pl * 0xD64;
        u8 *base = e + 0x2C;
        id = (((struct DuelZone *)(off + (u32)base))->card << 20) >> 20;
        DB.cardId = id;
        DB.revealEffect = HasFlipEffect(DefenderCardNumber(id), 1);
        if (CountActiveCardsOnField(0, 0x5FA) != 0 || CountActiveCardsOnField(1, 0x5FA) != 0)
            DB.revealEffect = 0;
        number = DefenderCardNumber(DB.cardId);
        if (number == 0x2DF || number == 0x492) {
            int opp = 1 - player;
            int absent;
            if ((DEF_ZONE_FLAGS(opp) & 1)
                && CountActiveCardsOnField(0, 0x5FA) == 0
                && (absent = CountActiveCardsOnField(1, 0x5FA)) == 0
                && CanPlaceSpellTrapCard(opp, DEF_ZONE_ID(opp)) != 0) {
                int zone = FindFreeSpellTrapZone(opp);
                u16 source, destination;
                sub_080197C0(player, DB.cardId);
                source = (u8)(1 - player) | BTH.defSlot << 8;
                destination = (u8)(1 - player) | (u8)zone << 8;
                MoveFieldCard(opp, source, destination);
                EquipCard(opp, destination, (u8)player | BTH.atkSlot << 8);
                DuelCmd_Push(player ? 0x8035 : 0x35, BTH.atkSlot, 1, 0);
                CalcBattle(player, 0);
                DB.flags8bit3 = 0;
                DB.flags14bit3 = 0;
                DB.damageA = absent;
                DB.damageB = absent;
                DB.revealEffect = 0;
                DEF_STAGE.stage += 4;
                CNTB.cnt = 0;
                return 0;
            }
            DB.revealEffect = 0;
        }
        gDuel.cnt++;
        return 0;
    }
    case 2:
        EventResponse_Request(1 - player, 0x11,
                     ((u8)player | BTH.atkSlot << 8) | ((u8)(1 - player) | BTH.defSlot << 8) << 16);
        gUnk_0201ADF6.cnt++;
        return 0;
    default:
        return 1;
    }
}
/* Battle damage presentation and defender responses (original int ABI). */
extern u8 gDuelCtrl[];
extern u8 gLinkState[];
extern const char gStrAskZeroAttackerAtkFmt[], gStrAskSubstituteTargetFmt[], gCardNames[];
extern const u16 gUnk_086247AA;
int CountFreeMonsterZones(int player);
int FindFreeMonsterZone(int player);
void QueueAddZoneLink(int player, u16 id, u16 pos, u16 kind);
void FlipFieldCard(int player, int zone, u16 arg);
void ShowRevealedCard(int player, u16 id);
void TriggerMysteriousPuppeteer(int player);
void sub_080197C0(int player, u16 id);
void MoveFieldCard(int player, u16 from, u16 to);
void CalcBattle(int player, u16 noAtk);
void DuelPrompt_Post(int player, int kind, u16 arg, u16 value);
u16 DuelLink_SendMessage(u16 command, u16 a, u16 b, u16 c);
void FormatStr(char *dst, const char *fmt, const char *arg);
u32 HasFlipEffect(u16 number, u16 flag);
struct FinishReply { u8 prefix[0x450]; u8 lo : 1; u8 ready : 1; u8 hi : 6; };
struct FinishSide {
    u8 zone : 3;
    u8 destroyed : 1;
    u8 marked : 1;
    u8 reserved : 3;
    u8 pad1;
    u16 cardId;
    u8 pad4[4];
    u16 power;
    u16 damage;
};
struct FinishBattle {
    u8 flags0 : 2;
    u8 reveal : 1;
    u8 flags3 : 5;
    u8 bit8 : 1;
    u8 defender : 3;
    u8 flags12 : 4;
    u16 cardId;
    u8 pad4[4];
    struct FinishSide side[2];
};
#define FINISH_BATTLE (*(struct FinishBattle *)&gBattle)
struct FinishStage {
    u8 prefix[0x1B14];
    u32 lo : 9;
    u16 stage : 8;
    u32 hi : 15;
};
#define FINISH_STEP(e) (((struct DuelGlobal *)(e))->cnt)
#define FINISH_PICK(e) (*((u8 *)(e) + 0x1B64))
#define FINISH_STAGE(e) (((struct FinishStage *)(e))->stage)
static inline u16 FinishCardNumber(u16 id)
{
    return ((const u16 *)0x08622AB4)[id & 0x7FF];
}
static inline struct DuelZone *FinishAttackerZone(int player)
{
    int p = player & 1;
    int zone = BTH.atkSlot;
    int off = zone * 0x94 + p * 0xD64;
    u8 *base = gDuelZones;
    return (struct DuelZone *)(off + (u32)base);
}
static inline struct DuelZone *FinishDefenderZone(int player)
{
    int p = player & 1;
    int zone = BTH.defSlot;
    int off = zone * 0x94 + p * 0xD64;
    u8 *base = gDuelZones;
    return (struct DuelZone *)(off + (u32)base);
}
#define FINISH_FLAGS6(z) (((u8 *)(z))[6])
#define FINISH_FLAGS7(z) (((u8 *)(z))[7])
#define FINISH_ID(z) (((z)->card << 20) >> 20)

static inline u16 FinishPosition(u8 first, u8 second)
{
    return first | second << 8;
}
static inline u32 FinishCardNumberWide(u32 id)
{
    return ((const u16 *)0x08622AB4)[id & 0x7FF];
}
static inline u16 FinishDestination(u8 player, u8 low, u8 high)
{
    int lower = low >> 1;
    int upper = (high & 1) << 7;
    return player | (upper | lower) << 8;
}
struct FinishHigh {
    u8 prefix[0x1B18];
    u8 value : 1;
};
int BattleStage_DamageCalc(int player)
{
    char revealText[256];
    char changeText[256];
    u8 *e;
    u32 state = gDuel.cnt;
    e = (u8 *)&gDuel;
    switch (state) {
    case 0: {
        int handled = 0;
        int i;
        int opp;
        switch (FinishCardNumber(FINISH_ID(FinishAttackerZone(player)))) {
        case 0x538:
            if (FINISH_FLAGS7(FinishAttackerZone(player)) & 0x20) {
                DuelCmd_Push(player ? 0x8092 : 0x92, BTH.atkSlot, 0, 0);
                handled = 1;
            }
            break;
        case 0x540:
            DuelCmd_Push(player ? 0x8092 : 0x92, BTH.atkSlot, 0, 0);
            break;
        }
        if (!handled)
            DuelCmd_Push(player ? 0x8035 : 0x35, BTH.atkSlot, 1, 0);
        i = 5;
        opp = 1 - player;
        for (; i <= 9; i++) {
            u8 *zone = (u8 *)ZBB(opp & 1, i);
            u16 id = (*(u32 *)zone << 20) >> 20;
            if (id != 0 && (zone[6] & 2) && !(zone[0x91] & 8)
                && FinishCardNumberWide(id) == 0x44B)
                QueueAddZoneLink(opp, FinishPosition(opp, i),
                             (u8)player | BTH.atkSlot << 8, 2);
        }
        gDuel.cnt++;
        return 0;
    }
    case 1: {
        int opp;
        u16 number;
        CalcBattle(player, 0);
        if (!(*(u8 *)&gBattle & 2)) {
            opp = 1 - player;
            if (FINISH_FLAGS6(FinishDefenderZone(opp)) & 2) {
                {
                    u16 id = FINISH_ID(FinishDefenderZone(opp));
                    number = FinishCardNumberWide(id);
                }
                switch (number) {
                case 0x172:
                case 0x173:
                case 0x174:
                    if (FINISH_FLAGS7(FinishDefenderZone(opp)) & 0x20) {
                        CNTB.cnt = 10;
                        return 0;
                    }
                    break;
                case 0x4DB:
                    if (FinishCardNumber(FINISH_BATTLE.cardId) == number
                        && CountFreeMonsterZones(opp) > 0 && CountMonsters(player) > 1) {
                        CNTB.cnt = 30;
                        return 0;
                    }
                    break;
                case 0x5F2:
                    if (CountMonsters(opp) > 1) {
                        CNTB.cnt = 20;
                        return 0;
                    }
                    break;
                }
            }
        }
        gDuel.cnt++;
        /* The normal path immediately performs step 2 as well. */
    }
    case 2:
        DuelCmd_Push(player ? 0x8030 : 0x30, FINISH_BATTLE.side[0].cardId, FINISH_BATTLE.side[1].cardId, 0);
        gDuel.cnt++;
        return 0;
    case 3: {
        u16 message = 0x31;
        if (player)
            message = 0x8031;
        { u16 attack = FINISH_BATTLE.side[0].power;
        u16 defense = FINISH_BATTLE.side[1].power;
        u8 *battle = (u8 *)&gBattle;
        int defender = FINISH_BATTLE.side[1].marked | FINISH_BATTLE.side[1].destroyed << 1;
        int attacker = FINISH_BATTLE.side[0].marked | FINISH_BATTLE.side[0].destroyed << 1;
        int combined; u16 packed;
        if (FINISH_BATTLE.side[0].damage != 0)
            combined = 4 | attacker;
        else
            combined = attacker;
        if (FINISH_BATTLE.side[1].damage != 0)
            packed = combined | ((defender | 4) << 8);
        else
            packed = combined | (defender << 8);
        DuelCmd_Push(message, attack, defense, packed);
        gDuel.cnt++;
        return 0;
    }
    }
    case 4: {
        int i = 0;
        u8 *side = (u8 *)&gBattle;
        for (; i <= 1; side += 12, i++) {
            u8 flags = side[8];
            if ((s32)((u32)flags << 28) < 0)
                DuelCmd_Push(i ? 0x8078 : 0x78, ((struct FinishSide *)(side + 8))->zone, 0, 0);
        }
        DuelCmd_Push(player ? 0x8012 : 0x12, 0, 0, 0);
        return 1;
    }
    case 10:
        if (player != 0) {
            FormatStr(revealText, gStrAskZeroAttackerAtkFmt,
                          ((const char (*)[64])0x0822C720)[FINISH_ID(FinishDefenderZone(1 - player))]);
            TextBoxOpen(0x204, 0xB16, 11, revealText);
            TextBoxSetMenu(1, 0, 0);
            CNTB.cnt++;
        } else if (!(gDuelCtrl[1] & 1)) {
            gTextBox.h14 = 1;
            gDuel.cnt++;
        } else {
            DuelLink_SendMessage(0xF057, FINISH_ID(FinishDefenderZone(1)), 0, 0);
            ((struct FinishReply *)gLinkState)->ready = 0;
        }
        gDuel.cnt++;
        return 0;
    case 11:
        if ((s32)((u32)gLinkState[0x450] << 30) < 0) {
            gTextBox.h14 = *(u16 *)(gLinkState + 0x45A);
            FINISH_STEP(e)++;
        }
        return 0;
    case 12:
        if (gTextBox.h14 != 0) {
            sub_080197C0(1 - player, FINISH_ID(FinishDefenderZone(1 - player)));
            DuelCmd_Push(player != 1 ? 0x8092 : 0x92, BTH.defSlot, 0, 0);
            DuelCmd_Push(player ? 0x803A : 0x3A, 0, 0, 0);
            CalcBattle(player, 1);
        }
        gDuel.cnt = 2;
        return 0;
    case 20:
        DuelPrompt_Post(1 - player, 0x14, BTH.defSlot, 0);
        gDuel.cnt++;
        return 0;
    case 21:
        DuelCmd_Push(player != 1 ? 0x8008 : 8, (u16)(1 - player), FINISH_PICK(e) << 8, 0);
        DuelCmd_Push(player != 1 ? 0x8038 : 0x38, (u8)(1 - player) | FINISH_PICK(e) << 8, 0, 0);
        FINISH_STAGE(e)--;
        FINISH_STEP(e) = 0;
        return 0;
    case 30:
        if (player != 0) {
            FormatStr(changeText, gStrAskSubstituteTargetFmt,
                          ((const char (*)[64])0x0822C720)[FINISH_ID(FinishDefenderZone(1 - player))]);
            TextBoxOpen(0x206, 0x713, 11, changeText);
            TextBoxSetMenu(1, 0, 0);
            CNTB.cnt++;
        } else if (!(gDuelCtrl[1] & 1)) {
            gTextBox.h14 = 1;
            gDuel.cnt++;
        } else {
            DuelLink_SendMessage(0xF057, FINISH_ID(FinishDefenderZone(1)), 0, 0);
            ((struct FinishReply *)gLinkState)->ready = 0;
        }
        gDuel.cnt++;
        return 0;
    case 31:
        if ((s32)((u32)gLinkState[0x450] << 30) < 0) {
            gTextBox.h14 = *(u16 *)(gLinkState + 0x45A);
            FINISH_STEP(e)++;
        }
        return 0;
    case 32:
        if (gTextBox.h14 == 0) {
            FINISH_STEP(e) = 2;
            return 0;
        }
        DuelPrompt_Post(1 - player, 0x10, BTH.atkSlot, 0);
        gDuel.cnt++;
        return 0;
    case 33: {
        u16 zone;
        u32 freshOne;
        u32 low;
        u16 lowerZone;
        /* FAKEMATCH: reuse r1 for the mask and loaded byte, leaving the zone in r2. */
        register u32 mask asm("r1");
        u8 *lowPtr;
        DuelCmd_Push(player ? 0x8008 : 8, (u16)player, FINISH_PICK(e) << 8, 0);
        zone = FindFreeMonsterZone(1 - player);
        /* FAKEMATCH: preserve the initialized halfword before splitting its bits. */
        asm("" : : "r"(zone));
        /* FAKEMATCH: stage the low-byte merge to preserve the original register
         * lifetimes; cache that byte for the packed destination. */
        mask = 127;
        lowerZone = zone & mask;
        lowPtr = e + 0x1B17;
        lowerZone <<= 1;
        low = 1;
        mask = *lowPtr;
        low &= mask;
        low |= lowerZone;
        *lowPtr = low;
        /* FAKEMATCH: the tied input initializes freshOne to 1; consuming the
         * high bits here schedules that constant before the high-byte pointer. */
        asm("" : "=r"(freshOne) : "0"(1), "r"(zone >> 7));
        ((struct FinishHigh *)e)->value = zone >> 7;
        MoveFieldCard(1 - player, (u8)player | FINISH_PICK(e) << 8,
                     FinishDestination(freshOne - player, low, e[0x1B18]));
        {
            struct FinishBattle *battle = (struct FinishBattle *)&gBattle;
            int low = e[0x1B17] >> 1;
            battle->defender = ((e[0x1B18] & 1) << 7) | low;
        }
        FINISH_STEP(e)++;
        return 0;
    }
    case 34: {
        int opp = 1 - player;
        if (!(FINISH_FLAGS6(FinishDefenderZone(opp)) & 2)) {
            FlipFieldCard(opp, BTH.defSlot, 0);
            ShowRevealedCard(player, FINISH_ID(FinishDefenderZone(opp)));
            TriggerMysteriousPuppeteer(opp);
        }
        CNTB.cnt++;
        return 0;
    }
    case 35: {
        struct FinishBattle *battle = (struct FinishBattle *)&gBattle;
        u16 id = FINISH_ID(FinishDefenderZone(1 - player));
        battle->cardId = id;
        battle->reveal = HasFlipEffect(FinishCardNumber(id), 1);
        if (CountActiveCardsOnField(0, 0x5FA) != 0 || CountActiveCardsOnField(1, 0x5FA) != 0)
            battle->reveal = 0;
        QueueAddZoneLink(1 - player, gUnk_086247AA, (u8)(1 - player) | BTH.defSlot << 8, 3);
        CalcBattle(player, 0);
        gDuel.cnt = 2;
        return 0;
    }
    default:
        return 1;
    }
}
