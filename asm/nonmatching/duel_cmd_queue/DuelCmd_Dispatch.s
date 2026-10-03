	thumb_func_start DuelCmd_Dispatch
DuelCmd_Dispatch: @ 0x0801ECA8
	push {lr}
	ldr r1, _0801ECD8 @ =0x0201CFB0
	ldr r0, _0801ECDC @ =0x00000808
	add r1, r1, r0
	mov r0, #9
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r1, _0801ECE0 @ =0x020185C0
	ldr r0, _0801ECE4 @ =0x00000FFF
	ldrh r2, [r1]
	and r0, r2
	sub r0, #1
	add r2, r1, #0
	cmp r0, #0xE4
	bls _0801ECCC
	b _0801F43C
_0801ECCC:
	lsl r0, r0, #2
	ldr r1, _0801ECE8 @ =0x0801ECEC
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801ECD8: .4byte 0x0201CFB0
_0801ECDC: .4byte 0x00000808
_0801ECE0: .4byte 0x020185C0
_0801ECE4: .4byte 0x00000FFF
_0801ECE8: .4byte 0x0801ECEC
_0801ECEC:
	.4byte _0801F080
	.4byte _0801F086
	.4byte _0801F08C
	.4byte _0801F092
	.4byte _0801F098
	.4byte _0801F09E
	.4byte _0801F0A4
	.4byte _0801F0AA
	.4byte _0801F0B0
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F0B6
	.4byte _0801F0BC
	.4byte _0801F0C2
	.4byte _0801F0C8
	.4byte _0801F0CE
	.4byte _0801F0D4
	.4byte _0801F0D4
	.4byte _0801F0D4
	.4byte _0801F0D4
	.4byte _0801F0D4
	.4byte _0801F0D4
	.4byte _0801F0D4
	.4byte _0801F0DA
	.4byte _0801F0E0
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F0E6
	.4byte _0801F0EC
	.4byte _0801F0F2
	.4byte _0801F0F8
	.4byte _0801F0FE
	.4byte _0801F104
	.4byte _0801F10A
	.4byte _0801F110
	.4byte _0801F116
	.4byte _0801F11C
	.4byte _0801F122
	.4byte _0801F128
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F12E
	.4byte _0801F134
	.4byte _0801F13A
	.4byte _0801F142
	.4byte _0801F14A
	.4byte _0801F150
	.4byte _0801F156
	.4byte _0801F15C
	.4byte _0801F162
	.4byte _0801F168
	.4byte _0801F16E
	.4byte _0801F174
	.4byte _0801F17A
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F180
	.4byte _0801F188
	.4byte _0801F190
	.4byte _0801F198
	.4byte _0801F19E
	.4byte _0801F1A6
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F1C0
	.4byte _0801F1AE
	.4byte _0801F1B4
	.4byte _0801F1BA
	.4byte _0801F1C6
	.4byte _0801F1CC
	.4byte _0801F1D2
	.4byte _0801F1D8
	.4byte _0801F1DE
	.4byte _0801F1E4
	.4byte _0801F1EA
	.4byte _0801F1F0
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F1F6
	.4byte _0801F1FC
	.4byte _0801F202
	.4byte _0801F208
	.4byte _0801F20E
	.4byte _0801F214
	.4byte _0801F21A
	.4byte _0801F220
	.4byte _0801F226
	.4byte _0801F244
	.4byte _0801F24A
	.4byte _0801F250
	.4byte _0801F22C
	.4byte _0801F232
	.4byte _0801F238
	.4byte _0801F23E
	.4byte _0801F256
	.4byte _0801F25C
	.4byte _0801F262
	.4byte _0801F268
	.4byte _0801F26E
	.4byte _0801F274
	.4byte _0801F27A
	.4byte _0801F280
	.4byte _0801F286
	.4byte _0801F28C
	.4byte _0801F292
	.4byte _0801F298
	.4byte _0801F29E
	.4byte _0801F2A4
	.4byte _0801F2AA
	.4byte _0801F2B0
	.4byte _0801F2B6
	.4byte _0801F2BC
	.4byte _0801F2C2
	.4byte _0801F2C8
	.4byte _0801F2CE
	.4byte _0801F2D4
	.4byte _0801F2DA
	.4byte _0801F2E0
	.4byte _0801F2E6
	.4byte _0801F2EC
	.4byte _0801F2F2
	.4byte _0801F2F8
	.4byte _0801F2FE
	.4byte _0801F304
	.4byte _0801F30A
	.4byte _0801F310
	.4byte _0801F316
	.4byte _0801F31C
	.4byte _0801F322
	.4byte _0801F328
	.4byte _0801F32E
	.4byte _0801F334
	.4byte _0801F33A
	.4byte _0801F340
	.4byte _0801F346
	.4byte _0801F34C
	.4byte _0801F352
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F358
	.4byte _0801F364
	.4byte _0801F35E
	.4byte _0801F36A
	.4byte _0801F370
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F376
	.4byte _0801F37C
	.4byte _0801F388
	.4byte _0801F382
	.4byte _0801F38E
	.4byte _0801F394
	.4byte _0801F43C
	.4byte _0801F39A
	.4byte _0801F43C
	.4byte _0801F43C
	.4byte _0801F3A6
	.4byte _0801F3A0
	.4byte _0801F3AC
	.4byte _0801F3B2
	.4byte _0801F3B8
	.4byte _0801F3BE
	.4byte _0801F3C4
	.4byte _0801F3CA
	.4byte _0801F3D0
	.4byte _0801F3DC
	.4byte _0801F3D6
	.4byte _0801F3E2
	.4byte _0801F3E8
	.4byte _0801F3EE
	.4byte _0801F3F4
	.4byte _0801F3FA
	.4byte _0801F400
	.4byte _0801F406
	.4byte _0801F40C
	.4byte _0801F412
	.4byte _0801F418
	.4byte _0801F43C
	.4byte _0801F41E
	.4byte _0801F424
	.4byte _0801F42A
	.4byte _0801F436
	.4byte _0801F430
	.4byte _0801F436
_0801F080:
	bl DuelCmd_TurnStart
	b _0801F44A
_0801F086:
	bl DuelCmd_TurnEnd
	b _0801F44A
_0801F08C:
	bl DuelCmd_ShowEndTurnHand
	b _0801F44A
_0801F092:
	bl DuelCmd_ShowDuelResult
	b _0801F44A
_0801F098:
	bl DuelCmd_ExodiaWinScene
	b _0801F44A
_0801F09E:
	bl DuelCmd_DestinyBoardWinScene
	b _0801F44A
_0801F0A4:
	bl DuelCmd_ShowChainBanner
	b _0801F44A
_0801F0AA:
	bl DuelCmd_PointAtCard
	b _0801F44A
_0801F0B0:
	bl DuelCmd_MoveCursor
	b _0801F44A
_0801F0B6:
	bl DuelCmd_ResetDuelState
	b _0801F44A
_0801F0BC:
	bl DuelCmd_SetFieldBackground
	b _0801F44A
_0801F0C2:
	bl DuelCmd_OpenDuelScreen
	b _0801F44A
_0801F0C8:
	bl DuelCmd_CloseDuelScreen
	b _0801F44A
_0801F0CE:
	bl DuelCmd_StartDuelBanner
	b _0801F44A
_0801F0D4:
	bl DuelCmd_SetNegationFlag
	b _0801F44A
_0801F0DA:
	bl DuelCmd_SetStatChangesReversed
	b _0801F44A
_0801F0E0:
	bl DuelCmd_SetAtkDefSwapped
	b _0801F44A
_0801F0E6:
	bl DuelCmd_StartBattleScene
	b _0801F44A
_0801F0EC:
	bl DuelCmd_PlayBattleScene
	b _0801F44A
_0801F0F2:
	bl DuelCmd_PrepareBattlePhase
	b _0801F44A
_0801F0F8:
	bl DuelCmd_Attack
	b _0801F44A
_0801F0FE:
	bl DuelCmd_DirectAttack
	b _0801F44A
_0801F104:
	bl DuelCmd_MarkAttacked
	b _0801F44A
_0801F10A:
	bl DuelCmd_SetBattleProtection
	b _0801F44A
_0801F110:
	bl DuelCmd_EndBattlePhase
	b _0801F44A
_0801F116:
	bl DuelCmd_SetAttackTarget
	b _0801F44A
_0801F11C:
	bl DuelCmd_SetAttacker
	b _0801F44A
_0801F122:
	bl DuelCmd_ZeroAttackerAtk
	b _0801F44A
_0801F128:
	bl DuelCmd_NegateAttack
	b _0801F44A
_0801F12E:
	bl DuelCmd_Surrender
	b _0801F44A
_0801F134:
	bl DuelCmd_ShowJustAMomentBanner
	b _0801F44A
_0801F13A:
	mov r0, #1
	bl DuelCmd_ChangeLifePoints
	b _0801F44A
_0801F142:
	mov r0, #0
	bl DuelCmd_ChangeLifePoints
	b _0801F44A
_0801F14A:
	bl DuelCmd_SkipNextDrawPhase
	b _0801F44A
_0801F150:
	bl DuelCmd_SkipNextStandbyPhase
	b _0801F44A
_0801F156:
	bl DuelCmd_SkipNextTurn
	b _0801F44A
_0801F15C:
	bl DuelCmd_SetExtraBattlePhase
	b _0801F44A
_0801F162:
	bl DuelCmd_SetPositionChangeLock
	b _0801F44A
_0801F168:
	bl DuelCmd_SetSummonLocks
	b _0801F44A
_0801F16E:
	bl DuelCmd_SetMagicTrapLockTurns
	b _0801F44A
_0801F174:
	bl DuelCmd_AdjustDelayedSummonCount
	b _0801F44A
_0801F17A:
	bl sub_08014B5C
	b _0801F44A
_0801F180:
	mov r0, #0
	bl DuelCmd_EnterPhase
	b _0801F44A
_0801F188:
	mov r0, #1
	bl DuelCmd_EnterPhase
	b _0801F44A
_0801F190:
	mov r0, #2
	bl DuelCmd_EnterPhase
	b _0801F44A
_0801F198:
	bl DuelCmd_EnterBattlePhase
	b _0801F44A
_0801F19E:
	mov r0, #4
	bl DuelCmd_EnterPhase
	b _0801F44A
_0801F1A6:
	mov r0, #5
	bl DuelCmd_EnterPhase
	b _0801F44A
_0801F1AE:
	bl DuelCmd_DrawCards
	b _0801F44A
_0801F1B4:
	bl DuelCmd_SendTopDeckCardsToGraveyard
	b _0801F44A
_0801F1BA:
	bl DuelCmd_BanishTopDeckCards
	b _0801F44A
_0801F1C0:
	bl DuelCmd_ShuffleDeck
	b _0801F44A
_0801F1C6:
	bl DuelCmd_AddDeckCardToHand
	b _0801F44A
_0801F1CC:
	bl DuelCmd_RemoveCardFromDeck
	b _0801F44A
_0801F1D2:
	bl DuelCmd_SummonFromDeck
	b _0801F44A
_0801F1D8:
	bl DuelCmd_SendDeckCardToGraveyard
	b _0801F44A
_0801F1DE:
	bl DuelCmd_BanishDeckCard
	b _0801F44A
_0801F1E4:
	bl DuelCmd_SetCrushCardTurns
	b _0801F44A
_0801F1EA:
	bl DuelCmd_AddCardToDeckTop
	b _0801F44A
_0801F1F0:
	bl DuelCmd_AddCardToDeckBottom
	b _0801F44A
_0801F1F6:
	bl DuelCmd_ShowCardDetail
	b _0801F44A
_0801F1FC:
	bl DuelCmd_ShowCardAssemble
	b _0801F44A
_0801F202:
	bl DuelCmd_ShowCardZoomIn
	b _0801F44A
_0801F208:
	bl DuelCmd_ShowCardEffect
	b _0801F44A
_0801F20E:
	bl DuelCmd_ShowCardScatter
	b _0801F44A
_0801F214:
	bl DuelCmd_ShowCardUnrollDown
	b _0801F44A
_0801F21A:
	bl DuelCmd_ShowCardUnrollSideways
	b _0801F44A
_0801F220:
	bl DuelCmd_PlaceCard
	b _0801F44A
_0801F226:
	bl DuelCmd_ClearZoneCard
	b _0801F44A
_0801F22C:
	bl DuelCmd_AddCardToGraveyard
	b _0801F44A
_0801F232:
	bl DuelCmd_AddCardToBanished
	b _0801F44A
_0801F238:
	bl DuelCmd_ChangePosition
	b _0801F44A
_0801F23E:
	bl DuelCmd_FlipCard
	b _0801F44A
_0801F244:
	bl DuelCmd_SendToGraveyard
	b _0801F44A
_0801F24A:
	bl DuelCmd_Banish
	b _0801F44A
_0801F250:
	bl DuelCmd_BanishFlagged
	b _0801F44A
_0801F256:
	bl DuelCmd_ReturnToHand
	b _0801F44A
_0801F25C:
	bl DuelCmd_ReturnToDeck
	b _0801F44A
_0801F262:
	bl DuelCmd_MoveToZone
	b _0801F44A
_0801F268:
	bl DuelCmd_AddEquipLink
	b _0801F44A
_0801F26E:
	bl DuelCmd_SwapZones
	b _0801F44A
_0801F274:
	bl DuelCmd_AddZoneLink
	b _0801F44A
_0801F27A:
	bl DuelCmd_RemoveZoneLink
	b _0801F44A
_0801F280:
	bl DuelCmd_SetZoneDeclaredValue
	b _0801F44A
_0801F286:
	bl DuelCmd_ResetZoneTurnCounterAndSetDeclaredValue
	b _0801F44A
_0801F28C:
	bl DuelCmd_SetZoneTurnCounter
	b _0801F44A
_0801F292:
	bl DuelCmd_AddZoneTurnCounter
	b _0801F44A
_0801F298:
	bl DuelCmd_SetDestroyedByOpponentFlag
	b _0801F44A
_0801F29E:
	bl DuelCmd_ClearZoneLinks
	b _0801F44A
_0801F2A4:
	bl DuelCmd_MoveZoneLinks
	b _0801F44A
_0801F2AA:
	bl DuelCmd_AddProhibition
	b _0801F44A
_0801F2B0:
	bl DuelCmd_RemoveProhibition
	b _0801F44A
_0801F2B6:
	bl DuelCmd_SetZoneStatusFlags
	b _0801F44A
_0801F2BC:
	bl DuelCmd_ClearZoneStatusFlags
	b _0801F44A
_0801F2C2:
	bl DuelCmd_SetEffectUnused
	b _0801F44A
_0801F2C8:
	bl DuelCmd_TributeMonster
	b _0801F44A
_0801F2CE:
	bl DuelCmd_PlantInOpponentDeck
	b _0801F44A
_0801F2D4:
	bl DuelCmd_SetDestroyCountdown
	b _0801F44A
_0801F2DA:
	bl DuelCmd_SetCannotAttack
	b _0801F44A
_0801F2E0:
	bl DuelCmd_SetCannotAttackNextTurn
	b _0801F44A
_0801F2E6:
	bl DuelCmd_HalveAttack
	b _0801F44A
_0801F2EC:
	bl DuelCmd_Nop99
	b _0801F44A
_0801F2F2:
	bl DuelCmd_Nop9A
	b _0801F44A
_0801F2F8:
	bl DuelCmd_Nop9B
	b _0801F44A
_0801F2FE:
	bl DuelCmd_Nop9C
	b _0801F44A
_0801F304:
	bl DuelCmd_Nop9D
	b _0801F44A
_0801F30A:
	bl DuelCmd_Nop9E
	b _0801F44A
_0801F310:
	bl DuelCmd_Nop9F
	b _0801F44A
_0801F316:
	bl DuelCmd_ClearZoneLinks2
	b _0801F44A
_0801F31C:
	bl DuelCmd_SetPositionLocked
	b _0801F44A
_0801F322:
	bl DuelCmd_SetReturnAfterBattle
	b _0801F44A
_0801F328:
	bl DuelCmd_SummonToken
	b _0801F44A
_0801F32E:
	bl DuelCmd_SetZoneCardWord
	b _0801F44A
_0801F334:
	bl DuelCmd_SendFusionMaterialToGrave
	b _0801F44A
_0801F33A:
	bl sub_08013104
	b _0801F44A
_0801F340:
	bl DuelCmd_MoveMonsterFaceDown
	b _0801F44A
_0801F346:
	bl DuelCmd_SetMagicalHatsCard
	b _0801F44A
_0801F34C:
	bl DuelCmd_BanishMonsterUntilEndPhase
	b _0801F44A
_0801F352:
	bl DuelCmd_ReturnBanishedMonster
	b _0801F44A
_0801F358:
	bl DuelCmd_NegateActivation
	b _0801F44A
_0801F35E:
	bl DuelCmd_NopB2
	b _0801F44A
_0801F364:
	bl DuelCmd_SetSpellTrapDisabled
	b _0801F44A
_0801F36A:
	bl DuelCmd_UpdateZoneLpPaid
	b _0801F44A
_0801F370:
	bl DuelCmd_IncrementZoneTurnCounter
	b _0801F44A
_0801F376:
	bl DuelCmd_SendHandCardToGraveyard
	b _0801F44A
_0801F37C:
	bl DuelCmd_BanishHandCard
	b _0801F44A
_0801F382:
	bl DuelCmd_ReturnHandCardToDeck
	b _0801F44A
_0801F388:
	bl DuelCmd_RemoveCardFromHand
	b _0801F44A
_0801F38E:
	bl DuelCmd_PlaceMonsterFromHand
	b _0801F44A
_0801F394:
	bl DuelCmd_PlaceSpellTrapFromHand
	b _0801F44A
_0801F39A:
	bl DuelCmd_ExchangeHandCards
	b _0801F44A
_0801F3A0:
	bl DuelCmd_AddCardToHand
	b _0801F44A
_0801F3A6:
	bl DuelCmd_CompactHand
	b _0801F44A
_0801F3AC:
	bl DuelCmd_SendHandFusionMaterialToGraveyard
	b _0801F44A
_0801F3B2:
	bl DuelCmd_BanishHandFusionMaterial
	b _0801F44A
_0801F3B8:
	bl DuelCmd_BanishHandCardFaceDown
	b _0801F44A
_0801F3BE:
	bl DuelCmd_ReturnBanishedCardToHand
	b _0801F44A
_0801F3C4:
	bl DuelCmd_ReturnGraveyardCardToDeckTop
	b _0801F44A
_0801F3CA:
	bl DuelCmd_ReturnGraveyardCardToDeckBottom
	b _0801F44A
_0801F3D0:
	bl DuelCmd_ReturnGraveyardCardToHand
	b _0801F44A
_0801F3D6:
	bl DuelCmd_BanishGraveyardCard
	b _0801F44A
_0801F3DC:
	bl DuelCmd_RemoveCardFromGraveyard
	b _0801F44A
_0801F3E2:
	bl DuelCmd_TakeOpponentGraveyardCard
	b _0801F44A
_0801F3E8:
	bl DuelCmd_ReturnGraveyardToDeck
	b _0801F44A
_0801F3EE:
	bl DuelCmd_AddCardToGraveyardNoRedraw
	b _0801F44A
_0801F3F4:
	bl DuelCmd_ClearPendingEquip
	b _0801F44A
_0801F3FA:
	bl DuelCmd_EquipGraveyardCardToOpponent
	b _0801F44A
_0801F400:
	bl sub_080106BC
	b _0801F44A
_0801F406:
	bl sub_08010708
	b _0801F44A
_0801F40C:
	bl DuelCmd_RemoveCardFromFusionDeck
	b _0801F44A
_0801F412:
	bl DuelCmd_SendFusionDeckCardToGraveyard
	b _0801F44A
_0801F418:
	bl DuelCmd_ReturnBanishedCardToGraveyard
	b _0801F44A
_0801F41E:
	bl DuelCmd_TossCoin
	b _0801F44A
_0801F424:
	bl DuelCmd_TossThreeCoins
	b _0801F44A
_0801F42A:
	bl DuelCmd_RollGracefulDice
	b _0801F44A
_0801F430:
	bl DuelCmd_RollPlainDie
	b _0801F44A
_0801F436:
	bl DuelCmd_RollSkullDice
	b _0801F44A
_0801F43C:
	ldr r0, _0801F450 @ =0x0000080D
	add r1, r2, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0801F44A:
	pop {r0}
	bx r0
	.align 2, 0
_0801F450: .4byte 0x0000080D
	thumb_func_end DuelCmd_Dispatch

