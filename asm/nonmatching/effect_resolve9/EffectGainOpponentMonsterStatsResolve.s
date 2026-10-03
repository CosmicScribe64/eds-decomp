	thumb_func_start EffectGainOpponentMonsterStatsResolve
EffectGainOpponentMonsterStatsResolve: @ 0x08039B90
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _08039BCC @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08039BDC
	cmp r0, #0x80
	bne _08039C40
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #1
	mov r2, #0
	bl CountMonstersFiltered
	cmp r0, #0
	beq _08039C40
	ldr r0, _08039BD0 @ =0x00000206
	ldr r1, _08039BD4 @ =0x00000613
	ldr r3, _08039BD8 @ =0x08083508
	mov r2, #0xB
	bl TextBoxOpen
_08039BC8:
	mov r0, #0x7F
	b _08039C42
_08039BCC: .4byte 0x02017A40
_08039BD0: .4byte 0x00000206
_08039BD4: .4byte 0x00000613
_08039BD8: .4byte gStrSelectStatsSourceMonster
_08039BDC:
	mov r0, #0xE0
	lsl r0, r0, #0x10
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08039BC8
	ldr r0, _08039C2C @ =0x0201CFB0
	ldr r2, _08039C30 @ =0x00000824
	add r1, r0, r2
	ldr r2, [r1]
	ldr r1, _08039C34 @ =0x0000082C
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #1
	and r2, r0
	mov r0, #0x94
	mul r0, r1
	ldr r1, _08039C38 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08039C3C @ =0x0201930C
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	ldrb r0, [r4, #2]
	lsl r2, r0, #0x1F
	lsr r0, r2, #0x1F
	add r2, r0, #0
	ldrh r4, [r4, #2]
	lsl r3, r4, #0x16
	lsr r3, r3, #0x1A
	lsl r3, r3, #8
	orr r2, r3
	mov r3, #8
	bl QueueAddZoneLink
	mov r0, #0x78
	b _08039C42
	.align 2, 0
_08039C2C: .4byte 0x0201CFB0
_08039C30: .4byte 0x00000824
_08039C34: .4byte 0x0000082C
_08039C38: .4byte 0x00000D64
_08039C3C: .4byte 0x0201930C
_08039C40:
	mov r0, #0
_08039C42:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectGainOpponentMonsterStatsResolve

