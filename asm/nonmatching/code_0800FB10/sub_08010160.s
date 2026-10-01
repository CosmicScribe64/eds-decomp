	thumb_func_start sub_08010160
sub_08010160: @ 0x08010160
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	ldr r4, _08010188 @ =0x020185C0
	ldrh r0, [r4]
	lsr r5, r0, #0xF
	ldr r1, _0801018C @ =0x0000080A
	add r7, r4, r1
	ldrb r3, [r7]
	lsl r0, r3, #0x19
	lsr r6, r0, #0x19
	cmp r6, #1
	beq _080101B2
	cmp r6, #1
	bgt _08010190
	cmp r6, #0
	beq _08010196
	b _08010280
	.align 2, 0
_08010188: .4byte 0x020185C0
_0801018C: .4byte 0x0000080A
_08010190:
	cmp r6, #2
	beq _08010256
	b _08010280
_08010196:
	add r0, r5, #0
	mov r1, #0xB
	bl sub_080240A8
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	b _08010276
_080101B2:
	ldr r2, _08010234 @ =0x020192E4
	add r0, r5, #0
	and r0, r6
	ldr r1, _08010238 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #4]
	cmp r0, #0
	beq _0801024C
	ldr r2, _0801023C @ =0x00000814
	add r2, r2, r4
	mov r8, r2
	add r0, r5, #0
	mov r1, #0
	bl sub_08009AD0
	and r6, r5
	mov r5, #2
	neg r5, r5
	ldr r0, [sp, #0]
	and r0, r5
	orr r0, r6
	mov r4, #0x1F
	neg r4, r4
	and r0, r4
	mov r1, #0x1C
	orr r0, r1
	ldr r3, _08010240 @ =0xFFFFC01F
	and r0, r3
	ldr r2, _08010244 @ =0xFFFFBFFF
	and r0, r2
	mov r1, #0x80
	lsl r1, r1, #8
	orr r0, r1
	str r0, [sp, #0]
	ldr r0, [sp, #4]
	and r0, r5
	orr r0, r6
	and r0, r4
	mov r1, #0x1A
	orr r0, r1
	and r0, r3
	and r0, r2
	ldr r1, _08010248 @ =0xFFFF7FFF
	and r0, r1
	str r0, [sp, #4]
	mov r1, r8
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl sub_080242C4
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	b _08010276
	.align 2, 0
_08010234: .4byte 0x020192E4
_08010238: .4byte 0x00000D64
_0801023C: .4byte 0x00000814
_08010240: .4byte 0xFFFFC01F
_08010244: .4byte 0xFFFFBFFF
_08010248: .4byte 0xFFFF7FFF
_0801024C:
	mov r0, #0x80
	neg r0, r0
	and r0, r3
	mov r1, #0xA
	b _08010276
_08010256:
	ldr r2, _0801027C @ =0x00000814
	add r4, r4, r2
	add r0, r4, #0
	bl sub_0800743C
	add r0, r5, #0
	add r1, r4, #0
	bl sub_08007C58
	bl sub_080611AC
	mov r0, #0x80
	neg r0, r0
	ldrb r1, [r7]
	and r0, r1
	mov r1, #1
_08010276:
	orr r0, r1
	strb r0, [r7]
	b _0801029A
_0801027C: .4byte 0x00000814
_08010280:
	add r0, r5, #0
	mov r1, #0xD
	mov r2, #0
	bl sub_08024134
	ldr r1, _080102A8 @ =0x020185C0
	ldr r2, _080102AC @ =0x0000080D
	add r1, r1, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0801029A:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080102A8: .4byte 0x020185C0
_080102AC: .4byte 0x0000080D
	thumb_func_end sub_08010160

