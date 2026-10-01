	thumb_func_start sub_08042BE0
sub_08042BE0: @ 0x08042BE0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x100
	ldr r2, _08042C00 @ =0x02017A40
	ldr r1, _08042C04 @ =0x00000492
	add r0, r2, r1
	ldrb r0, [r0]
	lsr r0, r0, #1
	add r4, r2, #0
	cmp r0, #5
	bls _08042BF6
	b _0804319C
_08042BF6:
	lsl r0, r0, #2
	ldr r1, _08042C08 @ =0x08042C0C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08042C00: .4byte 0x02017A40
_08042C04: .4byte 0x00000492
_08042C08: .4byte 0x08042C0C
_08042C0C:
	.4byte _08042C24
	.4byte _08042CC4
	.4byte _08042D6C
	.4byte _08042F5C
	.4byte _08043104
	.4byte _0804314C
_08042C24:
	ldr r2, _08042C5C @ =0x00000493
	add r1, r4, r2
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08042C88
	mov r3, #0x9A
	lsl r3, r3, #3
	add r0, r4, r3
	ldrh r2, [r0]
	ldr r0, _08042C60 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r4, _08042C64 @ =0x08621DE0
	add r0, r0, r4
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08042C70
	ldr r1, _08042C68 @ =0x08085330
	lsl r2, r2, #6
	ldr r5, _08042C6C @ =0x0822C720
	add r2, r2, r5
	b _08042C78
_08042C5C: .4byte 0x00000493
_08042C60: .4byte 0x000007FF
_08042C64: .4byte gUnk_08621DE0
_08042C68: .4byte gUnk_08085330
_08042C6C: .4byte gUnk_0822C720
_08042C70:
	ldr r1, _08042C80 @ =0x08085374
	lsl r2, r2, #6
	ldr r7, _08042C84 @ =0x0822C720
	add r2, r2, r7
_08042C78:
	mov r0, sp
	bl sub_080753F4
	b _08042C92
_08042C80: .4byte gUnk_08085374
_08042C84: .4byte gUnk_0822C720
_08042C88:
	ldr r1, _08042CB4 @ =0x000004BC
	add r0, r4, r1
	mov r1, sp
	bl sub_0804218C
_08042C92:
	mov r0, #0x81
	lsl r0, r0, #2
	ldr r1, _08042CB8 @ =0x00000916
	mov r2, #0xB
	mov r3, sp
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	ldr r2, _08042CBC @ =0x02017A40
	ldr r3, _08042CC0 @ =0x00000492
	add r2, r2, r3
	b _080430DE
	.align 2, 0
_08042CB4: .4byte 0x000004BC
_08042CB8: .4byte 0x00000916
_08042CBC: .4byte 0x02017A40
_08042CC0: .4byte 0x00000492
_08042CC4:
	ldr r0, _08042CDC @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	bne _08042CE4
	ldr r0, _08042CE0 @ =0x0000F055
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	b _080431D0
	.align 2, 0
_08042CDC: .4byte 0x0201AE60
_08042CE0: .4byte 0x0000F055
_08042CE4:
	ldr r1, _08042D04 @ =0x02017A40
	ldr r4, _08042D08 @ =0x00000493
	add r1, r1, r4
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08042D18
	ldr r0, _08042D0C @ =0x00000206
	ldr r1, _08042D10 @ =0x00000712
	ldr r3, _08042D14 @ =0x080853AC
	mov r2, #0xB
	bl sub_080602A4
	b _08042D24
	.align 2, 0
_08042D04: .4byte 0x02017A40
_08042D08: .4byte 0x00000493
_08042D0C: .4byte 0x00000206
_08042D10: .4byte 0x00000712
_08042D14: .4byte gUnk_080853AC
_08042D18:
	ldr r0, _08042D50 @ =0x00000206
	ldr r1, _08042D54 @ =0x00000712
	ldr r3, _08042D58 @ =0x080853F8
	mov r2, #0xB
	bl sub_080602A4
_08042D24:
	ldr r2, _08042D5C @ =0x02017A40
	ldr r5, _08042D60 @ =0x00000492
	add r2, r2, r5
	ldrb r3, [r2]
	lsr r1, r3, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	ldr r2, _08042D64 @ =0x020192E0
	ldr r7, _08042D68 @ =0x00001B2C
	add r2, r2, r7
	mov r0, #2
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	neg r1, r1
	and r0, r1
	b _080430EC
_08042D50: .4byte 0x00000206
_08042D54: .4byte 0x00000712
_08042D58: .4byte gUnk_080853F8
_08042D5C: .4byte 0x02017A40
_08042D60: .4byte 0x00000492
_08042D64: .4byte 0x020192E0
_08042D68: .4byte 0x00001B2C
_08042D6C:
	ldr r0, _08042D88 @ =0x020192E0
	ldr r2, _08042D8C @ =0x00001B2C
	add r0, r0, r2
	ldrb r1, [r0]
	mov r5, #1
	add r0, r5, #0
	and r0, r1
	cmp r0, #0
	beq _08042D90
	bl sub_0801DC04
_08042D82:
	mov r0, #0
	b _080431D2
	.align 2, 0
_08042D88: .4byte 0x020192E0
_08042D8C: .4byte 0x00001B2C
_08042D90:
	mov r2, #2
	add r0, r2, #0
	and r0, r1
	cmp r0, #0
	beq _08042DB0
	ldr r7, _08042DAC @ =0x00000492
	add r3, r4, r7
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	add r0, r5, #0
	b _0804318E
	.align 2, 0
_08042DAC: .4byte 0x00000492
_08042DB0:
	ldr r1, _08042DCC @ =0x03000040
	add r0, r2, #0
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08042DD4
	ldr r0, _08042DD0 @ =0x00000492
	add r1, r4, r0
	add r0, r5, #0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	b _08042D82
	.align 2, 0
_08042DCC: .4byte 0x03000040
_08042DD0: .4byte 0x00000492
_08042DD4:
	mov r0, #0xEE
	bl sub_08052F38
	cmp r0, #0
	beq _08042D82
	ldr r0, _08042E08 @ =0x0201CFB0
	ldr r3, _08042E0C @ =0x00000824
	add r1, r0, r3
	ldr r5, [r1]
	ldr r4, _08042E10 @ =0x00000828
	add r1, r0, r4
	ldr r4, [r1]
	ldr r7, _08042E14 @ =0x0000082C
	add r0, r0, r7
	ldr r7, [r0]
	bl sub_0805ECFC
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	cmp r4, #0xF
	bhi _08042D82
	lsl r0, r4, #2
	ldr r1, _08042E18 @ =0x08042E1C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08042E08: .4byte 0x0201CFB0
_08042E0C: .4byte 0x00000824
_08042E10: .4byte 0x00000828
_08042E14: .4byte 0x0000082C
_08042E18: .4byte 0x08042E1C
_08042E1C:
	.4byte _08042E5C
	.4byte _08042D82
	.4byte _08042D82
	.4byte _08042D82
	.4byte _08042D82
	.4byte _08042E5C
	.4byte _08042D82
	.4byte _08042D82
	.4byte _08042D82
	.4byte _08042D82
	.4byte _08042EF0
	.4byte _08042EF0
	.4byte _08042F48
	.4byte _08042F44
	.4byte _08042F48
	.4byte _08042F48
_08042E5C:
	cmp r2, #0
	beq _08042EEC
	ldr r1, _08042EAC @ =0x020192E0
	ldr r0, _08042EB0 @ =0x00001B2C
	add r6, r1, r0
	mov r0, #1
	ldrb r2, [r6]
	orr r0, r2
	strb r0, [r6]
	ldr r3, _08042EB4 @ =0x00001B2F
	add r2, r1, r3
	mov r0, #3
	ldrb r3, [r2]
	and r0, r3
	strb r0, [r2]
	ldr r0, _08042EB8 @ =0x00001B30
	add r1, r1, r0
	mov r0, #4
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r2, _08042EBC @ =0x02017A40
	ldr r3, _08042EC0 @ =0x00000493
	add r1, r2, r3
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08042EC4
	mov r1, #0x9A
	lsl r1, r1, #3
	add r0, r2, r1
	add r1, r5, #0
	add r2, r4, #0
	add r3, r7, #0
	bl sub_0801FD68
	b _08042ED2
	.align 2, 0
_08042EAC: .4byte 0x020192E0
_08042EB0: .4byte 0x00001B2C
_08042EB4: .4byte 0x00001B2F
_08042EB8: .4byte 0x00001B30
_08042EBC: .4byte 0x02017A40
_08042EC0: .4byte 0x00000493
_08042EC4:
	ldr r3, _08042EE4 @ =0x000004BC
	add r0, r2, r3
	add r1, r5, #0
	add r2, r4, #0
	add r3, r7, #0
	bl sub_08042078
_08042ED2:
	lsl r0, r0, #0x10
	lsr r0, r0, #6
	ldr r1, [r6]
	ldr r2, _08042EE8 @ =0xFC0003FF
	and r1, r2
	orr r1, r0
	str r1, [r6]
	b _08042D82
	.align 2, 0
_08042EE4: .4byte 0x000004BC
_08042EE8: .4byte 0xFC0003FF
_08042EEC:
	mov r0, #3
	b _08042F56
_08042EF0:
	cmp r2, #0
	beq _08042F40
	ldr r1, _08042F2C @ =0x020192E0
	ldr r4, _08042F30 @ =0x00001B2C
	add r3, r1, r4
	mov r0, #1
	ldrb r5, [r3]
	orr r0, r5
	strb r0, [r3]
	ldr r7, _08042F34 @ =0x00001B2F
	add r2, r1, r7
	mov r0, #3
	ldrb r4, [r2]
	and r0, r4
	strb r0, [r2]
	ldr r5, _08042F38 @ =0x00001B30
	add r1, r1, r5
	mov r0, #4
	neg r0, r0
	ldrb r7, [r1]
	and r0, r7
	strb r0, [r1]
	ldr r0, [r3]
	ldr r1, _08042F3C @ =0xFC0003FF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #3
	orr r0, r1
	str r0, [r3]
	b _08042D82
_08042F2C: .4byte 0x020192E0
_08042F30: .4byte 0x00001B2C
_08042F34: .4byte 0x00001B2F
_08042F38: .4byte 0x00001B30
_08042F3C: .4byte 0xFC0003FF
_08042F40:
	mov r0, #3
	b _08042F56
_08042F44:
	mov r0, #3
	b _08042F56
_08042F48:
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802AF34
	mov r0, #1
_08042F56:
	bl sub_08077AEC
	b _08042D82
_08042F5C:
	ldr r7, _08043074 @ =0x020192E0
	ldr r0, _08043078 @ =0x00001B2C
	add r1, r7, r0
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r1, _0804307C @ =0x02017A40
	ldr r3, _08043080 @ =0x00000493
	add r1, r1, r3
	mov r6, #1
	add r0, r6, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08042F8A
	mov r0, #7
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08042F8A:
	ldr r4, _08043084 @ =0x00001B33
	add r0, r7, r4
	ldrb r5, [r0]
	lsl r0, r5, #0x1E
	lsr r0, r0, #0x1F
	add r3, r6, #0
	and r3, r0
	ldr r0, _08043088 @ =0x00001B34
	add r2, r7, r0
	ldrh r1, [r2]
	lsl r1, r1, #0x17
	mov ip, r1
	lsr r1, r1, #0x18
	lsr r4, r5, #2
	add r0, r6, #0
	ldrb r2, [r2]
	and r0, r2
	lsl r2, r0, #6
	orr r2, r4
	add r1, r1, r2
	mov r0, #0x94
	mul r1, r0
	ldr r0, _0804308C @ =0x00000D64
	mul r0, r3
	add r1, r1, r0
	add r0, r7, #0
	add r0, #0x2C
	add r1, r1, r0
	mov r3, #2
	add r0, r3, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08042FEA
	add r0, r3, #0
	and r0, r5
	mov r3, #0x7F
	cmp r0, #0
	beq _08042FDA
	ldr r3, _08043090 @ =0x0000807F
_08042FDA:
	mov r4, ip
	lsr r1, r4, #0x18
	add r1, r1, r2
	add r0, r3, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08042FEA:
	ldr r6, _0804307C @ =0x02017A40
	ldr r4, _08043074 @ =0x020192E0
	ldr r5, _08043084 @ =0x00001B33
	add r0, r4, r5
	ldrb r2, [r0]
	lsr r5, r2, #2
	ldr r7, _08043088 @ =0x00001B34
	add r0, r4, r7
	mov r3, #1
	add r1, r3, #0
	ldrb r7, [r0]
	and r1, r7
	lsl r1, r1, #6
	orr r1, r5
	ldrh r5, [r0]
	lsr r0, r5, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	add r1, r1, r0
	ldr r7, _08043094 @ =0x000004BE
	add r5, r6, r7
	mov r0, #0x3F
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _08043098 @ =0xFFFFFC0F
	ldrh r7, [r5]
	and r0, r7
	orr r0, r1
	strh r0, [r5]
	lsl r2, r2, #0x1E
	lsr r2, r2, #0x1F
	and r3, r2
	lsl r0, r0, #0x16
	lsr r0, r0, #0x1A
	mov r1, #0x94
	mul r0, r1
	ldr r1, _0804308C @ =0x00000D64
	mul r1, r3
	add r0, r0, r1
	add r4, #0x2C
	add r0, r0, r4
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	ldr r2, _0804309C @ =0x000004BC
	add r1, r6, r2
	mov r7, #0
	strh r0, [r1]
	mov r0, #2
	neg r0, r0
	ldrb r3, [r5]
	and r0, r3
	strb r0, [r5]
	ldrh r0, [r1]
	bl sub_08047058
	add r4, r0, #0
	mov r0, #1
	neg r0, r0
	cmp r4, r0
	bne _080430A4
	mov r4, #0x90
	lsl r4, r4, #3
	add r0, r6, r4
	str r7, [r0]
	ldr r5, _080430A0 @ =0x00000484
	add r0, r6, r5
	str r7, [r0]
	b _080430C8
_08043074: .4byte 0x020192E0
_08043078: .4byte 0x00001B2C
_0804307C: .4byte 0x02017A40
_08043080: .4byte 0x00000493
_08043084: .4byte 0x00001B33
_08043088: .4byte 0x00001B34
_0804308C: .4byte 0x00000D64
_08043090: .4byte 0x0000807F
_08043094: .4byte 0x000004BE
_08043098: .4byte 0xFFFFFC0F
_0804309C: .4byte 0x000004BC
_080430A0: .4byte 0x00000484
_080430A4:
	mov r7, #0x90
	lsl r7, r7, #3
	add r3, r6, r7
	ldr r2, _080430F0 @ =0x0819A9D4
	lsl r1, r4, #1
	add r1, r1, r4
	lsl r1, r1, #3
	add r0, r2, #0
	add r0, #0x10
	add r0, r1, r0
	ldr r0, [r0]
	str r0, [r3]
	ldr r0, _080430F4 @ =0x00000484
	add r3, r6, r0
	add r2, #0x14
	add r1, r1, r2
	ldr r0, [r1]
	str r0, [r3]
_080430C8:
	ldr r2, _080430F8 @ =0x02017A40
	mov r1, #0xF9
	lsl r1, r1, #2
	add r0, r2, r1
	mov r1, #0
	strb r1, [r0]
	ldr r3, _080430FC @ =0x000003E5
	add r0, r2, r3
	strb r1, [r0]
	ldr r4, _08043100 @ =0x00000492
	add r2, r2, r4
_080430DE:
	ldrb r3, [r2]
	lsr r1, r3, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r3
	orr r0, r1
_080430EC:
	strb r0, [r2]
	b _08042D82
_080430F0: .4byte gUnk_0819A9D4
_080430F4: .4byte 0x00000484
_080430F8: .4byte 0x02017A40
_080430FC: .4byte 0x000003E5
_08043100: .4byte 0x00000492
_08043104:
	mov r5, #0x90
	lsl r5, r5, #3
	add r0, r4, r5
	ldr r2, [r0]
	cmp r2, #0
	beq _08043130
	ldr r7, _0804312C @ =0x000004BC
	add r0, r4, r7
	mov r3, #0x9A
	lsl r3, r3, #3
	add r1, r4, r3
	bl _call_via_r2
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08043126
	b _08042D82
_08043126:
	add r5, #0x12
	add r3, r4, r5
	b _08043134
_0804312C: .4byte 0x000004BC
_08043130:
	ldr r7, _08043148 @ =0x00000492
	add r3, r4, r7
_08043134:
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _08042D82
	.align 2, 0
_08043148: .4byte 0x00000492
_0804314C:
	ldr r1, _08043174 @ =0x00000484
	add r0, r4, r1
	ldr r2, [r0]
	cmp r2, #0
	beq _08043180
	ldr r3, _08043178 @ =0x000004BC
	add r0, r4, r3
	mov r5, #0x9A
	lsl r5, r5, #3
	add r1, r4, r5
	bl _call_via_r2
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0804316C
	b _08042D82
_0804316C:
	ldr r7, _0804317C @ =0x00000492
	add r3, r4, r7
	b _08043184
	.align 2, 0
_08043174: .4byte 0x00000484
_08043178: .4byte 0x000004BC
_0804317C: .4byte 0x00000492
_08043180:
	ldr r0, _08043198 @ =0x00000492
	add r3, r4, r0
_08043184:
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
_0804318E:
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _08042D82
	.align 2, 0
_08043198: .4byte 0x00000492
_0804319C:
	ldr r3, _080431B8 @ =0x00000493
	add r1, r2, r3
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _080431C4
	ldr r0, _080431BC @ =0x0000F056
	ldr r4, _080431C0 @ =0x000004BC
	add r1, r2, r4
	mov r2, #0x14
	bl sub_080229BC
	b _080431D0
_080431B8: .4byte 0x00000493
_080431BC: .4byte 0x0000F056
_080431C0: .4byte 0x000004BC
_080431C4:
	ldr r0, _080431DC @ =0x0000F053
	ldr r5, _080431E0 @ =0x000004BC
	add r1, r2, r5
	mov r2, #0x14
	bl sub_080229BC
_080431D0:
	mov r0, #1
_080431D2:
	add sp, #0x100
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080431DC: .4byte 0x0000F053
_080431E0: .4byte 0x000004BC
	thumb_func_end sub_08042BE0

