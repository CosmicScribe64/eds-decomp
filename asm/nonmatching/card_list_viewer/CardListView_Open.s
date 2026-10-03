	thumb_func_start CardListView_Open
CardListView_Open: @ 0x0802AF34
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r7, r0, #0
	add r5, r1, #0
	mov r8, r2
	mov r9, r3
	mov r0, #0
	mov sl, r0
	ldr r4, _0802AF9C @ =0x0201D81C
	mov r1, #0x80
	lsl r1, r1, #2
	add r0, r4, #0
	bl MemClear16
	add r2, r4, #0
	sub r2, #0xC
	mov r0, #1
	add r1, r7, #0
	and r1, r0
	lsl r1, r1, #1
	mov r0, #3
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	mov r0, #0xC0
	lsl r0, r0, #2
	add r4, r4, r0
	mov r1, sl
	strh r1, [r4]
	mov r0, #0x3D
	neg r0, r0
	ldrb r3, [r2, #8]
	and r0, r3
	mov r1, #4
	neg r1, r1
	and r0, r1
	strb r0, [r2, #8]
	add r0, r5, #1
	cmp r0, #0x10
	bls _0802AF90
	b _0802B140
_0802AF90:
	lsl r0, r0, #2
	ldr r1, _0802AFA0 @ =0x0802AFA4
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0802AF9C: .4byte 0x0201D81C
_0802AFA0: .4byte 0x0802AFA4
_0802AFA4:
	.4byte _0802B10C
	.4byte _0802B140
	.4byte _0802B140
	.4byte _0802B140
	.4byte _0802B140
	.4byte _0802B140
	.4byte _0802B140
	.4byte _0802B140
	.4byte _0802B140
	.4byte _0802B140
	.4byte _0802B140
	.4byte _0802B140
	.4byte _0802B140
	.4byte _0802B030
	.4byte _0802B0C0
	.4byte _0802AFE8
	.4byte _0802B078
_0802AFE8:
	ldr r2, _0802B020 @ =0x0201D810
	lsl r1, r7, #5
	mov r0, #0x1F
	ldrb r4, [r2]
	and r0, r4
	orr r0, r1
	strb r0, [r2]
	mov r0, #0x3D
	neg r0, r0
	ldrb r1, [r2, #8]
	and r0, r1
	mov r1, #8
	orr r0, r1
	strb r0, [r2, #8]
	ldr r3, _0802B024 @ =0x020192E4
	mov r0, #1
	and r0, r7
	ldr r1, _0802B028 @ =0x00000D64
	mul r1, r0
	add r0, r1, r3
	ldrb r0, [r0, #4]
	mov r4, #0xC3
	lsl r4, r4, #2
	add r2, r2, r4
	strh r0, [r2]
	ldr r0, _0802B02C @ =0x00000904
	b _0802B0F4
	.align 2, 0
_0802B020: .4byte 0x0201D810
_0802B024: .4byte 0x020192E4
_0802B028: .4byte 0x00000D64
_0802B02C: .4byte 0x00000904
_0802B030:
	ldr r2, _0802B068 @ =0x0201D810
	mov r0, #0x1F
	ldrb r1, [r2]
	and r0, r1
	mov r1, #0x40
	orr r0, r1
	strb r0, [r2]
	mov r0, #0x3D
	neg r0, r0
	ldrb r3, [r2, #8]
	and r0, r3
	mov r1, #8
	orr r0, r1
	strb r0, [r2, #8]
	ldr r3, _0802B06C @ =0x020192E4
	mov r0, #1
	and r0, r7
	ldr r1, _0802B070 @ =0x00000D64
	mul r1, r0
	add r0, r1, r3
	ldrb r0, [r0, #5]
	mov r4, #0xC3
	lsl r4, r4, #2
	add r2, r2, r4
	strh r0, [r2]
	ldr r0, _0802B074 @ =0x00000A44
	b _0802B0F4
	.align 2, 0
_0802B068: .4byte 0x0201D810
_0802B06C: .4byte 0x020192E4
_0802B070: .4byte 0x00000D64
_0802B074: .4byte 0x00000A44
_0802B078:
	ldr r2, _0802B0B0 @ =0x0201D810
	mov r0, #0x1F
	ldrb r1, [r2]
	and r0, r1
	mov r1, #0x60
	orr r0, r1
	strb r0, [r2]
	mov r0, #0x3D
	neg r0, r0
	ldrb r3, [r2, #8]
	and r0, r3
	mov r1, #8
	orr r0, r1
	strb r0, [r2, #8]
	ldr r3, _0802B0B4 @ =0x020192E4
	mov r0, #1
	and r0, r7
	ldr r1, _0802B0B8 @ =0x00000D64
	mul r1, r0
	add r0, r1, r3
	ldrb r0, [r0, #6]
	mov r4, #0xC3
	lsl r4, r4, #2
	add r2, r2, r4
	strh r0, [r2]
	ldr r0, _0802B0BC @ =0x00000B84
	b _0802B0F4
	.align 2, 0
_0802B0B0: .4byte 0x0201D810
_0802B0B4: .4byte 0x020192E4
_0802B0B8: .4byte 0x00000D64
_0802B0BC: .4byte 0x00000B84
_0802B0C0:
	ldr r2, _0802B0FC @ =0x0201D810
	mov r0, #0x1F
	ldrb r1, [r2]
	and r0, r1
	mov r1, #0xA0
	orr r0, r1
	strb r0, [r2]
	mov r0, #0x3D
	neg r0, r0
	ldrb r3, [r2, #8]
	and r0, r3
	mov r1, #8
	orr r0, r1
	strb r0, [r2, #8]
	ldr r3, _0802B100 @ =0x020192E4
	mov r0, #1
	and r0, r7
	ldr r1, _0802B104 @ =0x00000D64
	mul r1, r0
	add r0, r1, r3
	ldrb r0, [r0, #3]
	mov r4, #0xC3
	lsl r4, r4, #2
	add r2, r2, r4
	strh r0, [r2]
	ldr r0, _0802B108 @ =0x000007C4
_0802B0F4:
	add r3, r3, r0
	add r6, r1, r3
	b _0802B154
	.align 2, 0
_0802B0FC: .4byte 0x0201D810
_0802B100: .4byte 0x020192E4
_0802B104: .4byte 0x00000D64
_0802B108: .4byte 0x000007C4
_0802B10C:
	ldr r2, _0802B13C @ =0x0201D810
	mov r0, #0x1F
	ldrb r1, [r2]
	and r0, r1
	mov r1, #0x80
	orr r0, r1
	strb r0, [r2]
	mov r0, #0x3D
	neg r0, r0
	ldrb r3, [r2, #8]
	and r0, r3
	mov r1, #0x20
	orr r0, r1
	strb r0, [r2, #8]
	add r0, r7, #0
	mov r1, r8
	mov r2, r9
	bl CollectEffectTargets
	mov r4, sl
	cmp r4, #0
	beq _0802B17A
	b _0802B154
	.align 2, 0
_0802B13C: .4byte 0x0201D810
_0802B140:
	ldr r1, _0802B150 @ =0x0201D810
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	b _0802B1A0
	.align 2, 0
_0802B150: .4byte 0x0201D810
_0802B154:
	ldr r5, _0802B1B0 @ =0x0201D81C
	mov r4, #0
	mov r3, #0xC0
	lsl r3, r3, #2
	add r0, r5, r3
	ldrh r1, [r0]
	cmp r4, r1
	bge _0802B17A
	add r7, r0, #0
_0802B166:
	add r0, r5, #0
	add r5, #4
	add r1, r6, #0
	add r6, #4
	bl CopyDuelCard
	add r4, #1
	ldrh r2, [r7]
	cmp r4, r2
	blt _0802B166
_0802B17A:
	ldr r2, _0802B1B4 @ =0x0201D810
	mov r0, #4
	neg r0, r0
	ldrb r3, [r2, #5]
	and r0, r3
	mov r1, #0x1D
	neg r1, r1
	and r0, r1
	sub r1, #0x44
	and r0, r1
	strb r0, [r2, #5]
	mov r1, #0
	mov r0, #0
	strh r0, [r2, #6]
	mov r0, #1
	ldrb r4, [r2]
	orr r0, r4
	strb r0, [r2]
	strb r1, [r2, #1]
_0802B1A0:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802B1B0: .4byte 0x0201D81C
_0802B1B4: .4byte 0x0201D810
	thumb_func_end CardListView_Open

