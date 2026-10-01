	thumb_func_start sub_08069FE4
sub_08069FE4: @ 0x08069FE4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x18
	ldr r4, _0806A04C @ =0x03000040
	ldr r5, _0806A050 @ =0x000003FF
	ldrh r0, [r4, #6]
	and r5, r0
	ldr r6, _0806A054 @ =0x0201E138
	add r0, r6, #0
	bl sub_0807883C
	ldr r1, _0806A058 @ =0xFFFFF9E8
	add r1, r1, r6
	mov ip, r1
	ldr r2, _0806A05C @ =0x00001634
	add r0, r6, r2
	ldrh r2, [r0]
	add r2, #0x80
	mov r3, #0
	mov r8, r3
	strh r2, [r0]
	ldr r0, _0806A060 @ =0x00001636
	add r1, r6, r0
	ldrh r0, [r1]
	add r0, #0x80
	strh r0, [r1]
	ldr r1, _0806A064 @ =0x0400001C
	lsl r2, r2, #0x10
	lsr r3, r2, #0x18
	strh r3, [r1]
	add r1, #2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x18
	strh r0, [r1]
	ldr r2, _0806A068 @ =0x0000163B
	add r1, r6, r2
	ldrb r0, [r1]
	cmp r0, #0
	beq _0806A036
	b _0806A5FC
_0806A036:
	sub r2, #8
	add r0, r6, r2
	ldrb r7, [r0]
	cmp r7, #1
	bne _0806A042
	b _0806A2C4
_0806A042:
	cmp r7, #1
	bgt _0806A06C
	cmp r7, #0
	beq _0806A074
	b _0806A5EE
_0806A04C: .4byte 0x03000040
_0806A050: .4byte 0x000003FF
_0806A054: .4byte 0x0201E138
_0806A058: .4byte 0xFFFFF9E8
_0806A05C: .4byte 0x00001634
_0806A060: .4byte 0x00001636
_0806A064: .4byte 0x0400001C
_0806A068: .4byte 0x0000163B
_0806A06C:
	cmp r7, #2
	bne _0806A072
	b _0806A4F0
_0806A072:
	b _0806A5EE
_0806A074:
	cmp r5, #0x10
	beq _0806A15C
	cmp r5, #0x10
	bhi _0806A08A
	cmp r5, #1
	bne _0806A082
	b _0806A28C
_0806A082:
	cmp r5, #2
	bne _0806A088
	b _0806A248
_0806A088:
	b _0806A264
_0806A08A:
	cmp r5, #0x40
	beq _0806A0A0
	cmp r5, #0x40
	bhi _0806A09A
	cmp r5, #0x20
	bne _0806A098
	b _0806A1CC
_0806A098:
	b _0806A264
_0806A09A:
	cmp r5, #0x80
	beq _0806A110
	b _0806A264
_0806A0A0:
	ldr r3, _0806A0CC @ =0x00001631
	add r2, r6, r3
	ldrb r4, [r2]
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, ip
	ldr r5, _0806A0D0 @ =0x00001726
	add r0, r0, r5
	mov r1, #0xFF
	strb r1, [r0]
	ldr r0, _0806A0D4 @ =0x00001642
	add r1, r6, r0
	mov r0, #0x20
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0806A0DC
	ldr r0, _0806A0D8 @ =0x08087588
	ldrb r3, [r2]
	lsl r1, r3, #2
	b _0806A0E2
_0806A0CC: .4byte 0x00001631
_0806A0D0: .4byte 0x00001726
_0806A0D4: .4byte 0x00001642
_0806A0D8: .4byte gUnk_08087588
_0806A0DC:
	ldr r0, _0806A100 @ =0x0808756C
	ldrb r4, [r2]
	lsl r1, r4, #2
_0806A0E2:
	add r1, r1, r0
	ldrb r0, [r1]
	strb r0, [r2]
	ldr r2, _0806A104 @ =0x0201DB20
	ldr r5, _0806A108 @ =0x00001C49
	add r1, r2, r5
	ldrb r3, [r1]
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r4, _0806A10C @ =0x00001726
	add r0, r0, r4
	b _0806A22A
	.align 2, 0
_0806A100: .4byte gUnk_0808756C
_0806A104: .4byte 0x0201DB20
_0806A108: .4byte 0x00001C49
_0806A10C: .4byte 0x00001726
_0806A110:
	ldr r5, _0806A13C @ =0x00001631
	add r2, r6, r5
	ldrb r1, [r2]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, ip
	ldr r3, _0806A140 @ =0x00001726
	add r0, r0, r3
	mov r1, #0xFF
	strb r1, [r0]
	ldr r4, _0806A144 @ =0x00001642
	add r1, r6, r4
	mov r0, #0x20
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0806A14C
	ldr r0, _0806A148 @ =0x08087588
	ldrb r5, [r2]
	lsl r1, r5, #2
	b _0806A152
_0806A13C: .4byte 0x00001631
_0806A140: .4byte 0x00001726
_0806A144: .4byte 0x00001642
_0806A148: .4byte gUnk_08087588
_0806A14C:
	ldr r0, _0806A158 @ =0x0808756C
	ldrb r3, [r2]
	lsl r1, r3, #2
_0806A152:
	add r0, #1
	b _0806A210
	.align 2, 0
_0806A158: .4byte gUnk_0808756C
_0806A15C:
	ldr r3, _0806A188 @ =0x00001631
	add r2, r6, r3
	ldrb r4, [r2]
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, ip
	ldr r5, _0806A18C @ =0x00001726
	add r0, r0, r5
	mov r1, #0xFF
	strb r1, [r0]
	ldr r0, _0806A190 @ =0x00001642
	add r1, r6, r0
	mov r0, #0x20
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0806A198
	ldr r0, _0806A194 @ =0x08087588
	ldrb r3, [r2]
	lsl r1, r3, #2
	b _0806A19E
_0806A188: .4byte 0x00001631
_0806A18C: .4byte 0x00001726
_0806A190: .4byte 0x00001642
_0806A194: .4byte gUnk_08087588
_0806A198:
	ldr r0, _0806A1BC @ =0x0808756C
	ldrb r4, [r2]
	lsl r1, r4, #2
_0806A19E:
	add r0, #2
	add r1, r1, r0
	ldrb r0, [r1]
	strb r0, [r2]
	ldr r2, _0806A1C0 @ =0x0201DB20
	ldr r5, _0806A1C4 @ =0x00001C49
	add r1, r2, r5
	ldrb r3, [r1]
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r4, _0806A1C8 @ =0x00001726
	add r0, r0, r4
	b _0806A22A
_0806A1BC: .4byte gUnk_0808756C
_0806A1C0: .4byte 0x0201DB20
_0806A1C4: .4byte 0x00001C49
_0806A1C8: .4byte 0x00001726
_0806A1CC:
	ldr r0, _0806A1F8 @ =0x00001631
	add r2, r6, r0
	ldrb r1, [r2]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, ip
	ldr r3, _0806A1FC @ =0x00001726
	add r0, r0, r3
	mov r1, #0xFF
	strb r1, [r0]
	ldr r4, _0806A200 @ =0x00001642
	add r0, r6, r4
	ldrb r0, [r0]
	and r5, r0
	cmp r5, #0
	beq _0806A208
	ldr r0, _0806A204 @ =0x08087588
	ldrb r5, [r2]
	lsl r1, r5, #2
	b _0806A20E
	.align 2, 0
_0806A1F8: .4byte 0x00001631
_0806A1FC: .4byte 0x00001726
_0806A200: .4byte 0x00001642
_0806A204: .4byte gUnk_08087588
_0806A208:
	ldr r0, _0806A238 @ =0x0808756C
	ldrb r3, [r2]
	lsl r1, r3, #2
_0806A20E:
	add r0, #3
_0806A210:
	add r1, r1, r0
	ldrb r0, [r1]
	strb r0, [r2]
	ldr r2, _0806A23C @ =0x0201DB20
	ldr r4, _0806A240 @ =0x00001C49
	add r1, r2, r4
	ldrb r5, [r1]
	lsl r0, r5, #2
	add r0, r0, r5
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r1, _0806A244 @ =0x00001726
	add r0, r0, r1
_0806A22A:
	mov r1, #1
	strb r1, [r0]
	mov r0, #0
	bl sub_08077AEC
	b _0806A294
	.align 2, 0
_0806A238: .4byte gUnk_0808756C
_0806A23C: .4byte 0x0201DB20
_0806A240: .4byte 0x00001C49
_0806A244: .4byte 0x00001726
_0806A248:
	ldrb r0, [r6, #6]
	cmp r0, #0
	bne _0806A256
	ldr r2, _0806A260 @ =0x00001639
	add r1, r6, r2
	mov r0, #1
	strb r0, [r1]
_0806A256:
	mov r0, #2
	bl sub_08077AEC
	b _0806A294
	.align 2, 0
_0806A260: .4byte 0x00001639
_0806A264:
	ldr r2, _0806A280 @ =0x0201DB20
	ldr r3, _0806A284 @ =0x00001C49
	add r1, r2, r3
	ldrb r4, [r1]
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r5, _0806A288 @ =0x00001726
	add r0, r0, r5
	mov r1, #1
	strb r1, [r0]
	b _0806A294
	.align 2, 0
_0806A280: .4byte 0x0201DB20
_0806A284: .4byte 0x00001C49
_0806A288: .4byte 0x00001726
_0806A28C:
	strb r5, [r1]
	mov r0, #1
	bl sub_08077AEC
_0806A294:
	ldr r1, _0806A2B4 @ =0x0201DB20
	ldr r2, _0806A2B8 @ =0x00001C52
	add r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0
	bne _0806A2A2
	b _0806A5EE
_0806A2A2:
	ldr r3, _0806A2BC @ =0x00001C50
	add r0, r1, r3
	ldrb r0, [r0]
	cmp r0, #8
	bls _0806A2AE
	b _0806A5EE
_0806A2AE:
	ldr r4, _0806A2C0 @ =0x00001C49
	add r0, r1, r4
	b _0806A4D0
_0806A2B4: .4byte 0x0201DB20
_0806A2B8: .4byte 0x00001C52
_0806A2BC: .4byte 0x00001C50
_0806A2C0: .4byte 0x00001C49
_0806A2C4:
	cmp r5, #0x10
	beq _0806A384
	cmp r5, #0x10
	bhi _0806A2DA
	cmp r5, #1
	bne _0806A2D2
	b _0806A484
_0806A2D2:
	cmp r5, #2
	bne _0806A2D8
	b _0806A41C
_0806A2D8:
	b _0806A48E
_0806A2DA:
	cmp r5, #0x40
	beq _0806A2EE
	cmp r5, #0x40
	bhi _0806A2E8
	cmp r5, #0x20
	beq _0806A3D0
	b _0806A48E
_0806A2E8:
	cmp r5, #0x80
	beq _0806A338
	b _0806A48E
_0806A2EE:
	ldr r2, _0806A328 @ =0x080875BC
	ldr r5, _0806A32C @ =0x00001632
	add r3, r6, r5
	ldrb r0, [r3]
	add r1, r0, r2
	ldrb r4, [r1]
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, ip
	ldr r4, _0806A330 @ =0x00001726
	add r0, r0, r4
	mov r1, #0xFF
	strb r1, [r0]
	ldr r1, _0806A334 @ =0x080875A4
	ldrb r5, [r3]
	lsl r0, r5, #2
	add r0, r0, r1
	ldrb r0, [r0]
	strb r0, [r3]
	add r2, r0, r2
	ldrb r1, [r2]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, ip
	add r0, r0, r4
	strb r7, [r0]
	b _0806A468
_0806A328: .4byte gUnk_080875BC
_0806A32C: .4byte 0x00001632
_0806A330: .4byte 0x00001726
_0806A334: .4byte gUnk_080875A4
_0806A338:
	ldr r2, _0806A374 @ =0x080875BC
	ldr r4, _0806A378 @ =0x00001632
	add r3, r6, r4
	ldrb r5, [r3]
	add r1, r5, r2
	ldrb r4, [r1]
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, ip
	ldr r4, _0806A37C @ =0x00001726
	add r0, r0, r4
	mov r1, #0xFF
	strb r1, [r0]
	ldr r0, _0806A380 @ =0x080875A4
	ldrb r5, [r3]
	lsl r1, r5, #2
	add r0, #1
	add r1, r1, r0
	ldrb r0, [r1]
	strb r0, [r3]
	add r2, r0, r2
	ldrb r1, [r2]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, ip
	add r0, r0, r4
	strb r7, [r0]
	b _0806A468
_0806A374: .4byte gUnk_080875BC
_0806A378: .4byte 0x00001632
_0806A37C: .4byte 0x00001726
_0806A380: .4byte gUnk_080875A4
_0806A384:
	ldr r2, _0806A3C0 @ =0x080875BC
	ldr r4, _0806A3C4 @ =0x00001632
	add r3, r6, r4
	ldrb r5, [r3]
	add r1, r5, r2
	ldrb r4, [r1]
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, ip
	ldr r4, _0806A3C8 @ =0x00001726
	add r0, r0, r4
	mov r1, #0xFF
	strb r1, [r0]
	ldr r0, _0806A3CC @ =0x080875A4
	ldrb r5, [r3]
	lsl r1, r5, #2
	add r0, #2
	add r1, r1, r0
	ldrb r0, [r1]
	strb r0, [r3]
	add r2, r0, r2
	ldrb r1, [r2]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, ip
	add r0, r0, r4
	strb r7, [r0]
	b _0806A468
_0806A3C0: .4byte gUnk_080875BC
_0806A3C4: .4byte 0x00001632
_0806A3C8: .4byte 0x00001726
_0806A3CC: .4byte gUnk_080875A4
_0806A3D0:
	ldr r2, _0806A40C @ =0x080875BC
	ldr r4, _0806A410 @ =0x00001632
	add r3, r6, r4
	ldrb r5, [r3]
	add r1, r5, r2
	ldrb r4, [r1]
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, ip
	ldr r4, _0806A414 @ =0x00001726
	add r0, r0, r4
	mov r1, #0xFF
	strb r1, [r0]
	ldr r0, _0806A418 @ =0x080875A4
	ldrb r5, [r3]
	lsl r1, r5, #2
	add r0, #3
	add r1, r1, r0
	ldrb r0, [r1]
	strb r0, [r3]
	add r2, r0, r2
	ldrb r1, [r2]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, ip
	add r0, r0, r4
	strb r7, [r0]
	b _0806A468
_0806A40C: .4byte gUnk_080875BC
_0806A410: .4byte 0x00001632
_0806A414: .4byte 0x00001726
_0806A418: .4byte gUnk_080875A4
_0806A41C:
	mov r2, r8
	strb r2, [r0]
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0x80
	lsl r3, r3, #2
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	ldrh r1, [r2]
	ldr r0, _0806A470 @ =0x0000FEFF
	and r0, r1
	strh r0, [r2]
	ldr r1, _0806A474 @ =0x080875BC
	ldr r4, _0806A478 @ =0x00001632
	add r0, r6, r4
	ldrb r0, [r0]
	add r1, r0, r1
	ldrb r5, [r1]
	lsl r0, r5, #2
	add r0, r0, r5
	lsl r0, r0, #2
	add r0, ip
	ldr r2, _0806A47C @ =0x00001726
	add r0, r0, r2
	mov r1, #0xFF
	strb r1, [r0]
	ldr r0, _0806A480 @ =0x00001631
	add r1, r6, r0
	ldrb r3, [r1]
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, ip
	add r0, r0, r2
	mov r4, r8
	strb r4, [r0]
_0806A468:
	mov r0, #0
	bl sub_08077AEC
	b _0806A4AC
_0806A470: .4byte 0x0000FEFF
_0806A474: .4byte gUnk_080875BC
_0806A478: .4byte 0x00001632
_0806A47C: .4byte 0x00001726
_0806A480: .4byte 0x00001631
_0806A484:
	strb r5, [r1]
	mov r0, #1
	bl sub_08077AEC
	b _0806A4AC
_0806A48E:
	ldr r2, _0806A4D8 @ =0x0201DB20
	ldr r1, _0806A4DC @ =0x080875BC
	ldr r5, _0806A4E0 @ =0x00001C4A
	add r0, r2, r5
	ldrb r0, [r0]
	add r1, r0, r1
	ldrb r3, [r1]
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r4, _0806A4E4 @ =0x00001726
	add r0, r0, r4
	mov r1, #1
	strb r1, [r0]
_0806A4AC:
	ldr r1, _0806A4D8 @ =0x0201DB20
	ldr r5, _0806A4E8 @ =0x00001C52
	add r0, r1, r5
	ldrb r0, [r0]
	cmp r0, #0
	bne _0806A4BA
	b _0806A5EE
_0806A4BA:
	ldr r2, _0806A4EC @ =0x00001C50
	add r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #8
	bls _0806A4C6
	b _0806A5EE
_0806A4C6:
	ldr r0, _0806A4DC @ =0x080875BC
	ldr r3, _0806A4E0 @ =0x00001C4A
	add r1, r1, r3
	ldrb r1, [r1]
	add r0, r1, r0
_0806A4D0:
	ldrb r0, [r0]
	bl sub_08069DD8
	b _0806A5EE
_0806A4D8: .4byte 0x0201DB20
_0806A4DC: .4byte gUnk_080875BC
_0806A4E0: .4byte 0x00001C4A
_0806A4E4: .4byte 0x00001726
_0806A4E8: .4byte 0x00001C52
_0806A4EC: .4byte 0x00001C50
_0806A4F0:
	cmp r5, #1
	beq _0806A514
	cmp r5, #2
	bne _0806A534
	ldrb r0, [r6, #6]
	cmp r0, #0
	bne _0806A5EE
	ldr r4, _0806A510 @ =0x00001639
	add r1, r6, r4
	mov r0, #1
	strb r0, [r1]
	mov r0, #2
	bl sub_08077AEC
	b _0806A5EE
	.align 2, 0
_0806A510: .4byte 0x00001639
_0806A514:
	ldr r0, _0806A52C @ =0x086F41A0
	ldr r1, _0806A530 @ =0x06003C00
	mov r2, #0x7F
	and r3, r2
	add r2, r3, #0
	bl sub_08069F40
	mov r0, #1
	bl sub_08077AEC
	b _0806A5EE
	.align 2, 0
_0806A52C: .4byte gUnk_086F41A0
_0806A530: .4byte 0x06003C00
_0806A534:
	ldr r5, _0806A58C @ =0x000011D6
	add r0, r6, r5
	mov r7, #1
	strb r7, [r0]
	ldr r1, _0806A590 @ =0x0000163C
	add r0, r6, r1
	mov r2, r8
	strb r2, [r0]
	ldr r3, _0806A594 @ =0x00000414
	add r1, r4, r3
	ldr r0, _0806A598 @ =0x08069FAD
	str r0, [r1]
	ldr r4, _0806A59C @ =0x00001643
	add r5, r6, r4
	ldrb r2, [r5]
	cmp r2, #5
	bgt _0806A5B0
	cmp r2, #3
	blt _0806A5B0
	ldr r0, _0806A5A0 @ =0x00001604
	add r4, r6, r0
	ldrb r0, [r4]
	ldr r3, _0806A5A4 @ =0x00001631
	add r1, r6, r3
	ldrb r1, [r1]
	bl sub_08069284
	mov r0, r8
	strb r0, [r5]
	ldr r1, _0806A5A8 @ =0x0000162A
	add r0, r6, r1
	ldrb r4, [r4]
	add r0, r4, r0
	strb r7, [r0]
	ldr r3, _0806A5AC @ =0x00001625
	add r2, r6, r3
	mov r0, #8
	neg r0, r0
	ldrb r4, [r2]
	and r0, r4
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	b _0806A5C8
_0806A58C: .4byte 0x000011D6
_0806A590: .4byte 0x0000163C
_0806A594: .4byte 0x00000414
_0806A598: .4byte sub_08069FAC
_0806A59C: .4byte 0x00001643
_0806A5A0: .4byte 0x00001604
_0806A5A4: .4byte 0x00001631
_0806A5A8: .4byte 0x0000162A
_0806A5AC: .4byte 0x00001625
_0806A5B0:
	ldr r2, _0806A6A8 @ =0x0201DB20
	ldr r5, _0806A6AC @ =0x00001C1C
	add r0, r2, r5
	ldrb r0, [r0]
	ldr r3, _0806A6B0 @ =0x00001C49
	add r1, r2, r3
	ldrb r1, [r1]
	ldr r4, _0806A6B4 @ =0x00001C4A
	add r2, r2, r4
	ldrb r2, [r2]
	bl sub_08069284
_0806A5C8:
	ldr r0, _0806A6B8 @ =0x03000040
	ldr r5, _0806A6BC @ =0x00000414
	add r0, r0, r5
	mov r1, #0
	str r1, [r0]
	ldr r0, _0806A6C0 @ =0x086F41A0
	ldr r1, _0806A6C4 @ =0x06003C00
	mov r2, #0x80
	bl sub_08069F40
	ldr r1, _0806A6A8 @ =0x0201DB20
	ldr r0, _0806A6C8 @ =0x00001C51
	add r2, r1, r0
	mov r0, #1
	strb r0, [r2]
	ldr r2, _0806A6CC @ =0x00001C4B
	add r1, r1, r2
	mov r0, #3
	strb r0, [r1]
_0806A5EE:
	ldr r0, _0806A6A8 @ =0x0201DB20
	ldr r3, _0806A6D0 @ =0x00001C53
	add r0, r0, r3
	ldrb r0, [r0]
	cmp r0, #0
	bne _0806A5FC
	b _0806A7F2
_0806A5FC:
	ldr r7, _0806A6A8 @ =0x0201DB20
	ldr r4, _0806A6CC @ =0x00001C4B
	add r4, r4, r7
	mov r8, r4
	ldrb r5, [r4]
	cmp r5, #1
	bne _0806A60C
	b _0806A740
_0806A60C:
	cmp r5, #1
	ble _0806A612
	b _0806A7F2
_0806A612:
	cmp r5, #0
	beq _0806A618
	b _0806A7F2
_0806A618:
	ldr r0, _0806A6D0 @ =0x00001C53
	add r6, r7, r0
	ldrb r0, [r6]
	cmp r0, #0xD
	beq _0806A624
	b _0806A724
_0806A624:
	strb r5, [r6]
	ldr r1, _0806A6B0 @ =0x00001C49
	add r3, r7, r1
	ldrb r2, [r3]
	lsl r0, r2, #2
	add r0, r0, r2
	lsl r0, r0, #2
	add r0, r0, r7
	ldr r4, _0806A6D4 @ =0x00001726
	add r0, r0, r4
	mov r1, #0xFF
	strb r1, [r0]
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _0806A6D8 @ =0x0000FDFF
	and r0, r1
	strh r0, [r2]
	ldrh r0, [r2]
	mov r4, #0x80
	lsl r4, r4, #1
	add r1, r4, #0
	orr r0, r1
	strh r0, [r2]
	ldrb r2, [r3]
	cmp r2, #5
	bgt _0806A6E8
	cmp r2, #4
	blt _0806A6E8
	ldr r0, _0806A6AC @ =0x00001C1C
	add r1, r7, r0
	ldr r3, _0806A6DC @ =0x00001C3F
	add r0, r7, r3
	ldrb r4, [r1]
	add r0, r4, r0
	strb r2, [r0]
	ldr r0, _0806A6E0 @ =0x00001C3D
	add r3, r7, r0
	mov r2, #8
	neg r2, r2
	add r0, r2, #0
	ldrb r4, [r3]
	and r0, r4
	mov r4, #3
	orr r0, r4
	strb r0, [r3]
	ldr r0, _0806A6E4 @ =0x00001C42
	add r0, r0, r7
	mov ip, r0
	ldrb r0, [r1]
	add r0, ip
	ldr r1, _0806A6B4 @ =0x00001C4A
	add r7, r7, r1
	ldrb r1, [r7]
	strb r1, [r0]
	ldrb r0, [r3]
	and r2, r0
	orr r2, r4
	strb r2, [r3]
	strb r5, [r6]
	bl sub_08069E64
	mov r0, #2
	mov r1, r8
	strb r0, [r1]
	b _0806A7F2
_0806A6A8: .4byte 0x0201DB20
_0806A6AC: .4byte 0x00001C1C
_0806A6B0: .4byte 0x00001C49
_0806A6B4: .4byte 0x00001C4A
_0806A6B8: .4byte 0x03000040
_0806A6BC: .4byte 0x00000414
_0806A6C0: .4byte gUnk_086F41A0
_0806A6C4: .4byte 0x06003C00
_0806A6C8: .4byte 0x00001C51
_0806A6CC: .4byte 0x00001C4B
_0806A6D0: .4byte 0x00001C53
_0806A6D4: .4byte 0x00001726
_0806A6D8: .4byte 0x0000FDFF
_0806A6DC: .4byte 0x00001C3F
_0806A6E0: .4byte 0x00001C3D
_0806A6E4: .4byte 0x00001C42
_0806A6E8:
	ldr r2, _0806A710 @ =0x0201DB20
	ldr r3, _0806A714 @ =0x00001C4B
	add r1, r2, r3
	mov r3, #0
	mov r0, #1
	strb r0, [r1]
	ldr r1, _0806A718 @ =0x080875BC
	ldr r4, _0806A71C @ =0x00001C4A
	add r0, r2, r4
	ldrb r0, [r0]
	add r1, r0, r1
	ldrb r5, [r1]
	lsl r0, r5, #2
	add r0, r0, r5
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r1, _0806A720 @ =0x00001726
	add r0, r0, r1
	strb r3, [r0]
	b _0806A7F2
_0806A710: .4byte 0x0201DB20
_0806A714: .4byte 0x00001C4B
_0806A718: .4byte gUnk_080875BC
_0806A71C: .4byte 0x00001C4A
_0806A720: .4byte 0x00001726
_0806A724:
	add r0, #1
	strb r0, [r6]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0xB
	bhi _0806A7F2
	ldr r2, _0806A73C @ =0x00001C49
	add r0, r7, r2
	ldrb r0, [r0]
	bl sub_08069E20
	b _0806A7F2
_0806A73C: .4byte 0x00001C49
_0806A740:
	ldr r3, _0806A7B8 @ =0x00001C53
	add r3, r3, r7
	mov ip, r3
	ldrb r0, [r3]
	cmp r0, #0xD
	bne _0806A7D4
	ldr r5, _0806A7BC @ =0x00001C1C
	add r4, r7, r5
	ldr r0, _0806A7C0 @ =0x00001C3F
	add r1, r7, r0
	ldrb r2, [r4]
	add r1, r2, r1
	ldr r3, _0806A7C4 @ =0x00001C49
	add r0, r7, r3
	ldrb r0, [r0]
	mov r6, #0
	strb r0, [r1]
	add r5, #0x21
	add r3, r7, r5
	mov r2, #8
	neg r2, r2
	add r0, r2, #0
	ldrb r1, [r3]
	and r0, r1
	mov r5, #3
	orr r0, r5
	strb r0, [r3]
	ldr r1, _0806A7C8 @ =0x00001C42
	add r0, r7, r1
	ldrb r4, [r4]
	add r0, r4, r0
	add r1, #8
	add r4, r7, r1
	ldrb r1, [r4]
	strb r1, [r0]
	ldrb r0, [r3]
	and r2, r0
	orr r2, r5
	strb r2, [r3]
	mov r1, ip
	strb r6, [r1]
	ldr r1, _0806A7CC @ =0x080875BC
	ldrb r4, [r4]
	add r1, r4, r1
	ldrb r2, [r1]
	lsl r0, r2, #2
	add r0, r0, r2
	lsl r0, r0, #2
	add r0, r0, r7
	ldr r3, _0806A7D0 @ =0x00001726
	add r0, r0, r3
	mov r1, #0xFF
	strb r1, [r0]
	bl sub_08069E64
	mov r0, #2
	mov r4, r8
	strb r0, [r4]
	b _0806A7F2
	.align 2, 0
_0806A7B8: .4byte 0x00001C53
_0806A7BC: .4byte 0x00001C1C
_0806A7C0: .4byte 0x00001C3F
_0806A7C4: .4byte 0x00001C49
_0806A7C8: .4byte 0x00001C42
_0806A7CC: .4byte gUnk_080875BC
_0806A7D0: .4byte 0x00001726
_0806A7D4:
	add r0, #1
	mov r5, ip
	strb r0, [r5]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0xB
	bhi _0806A7F2
	ldr r0, _0806A828 @ =0x080875BC
	ldr r2, _0806A82C @ =0x00001C4A
	add r1, r7, r2
	ldrb r1, [r1]
	add r0, r1, r0
	ldrb r0, [r0]
	bl sub_08069E20
_0806A7F2:
	ldr r4, _0806A830 @ =0x0201DB20
	ldr r3, _0806A834 @ =0x0000061E
	add r5, r4, r3
	ldrb r0, [r5]
	cmp r0, #2
	bne _0806A840
	ldr r5, _0806A838 @ =0x00001C48
	add r2, r4, r5
	mov r0, #0x1F
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #8
	orr r0, r1
	strb r0, [r2]
	ldr r3, _0806A83C @ =0x00001C3D
	add r2, r4, r3
	mov r0, #8
	neg r0, r0
	ldrb r4, [r2]
	and r0, r4
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	mov r0, #1
	b _0806A900
	.align 2, 0
_0806A828: .4byte gUnk_080875BC
_0806A82C: .4byte 0x00001C4A
_0806A830: .4byte 0x0201DB20
_0806A834: .4byte 0x0000061E
_0806A838: .4byte 0x00001C48
_0806A83C: .4byte 0x00001C3D
_0806A840:
	cmp r0, #3
	bne _0806A876
	ldr r1, _0806A90C @ =0x04000050
	ldr r2, _0806A910 @ =0x00003F44
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #0x10
	bl sub_0807B4A8
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0x80
	lsl r3, r3, #3
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	mov r0, #0
	strb r0, [r5]
	ldr r5, _0806A914 @ =0x00001C51
	add r1, r4, r5
	mov r0, #0xFF
	strb r0, [r1]
	ldr r0, _0806A918 @ =0x00001C52
	add r1, r4, r0
	mov r0, #1
	strb r0, [r1]
_0806A876:
	ldr r1, _0806A914 @ =0x00001C51
	add r6, r4, r1
	ldrb r1, [r6]
	mov r0, #0
	ldsb r0, [r6, r0]
	cmp r0, #0
	beq _0806A8CC
	ldr r2, _0806A91C @ =0x00001C50
	add r5, r4, r2
	ldrb r3, [r5]
	add r0, r1, r3
	mov r7, #0
	strb r0, [r5]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #8
	bne _0806A89A
	strb r7, [r6]
_0806A89A:
	ldrb r0, [r5]
	cmp r0, #0x10
	bne _0806A8C6
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _0806A920 @ =0x0000FBFF
	and r0, r1
	strh r0, [r2]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r2, #0xC3
	lsl r2, r2, #3
	add r3, r4, r2
	mov r0, #0
	mov r2, #0
	bl sub_080787F4
	strb r7, [r6]
	ldr r3, _0806A918 @ =0x00001C52
	add r0, r4, r3
	strb r7, [r0]
_0806A8C6:
	ldrb r0, [r5]
	bl sub_0807B4A8
_0806A8CC:
	ldr r5, _0806A924 @ =0x0201F238
	add r0, r5, #0
	bl sub_0807871C
	mov r1, #0
	str r1, [sp, #0]
	str r1, [sp, #4]
	mov r0, #3
	str r0, [sp, #8]
	str r1, [sp, #0xC]
	str r1, [sp, #0x10]
	ldr r0, _0806A928 @ =0xFFFFE8E8
	add r4, r5, r0
	str r4, [sp, #0x14]
	add r0, r5, #0
	mov r2, #0
	mov r3, #0
	bl sub_08078534
	add r0, r4, #0
	bl sub_0807A298
	add r0, r4, #0
	bl sub_0807A2EC
	mov r0, #0
_0806A900:
	add sp, #0x18
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0806A90C: .4byte 0x04000050
_0806A910: .4byte 0x00003F44
_0806A914: .4byte 0x00001C51
_0806A918: .4byte 0x00001C52
_0806A91C: .4byte 0x00001C50
_0806A920: .4byte 0x0000FBFF
_0806A924: .4byte 0x0201F238
_0806A928: .4byte 0xFFFFE8E8
	thumb_func_end sub_08069FE4

