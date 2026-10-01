	thumb_func_start sub_08028AEC
sub_08028AEC: @ 0x08028AEC
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r1, _08028CD4 @ =0xFFFFFE80
	ldr r5, _08028CD8 @ =0x02020E08
	mov r0, #0
	mov r2, #0
	add r3, r5, #0
	bl sub_080787F4
	mov r0, #0xC0
	lsl r0, r0, #0x13
	ldr r1, _08028CDC @ =0x086A12EC
	mov r2, #0x96
	lsl r2, r2, #8
	bl sub_08075294
	mov r0, #0xA0
	lsl r0, r0, #0x13
	ldr r1, _08028CE0 @ =0x086AA8EC
	mov r2, #0x80
	lsl r2, r2, #2
	bl sub_08075294
	ldr r0, _08028CE4 @ =0x05000200
	ldr r1, _08028CE8 @ =0x086AAAEC
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _08028CEC @ =0x05000220
	ldr r1, _08028CF0 @ =0x086AAB00
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _08028CF4 @ =0x05000240
	ldr r1, _08028CF8 @ =0x086AAB20
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _08028CFC @ =0x05000260
	ldr r1, _08028D00 @ =0x086AAB40
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _08028D04 @ =0x05000280
	ldr r1, _08028D08 @ =0x086AAB60
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _08028D0C @ =0x050002A0
	ldr r1, _08028D10 @ =0x086AABA0
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _08028D14 @ =0x050002C0
	ldr r1, _08028D18 @ =0x086AABBC
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _08028D1C @ =0x050002E0
	ldr r1, _08028D20 @ =0x086AABDC
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _08028D24 @ =0x05000320
	ldr r1, _08028D28 @ =0x086AAC00
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _08028D2C @ =0x05000340
	ldr r1, _08028D30 @ =0x086AAB80
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _08028D34 @ =0x050003A0
	ldr r1, _08028D38 @ =0x086AAC20
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _08028D3C @ =0x086AAC28
	mov r1, #0
	mov r2, #4
	mov r3, #8
	bl sub_08028AB8
	ldr r0, _08028D40 @ =0x086AB028
	mov r1, #4
	mov r2, #4
	mov r3, #8
	bl sub_08028AB8
	ldr r0, _08028D44 @ =0x086AB428
	mov r1, #8
	mov r2, #4
	mov r3, #8
	bl sub_08028AB8
	ldr r0, _08028D48 @ =0x086AB828
	mov r4, #0x80
	lsl r4, r4, #1
	add r1, r4, #0
	mov r2, #0x10
	mov r3, #4
	bl sub_08028AB8
	ldr r0, _08028D4C @ =0x086AC028
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r2, #0x10
	mov r3, #4
	bl sub_08028AB8
	ldr r0, _08028D50 @ =0x086AE028
	mov r1, #0x10
	mov r2, #0x10
	mov r3, #4
	bl sub_08028AB8
	ldr r0, _08028D54 @ =0x086AE828
	mov r1, #0x90
	mov r2, #0x10
	mov r3, #4
	bl sub_08028AB8
	ldr r0, _08028D58 @ =0x086AF828
	mov r1, #0x88
	lsl r1, r1, #1
	mov r2, #4
	mov r3, #4
	bl sub_08028AB8
	ldr r0, _08028D5C @ =0x086AF028
	mov r1, #0xC8
	lsl r1, r1, #1
	mov r2, #0x10
	mov r3, #4
	bl sub_08028AB8
	mov r2, #0
	ldr r0, _08028D60 @ =0xFFFFF508
	add r3, r5, r0
	mov r1, #0xC3
	lsl r1, r1, #3
	mov r8, r1
	mov ip, r2
	add r5, r3, #0
	ldr r7, _08028D64 @ =0x0000061A
	mov r9, r7
	ldr r6, _08028D68 @ =0x0000061C
_08028C18:
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #3
	add r0, r0, r3
	mov r7, r8
	add r1, r0, r7
	strh r4, [r1]
	mov r7, r9
	add r1, r0, r7
	strh r4, [r1]
	add r0, r0, r6
	mov r1, ip
	strh r1, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #4
	bls _08028C18
	mov r2, #0
	ldr r7, _08028D6C @ =0x02020310
	mov ip, r7
	ldr r7, _08028D70 @ =0x00000AAD
	mov r3, #0
	ldr r6, _08028D74 @ =0x00000AAC
	ldr r4, _08028D78 @ =0x00000AAE
_08028C4A:
	lsl r0, r2, #2
	add r0, ip
	add r1, r0, r7
	strb r3, [r1]
	add r1, r0, r6
	strb r3, [r1]
	add r0, r0, r4
	strb r3, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #3
	bls _08028C4A
	mov r2, #0
	ldr r4, _08028D6C @ =0x02020310
	ldr r3, _08028D7C @ =0x00000AC4
	mov r1, #0
_08028C6C:
	lsl r0, r2, #2
	add r0, r0, r4
	add r0, r0, r3
	strh r1, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #1
	bls _08028C6C
	ldr r1, _08028D80 @ =0x00000AD4
	add r0, r5, r1
	mov r2, #0
	strb r2, [r0]
	ldr r7, _08028D84 @ =0x00000AD5
	add r0, r5, r7
	strb r2, [r0]
	ldr r0, _08028D88 @ =0x00000ABC
	add r1, r5, r0
	mov r0, #0xFF
	strb r0, [r1]
	ldr r1, _08028D8C @ =0x00000ABD
	add r0, r5, r1
	strb r2, [r0]
	sub r7, #0x15
	add r0, r5, r7
	mov r3, #0
	strh r2, [r0]
	ldr r0, _08028D90 @ =0x00000ACE
	add r1, r5, r0
	mov r0, #0xF4
	strb r0, [r1]
	ldr r1, _08028D94 @ =0x00000ACF
	add r0, r5, r1
	strb r3, [r0]
	add r7, #0x10
	add r0, r5, r7
	strh r2, [r0]
	add r1, #3
	add r0, r5, r1
	strh r2, [r0]
	mov r1, #0x80
	lsl r1, r1, #0x13
	ldr r2, _08028D98 @ =0x00001F04
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #1
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08028CD4: .4byte 0xFFFFFE80
_08028CD8: .4byte 0x02020E08
_08028CDC: .4byte gUnk_086A12EC
_08028CE0: .4byte gUnk_086AA8EC
_08028CE4: .4byte 0x05000200
_08028CE8: .4byte gUnk_086AAAEC
_08028CEC: .4byte 0x05000220
_08028CF0: .4byte gUnk_086AAB00
_08028CF4: .4byte 0x05000240
_08028CF8: .4byte gUnk_086AAB20
_08028CFC: .4byte 0x05000260
_08028D00: .4byte gUnk_086AAB40
_08028D04: .4byte 0x05000280
_08028D08: .4byte gUnk_086AAB60
_08028D0C: .4byte 0x050002A0
_08028D10: .4byte gUnk_086AABA0
_08028D14: .4byte 0x050002C0
_08028D18: .4byte gUnk_086AABBC
_08028D1C: .4byte 0x050002E0
_08028D20: .4byte gUnk_086AABDC
_08028D24: .4byte 0x05000320
_08028D28: .4byte gUnk_086AAC00
_08028D2C: .4byte 0x05000340
_08028D30: .4byte gUnk_086AAB80
_08028D34: .4byte 0x050003A0
_08028D38: .4byte gUnk_086AAC20
_08028D3C: .4byte gUnk_086AAC28
_08028D40: .4byte gUnk_086AB028
_08028D44: .4byte gUnk_086AB428
_08028D48: .4byte gUnk_086AB828
_08028D4C: .4byte gUnk_086AC028
_08028D50: .4byte gUnk_086AE028
_08028D54: .4byte gUnk_086AE828
_08028D58: .4byte gUnk_086AF828
_08028D5C: .4byte gUnk_086AF028
_08028D60: .4byte 0xFFFFF508
_08028D64: .4byte 0x0000061A
_08028D68: .4byte 0x0000061C
_08028D6C: .4byte 0x02020310
_08028D70: .4byte 0x00000AAD
_08028D74: .4byte 0x00000AAC
_08028D78: .4byte 0x00000AAE
_08028D7C: .4byte 0x00000AC4
_08028D80: .4byte 0x00000AD4
_08028D84: .4byte 0x00000AD5
_08028D88: .4byte 0x00000ABC
_08028D8C: .4byte 0x00000ABD
_08028D90: .4byte 0x00000ACE
_08028D94: .4byte 0x00000ACF
_08028D98: .4byte 0x00001F04
	thumb_func_end sub_08028AEC

