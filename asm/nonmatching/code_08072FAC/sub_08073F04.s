	thumb_func_start sub_08073F04
sub_08073F04: @ 0x08073F04
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	str r0, [sp, #0]
	str r1, [sp, #4]
	ldr r4, _08074070 @ =0x03006676
	add r0, r4, #0
	bl sub_080740BC
	sub r3, r4, #6
	mov r2, #0
	strh r2, [r3]
	sub r6, r4, #4
	strh r2, [r6]
	sub r1, r4, #2
	mov r0, #0xF
	ldrh r1, [r1]
	and r0, r1
	cmp r0, #3
	beq _08073F34
	b _080740A8
_08073F34:
	add r0, r4, #0
	sub r0, #0x1A
	str r2, [r0]
	ldr r1, _08074074 @ =0xFFFFF4EA
	add r7, r4, r1
	add r5, r7, #0
	add r4, r3, #0
	ldr r2, _08074078 @ =0x00000AF4
	add r2, r2, r7
	mov sl, r2
_08073F48:
	ldrh r1, [r4]
	lsl r0, r1, #4
	ldr r3, _0807407C @ =0x00000B16
	add r3, r3, r5
	mov r9, r3
	add r0, r9
	ldrh r3, [r0]
	mov r2, #0xF0
	lsl r2, r2, #8
	add r0, r2, #0
	add r2, r0, #0
	and r2, r3
	mov r0, #0x80
	lsl r0, r0, #5
	cmp r2, r0
	beq _08074036
	mov r0, #0xC0
	lsl r0, r0, #6
	cmp r2, r0
	bne _08074036
	lsl r1, r1, #1
	ldr r7, _08074080 @ =0x03006658
	add r1, r1, r7
	ldr r2, _08074084 @ =0x000001FF
	add r0, r2, #0
	and r0, r3
	strh r0, [r1]
	ldrh r3, [r4]
	lsl r0, r3, #4
	ldr r7, _08074088 @ =0x00000B18
	add r1, r5, r7
	add r0, r0, r1
	ldr r1, _0807408C @ =0x00000A1A
	add r1, r1, r5
	mov r8, r1
	add r3, r8
	ldrb r2, [r3]
	add r1, r2, #1
	strb r1, [r3]
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r1, r2, #8
	add r1, r1, r2
	lsl r1, r1, #2
	ldrh r3, [r4]
	lsl r2, r3, #8
	add r2, r2, r3
	lsl r2, r2, #1
	mov r7, #0x83
	lsl r7, r7, #2
	add r6, r5, r7
	add r2, r2, r6
	add r1, r1, r2
	lsl r3, r3, #1
	add r3, sl
	ldrh r7, [r3]
	lsl r2, r7, #1
	add r3, r7, #0
	add r2, r2, r3
	lsl r2, r2, #2
	add r1, r1, r2
	mov r2, #7
	bl CpuSet
	ldrh r1, [r4]
	lsl r0, r1, #1
	add r0, sl
	ldrh r1, [r0]
	add r1, #1
	strh r1, [r0]
	ldrh r0, [r4]
	ldr r2, [sp, #0]
	cmp r0, r2
	bne _08074016
	lsl r0, r0, #1
	ldr r3, _08074080 @ =0x03006658
	add r0, r0, r3
	ldrh r0, [r0]
	ldr r7, _08074090 @ =0x03006672
	strh r0, [r7]
	ldr r0, _08074094 @ =0x00000A18
	add r2, r5, r0
	ldrh r1, [r4]
	add r2, r1, r2
	ldrb r1, [r2]
	add r0, r1, #1
	strb r0, [r2]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	lsl r0, r1, #8
	add r0, r0, r1
	lsl r0, r0, #2
	ldrh r2, [r4]
	lsl r1, r2, #8
	add r1, r1, r2
	lsl r1, r1, #1
	add r1, r1, r6
	add r0, r0, r1
	ldrh r3, [r7]
	lsr r2, r3, #1
	ldr r1, [sp, #4]
	bl CpuSet
_08074016:
	ldrh r2, [r4]
	add r2, r8
	mov r1, #1
	add r0, r1, #0
	ldrb r7, [r2]
	and r0, r7
	strb r0, [r2]
	ldr r2, _08074094 @ =0x00000A18
	add r0, r5, r2
	ldrh r3, [r4]
	add r0, r3, r0
	ldrb r7, [r0]
	and r1, r7
	strb r1, [r0]
	ldr r7, _08074074 @ =0xFFFFF4EA
	add r7, r9
_08074036:
	ldrh r0, [r4]
	lsl r1, r0, #1
	ldr r2, _08074098 @ =0x00000AF8
	add r0, r5, r2
	add r1, r1, r0
	mov r2, #0
	strh r2, [r1]
	ldrh r3, [r4]
	lsl r0, r3, #1
	add r0, sl
	strh r2, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
	ldr r1, _0807409C @ =0x0300665C
	ldr r0, [r1]
	add r0, #1
	str r0, [r1]
	cmp r0, #1
	bgt _08074060
	b _08073F48
_08074060:
	ldr r3, _080740A0 @ =0x00000A26
	add r0, r7, r3
	strh r2, [r0]
	ldr r1, _080740A4 @ =0x00000B12
	add r0, r7, r1
	ldrh r0, [r0]
	b _080740AA
	.align 2, 0
_08074070: .4byte 0x03006676
_08074074: .4byte 0xFFFFF4EA
_08074078: .4byte 0x00000AF4
_0807407C: .4byte 0x00000B16
_08074080: .4byte 0x03006658
_08074084: .4byte 0x000001FF
_08074088: .4byte 0x00000B18
_0807408C: .4byte 0x00000A1A
_08074090: .4byte 0x03006672
_08074094: .4byte 0x00000A18
_08074098: .4byte 0x00000AF8
_0807409C: .4byte 0x0300665C
_080740A0: .4byte 0x00000A26
_080740A4: .4byte 0x00000B12
_080740A8:
	mov r0, #0
_080740AA:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08073F04
	.align 2, 0

