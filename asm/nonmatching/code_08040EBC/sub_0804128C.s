	thumb_func_start sub_0804128C
sub_0804128C: @ 0x0804128C
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	ldr r0, _080412A4 @ =0x02017A40
	ldr r1, _080412A8 @ =0x000003E5
	add r2, r0, r1
	ldrb r0, [r2]
	cmp r0, #0
	beq _080412AC
	cmp r0, #1
	beq _08041340
	b _08041394
	.align 2, 0
_080412A4: .4byte 0x02017A40
_080412A8: .4byte 0x000003E5
_080412AC:
	mov r0, #8
	neg r0, r0
	ldrb r2, [r5, #0xA]
	and r0, r2
	strb r0, [r5, #0xA]
	mov r3, #0
	mov ip, r3
	mov r4, #0
	ldr r7, _08041324 @ =0x0201930C
	ldrb r5, [r5, #2]
	lsl r5, r5, #0x1F
	mov r3, #1
	ldr r6, _08041328 @ =0x00000D64
_080412C6:
	lsr r1, r5, #0x1F
	sub r1, r3, r1
	and r1, r3
	mov r0, #0x94
	add r2, r4, #0
	mul r2, r0
	add r0, r1, #0
	mul r0, r6
	add r0, r2, r0
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080412FE
	lsr r0, r5, #0x1F
	sub r0, r3, r0
	and r0, r3
	add r1, r0, #0
	mul r1, r6
	add r1, r2, r1
	add r1, r1, r7
	mov r0, #3
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #1
	bne _080412FE
	mov r0, #1
	mov ip, r0
_080412FE:
	add r4, #1
	cmp r4, #4
	ble _080412C6
	mov r1, ip
	cmp r1, #0
	beq _08041386
	ldr r0, _0804132C @ =0x00000206
	ldr r1, _08041330 @ =0x00000712
	ldr r3, _08041334 @ =0x08084AA8
	mov r2, #0xB
	bl sub_080602A4
	ldr r0, _08041338 @ =0x02017A40
	ldr r2, _0804133C @ =0x000003E5
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _08041394
_08041324: .4byte 0x0201930C
_08041328: .4byte 0x00000D64
_0804132C: .4byte 0x00000206
_08041330: .4byte 0x00000712
_08041334: .4byte gUnk_08084AA8
_08041338: .4byte 0x02017A40
_0804133C: .4byte 0x000003E5
_08041340:
	ldr r1, _08041354 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08041358
	mov r0, #0
	strb r0, [r2]
	b _08041396
	.align 2, 0
_08041354: .4byte 0x03000040
_08041358:
	mov r0, #0x90
	lsl r0, r0, #0x10
	bl sub_08052F38
	cmp r0, #0
	beq _08041394
	ldr r0, _0804138C @ =0x0201CFB0
	ldr r3, _08041390 @ =0x00000824
	add r1, r0, r3
	ldr r1, [r1]
	add r3, #4
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r5, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08041394
_08041386:
	mov r0, #1
	b _08041396
	.align 2, 0
_0804138C: .4byte 0x0201CFB0
_08041390: .4byte 0x00000824
_08041394:
	mov r0, #0
_08041396:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0804128C

