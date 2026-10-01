	thumb_func_start sub_08024134
sub_08024134: @ 0x08024134
	push {r4, r5, r6, lr}
	add r6, r0, #0
	add r3, r1, #0
	ldr r1, _08024174 @ =0x0201CFB0
	ldr r4, _08024178 @ =0x00000824
	add r0, r1, r4
	str r6, [r0]
	ldr r0, _0802417C @ =0x00000828
	add r5, r1, r0
	str r3, [r5]
	add r0, #4
	add r4, r1, r0
	str r2, [r4]
	cmp r3, #0
	bne _0802416C
	cmp r2, #0xA
	bne _0802415A
	str r2, [r5]
	str r3, [r4]
_0802415A:
	ldr r1, [r4]
	cmp r1, #4
	ble _08024168
	mov r0, #5
	str r0, [r5]
	sub r0, r1, #5
	str r0, [r4]
_08024168:
	cmp r3, #0
	beq _08024170
_0802416C:
	cmp r3, #5
	bne _08024180
_08024170:
	add r4, r3, r2
	b _08024182
_08024174: .4byte 0x0201CFB0
_08024178: .4byte 0x00000824
_0802417C: .4byte 0x00000828
_08024180:
	add r4, r3, #0
_08024182:
	add r0, r6, #0
	add r1, r3, #0
	bl sub_080623AC
	ldr r3, _080241B4 @ =0x081A42A4
	lsl r1, r4, #3
	lsl r2, r6, #7
	add r1, r1, r2
	add r3, #4
	add r1, r1, r3
	ldr r1, [r1]
	bl sub_080240D4
	ldr r1, _080241B8 @ =0x0201CFB0
	ldr r2, _080241BC @ =0x00000824
	add r0, r1, r2
	ldr r0, [r0]
	ldr r4, _080241C0 @ =0x00000828
	add r1, r1, r4
	ldr r1, [r1]
	bl sub_080240A8
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_080241B4: .4byte gUnk_081A42A4
_080241B8: .4byte 0x0201CFB0
_080241BC: .4byte 0x00000824
_080241C0: .4byte 0x00000828
	thumb_func_end sub_08024134

