	thumb_func_start sub_080616D0
sub_080616D0: @ 0x080616D0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r1, _080616FC @ =0x0201CFB0
	mov r0, #6
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #6
	bne _080617CE
	mov r7, #0
_080616EA:
	cmp r7, #0
	beq _08061700
	add r0, r7, #0
	bl sub_0800A368
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08061702
	.align 2, 0
_080616FC: .4byte 0x0201CFB0
_08061700:
	mov r0, #1
_08061702:
	mov sl, r0
	add r0, r7, #0
	mov r1, #0xB
	mov r2, #0
	bl sub_080623EC
	mov r8, r0
	add r0, #0x20
	add r1, r7, #1
	str r1, [sp, #0]
	cmp r0, #0xBF
	bhi _080617C8
	ldr r0, _08061768 @ =0x020192E4
	mov r1, #1
	and r1, r7
	ldr r2, _0806176C @ =0x00000D64
	mul r1, r2
	add r0, r1, r0
	ldrb r6, [r0, #2]
	mov r5, #0
	cmp r5, r6
	bge _080617C8
	mov r9, r1
_08061730:
	ldr r4, _08061770 @ =0x02019968
	add r4, r9
	lsl r0, r5, #2
	add r4, r4, r0
	add r0, r7, #0
	add r1, r5, #0
	add r2, r6, #0
	bl sub_0806236C
	add r3, r0, #0
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	mov r0, sl
	cmp r0, #0
	beq _08061774
	add r0, r4, #0
	str r3, [sp, #4]
	bl sub_08062140
	mov r1, #0x80
	lsl r1, r1, #5
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldr r3, [sp, #4]
	b _08061776
	.align 2, 0
_08061768: .4byte 0x020192E4
_0806176C: .4byte 0x00000D64
_08061770: .4byte 0x02019968
_08061774:
	mov r0, #0x40
_08061776:
	add r2, r0, #0
	cmp r4, #0
	beq _080617C2
	ldr r1, _080617E0 @ =0x020192E0
	ldr r4, _080617E4 @ =0x00001B2C
	add r1, r1, r4
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _080617AC
	ldr r1, _080617E8 @ =0x0201CFB0
	ldr r4, _080617EC @ =0x00000824
	add r0, r1, r4
	ldr r0, [r0]
	cmp r0, r7
	bne _080617AC
	add r4, #4
	add r0, r1, r4
	ldr r0, [r0]
	cmp r0, #0xB
	bne _080617AC
	add r4, #4
	add r0, r1, r4
	ldr r0, [r0]
	cmp r0, r5
	beq _080617C2
_080617AC:
	mov r1, r8
	lsl r0, r1, #0x10
	orr r0, r3
	mov r4, #0x80
	lsl r4, r4, #3
	add r2, r2, r4
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r1, #0x80
	bl sub_080761F0
_080617C2:
	add r5, #1
	cmp r5, r6
	blt _08061730
_080617C8:
	ldr r7, [sp, #0]
	cmp r7, #1
	ble _080616EA
_080617CE:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080617E0: .4byte 0x020192E0
_080617E4: .4byte 0x00001B2C
_080617E8: .4byte 0x0201CFB0
_080617EC: .4byte 0x00000824
	thumb_func_end sub_080616D0

