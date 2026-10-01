	thumb_func_start sub_08010794
sub_08010794: @ 0x08010794
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	ldr r4, _080107C4 @ =0x020185C0
	ldrh r0, [r4]
	lsr r2, r0, #0xF
	ldrh r3, [r4, #2]
	ldr r1, _080107C8 @ =0x0000080A
	add r1, r1, r4
	mov sl, r1
	ldrb r5, [r1]
	lsl r0, r5, #0x19
	lsr r7, r0, #0x19
	cmp r7, #1
	beq _080107F4
	cmp r7, #1
	bgt _080107CC
	cmp r7, #0
	beq _080107D2
	b _080108CC
	.align 2, 0
_080107C4: .4byte 0x020185C0
_080107C8: .4byte 0x0000080A
_080107CC:
	cmp r7, #2
	beq _080108B8
	b _080108CC
_080107D2:
	add r0, r2, #0
	mov r1, #0xB
	bl sub_080240A8
	mov r7, sl
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r7]
	b _080108E0
_080107F4:
	ldr r0, _0801089C @ =0x00000814
	add r0, r0, r4
	mov r9, r0
	add r0, r2, #0
	and r0, r7
	ldr r1, _080108A0 @ =0x00000D64
	add r5, r0, #0
	mul r5, r1
	ldr r6, _080108A4 @ =0x02019968
	add r1, r5, r6
	lsl r4, r3, #2
	add r1, r1, r4
	mov r0, r9
	str r2, [sp, #8]
	str r3, [sp, #0xC]
	bl sub_08007558
	add r4, r4, r5
	add r4, r4, r6
	ldr r0, _080108A8 @ =0xFFFFF000
	ldrh r1, [r4]
	and r0, r1
	strh r0, [r4]
	ldr r2, [sp, #8]
	and r2, r7
	mov r5, #2
	neg r5, r5
	mov r8, r5
	ldr r0, [sp, #0]
	and r0, r5
	orr r0, r2
	mov r6, #0x1F
	neg r6, r6
	and r0, r6
	mov r1, #0x16
	orr r0, r1
	ldr r1, _080108AC @ =0x000001FF
	ldr r3, [sp, #0xC]
	and r3, r1
	lsl r1, r3, #5
	ldr r5, _080108B0 @ =0xFFFFC01F
	and r0, r5
	orr r0, r1
	ldr r4, _080108B4 @ =0xFFFFBFFF
	and r0, r4
	mov r3, #0x80
	lsl r3, r3, #8
	orr r0, r3
	str r0, [sp, #0]
	mov r1, r9
	ldr r0, [r1]
	lsl r2, r0, #0x13
	lsr r2, r2, #0x1F
	and r2, r7
	ldr r1, [sp, #4]
	mov r7, r8
	and r1, r7
	orr r1, r2
	and r1, r6
	mov r2, #0x1C
	orr r1, r2
	and r1, r5
	and r1, r4
	orr r1, r3
	str r1, [sp, #4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl sub_080242C4
	mov r0, sl
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
	mov r1, sl
	b _080108DE
_0801089C: .4byte 0x00000814
_080108A0: .4byte 0x00000D64
_080108A4: .4byte 0x02019968
_080108A8: .4byte 0xFFFFF000
_080108AC: .4byte 0x000001FF
_080108B0: .4byte 0xFFFFC01F
_080108B4: .4byte 0xFFFFBFFF
_080108B8:
	ldrh r0, [r4, #4]
	cmp r0, #0
	beq _080108C4
	add r0, r2, #0
	bl sub_0800A0A8
_080108C4:
	ldr r2, _080108F0 @ =0x00000814
	add r0, r4, r2
	bl sub_080096F4
_080108CC:
	bl sub_080611AC
	ldr r1, _080108F4 @ =0x020185C0
	ldr r5, _080108F8 @ =0x0000080D
	add r1, r1, r5
	mov r0, #0x21
	neg r0, r0
	ldrb r7, [r1]
	and r0, r7
_080108DE:
	strb r0, [r1]
_080108E0:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080108F0: .4byte 0x00000814
_080108F4: .4byte 0x020185C0
_080108F8: .4byte 0x0000080D
	thumb_func_end sub_08010794

