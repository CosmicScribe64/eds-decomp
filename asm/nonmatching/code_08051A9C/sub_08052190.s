	thumb_func_start sub_08052190
sub_08052190: @ 0x08052190
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r5, _080522A0 @ =0x0201AE60
	ldrh r0, [r5, #8]
	add r0, #1
	lsl r7, r0, #3
	add r0, r5, #0
	add r0, #0x21
	ldrb r0, [r0]
	ldrh r1, [r5, #0xE]
	sub r0, r0, r1
	lsl r0, r0, #3
	str r0, [sp, #0]
	add r6, r5, #0
	add r6, #0x23
	ldrb r0, [r6]
	cmp r0, #0
	bne _08052202
	ldr r0, _080522A4 @ =0x050003E0
	ldr r1, _080522A8 @ =0x0822C300
	mov r2, #0x20
	bl sub_08075294
	mov r0, #0xC
	mov r1, #2
	bl sub_08074B08
	ldr r2, _080522AC @ =0x00000A0F
	ldr r4, _080522B0 @ =0x0819D214
	ldrh r1, [r5, #0x14]
	lsl r0, r1, #2
	add r0, r0, r4
	ldr r3, [r0]
	mov r0, #1
	mov r1, #1
	bl sub_0807501C
	ldr r2, _080522B4 @ =0x00000A02
	ldrh r5, [r5, #0x14]
	lsl r0, r5, #2
	add r0, r0, r4
	ldr r3, [r0]
	mov r0, #0
	mov r1, #0
	bl sub_0807501C
	ldr r0, _080522B8 @ =0x06016C80
	mov r1, #0
	bl sub_08075114
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
_08052202:
	mov r4, #0xD9
	lsl r4, r4, #2
	ldr r0, [sp, #0]
	lsl r5, r0, #0x10
	add r0, r7, #0
	orr r0, r5
	ldr r1, _080522BC @ =0x00004040
	mov sl, r1
	mov r1, #0xF0
	lsl r1, r1, #8
	mov r9, r1
	add r2, r4, #0
	mov r1, r9
	orr r2, r1
	mov r1, sl
	bl sub_080761F0
	add r4, #4
	mov r0, #0x20
	add r0, r0, r7
	mov r8, r0
	orr r0, r5
	add r2, r4, #0
	mov r1, r9
	orr r2, r1
	mov r1, sl
	bl sub_080761F0
	add r4, #4
	add r6, r7, #0
	add r6, #0x40
	orr r5, r6
	add r2, r4, #0
	mov r0, r9
	orr r2, r0
	add r0, r5, #0
	mov r1, sl
	bl sub_080761F0
	add r4, #4
	ldr r5, [sp, #0]
	add r5, #8
	lsl r5, r5, #0x10
	orr r7, r5
	add r2, r4, #0
	mov r1, r9
	orr r2, r1
	add r0, r7, #0
	mov r1, sl
	bl sub_080761F0
	add r4, #4
	mov r0, r8
	orr r0, r5
	mov r8, r0
	add r2, r4, #0
	mov r1, r9
	orr r2, r1
	mov r1, sl
	bl sub_080761F0
	add r4, #4
	orr r6, r5
	mov r0, r9
	orr r4, r0
	add r0, r6, #0
	mov r1, sl
	add r2, r4, #0
	bl sub_080761F0
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080522A0: .4byte 0x0201AE60
_080522A4: .4byte 0x050003E0
_080522A8: .4byte gUnk_0822C300
_080522AC: .4byte 0x00000A0F
_080522B0: .4byte gUnk_0819D214
_080522B4: .4byte 0x00000A02
_080522B8: .4byte 0x06016C80
_080522BC: .4byte 0x00004040
	thumb_func_end sub_08052190

