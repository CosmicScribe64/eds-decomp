	thumb_func_start sub_0803F344
sub_0803F344: @ 0x0803F344
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	ldrb r2, [r5, #2]
	mov r3, #1
	add r0, r3, #0
	and r0, r2
	cmp r0, #0
	beq _0803F37E
	mov r0, #8
	neg r0, r0
	ldrb r1, [r5, #0xA]
	and r0, r1
	strb r0, [r5, #0xA]
	mov r4, #1
	neg r4, r4
	mov r0, #0
	add r1, r4, #0
	mov r2, #1
	bl sub_0805748C
	add r2, r0, #0
	cmp r2, r4
	bgt _0803F374
	b _0803F5D4
_0803F374:
	add r0, r5, #0
	mov r1, #0
	bl sub_0803DDAC
	b _0803F5D4
_0803F37E:
	ldr r0, _0803F3B8 @ =0x02017A40
	ldr r1, _0803F3BC @ =0x000003E5
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0
	beq _0803F38C
	b _0803F4B0
_0803F38C:
	mov r0, #8
	neg r0, r0
	ldrb r1, [r5, #0xA]
	and r0, r1
	strb r0, [r5, #0xA]
	ldr r0, _0803F3C0 @ =0x000007FF
	ldrh r1, [r5]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0803F3C4 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0803F3C8 @ =0x00000403
	cmp r1, r0
	beq _0803F40C
	cmp r1, r0
	bgt _0803F3CC
	mov r0, #0xA0
	lsl r0, r0, #2
	cmp r1, r0
	beq _0803F3E4
	b _0803F468
_0803F3B8: .4byte 0x02017A40
_0803F3BC: .4byte 0x000003E5
_0803F3C0: .4byte 0x000007FF
_0803F3C4: .4byte gUnk_08622AB4
_0803F3C8: .4byte 0x00000403
_0803F3CC:
	ldr r0, _0803F3DC @ =0x0000042C
	cmp r1, r0
	beq _0803F434
	ldr r0, _0803F3E0 @ =0x000005EA
	cmp r1, r0
	beq _0803F434
	b _0803F468
	.align 2, 0
_0803F3DC: .4byte 0x0000042C
_0803F3E0: .4byte 0x000005EA
_0803F3E4:
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r3, r0
	mov r1, #0
	mov r2, #0
	bl sub_080088A4
	cmp r0, #0
	bne _0803F3F8
	b _0803F5D4
_0803F3F8:
	ldr r0, _0803F400 @ =0x00000206
	ldr r1, _0803F404 @ =0x00000712
	ldr r3, _0803F408 @ =0x080841AC
	b _0803F452
_0803F400: .4byte 0x00000206
_0803F404: .4byte 0x00000712
_0803F408: .4byte gUnk_080841AC
_0803F40C:
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r3, r0
	mov r1, #0
	mov r2, #0
	bl sub_080088A4
	cmp r0, #0
	bne _0803F420
	b _0803F5D4
_0803F420:
	ldr r0, _0803F428 @ =0x00000206
	ldr r1, _0803F42C @ =0x00000712
	ldr r3, _0803F430 @ =0x08084200
	b _0803F452
_0803F428: .4byte 0x00000206
_0803F42C: .4byte 0x00000712
_0803F430: .4byte gUnk_08084200
_0803F434:
	ldrb r5, [r5, #2]
	lsl r1, r5, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #1
	mov r2, #0
	bl sub_080088A4
	cmp r0, #0
	bne _0803F44C
	b _0803F5D4
_0803F44C:
	ldr r0, _0803F45C @ =0x00000206
	ldr r1, _0803F460 @ =0x00000712
	ldr r3, _0803F464 @ =0x08084244
_0803F452:
	mov r2, #0xB
	bl sub_080602A4
	b _0803F48C
	.align 2, 0
_0803F45C: .4byte 0x00000206
_0803F460: .4byte 0x00000712
_0803F464: .4byte gUnk_08084244
_0803F468:
	ldrb r5, [r5, #2]
	lsl r1, r5, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0
	mov r2, #0
	bl sub_080088A4
	cmp r0, #0
	bne _0803F480
	b _0803F5D4
_0803F480:
	ldr r0, _0803F49C @ =0x00000206
	ldr r1, _0803F4A0 @ =0x00000712
	ldr r3, _0803F4A4 @ =0x08083DCC
	mov r2, #0xB
	bl sub_080602A4
_0803F48C:
	ldr r0, _0803F4A8 @ =0x02017A40
	ldr r2, _0803F4AC @ =0x000003E5
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0803F5E6
	.align 2, 0
_0803F49C: .4byte 0x00000206
_0803F4A0: .4byte 0x00000712
_0803F4A4: .4byte gUnk_08083DCC
_0803F4A8: .4byte 0x02017A40
_0803F4AC: .4byte 0x000003E5
_0803F4B0:
	ldr r0, _0803F4D8 @ =0x000007FF
	ldrh r1, [r5]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0803F4DC @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _0803F4E0 @ =0x0000042C
	cmp r1, r0
	beq _0803F506
	cmp r1, r0
	bgt _0803F4E8
	mov r0, #0xA0
	lsl r0, r0, #2
	cmp r1, r0
	beq _0803F500
	ldr r0, _0803F4E4 @ =0x00000403
	cmp r1, r0
	beq _0803F500
	b _0803F50A
_0803F4D8: .4byte 0x000007FF
_0803F4DC: .4byte gUnk_08622AB4
_0803F4E0: .4byte 0x0000042C
_0803F4E4: .4byte 0x00000403
_0803F4E8:
	ldr r0, _0803F4F8 @ =0x000004DC
	cmp r1, r0
	beq _0803F506
	ldr r0, _0803F4FC @ =0x000005EA
	cmp r1, r0
	beq _0803F506
	b _0803F50A
	.align 2, 0
_0803F4F8: .4byte 0x000004DC
_0803F4FC: .4byte 0x000005EA
_0803F500:
	mov r4, #0xF0
	lsl r4, r4, #0x10
	b _0803F50A
_0803F506:
	mov r4, #0xE0
	lsl r4, r4, #0x10
_0803F50A:
	add r0, r4, #0
	bl sub_08052F38
	cmp r0, #0
	beq _0803F5E6
	ldr r0, _0803F570 @ =0x0201CFB0
	ldr r2, _0803F574 @ =0x00000824
	add r1, r0, r2
	ldr r7, [r1]
	add r2, #4
	add r1, r0, r2
	add r2, #4
	add r0, r0, r2
	ldr r1, [r1]
	ldr r0, [r0]
	add r6, r1, r0
	mov r2, #1
	and r2, r7
	mov r0, #0x94
	mul r0, r6
	ldr r1, _0803F578 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803F57C @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	ldrh r0, [r5]
	add r1, r7, #0
	add r2, r6, #0
	bl sub_0802B1B8
	cmp r0, #0
	beq _0803F5E0
	ldr r2, _0803F580 @ =0x000007FF
	add r0, r2, #0
	ldrh r1, [r5]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0803F584 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0803F588 @ =0x0000042C
	cmp r1, r0
	beq _0803F58C
	add r0, #0xB0
	cmp r1, r0
	beq _0803F5AC
	b _0803F5CA
	.align 2, 0
_0803F570: .4byte 0x0201CFB0
_0803F574: .4byte 0x00000824
_0803F578: .4byte 0x00000D64
_0803F57C: .4byte 0x0201930C
_0803F580: .4byte 0x000007FF
_0803F584: .4byte gUnk_08622AB4
_0803F588: .4byte 0x0000042C
_0803F58C:
	and r4, r2
	lsl r0, r4, #1
	ldr r2, _0803F5A4 @ =0x08622AB4
	add r0, r0, r2
	ldr r1, _0803F5A8 @ =0x00000547
	ldrh r0, [r0]
	cmp r0, r1
	bne _0803F5AC
_0803F59C:
	mov r0, #3
	bl sub_08077AEC
	b _0803F5E6
_0803F5A4: .4byte gUnk_08622AB4
_0803F5A8: .4byte 0x00000547
_0803F5AC:
	mov r2, #1
	and r2, r7
	mov r0, #0x94
	add r1, r6, #0
	mul r1, r0
	ldr r0, _0803F5D8 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0803F5DC @ =0x0201930C
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803F59C
_0803F5CA:
	add r0, r5, #0
	add r1, r7, #0
	add r2, r6, #0
	bl sub_0803DDAC
_0803F5D4:
	mov r0, #1
	b _0803F5E8
_0803F5D8: .4byte 0x00000D64
_0803F5DC: .4byte 0x0201930C
_0803F5E0:
	mov r0, #3
	bl sub_08077AEC
_0803F5E6:
	mov r0, #0
_0803F5E8:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0803F344
	.align 2, 0

