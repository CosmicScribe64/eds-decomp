#ifndef GUARD_CHAIN_H
#define GUARD_CHAIN_H

/*
 * The chain: card activations, the responses to them, and their resolution.
 *
 * Activations and triggers are queued with Chain_AddPending. Every frame DuelMainStep calls Chain_Update,
 * which copies gChain.pending into gChain.links and starts Chain_Build. Chain_Build runs each link's
 * chainA (cost) and chainB (target) handlers from gCardEffects (include/effect.h) and asks both players
 * whether they respond (Chain_AskResponse); a response is appended to links, and the build goes on from it.
 * When nobody responds, Chain_Resolve resolves the links from the last one back, calling each card's resolve
 * handler with gChain.effectStep as its step.
 *
 * Event responses: after a game event (summon, attack, damage, ...) EventResponse_Request opens a response
 * window (gChain.responseEvent, enum ResponseEventKind in constants/duel.h), and EventResponse_Run asks each
 * player in turn whether to activate a Quick-Play Magic or a Trap.
 *
 * Link duels: a link owned by the partner (player 1) runs its handlers on the partner's GBA
 * (Chain_IsPartnerEntry); the messages are in include/duel_link.h.
 *
 * Every prototype is the function's definition as compiled. Units that call a function through another
 * local declaration (other widths or argument counts) keep that view as a commented local alias prototype
 * (build/readability/proto_mismatches.txt).
 */

#include "global.h"
#include "duel.h"

/* gChain.scratch.deckReorder.mode: steps of Big Eye's top-of-deck reorder (DeckReorder_Run, duel_prompt.h). */
enum DeckReorderMode {
    DECK_REORDER_INIT = 0,              /* open the prompt (human) */
    DECK_REORDER_SELECT = 1,            /* pick a card and move it left or right */
    DECK_REORDER_SWAP_LEFT = 10,        /* animate a swap with the left neighbour */
    DECK_REORDER_DONE = 15,             /* confirmed with A */
    DECK_REORDER_SWAP_RIGHT = 20        /* animate a swap with the right neighbour */
};

/* gChain.askStep in EventResponse_Run (the case values are decimal). */
enum EventResponseStep {
    EVRESP_START = 0,                   /* fill gEventResponseEntry; skip the player if nothing can respond */
    EVRESP_ASK = 1,                     /* human: Yes/No box; CPU and link partner branch off */
    EVRESP_WAIT_ANSWER = 2,             /* No -> EVRESP_NEXT_PLAYER */
    EVRESP_PICK_CARD = 10,              /* cursor pick and card menu (EventResponse_GetCommands) */
    EVRESP_ACTIVATE_PICKED = 11,        /* activate the chosen card */
    EVRESP_LINK_QUERY = 100,            /* send LINKMSG_ACTIVATE_QUERY to the partner */
    EVRESP_LINK_WAIT = 101,             /* wait for the partner's reply */
    EVRESP_CPU_SEARCH = 200,            /* CPU looks for a spell/trap zone card to activate */
    EVRESP_CPU_ACTIVATE = 201,          /* CPU flips and queues it */
    EVRESP_NEXT_PLAYER = 240            /* switch askPlayer; done when back at firstAskPlayer */
};

/* gChain.queryStep in DuelLink_AnswerActivateQuery (include/duel_link.h); 6 and above send the answer. */
enum LinkQueryStep {
    LINKQUERY_ASK = 0,
    LINKQUERY_WAIT_ANSWER = 1,
    LINKQUERY_PICK_CARD = 2,
    LINKQUERY_SETUP_HANDLERS = 3,       /* look up the chosen card's chainA/chainB */
    LINKQUERY_RUN_CHAIN_A = 4,
    LINKQUERY_RUN_CHAIN_B = 5,
    LINKQUERY_SEND_RESULT = 6
};

/*
 * One chain entry (0x14 bytes): a link of the chain, a pending activation or trigger, or the game event of a
 * response window. Effect handlers (include/effect.h) get the link they run for as their first argument and
 * the link it responds to (chainedTo) as the second. Chain_Add builds an entry from two words:
 *   packed = card | zone << 16 | kind << 21 | event << 25 | player << 31,   locs = loc0 | loc1 << 16.
 * Many matched handlers read the player bit as the raw byte `((u8 *)entry)[2] & 1`.
 * Bitfield containers: player and kind are u8, zone and event u16, which is how the 26 effect/response units
 * declare them (the container changes the loads agbcc emits). duel_main and duel_setup access an all-u16 view
 * (u16 player:1; u16 kind:3) and keep it as a local view.
 */
struct ChainEntry {
    u16 card;                   /* +0x00: card ID (card number: gCardIdToNumber[card & 0x7FF]) */
    u8 player:1;                /* +0x02 bit 0: activating player (1 = the CPU or the link partner) */
    u8 kind:3;                  /* +0x02 bits 1-3: enum ChainEntryKind (constants/duel.h) */
    u16 zone:6;                 /* +0x02 bits 4-9: zone of the activated card */
    u16 event:6;                /* +0x02 bits 10-15: enum ResponseEventKind of the triggering event */
    u8 skipChainA:1;            /* +0x04 bit 0: do not run chainA (set on entries from the link partner) */
    u8 skipChainB:1;            /* +0x04 bit 1: do not run chainB */
    u8 negated:1;               /* +0x04 bit 2: activation negated; most handlers return at once */
    u8 destroyIfNegated:1;      /* +0x04 bit 3: a card that would stay on the field goes to the graveyard */
    u8 flag4_4:1;               /* +0x04 bit 4: cleared by Chain_Add; meaning unknown */
    u8 unk4_5:3;
    u8 unk5;                    /* +0x05 */
    u16 loc0;                   /* +0x06: event argument, low half: player | zone << 8 of the card concerned
                                 * (events 8, 16, 19-21, 27, 30), the acting player (26, 29), or the damage (13-15) */
    u16 loc1;                   /* +0x08: event argument, high half: the attacked monster (player | zone << 8),
                                 * or the 'you'/'opponent' selector of events 13-15, 26 and 29 */
    u8 numTargets:3;            /* +0x0A bits 0-2: number of chosen targets */
    u8 unkA_3:5;
    u8 unkB;                    /* +0x0B */
    u16 targets[3];             /* +0x0C: chosen targets: player | zone << 8; Graverobber, Call of the Haunted and
                                 * Premature Burial store a card word (targets[0] low half, targets[1] high half) */
    u16 unk12;                  /* +0x12 */
};

/* A list of up to 16 chain links and its count: the layout of gChain.links / linkCount and of the
 * partner's chain in gLinkState.rxChainList / rxChainListCount (include/duel_link.h). */
struct ChainList {
    struct ChainEntry entries[16];  /* +0x000 */
    u16 count;                      /* +0x140: valid entries */
};

/*
 * Big Eye's top-of-deck reorder (DeckReorder_* in duel_prompt.h), in gChain.scratch.deckReorder. The first
 * eight bytes are one bitfield group; timer and unk36 cross byte boundaries, which agbcc allows (it packs
 * bitfields without moving them to the next container). The cards are written back to the deck in order.
 */
struct DeckReorderState {
    u32 cursor:8;               /* +0x0 bits 0-7: selected card 0-4 */
    u32 unk8:4;                 /* bits 8-11 */
    u32 mode:8;                 /* bits 12-19: enum DeckReorderMode */
    u32 phase:8;                /* bits 20-27: swap animation phase 0-16 (index into gCardJumpArc) */
    u32 timer:8;                /* bits 28-35: CPU think timer, 30 frames per step (+0x3 high nibble, +0x4 low) */
    u32 unk36:8;                /* bits 36-43: cleared by Big Eye */
    u32 unk44:20;               /* bits 44-63 */
    struct DuelCard cards[5];   /* +0x08: the five top deck cards being reordered */
};

/*
 * gChain (0x02017A40, 0x56C bytes, cleared by Duel_Setup): the pending activations, the chain, the state of
 * Chain_Build / Chain_Resolve and of the response windows, and the scratch bytes of the effect handlers.
 * A handler's step byte belongs to the stage it runs in: costStep (chainA), targetStep (chainB) and
 * effectStep (resolve); the scratch bytes after them are shared by many handlers with per-card meanings.
 */
struct ChainState {
    struct ChainEntry pending[32];      /* +0x000: activations and triggers waiting to start a new chain */
    struct ChainEntry links[16];        /* +0x280: the chain, resolved from the last link back */
    u16 linkCount;                      /* +0x3C0: entries in links */
    u16 unk3C2;
    u16 pendingCount;                   /* +0x3C4: entries in pending */
    u8 unk3C6[0x3D0 - 0x3C6];
    u8 building:1;                      /* +0x3D0 bit 0: Chain_Build is running */
    u8 buildStep:7;                     /* +0x3D0 bits 1-7: step of Chain_Build */
    u8 buildIndex;                      /* +0x3D1: link Chain_Build is setting up */
    u8 resolving:1;                     /* +0x3D2 bit 0: Chain_Resolve is running */
    u8 resolveStep:7;                   /* +0x3D2 bits 1-7: stage of Chain_Resolve */
    u8 unk3D3;                          /* +0x3D3: cleared when building ends */
    u16 unk3D4;
    s16 effectIndex;                    /* +0x3D6: gCardEffects row of the resolving link, -1 if none */
    /* +0x3D8: resolve handler of the resolving link, called with (last link, previous link or NULL) */
    u32 (*resolve)(struct ChainEntry *link, struct ChainEntry *chainedTo);
    struct DuelCard savedCard;          /* +0x3DC: card word of the resolving link's zone, saved before the
                                         * zone is cleared (alias gUnk_02017E1C, unused) */
    u8 effectStep;                      /* +0x3E0: step of the running resolve handler (enum EffectStep);
                                         * Chain_Resolve starts it at 0x80 (also the symbol gChainEffectWork) */
    u8 effectSubStep;                   /* +0x3E1: resolve scratch: cards or discards left, side being swept,
                                         * index into scratch.effect.effectCards */
    u8 effectCounter;                   /* +0x3E2: resolve scratch counter (the roulette's current pick) */
    u8 unk3E3;
    u8 costStep;                        /* +0x3E4: step of the running chainA (cost) handler */
    u8 targetStep;                      /* +0x3E5: step of the running chainB (target) handler */
    u8 targetWork;                      /* +0x3E6: chainB scratch; also the step of the Deck Edit popup that
                                         * EffectProhibitionChainB runs */
    u8 targetWork2;                     /* +0x3E7: second chainB scratch byte */
    struct DuelCard effectCard;         /* +0x3E8: card word kept between handler steps (the ritual monster,
                                         * Parasite Paracide taken from the deck) */
    struct DuelZone effectSavedZone;    /* +0x3EC: copy of a whole zone (Magical Hats' target) */
    /* +0x480 / +0x484: cost and target handlers of the link Chain_Build is setting up */
    u16 (*chainA)(struct ChainEntry *link, struct ChainEntry *chainedTo);
    u16 (*chainB)(struct ChainEntry *link, struct ChainEntry *chainedTo);
    u8 askScreenOpened:1;               /* +0x488 bit 0: Chain_AskResponse queued 'open the duel screen' */
    u8 unk488_1:7;
    u8 unk489;
    u16 responseEvent;                  /* +0x48A: enum ResponseEventKind of the open response window */
    u32 responseEventArg;               /* +0x48C: its argument: loc0 | loc1 << 16 of responseEntry */
    u8 askStep;                         /* +0x490: enum EventResponseStep; also the step of Chain_AskResponse */
    u8 aiZone:4;                        /* +0x491 bits 0-3: zone (5-9) of the Magic/Trap the CPU chose */
    u8 askPlayer:1;                     /* +0x491 bit 4: player being asked */
    u8 firstAskPlayer:1;                /* +0x491 bit 5: player asked first; the window closes back at it */
    u8 requestPending:1;                /* +0x491 bit 6: a response window is open (EventResponse_Update) */
    u8 responseAdded:1;                 /* +0x491 bit 7: a response was queued; Chain_Build restarts for it */
    u8 unk492_0:1;                      /* +0x492 bit 0: kept by the link query handlers */
    u8 queryStep:7;                     /* +0x492 bits 1-7: enum LinkQueryStep */
    u8 queryIsChainLink:1;              /* +0x493 bit 0: the partner asks about a chain link (LINKMSG_CHAIN_QUERY)
                                         * rather than a game event (LINKMSG_ACTIVATE_QUERY) */
    u8 unk493_1:7;
    u8 unk494[0x4A8 - 0x494];
    struct ChainEntry responseEntry;    /* +0x4A8: the event of the open window (card 0, player = askPlayer,
                                         * event, loc0/loc1); scratch ref of the activation tests */
    struct ChainEntry queryEntry;       /* +0x4BC: the partner's event or link, then the answering card */
    struct ChainEntry queryLink;        /* +0x4D0: the partner's chain link (chainedTo of the answer) */
    struct ChainEntry proxyLink;        /* +0x4E4: copy of a link run on behalf of another card (Fairy's Hand
                                         * Mirror, the effect-copy dispatchers) */
    /* +0x4F8: resolve handler of the proxied card, called with (&proxyLink, NULL). Declared int-returning:
     * the ROM keeps r0 unnormalised. */
    int (*proxyResolve)(struct ChainEntry *link, struct ChainEntry *chainedTo);
    u8 handPickTimer;                   /* +0x4FC: frames of the random hand-cursor hops (random discards) */
    u8 handPickCount;                   /* +0x4FD: hand cards still to discard or banish */
    u16 unk4FE;
    u16 fusionResult;                   /* +0x500: card ID of the Fusion monster picked in the Fusion Deck */
    u8 fusionMaterialCount:2;           /* +0x502 bits 0-1: 2 or 3 materials (the CPU counts it down) */
    u8 fusionPicksLeft:2;               /* +0x502 bits 2-3: material picks left minus one (human) */
    u8 fusionSubstitutesUsed:4;         /* +0x502 bits 4-7: substitutes picked; only one is allowed */
    u8 unk503;
    u16 fusionMaterials[3];             /* +0x504: card IDs of the materials still to pick (0 = picked) */
    u16 fusionMaterialSlots[3];         /* +0x50A: hand index or zone of each picked material */
    u8 effectCount;                     /* +0x510: resolve scratch counter (side round, marker count) */
    u8 effectZone;                      /* +0x511: zone being scanned by a resolve handler */
    u8 unk512[0x53C - 0x512];
    union {
        struct {
            u8 unk53C[6];
            u16 savedValue;                 /* +0x542: value kept across steps (Widespread Ruin's top ATK) */
            struct DuelCard effectCards[8]; /* +0x544: cards taken by a handler (Painful Choice's five picks,
                                             * the card taken from the graveyard or deck) */
        } effect;
        struct DeckReorderState deckReorder; /* +0x53C: Big Eye's reorder; cards overlaps effectCards */
    } scratch;                          /* +0x53C: per-effect scratch views of the same bytes */
    u8 unk564[0x56C - 0x564];
};

extern struct ChainState gChain;                    /* 0x02017A40 */

/*
 * Symbols for addresses inside gChain. The matched code accesses these through their own names, so they stay
 * separate symbols; the access form (symbol or gChain member) is a matching choice.
 */
extern u8 gChainEffectWork[];                       /* 0x02017E20 = &gChain.effectStep (byte view) */
extern struct ChainEntry gEventResponseEntry;       /* 0x02017EE8 = gChain.responseEntry */
extern struct ChainEntry gChainQueryEntry;          /* 0x02017EFC = gChain.queryEntry */
extern struct ChainEntry gChainQueryEntryRaw;       /* 0x02017F10 = gChain.queryLink */
extern struct ChainEntry gChainProxyLink;           /* 0x02017F24 = gChain.proxyLink */

/* ---- Adding to the chain ---- */

/* Append an entry to gChain.pending (toChain 0) or gChain.links (1); ignored for card ID 0. On the link
 * partner's turn the entry is sent to the partner (LINKMSG_ADD_ACTION, player bit flipped) instead. */
void Chain_Add(u16 toChain, u32 packed, u32 locs);
/* Chain_Add(0, ...): queue an activation or trigger; Chain_Update starts a new chain with it. */
void Chain_AddPending(u32 packed, u32 locs);
/* Chain_Add(1, ...): add a response link to the chain being built. */
void Chain_AddLink(u32 packed, u32 locs);
/* Append a copy of an entry received from the link partner (player 1, chainA/chainB already run). */
void Chain_AddPartnerEntry(u16 toChain, struct ChainEntry *src);

/* ---- Building and resolving ---- */

/* Per-frame driver called by DuelMainStep: runs Chain_Build or Chain_Resolve, or starts a new chain from
 * the pending list; 0 when there is nothing to do. */
u16 Chain_Update(void);
/* Set up each link (cost and target handlers) and ask both players for responses; then start resolving. */
int Chain_Build(void);
/* Ask `player` (0 human, 1 CPU or link partner) whether to respond to `link`; 1 when settled. */
s32 Chain_AskResponse(struct ChainEntry *link, u32 player);
/* Card-menu command mask for answering `e` with the card at (player, area, index): 0x41 or 1. */
u32 Chain_GetResponseCommands(struct ChainEntry *e, int player, int area, int index);
/* Resolve the chain from the last link back (negation checks, resolve handlers, graveyard moves). */
u32 Chain_Resolve(void);
/* 1 when the entry's handlers run on the link partner's GBA (player 1 in a link duel, not Time Machine). */
u32 Chain_IsPartnerEntry(struct ChainEntry *entry);
/* Does the activated card leave the field after resolving? 0 for monsters and cards that stay (Field,
 * Equip, Continuous), unless destroyIfNegated is set; 1 for Normal Magic and Trap cards. */
u32 Chain_CardGoesToGrave(struct ChainEntry *entry);

/* ---- Activation tests outside a chain ---- */

/* Can the Magic/Trap in (player, zone 5-9) be activated now? Fills ref; returns the CanActivateEffect
 * result narrowed to u16. */
int CanActivateFieldCard(struct ChainEntry *ref, int player, int zone);
/* Can hand card handIdx (a Magic) be activated now? Fills ref; returns the CanActivateEffect result
 * narrowed to u16. */
int CanActivateHandCard(struct ChainEntry *ref, int player, int handIdx);

/* ---- Event response windows ---- */

/* Open a response window for a game event (enum ResponseEventKind, arg = loc0 | loc1 << 16), asking
 * `player` first; forwarded to the partner on its turn of a link duel. */
void EventResponse_Request(int player, u16 event, u32 arg);
/* Per-frame driver: runs EventResponse_Run while a window is open; 1 while it was open. */
int EventResponse_Update(void);
/* One step of the open window (enum EventResponseStep); 1 when it is finished. */
int EventResponse_Run(void);
/* 1 if the player holds a spell/trap zone card (or, on their own turn, a hand card) of spell speed > 1
 * that can answer the event. */
int EventResponse_CanPlayerRespond(int player);
/* Card-menu command mask for the card under the cursor in a response window: 0x41 or 1. */
int EventResponse_GetCommands(struct ChainEntry *ref, int player, int area, int index);
/* Write the event's description and 'Do you wish to activate a Quick-play Magic or Trap card?' into buf. */
void EventResponse_BuildPromptText(struct ChainEntry *ev, u8 *buf);
/* Dead code: 1 if one of list's entries (a struct ChainList) is at (player, zone). */
int IsZoneInChainList(u8 *list, int player, int zone);

/* Compile-time layout checks (agbcc pads every struct to a multiple of 4 bytes). */
typedef char chain_h_check_entry[sizeof(struct ChainEntry) == 0x14 ? 1 : -1];
typedef char chain_h_check_entry_loc0[(u32)&((struct ChainEntry *)0)->loc0 == 0x6 ? 1 : -1];
typedef char chain_h_check_entry_targets[(u32)&((struct ChainEntry *)0)->targets == 0xC ? 1 : -1];
typedef char chain_h_check_list[sizeof(struct ChainList) == 0x144 ? 1 : -1];
typedef char chain_h_check_list_count[(u32)&((struct ChainList *)0)->count == 0x140 ? 1 : -1];
typedef char chain_h_check_reorder[sizeof(struct DeckReorderState) == 0x1C ? 1 : -1];
typedef char chain_h_check_reorder_cards[(u32)&((struct DeckReorderState *)0)->cards == 0x8 ? 1 : -1];
typedef char chain_h_check_state[sizeof(struct ChainState) == 0x56C ? 1 : -1];
typedef char chain_h_check_links[(u32)&((struct ChainState *)0)->links == 0x280 ? 1 : -1];
typedef char chain_h_check_link_count[(u32)&((struct ChainState *)0)->linkCount == 0x3C0 ? 1 : -1];
typedef char chain_h_check_pending_count[(u32)&((struct ChainState *)0)->pendingCount == 0x3C4 ? 1 : -1];
typedef char chain_h_check_build_index[(u32)&((struct ChainState *)0)->buildIndex == 0x3D1 ? 1 : -1];
typedef char chain_h_check_effect_index[(u32)&((struct ChainState *)0)->effectIndex == 0x3D6 ? 1 : -1];
typedef char chain_h_check_resolve[(u32)&((struct ChainState *)0)->resolve == 0x3D8 ? 1 : -1];
typedef char chain_h_check_saved_card[(u32)&((struct ChainState *)0)->savedCard == 0x3DC ? 1 : -1];
typedef char chain_h_check_effect_step[(u32)&((struct ChainState *)0)->effectStep == 0x3E0 ? 1 : -1];
typedef char chain_h_check_cost_step[(u32)&((struct ChainState *)0)->costStep == 0x3E4 ? 1 : -1];
typedef char chain_h_check_effect_card[(u32)&((struct ChainState *)0)->effectCard == 0x3E8 ? 1 : -1];
typedef char chain_h_check_saved_zone[(u32)&((struct ChainState *)0)->effectSavedZone == 0x3EC ? 1 : -1];
typedef char chain_h_check_chain_a[(u32)&((struct ChainState *)0)->chainA == 0x480 ? 1 : -1];
typedef char chain_h_check_event[(u32)&((struct ChainState *)0)->responseEvent == 0x48A ? 1 : -1];
typedef char chain_h_check_ask_step[(u32)&((struct ChainState *)0)->askStep == 0x490 ? 1 : -1];
typedef char chain_h_check_response_entry[(u32)&((struct ChainState *)0)->responseEntry == 0x4A8 ? 1 : -1];
typedef char chain_h_check_query_entry[(u32)&((struct ChainState *)0)->queryEntry == 0x4BC ? 1 : -1];
typedef char chain_h_check_query_link[(u32)&((struct ChainState *)0)->queryLink == 0x4D0 ? 1 : -1];
typedef char chain_h_check_proxy_link[(u32)&((struct ChainState *)0)->proxyLink == 0x4E4 ? 1 : -1];
typedef char chain_h_check_proxy_resolve[(u32)&((struct ChainState *)0)->proxyResolve == 0x4F8 ? 1 : -1];
typedef char chain_h_check_pick_timer[(u32)&((struct ChainState *)0)->handPickTimer == 0x4FC ? 1 : -1];
typedef char chain_h_check_fusion[(u32)&((struct ChainState *)0)->fusionResult == 0x500 ? 1 : -1];
typedef char chain_h_check_fusion_mats[(u32)&((struct ChainState *)0)->fusionMaterials == 0x504 ? 1 : -1];
typedef char chain_h_check_fusion_slots[(u32)&((struct ChainState *)0)->fusionMaterialSlots == 0x50A ? 1 : -1];
typedef char chain_h_check_effect_count[(u32)&((struct ChainState *)0)->effectCount == 0x510 ? 1 : -1];
typedef char chain_h_check_scratch[(u32)&((struct ChainState *)0)->scratch == 0x53C ? 1 : -1];
typedef char chain_h_check_saved_value[(u32)&((struct ChainState *)0)->scratch.effect.savedValue == 0x542 ? 1 : -1];
typedef char chain_h_check_effect_cards[(u32)&((struct ChainState *)0)->scratch.effect.effectCards == 0x544 ? 1 : -1];
typedef char chain_h_check_reorder_at[(u32)&((struct ChainState *)0)->scratch.deckReorder.cards == 0x544 ? 1 : -1];

#endif /* GUARD_CHAIN_H */
