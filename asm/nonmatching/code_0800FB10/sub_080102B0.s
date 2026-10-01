	thumb_func_start sub_080102B0
sub_080102B0: @ 0x080102B0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r5, _08010304 @ =0x020185C0
	ldrh r0, [r5]
	lsr r6, r0, #0xF
	mov r0, #1
	sub r4, r0, r6
	ldrh r1, [r5, #2]
	mov r8, r1
	ldr r2, _08010308 @ =0x0000080A
	add r2, r2, r5
	mov r9, r2
	ldrb r1, [r2]
	lsl r0, r1, #0x19
	lsr r7, r0, #0x19
	cmp r7, #0
	beq _08010314
	cmp r7, #1
	beq _0801031E
	ldr r2, _0801030C @ =0x00000814
	add r1, r5, r2
	mov r0, #4
	ldrb r2, [r1, #2]
	orr r0, r2
	strb r0, [r1, #2]
	add r0, r6, #0
	bl sub_08009EAC
	bl sub_080611AC
	ldr r0, _08010310 @ =0x0000080D
	add r1, r5, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	b _080103B2
	.align 2, 0
_08010304: .4byte 0x020185C0
_08010308: .4byte 0x0000080A
_0801030C: .4byte 0x00000814
_08010310: .4byte 0x0000080D
_08010314:
	add r0, r4, #0
	mov r1, #0xE
	bl sub_080240A8
	b _0801039A
_0801031E:
	ldr r2, _080103C4 @ =0x00000814
	add r2, r2, r5
	mov sl, r2
	add r0, r4, #0
	mov r1, r8
	bl sub_08009C08
	add r0, r4, #0
	mov r1, r8
	bl sub_08009BA8
	lsl r1, r4, #0x10
	lsr r1, r1, #0x10
	and r1, r7
	mov r3, #2
	neg r3, r3
	ldr r0, [sp, #0]
	and r0, r3
	orr r0, r1
	mov r2, #0x1F
	neg r2, r2
	and r0, r2
	mov r1, #0x1C
	orr r0, r1
	ldr r4, _080103C8 @ =0xFFFFC01F
	and r0, r4
	ldr r5, _080103CC @ =0xFFFFBFFF
	and r0, r5
	mov r1, #0x80
	lsl r1, r1, #8
	orr r0, r1
	str r0, [sp, #0]
	add r1, r6, #0
	and r1, r7
	ldr r0, [sp, #4]
	and r0, r3
	orr r0, r1
	and r0, r2
	mov r1, #0x16
	orr r0, r1
	str r0, [sp, #4]
	ldr r2, _080103D0 @ =0x020192E4
	and r6, r7
	ldr r1, _080103D4 @ =0x00000D64
	mul r1, r6
	add r1, r1, r2
	ldrb r1, [r1, #2]
	lsl r1, r1, #5
	and r0, r4
	orr r0, r1
	and r0, r5
	ldr r1, _080103D8 @ =0xFFFF7FFF
	and r0, r1
	str r0, [sp, #4]
	mov r1, sl
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl sub_080242C4
_0801039A:
	mov r0, r9
	ldrb r2, [r0]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	mov r1, r9
_080103B2:
	strb r0, [r1]
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080103C4: .4byte 0x00000814
_080103C8: .4byte 0xFFFFC01F
_080103CC: .4byte 0xFFFFBFFF
_080103D0: .4byte 0x020192E4
_080103D4: .4byte 0x00000D64
_080103D8: .4byte 0xFFFF7FFF
	thumb_func_end sub_080102B0

