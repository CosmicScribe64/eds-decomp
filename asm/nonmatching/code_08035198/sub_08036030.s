	thumb_func_start sub_08036030
sub_08036030: @ 0x08036030
	push {r4, r5, r6, r7, lr}
	add r7, r0, #0
	add r5, r1, #0
	ldr r6, _080360CC @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r6, r1
	ldrb r0, [r0]
	cmp r0, #0x80
	bne _0803608C
	ldr r2, _080360D0 @ =0x000004E4
	add r4, r6, r2
	add r0, r4, #0
	add r1, r7, #0
	mov r2, #0x14
	bl sub_08075294
	ldrh r0, [r5]
	strh r0, [r4]
	ldrb r3, [r5, #2]
	lsl r1, r3, #0x1F
	ldr r0, _080360D4 @ =0x000004E6
	add r2, r6, r0
	lsr r1, r1, #0x1F
	mov r0, #2
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	ldrh r0, [r5]
	bl sub_08047058
	mov r1, #0x9F
	lsl r1, r1, #3
	add r3, r6, r1
	ldr r2, _080360D8 @ =0x0819A9D4
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r2, #4
	add r1, r1, r2
	ldr r0, [r1]
	str r0, [r3]
	cmp r0, #0
	beq _080360AE
_0803608C:
	ldr r4, _080360CC @ =0x02017A40
	mov r2, #0x9F
	lsl r2, r2, #3
	add r1, r4, r2
	ldr r3, _080360D0 @ =0x000004E4
	add r0, r4, r3
	ldr r2, [r1]
	mov r1, #0
	bl _call_via_r2
	mov r1, #0xF8
	lsl r1, r1, #2
	add r4, r4, r1
	strb r0, [r4]
	lsl r0, r0, #0x18
	cmp r0, #0
	bne _080360E0
_080360AE:
	mov r0, #1
	ldrb r7, [r7, #2]
	and r0, r7
	mov r1, #0xB0
	cmp r0, #0
	beq _080360BC
	ldr r1, _080360DC @ =0x000080B0
_080360BC:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0
	b _080360E2
_080360CC: .4byte 0x02017A40
_080360D0: .4byte 0x000004E4
_080360D4: .4byte 0x000004E6
_080360D8: .4byte gUnk_0819A9D4
_080360DC: .4byte 0x000080B0
_080360E0:
	ldrb r0, [r4]
_080360E2:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08036030

