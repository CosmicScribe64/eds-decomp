	thumb_func_start sub_0804325C
sub_0804325C: @ 0x0804325C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	bl sub_080431E4
	ldr r5, _08043328 @ =0x020192E0
	ldr r1, _0804332C @ =0x00001ACC
	add r4, r5, r1
	lsl r0, r0, #7
	mov r1, #0x7F
	ldrb r2, [r4]
	and r1, r2
	orr r1, r0
	strb r1, [r4]
	bl sub_08043230
	mov r6, #1
	add r1, r6, #0
	and r1, r0
	lsl r1, r1, #6
	mov r0, #0x41
	neg r0, r0
	ldrb r3, [r4]
	and r0, r3
	orr r0, r1
	strb r0, [r4]
	ldr r0, _08043330 @ =0x00001ACD
	add r5, r5, r0
	mov r0, #2
	neg r0, r0
	ldrb r1, [r5]
	and r0, r1
	strb r0, [r5]
	ldr r4, _08043334 @ =0x00000601
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bne _080432BE
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	beq _080432C4
_080432BE:
	ldrb r0, [r5]
	orr r0, r6
	strb r0, [r5]
_080432C4:
	mov r7, #0
	ldr r2, _08043338 @ =0x0201930C
	mov r9, r2
	ldr r3, _0804333C @ =0x000007FF
	mov r8, r3
_080432CE:
	mov r6, #5
	add r0, r7, #1
	str r0, [sp, #0]
	add r0, r7, #0
	mov r1, #1
	and r0, r1
	ldr r3, _08043340 @ =0x00000D64
	add r2, r0, #0
	mul r2, r3
	mov sl, r2
_080432E2:
	mov r0, #0x94
	mul r0, r6
	add r0, sl
	mov r2, r9
	add r1, r0, r2
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	bne _080432F8
	b _080434EA
_080432F8:
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08043304
	b _080434EA
_08043304:
	mov r4, #0
	add r5, r2, #0
	add r0, r5, #0
	mov r3, r8
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08043344 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _08043348
	cmp r0, #0x16
	beq _08043352
	b _08043364
_08043328: .4byte 0x020192E0
_0804332C: .4byte 0x00001ACC
_08043330: .4byte 0x00001ACD
_08043334: .4byte 0x00000601
_08043338: .4byte 0x0201930C
_0804333C: .4byte 0x000007FF
_08043340: .4byte 0x00000D64
_08043344: .4byte gUnk_08621DE0
_08043348:
	mov r1, #0xD5
	lsl r1, r1, #5
	add r1, r9
	mov r0, #0x80
	b _0804335A
_08043352:
	mov r1, #0xD5
	lsl r1, r1, #5
	add r1, r9
	mov r0, #0x40
_0804335A:
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08043364
	mov r4, #1
_08043364:
	add r0, r5, #0
	mov r2, r8
	and r0, r2
	lsl r0, r0, #2
	ldr r3, _0804338C @ =0x08621DE0
	add r0, r0, r3
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08043390
	cmp r0, #0x15
	blt _08043390
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _08043392
_0804338C: .4byte gUnk_08621DE0
_08043390:
	mov r0, #0
_08043392:
	cmp r0, #3
	beq _080433A6
	cmp r0, #3
	bgt _080433A0
	cmp r0, #2
	beq _080433B4
	b _08043406
_080433A0:
	cmp r0, #4
	beq _080433C4
	b _08043406
_080433A6:
	ldr r1, _080433B0 @ =0x0201ADAD
	ldrb r1, [r1]
	and r0, r1
	b _08043400
	.align 2, 0
_080433B0: .4byte 0x0201ADAD
_080433B4:
	mov r0, #4
	ldr r2, _080433C0 @ =0x0201ADAD
	ldrb r2, [r2]
	and r0, r2
	b _08043400
	.align 2, 0
_080433C0: .4byte 0x0201ADAD
_080433C4:
	add r0, r5, #0
	mov r3, r8
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _080433E4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _080433E8
	cmp r0, #0x16
	beq _080433F8
	b _08043406
_080433E4: .4byte gUnk_08621DE0
_080433E8:
	mov r0, #0x10
	ldr r2, _080433F4 @ =0x0201ADAD
	ldrb r2, [r2]
	and r0, r2
	b _08043400
	.align 2, 0
_080433F4: .4byte 0x0201ADAD
_080433F8:
	mov r0, #8
	ldr r3, _0804350C @ =0x0201ADAD
	ldrb r3, [r3]
	and r0, r3
_08043400:
	cmp r0, #0
	beq _08043406
	mov r4, #1
_08043406:
	add r0, r5, #0
	mov r1, r8
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08043510 @ =0x08622AB4
	add r0, r0, r2
	ldr r1, _08043514 @ =0x00000603
	ldrh r0, [r0]
	cmp r0, r1
	bne _0804341C
	mov r4, #0
_0804341C:
	ldr r1, _08043518 @ =0x02015EE8
	mov r3, #1
	add r0, r3, #0
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _08043436
	mov r0, #2
	ldr r1, _0804351C @ =0x0201ADF2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _080434EA
_08043436:
	cmp r4, #0
	bne _08043482
	add r2, r7, #0
	and r2, r3
	mov r0, #0x94
	add r1, r6, #0
	mul r1, r0
	ldr r3, _08043520 @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r1, r1, r0
	ldr r0, _08043524 @ =0x0201930C
	add r1, r1, r0
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0804347E
	mov r0, #0xB1
	cmp r7, #0
	beq _08043464
	ldr r0, _08043528 @ =0x000080B1
_08043464:
	lsl r1, r6, #0x10
	lsr r1, r1, #0x10
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r0, _0804352C @ =0x08085434
	add r1, r7, #0
	add r2, r6, #0
	bl sub_0801A7DC
	bl sub_0801A7E8
_0804347E:
	cmp r4, #0
	beq _080434EA
_08043482:
	mov r0, #0x94
	mul r0, r6
	add r0, sl
	add r0, r9
	add r0, #0x91
	mov r1, #8
	ldrb r0, [r0]
	and r1, r0
	cmp r1, #0
	bne _080434EA
	mov r4, #1
	ldr r0, _08043530 @ =0x08085448
	add r1, r7, #0
	add r2, r6, #0
	bl sub_0801A7DC
	bl sub_0801A7E8
	mov r1, r8
	and r5, r1
	lsl r0, r5, #1
	ldr r2, _08043510 @ =0x08622AB4
	add r0, r0, r2
	ldr r1, _08043534 @ =0x00000409
	ldrh r0, [r0]
	cmp r0, r1
	bne _080434D2
	mov r0, #0
	ldr r1, _08043538 @ =0x000002EF
	bl sub_08008524
	cmp r0, #0
	bne _080434D2
	mov r0, #1
	ldr r1, _08043538 @ =0x000002EF
	bl sub_08008524
	neg r1, r0
	orr r1, r0
	lsr r4, r1, #0x1F
_080434D2:
	cmp r4, #0
	beq _080434EA
	mov r0, #0xB1
	cmp r7, #0
	beq _080434DE
	ldr r0, _08043528 @ =0x000080B1
_080434DE:
	lsl r1, r6, #0x10
	lsr r1, r1, #0x10
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
_080434EA:
	add r6, #1
	cmp r6, #0xA
	bgt _080434F2
	b _080432E2
_080434F2:
	ldr r7, [sp, #0]
	cmp r7, #1
	bgt _080434FA
	b _080432CE
_080434FA:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0804350C: .4byte 0x0201ADAD
_08043510: .4byte gUnk_08622AB4
_08043514: .4byte 0x00000603
_08043518: .4byte 0x02015EE8
_0804351C: .4byte 0x0201ADF2
_08043520: .4byte 0x00000D64
_08043524: .4byte 0x0201930C
_08043528: .4byte 0x000080B1
_0804352C: .4byte gUnk_08085434
_08043530: .4byte gUnk_08085448
_08043534: .4byte 0x00000409
_08043538: .4byte 0x000002EF
	thumb_func_end sub_0804325C

