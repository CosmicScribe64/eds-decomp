	thumb_func_start DrawAreaTiles
DrawAreaTiles: @ 0x08061004
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r4, r0, #0
	add r3, r1, #0
	ldr r6, _08061040 @ =0x081A42A4
	lsl r0, r3, #3
	lsl r1, r4, #7
	add r1, r0, r1
	add r0, r1, r6
	ldr r0, [r0]
	cmp r0, #0
	bge _08061020
	add r0, #7
_08061020:
	asr r5, r0, #3
	add r0, r6, #4
	add r0, r1, r0
	ldr r0, [r0]
	cmp r0, #0
	bge _0806102E
	add r0, #7
_0806102E:
	asr r7, r0, #3
	cmp r3, #0xF
	bls _08061036
	b _080611A0
_08061036:
	lsl r0, r3, #2
	ldr r1, _08061044 @ =0x08061048
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08061040: .4byte gDuelZonePositions
_08061044: .4byte 0x08061048
_08061048:
	.4byte _08061088
	.4byte _080611A0
	.4byte _080611A0
	.4byte _080611A0
	.4byte _080611A0
	.4byte _08061088
	.4byte _080611A0
	.4byte _080611A0
	.4byte _080611A0
	.4byte _080611A0
	.4byte _08061088
	.4byte _080611A0
	.4byte _08061092
	.4byte _080610B8
	.4byte _080610D8
	.4byte _0806112A
_08061088:
	add r1, r3, r2
	add r0, r4, #0
	bl DrawZoneTiles
	b _080611A0
_08061092:
	ldr r2, _080610AC @ =0x020192E4
	mov r0, #1
	and r0, r4
	ldr r1, _080610B0 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #5]
	cmp r0, #0
	beq _08061120
	cmp r0, #5
	bls _08061176
_080610A8:
	ldr r2, _080610B4 @ =0x00001230
	b _08061178
_080610AC: .4byte 0x020192E4
_080610B0: .4byte 0x00000D64
_080610B4: .4byte 0x00001230
_080610B8:
	ldr r2, _080610D0 @ =0x020192E4
	mov r0, #1
	and r0, r4
	ldr r1, _080610D4 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #3]
	cmp r0, #0
	beq _08061120
	cmp r0, #5
	bhi _080610A8
	b _08061176
_080610D0: .4byte 0x020192E4
_080610D4: .4byte 0x00000D64
_080610D8:
	ldr r6, _08061114 @ =0x020192E4
	mov r0, #1
	and r0, r4
	ldr r1, _08061118 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r3, r2, r6
	ldrb r0, [r3, #4]
	cmp r0, #0
	beq _08061120
	ldr r0, _0806111C @ =0x00000904
	add r1, r6, r0
	add r1, r2, r1
	ldrb r3, [r3, #4]
	lsl r0, r3, #2
	sub r0, #4
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	bl GetCardIconBgTile
	add r2, r0, #0
	mov r1, #0x80
	lsl r1, r1, #6
	add r2, r2, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	b _08061178
	.align 2, 0
_08061114: .4byte 0x020192E4
_08061118: .4byte 0x00000D64
_0806111C: .4byte 0x00000904
_08061120:
	add r0, r5, #0
	add r1, r7, #0
	bl ClearTileBlock4x4
	b _080611A0
_0806112A:
	ldr r2, _08061184 @ =0x020192E4
	mov r8, r2
	mov r0, #1
	and r0, r4
	ldr r1, _08061188 @ =0x00000D64
	add r6, r0, #0
	mul r6, r1
	add r4, r6, r2
	ldrb r0, [r4, #6]
	cmp r0, #0
	beq _08061198
	ldr r1, _0806118C @ =0x00000B84
	add r1, r8
	add r1, r6, r1
	ldrb r2, [r4, #6]
	lsl r0, r2, #2
	sub r0, #4
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	bl GetCardIconBgTile
	mov r1, #0x80
	lsl r1, r1, #6
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldrb r0, [r4, #6]
	sub r0, #1
	lsl r0, r0, #1
	add r0, r0, r6
	ldr r1, _08061190 @ =0x00000CC4
	add r1, r8
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #2
	bne _08061178
_08061176:
	ldr r2, _08061194 @ =0x00001070
_08061178:
	add r0, r5, #0
	add r1, r7, #0
	bl FillTileBlock4x4
	b _080611A0
	.align 2, 0
_08061184: .4byte 0x020192E4
_08061188: .4byte 0x00000D64
_0806118C: .4byte 0x00000B84
_08061190: .4byte 0x00000CC4
_08061194: .4byte 0x00001070
_08061198:
	add r0, r5, #0
	add r1, r7, #0
	bl ClearTileBlock4x4
_080611A0:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end DrawAreaTiles
	.align 2, 0

