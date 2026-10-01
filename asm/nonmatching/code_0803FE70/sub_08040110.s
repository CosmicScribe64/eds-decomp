	thumb_func_start sub_08040110
sub_08040110: @ 0x08040110
	push {r4, r5, r6, r7, lr}
	sub sp, #0x80
	add r4, r0, #0
	add r7, r1, #0
	ldr r0, _08040168 @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0804016C @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08040170 @ =0x00000525
	ldrh r0, [r0]
	cmp r0, r1
	bne _08040198
	mov r0, #0xFC
	ldrb r1, [r4, #3]
	and r0, r1
	cmp r0, #0x40
	bne _08040198
	ldr r0, _08040174 @ =0x02017A40
	ldr r2, _08040178 @ =0x000003E5
	add r5, r0, r2
	ldrb r0, [r5]
	cmp r0, #0
	bne _0804017C
	mov r0, #8
	neg r0, r0
	ldrb r1, [r4, #0xA]
	and r0, r1
	strb r0, [r4, #0xA]
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldrh r4, [r4, #8]
	lsr r2, r4, #8
	mov r1, #0x14
	mov r3, #0
	bl sub_08022678
_0804015E:
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _0804028A
	.align 2, 0
_08040168: .4byte 0x000007FF
_0804016C: .4byte gUnk_08622AB4
_08040170: .4byte 0x00000525
_08040174: .4byte 0x02017A40
_08040178: .4byte 0x000003E5
_0804017C:
	ldrb r0, [r4, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	ldr r0, _08040190 @ =0x020192E0
	ldr r2, _08040194 @ =0x00001B64
	add r0, r0, r2
	ldrh r2, [r0]
	add r0, r4, #0
	b _0804026C
	.align 2, 0
_08040190: .4byte 0x020192E0
_08040194: .4byte 0x00001B64
_08040198:
	ldr r0, _080401CC @ =0x02017A40
	ldr r1, _080401D0 @ =0x000003E5
	add r5, r0, r1
	ldrb r0, [r5]
	cmp r0, #0
	bne _080401E4
	ldr r1, _080401D4 @ =0x08084658
	ldrh r7, [r7]
	lsl r2, r7, #6
	ldr r0, _080401D8 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl sub_080753F4
	ldr r0, _080401DC @ =0x00000206
	ldr r1, _080401E0 @ =0x00000712
	mov r2, #0xB
	mov r3, sp
	bl sub_080602A4
	mov r0, #8
	neg r0, r0
	ldrb r1, [r4, #0xA]
	and r0, r1
	strb r0, [r4, #0xA]
	b _0804015E
_080401CC: .4byte 0x02017A40
_080401D0: .4byte 0x000003E5
_080401D4: .4byte gUnk_08084658
_080401D8: .4byte gUnk_0822C720
_080401DC: .4byte 0x00000206
_080401E0: .4byte 0x00000712
_080401E4:
	ldr r0, _08040200 @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08040204 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08040208 @ =0x00000431
	cmp r1, r0
	beq _0804020C
	add r0, #0xF4
	cmp r1, r0
	beq _08040214
	b _08040216
_08040200: .4byte 0x000007FF
_08040204: .4byte gUnk_08622AB4
_08040208: .4byte 0x00000431
_0804020C:
	ldr r2, _08040210 @ =0x00F000F0
	b _08040216
_08040210: .4byte 0x00F000F0
_08040214:
	mov r2, #0xF0
_08040216:
	add r0, r2, #0
	bl sub_08052F38
	cmp r0, #0
	beq _0804028A
	ldr r0, _08040274 @ =0x0201CFB0
	ldr r2, _08040278 @ =0x00000824
	add r1, r0, r2
	ldr r6, [r1]
	add r2, #4
	add r1, r0, r2
	add r2, #4
	add r0, r0, r2
	ldr r1, [r1]
	ldr r0, [r0]
	add r5, r1, r0
	ldr r0, _0804027C @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08040280 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	add r1, r7, #0
	add r2, r6, #0
	add r3, r5, #0
	bl sub_0802F5F8
	cmp r0, #0
	beq _08040284
	mov r0, #1
	sub r0, r0, r6
	lsl r0, r0, #0x18
	lsl r1, r5, #0x18
	lsr r0, r0, #8
	orr r0, r1
	lsr r0, r0, #0x10
	ldrh r7, [r7, #0xC]
	cmp r7, r0
	beq _08040284
	add r0, r4, #0
	add r1, r6, #0
	add r2, r5, #0
_0804026C:
	bl sub_0803DDAC
	mov r0, #1
	b _0804028C
_08040274: .4byte 0x0201CFB0
_08040278: .4byte 0x00000824
_0804027C: .4byte 0x000007FF
_08040280: .4byte gUnk_08622AB4
_08040284:
	mov r0, #3
	bl sub_08077AEC
_0804028A:
	mov r0, #0
_0804028C:
	add sp, #0x80
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08040110

