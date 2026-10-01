	thumb_func_start sub_0800DF94
sub_0800DF94: @ 0x0800DF94
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r6, _0800DFC4 @ =0x020185C0
	ldrh r0, [r6]
	lsr r7, r0, #0xF
	ldrh r5, [r6, #2]
	ldr r1, _0800DFC8 @ =0x0000080A
	add r1, r1, r6
	mov sl, r1
	ldrb r2, [r1]
	lsl r0, r2, #0x19
	lsr r4, r0, #0x19
	cmp r4, #1
	beq _0800E00C
	cmp r4, #1
	bgt _0800DFCC
	cmp r4, #0
	beq _0800DFD2
	b _0800E1A8
	.align 2, 0
_0800DFC4: .4byte 0x020185C0
_0800DFC8: .4byte 0x0000080A
_0800DFCC:
	cmp r4, #2
	beq _0800E0B8
	b _0800E1A8
_0800DFD2:
	mov r0, #0x94
	mul r0, r5
	ldr r1, _0800DFF0 @ =0x00000D64
	mul r1, r7
	add r0, r0, r1
	ldr r1, _0800DFF4 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0800DFFC
	ldr r3, _0800DFF8 @ =0x0000080D
	add r1, r6, r3
	b _0800E1BC
	.align 2, 0
_0800DFF0: .4byte 0x00000D64
_0800DFF4: .4byte 0x0201930C
_0800DFF8: .4byte 0x0000080D
_0800DFFC:
	add r0, r5, #0
	bl sub_08062354
	add r1, r0, #0
	add r0, r7, #0
	bl sub_080240A8
	b _0800E080
_0800E00C:
	mov r0, #0x11
	bl sub_08077AEC
	add r0, r7, #0
	and r0, r4
	mov r1, #2
	neg r1, r1
	ldr r2, [sp, #0]
	and r2, r1
	orr r2, r0
	mov r0, #0x1F
	neg r0, r0
	and r2, r0
	ldr r0, _0800E09C @ =0x000001FF
	and r0, r5
	lsl r0, r0, #5
	ldr r1, _0800E0A0 @ =0xFFFFC01F
	and r2, r1
	orr r2, r0
	str r2, [sp, #0]
	add r1, r7, #0
	and r1, r4
	mov r0, #0x94
	add r3, r5, #0
	mul r3, r0
	ldr r0, _0800E0A4 @ =0x00000D64
	mul r0, r1
	add r3, r3, r0
	ldr r0, _0800E0A8 @ =0x0201930C
	add r3, r3, r0
	ldrb r1, [r3, #6]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	and r0, r4
	lsl r0, r0, #0xE
	ldr r1, _0800E0AC @ =0xFFFFBFFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldrb r3, [r3, #6]
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	and r0, r4
	lsl r0, r0, #0xF
	ldr r2, _0800E0B0 @ =0xFFFF7FFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldr r1, _0800E0B4 @ =0x0868CAC0
	mov r0, sp
	mov r2, #0
	mov r3, #0
	bl sub_08024380
	add r0, r7, #0
	add r1, r5, #0
	bl sub_08060FD0
_0800E080:
	mov r3, sl
	ldrb r2, [r3]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _0800E1C6
	.align 2, 0
_0800E09C: .4byte 0x000001FF
_0800E0A0: .4byte 0xFFFFC01F
_0800E0A4: .4byte 0x00000D64
_0800E0A8: .4byte 0x0201930C
_0800E0AC: .4byte 0xFFFFBFFF
_0800E0B0: .4byte 0xFFFF7FFF
_0800E0B4: .4byte gUnk_0868CAC0
_0800E0B8:
	mov r0, #1
	mov r9, r0
	add r1, r7, #0
	and r1, r0
	mov r0, #0x94
	add r2, r5, #0
	mul r2, r0
	mov ip, r2
	ldr r0, _0800E190 @ =0x00000D64
	mul r1, r0
	add r4, r2, r1
	ldr r2, _0800E194 @ =0x0201930C
	add r4, r4, r2
	mov r0, #0x10
	ldrb r3, [r4, #2]
	orr r0, r3
	strb r0, [r4, #2]
	ldr r0, _0800E198 @ =0x00000814
	add r0, r0, r6
	mov r8, r0
	add r1, r1, r2
	add r1, ip
	bl sub_08007558
	add r0, r7, #0
	add r1, r5, #0
	bl sub_08008E80
	mov r1, sp
	mov r6, #2
	neg r6, r6
	add r0, r6, #0
	ldrb r1, [r1]
	and r0, r1
	orr r0, r7
	mov r1, sp
	strb r0, [r1]
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	mov r1, sp
	strb r0, [r1]
	ldr r1, _0800E19C @ =0x000001FF
	add r0, r1, #0
	and r5, r0
	lsl r2, r5, #5
	mov r1, sp
	ldr r5, _0800E1A0 @ =0xFFFFC01F
	add r0, r5, #0
	ldrh r1, [r1]
	and r0, r1
	orr r0, r2
	mov r1, sp
	strh r0, [r1]
	ldrb r2, [r4, #6]
	lsl r1, r2, #0x1F
	mov r3, sp
	lsr r1, r1, #0x1F
	lsl r1, r1, #6
	ldrb r2, [r3, #1]
	mov r0, #0x41
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #1]
	ldrb r4, [r4, #6]
	lsl r1, r4, #0x1E
	lsr r1, r1, #0x1F
	lsl r1, r1, #7
	mov r4, #0x7F
	mov r2, #0x7F
	and r0, r2
	orr r0, r1
	strb r0, [r3, #1]
	mov r3, r9
	and r7, r3
	ldr r0, [sp, #4]
	and r0, r6
	orr r0, r7
	mov r1, #0x1E
	orr r0, r1
	and r0, r5
	ldr r1, _0800E1A4 @ =0xFFFFBFFF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #8
	orr r0, r1
	str r0, [sp, #4]
	mov r1, r8
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl sub_080242C4
	mov r3, sl
	ldrb r2, [r3]
	lsl r0, r2, #0x19
	lsr r0, r0, #0x19
	add r0, #1
	and r0, r4
	mov r1, #0x80
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r3]
	b _0800E1C6
_0800E190: .4byte 0x00000D64
_0800E194: .4byte 0x0201930C
_0800E198: .4byte 0x00000814
_0800E19C: .4byte 0x000001FF
_0800E1A0: .4byte 0xFFFFC01F
_0800E1A4: .4byte 0xFFFFBFFF
_0800E1A8:
	add r0, r7, #0
	mov r1, #0
	add r2, r5, #0
	bl sub_08024134
	bl sub_080611AC
	ldr r1, _0800E1D8 @ =0x020185C0
	ldr r0, _0800E1DC @ =0x0000080D
	add r1, r1, r0
_0800E1BC:
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0800E1C6:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800E1D8: .4byte 0x020185C0
_0800E1DC: .4byte 0x0000080D
	thumb_func_end sub_0800DF94

