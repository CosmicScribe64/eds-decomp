	thumb_func_start sub_080119F8
sub_080119F8: @ 0x080119F8
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	ldr r4, _08011A30 @ =0x020185C0
	ldrh r0, [r4]
	lsr r6, r0, #0xF
	ldrh r7, [r4, #2]
	ldr r1, _08011A34 @ =0x0000080A
	add r0, r4, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x19
	lsr r5, r0, #0x19
	cmp r5, #0
	beq _08011A3C
	cmp r5, #1
	beq _08011A78
	bl sub_080611AC
	ldr r2, _08011A38 @ =0x0000080D
	add r1, r4, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	b _08011BB4
_08011A30: .4byte 0x020185C0
_08011A34: .4byte 0x0000080A
_08011A38: .4byte 0x0000080D
_08011A3C:
	cmp r7, #9
	bgt _08011A4A
	add r1, r7, #5
	add r0, r6, #0
	bl sub_08060FD0
	b _08011A52
_08011A4A:
	add r0, r6, #0
	mov r1, #0xA
	bl sub_08060FD0
_08011A52:
	ldr r2, _08011A70 @ =0x020185C0
	ldr r0, _08011A74 @ =0x0000080A
	add r2, r2, r0
	ldrb r3, [r2]
	lsl r1, r3, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _08011BB4
_08011A70: .4byte 0x020185C0
_08011A74: .4byte 0x0000080A
_08011A78:
	cmp r7, #9
	bgt _08011AF4
	ldr r1, _08011ADC @ =0x00000814
	add r0, r4, r1
	add r1, r6, #0
	and r1, r5
	ldr r2, _08011AE0 @ =0x00000D64
	add r4, r1, #0
	mul r4, r2
	ldr r2, _08011AE4 @ =0x0201930C
	add r1, r4, r2
	mov r2, #0x94
	add r3, r7, #0
	mul r3, r2
	mov r8, r3
	mov r2, #0xB9
	lsl r2, r2, #2
	add r2, r8
	add r1, r1, r2
	bl sub_08007558
	add r1, r7, #5
	add r0, r6, #0
	bl sub_08008E44
	lsl r3, r6, #0x10
	lsr r0, r3, #0x10
	and r0, r5
	mov r1, #2
	neg r1, r1
	ldr r2, [sp, #0]
	and r2, r1
	orr r2, r0
	mov r0, #0x1F
	neg r0, r0
	and r2, r0
	mov r0, #0xA
	orr r2, r0
	ldr r0, _08011AE8 @ =0x000001FF
	and r7, r0
	lsl r1, r7, #5
	ldr r0, _08011AEC @ =0xFFFFC01F
	and r2, r0
	orr r2, r1
	str r2, [sp, #0]
	add r4, r8
	ldr r0, _08011AF0 @ =0x020195F0
	add r4, r4, r0
	b _08011B44
	.align 2, 0
_08011ADC: .4byte 0x00000814
_08011AE0: .4byte 0x00000D64
_08011AE4: .4byte 0x0201930C
_08011AE8: .4byte 0x000001FF
_08011AEC: .4byte 0xFFFFC01F
_08011AF0: .4byte 0x020195F0
_08011AF4:
	ldr r2, _08011BC4 @ =0x00000814
	add r0, r4, r2
	add r1, r6, #0
	and r1, r5
	ldr r2, _08011BC8 @ =0x00000D64
	add r3, r1, #0
	mul r3, r2
	mov r8, r3
	ldr r1, _08011BCC @ =0x0201930C
	mov r9, r1
	mov r1, r8
	add r1, r9
	mov r2, #0x94
	add r4, r7, #0
	mul r4, r2
	add r1, r1, r4
	bl sub_08007558
	add r0, r6, #0
	add r1, r7, #0
	bl sub_08008E44
	lsl r3, r6, #0x10
	lsr r0, r3, #0x10
	and r0, r5
	mov r1, #2
	neg r1, r1
	ldr r2, [sp, #0]
	and r2, r1
	orr r2, r0
	mov r0, #0x1F
	neg r0, r0
	and r2, r0
	mov r0, #0x14
	orr r2, r0
	ldr r0, _08011BD0 @ =0xFFFFC01F
	and r2, r0
	str r2, [sp, #0]
	add r4, r8
	add r4, r9
_08011B44:
	ldrb r1, [r4, #6]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	and r0, r5
	lsl r0, r0, #0xE
	ldr r1, _08011BD4 @ =0xFFFFBFFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldrb r4, [r4, #6]
	lsl r0, r4, #0x1E
	lsr r0, r0, #0x1F
	and r0, r5
	lsl r0, r0, #0xF
	ldr r2, _08011BD8 @ =0xFFFF7FFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	lsr r2, r3, #0x10
	mov r1, #2
	neg r1, r1
	ldr r0, [sp, #4]
	and r0, r1
	orr r0, r2
	sub r1, #0x1D
	and r0, r1
	mov r1, #0x1C
	orr r0, r1
	ldr r1, _08011BD0 @ =0xFFFFC01F
	and r0, r1
	sub r1, #0x20
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #8
	orr r0, r1
	str r0, [sp, #4]
	ldr r4, _08011BDC @ =0x02018DD4
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl sub_080242C4
	sub r4, #0xA
	ldrb r2, [r4]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4]
_08011BB4:
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08011BC4: .4byte 0x00000814
_08011BC8: .4byte 0x00000D64
_08011BCC: .4byte 0x0201930C
_08011BD0: .4byte 0xFFFFC01F
_08011BD4: .4byte 0xFFFFBFFF
_08011BD8: .4byte 0xFFFF7FFF
_08011BDC: .4byte 0x02018DD4
	thumb_func_end sub_080119F8

