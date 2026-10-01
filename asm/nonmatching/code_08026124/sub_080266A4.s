	thumb_func_start sub_080266A4
sub_080266A4: @ 0x080266A4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0x20
	ldr r5, _080266D8 @ =0x02020C28
	mov r0, #0x80
	lsl r0, r0, #2
	add r6, r5, r0
	add r0, r6, #0
	bl sub_0807883C
	ldr r1, _080266DC @ =0xFFFFF6E8
	add r1, r1, r5
	mov r8, r1
	mov r2, #0xCA
	lsl r2, r2, #1
	add r4, r5, r2
	ldrb r0, [r4]
	cmp r0, #1
	beq _0802671C
	cmp r0, #1
	bgt _080266E0
	cmp r0, #0
	beq _080266EA
	b _080267FC
_080266D8: .4byte 0x02020C28
_080266DC: .4byte 0xFFFFF6E8
_080266E0:
	cmp r0, #2
	beq _0802674C
	cmp r0, #3
	beq _0802676C
	b _080267FC
_080266EA:
	ldrb r3, [r5, #0xD]
	cmp r3, #5
	beq _080266F2
	b _080267FC
_080266F2:
	mov r0, #0xFF
	strb r0, [r5, #0xE]
	mov r0, #1
	mov r1, #0xA0
	mov r2, #0
	add r3, r6, #0
	bl sub_080787F4
	ldr r1, _08026718 @ =0x04000050
	mov r0, #0x90
	strh r0, [r1]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	mov r0, #0x13
	bl sub_08077AEC
	b _080267FC
	.align 2, 0
_08026718: .4byte 0x04000050
_0802671C:
	ldr r0, _08026744 @ =0x00000202
	add r1, r5, r0
	ldr r0, _08026748 @ =0x000007FF
	ldrh r1, [r1]
	cmp r1, r0
	bls _080267FC
	mov r2, #0x81
	lsl r2, r2, #2
	add r1, r5, r2
	mov r3, #0
	ldsh r0, [r1, r3]
	neg r0, r0
	mov r2, #0
	strh r0, [r1]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	strb r2, [r5, #0xE]
	strb r2, [r5, #0xF]
	b _080267FC
_08026744: .4byte 0x00000202
_08026748: .4byte 0x000007FF
_0802674C:
	ldr r0, _08026768 @ =0x00000206
	add r1, r5, r0
	ldrb r2, [r1]
	cmp r2, #3
	bne _08026760
	mov r0, #0
	strb r0, [r1]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08026760:
	mov r0, #0xFF
	strb r0, [r5, #0xE]
	b _080267FC
	.align 2, 0
_08026768: .4byte 0x00000206
_0802676C:
	mov r3, #0xCC
	lsl r3, r3, #1
	add r0, r5, r3
	bl sub_080261A8
	mov r6, #0
	mov r4, #0xC8
	lsl r4, r4, #1
	add r0, r5, r4
	ldrh r1, [r0]
	cmp r6, r1
	bcs _080267BA
	mov r7, #0
	mov r9, r8
	mov r8, r0
_0802678A:
	ldr r0, [r5, #4]
	ldrb r1, [r5, #0x10]
	ldrb r2, [r5, #0xC]
	ldrh r3, [r5, #8]
	ldrh r4, [r5, #0xA]
	str r4, [sp, #0]
	str r7, [sp, #4]
	str r7, [sp, #8]
	str r7, [sp, #0xC]
	mov r4, #1
	str r4, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	mov r4, r9
	str r4, [sp, #0x1C]
	bl sub_08077EF4
	add r0, r6, #1
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	mov r0, r8
	ldrh r0, [r0]
	cmp r6, r0
	bcc _0802678A
_080267BA:
	ldr r4, _080267F0 @ =0x02020310
	add r0, r4, #0
	bl sub_0807A298
	add r0, r4, #0
	bl sub_0807A2EC
	ldr r1, _080267F4 @ =0x00000B18
	add r3, r4, r1
	mov r0, #1
	mov r1, #0x20
	mov r2, #0
	bl sub_080787F4
	mov r2, #0xB2
	lsl r2, r2, #4
	add r0, r4, r2
	mov r1, #0
	strb r1, [r0]
	ldr r3, _080267F8 @ =0x00000B16
	add r4, r4, r3
	strb r1, [r4]
	mov r0, #0x14
	bl sub_08077AEC
	mov r0, #1
	b _080268B8
_080267F0: .4byte 0x02020310
_080267F4: .4byte 0x00000B18
_080267F8: .4byte 0x00000B16
_080267FC:
	ldr r2, _080268C8 @ =0x02020310
	ldr r4, _080268CC @ =0x00000925
	add r0, r2, r4
	ldrb r1, [r0]
	ldr r3, _080268D0 @ =0x00001735
	add r0, r2, r3
	strb r1, [r0]
	ldr r4, _080268D4 @ =0x00000AAC
	add r0, r2, r4
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802682A
	ldr r0, _080268D8 @ =0x00001734
	add r2, r2, r0
	lsl r0, r1, #0x18
	lsr r0, r0, #0x18
	ldrb r3, [r2]
	cmp r0, r3
	beq _0802682A
	strb r1, [r2]
	mov r0, #0x13
	bl sub_08077AEC
_0802682A:
	mov r6, #0
	ldr r1, _080268C8 @ =0x02020310
	ldr r4, _080268DC @ =0x00000AA8
	add r0, r1, r4
	ldrh r0, [r0]
	cmp r6, r0
	bcs _08026856
	ldr r0, _080268E0 @ =0x00000918
	add r7, r1, r0
	add r4, r1, r4
_0802683E:
	lsl r0, r6, #2
	add r0, r0, r6
	lsl r0, r0, #2
	add r0, r0, r7
	bl sub_080786D0
	add r0, r6, #1
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	ldrh r1, [r4]
	cmp r6, r1
	bcc _0802683E
_08026856:
	mov r6, #0
	ldr r1, _080268C8 @ =0x02020310
	ldr r2, _080268DC @ =0x00000AA8
	add r0, r1, r2
	ldrh r3, [r0]
	cmp r6, r3
	bcs _0802689C
	mov r7, #0
	mov r9, r1
	mov r8, r0
_0802686A:
	ldr r0, [r5, #4]
	ldrb r1, [r5, #0x10]
	ldrb r2, [r5, #0xC]
	ldrh r3, [r5, #8]
	ldrh r4, [r5, #0xA]
	str r4, [sp, #0]
	str r7, [sp, #4]
	str r7, [sp, #8]
	str r7, [sp, #0xC]
	mov r4, #1
	str r4, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	mov r4, r9
	str r4, [sp, #0x1C]
	bl sub_08077EF4
	add r5, #0x14
	add r0, r6, #1
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	mov r0, r8
	ldrh r0, [r0]
	cmp r6, r0
	bcc _0802686A
_0802689C:
	ldr r4, _080268C8 @ =0x02020310
	add r0, r4, #0
	bl sub_0807A298
	add r0, r4, #0
	bl sub_0807A2EC
	mov r1, #0xB2
	lsl r1, r1, #4
	add r4, r4, r1
	add r0, r4, #0
	bl sub_0807B0D0
	mov r0, #0
_080268B8:
	add sp, #0x20
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080268C8: .4byte 0x02020310
_080268CC: .4byte 0x00000925
_080268D0: .4byte 0x00001735
_080268D4: .4byte 0x00000AAC
_080268D8: .4byte 0x00001734
_080268DC: .4byte 0x00000AA8
_080268E0: .4byte 0x00000918
	thumb_func_end sub_080266A4

