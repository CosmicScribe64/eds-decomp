	thumb_func_start sub_08036254
sub_08036254: @ 0x08036254
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0xA0
	add r5, r0, #0
	add r2, r1, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	beq _0803626E
	b _080364C6
_0803626E:
	ldr r1, _0803628C @ =0x02017A40
	mov r3, #0xF8
	lsl r3, r3, #2
	add r0, r1, r3
	ldrb r0, [r0]
	sub r0, #0x7C
	add r7, r1, #0
	cmp r0, #4
	bls _08036282
	b _080364AC
_08036282:
	lsl r0, r0, #2
	ldr r1, _08036290 @ =0x08036294
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0803628C: .4byte 0x02017A40
_08036290: .4byte 0x08036294
_08036294:
	.4byte _080363D0
	.4byte _0803639C
	.4byte _08036314
	.4byte _080362E8
	.4byte _080362A8
_080362A8:
	add r0, r5, #0
	add r1, r2, #0
	mov r2, #0
	bl sub_0802EA50
	cmp r0, #0
	bne _080362B8
	b _080364C6
_080362B8:
	ldr r0, _080362D4 @ =0x00000206
	ldr r1, _080362D8 @ =0x00000512
	ldr r3, _080362DC @ =0x08082E7C
	mov r2, #0xB
	bl sub_080602A4
	ldr r0, _080362E0 @ =0x02017A40
	ldr r1, _080362E4 @ =0x000003E1
	add r0, r0, r1
	mov r1, #5
	strb r1, [r0]
	mov r0, #0x7F
	b _080364C8
	.align 2, 0
_080362D4: .4byte 0x00000206
_080362D8: .4byte 0x00000512
_080362DC: .4byte gUnk_08082E7C
_080362E0: .4byte 0x02017A40
_080362E4: .4byte 0x000003E1
_080362E8:
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _0803630C @ =0x000007FF
	ldrh r5, [r5]
	and r2, r5
	lsl r2, r2, #1
	ldr r3, _08036310 @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl sub_0802AF34
	mov r0, #0x7E
	b _080364C8
	.align 2, 0
_0803630C: .4byte 0x000007FF
_08036310: .4byte gUnk_08622AB4
_08036314:
	ldr r0, _0803637C @ =0x000003E1
	add r6, r7, r0
	ldrb r0, [r6]
	sub r0, #1
	strb r0, [r6]
	ldr r1, _08036380 @ =0x0201D810
	ldrb r2, [r1, #5]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1E
	ldrh r3, [r1, #6]
	add r0, r3, r0
	lsl r0, r0, #2
	add r1, #0xC
	add r4, r0, r1
	mov r0, #1
	ldrb r5, [r5, #2]
	and r0, r5
	mov r3, #0x65
	cmp r0, #0
	beq _0803633E
	ldr r3, _08036384 @ =0x00008065
_0803633E:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldrb r1, [r6]
	lsl r0, r1, #2
	ldr r2, _08036388 @ =0x00000544
	add r1, r7, r2
	add r0, r0, r1
	add r1, r4, #0
	bl sub_08007558
	ldrb r0, [r6]
	cmp r0, #0
	beq _08036398
	add r4, sp, #0x20
	ldr r1, _0803638C @ =0x08082E9C
	add r2, r0, #0
	add r0, r4, #0
	bl sub_08075434
	ldr r0, _08036390 @ =0x00000206
	ldr r1, _08036394 @ =0x00000512
	mov r2, #0xB
	add r3, r4, #0
	bl sub_080602A4
	mov r0, #0x7F
	b _080364C8
_0803637C: .4byte 0x000003E1
_08036380: .4byte 0x0201D810
_08036384: .4byte 0x00008065
_08036388: .4byte 0x00000544
_0803638C: .4byte gUnk_08082E9C
_08036390: .4byte 0x00000206
_08036394: .4byte 0x00000512
_08036398:
	mov r0, #0x7D
	b _080364C8
_0803639C:
	ldrb r1, [r5, #2]
	ldr r3, _080363CC @ =0x02017F84
	mov r2, sp
	mov r7, #4
_080363A4:
	ldmia r3!, {r0}
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	strh r0, [r2]
	add r2, #2
	sub r7, #1
	cmp r7, #0
	bge _080363A4
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0xC
	mov r2, sp
	mov r3, #5
	bl sub_080226CC
	mov r0, #0x7C
	b _080364C8
	.align 2, 0
_080363CC: .4byte 0x02017F84
_080363D0:
	mov r3, #1
	mov r9, r3
	mov r7, #0
_080363D6:
	lsl r1, r7, #2
	ldr r0, _08036420 @ =0x02017F84
	add r4, r1, r0
	add r6, r4, #0
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	ldr r0, _08036424 @ =0x020192E0
	ldr r2, _08036428 @ =0x00001B64
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r1, r0
	bne _08036430
	mov r3, r9
	cmp r3, #0
	beq _08036430
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08019820
	mov r0, #1
	ldrb r3, [r5, #2]
	and r0, r3
	mov r3, #0xCB
	cmp r0, #0
	beq _0803640E
	ldr r3, _0803642C @ =0x000080CB
_0803640E:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0
	mov r9, r0
	b _0803648C
_08036420: .4byte 0x02017F84
_08036424: .4byte 0x020192E0
_08036428: .4byte 0x00001B64
_0803642C: .4byte 0x000080CB
_08036430:
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, [r4]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl sub_08019800
	mov r2, #1
	mov r8, r2
	mov r0, r8
	ldrb r3, [r5, #2]
	and r0, r3
	mov r3, #0xD7
	cmp r0, #0
	beq _08036452
	ldr r3, _08036498 @ =0x000080D7
_08036452:
	ldrh r1, [r6]
	ldrh r2, [r6, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r3, [r4]
	lsl r0, r3, #0x14
	lsr r2, r0, #0x14
	ldr r0, _0803649C @ =0x000007FF
	and r0, r2
	lsl r0, r0, #1
	ldr r1, _080364A0 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _080364A4 @ =0x000004DA
	ldrh r0, [r0]
	cmp r0, r1
	bne _0803648C
	lsl r0, r3, #0x13
	lsr r0, r0, #0x1F
	mov r3, r8
	and r0, r3
	lsl r0, r0, #0x1F
	ldr r1, _080364A8 @ =0x3C600000
	orr r2, r1
	orr r0, r2
	mov r1, #0
	bl sub_0801FBCC
_0803648C:
	add r7, #1
	cmp r7, #4
	ble _080363D6
	mov r0, #0x64
	b _080364C8
	.align 2, 0
_08036498: .4byte 0x000080D7
_0803649C: .4byte 0x000007FF
_080364A0: .4byte gUnk_08622AB4
_080364A4: .4byte 0x000004DA
_080364A8: .4byte 0x3C600000
_080364AC:
	mov r0, #1
	ldrb r5, [r5, #2]
	and r0, r5
	mov r1, #0x60
	cmp r0, #0
	beq _080364BA
	ldr r1, _080364D8 @ =0x00008060
_080364BA:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_080364C6:
	mov r0, #0
_080364C8:
	add sp, #0xA0
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080364D8: .4byte 0x00008060
	thumb_func_end sub_08036254

