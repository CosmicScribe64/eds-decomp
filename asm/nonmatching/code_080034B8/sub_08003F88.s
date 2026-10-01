	thumb_func_start sub_08003F88
sub_08003F88: @ 0x08003F88
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r1, _08004054 @ =0x0201F814
	mov r0, #6
	ldrb r2, [r1]
	and r0, r2
	cmp r0, #0
	bne _08004042
	ldrb r1, [r1, #1]
	lsr r0, r1, #5
	mov r7, #4
	mov r8, r7
	cmp r0, #3
	bhi _08003FB0
	mov r0, #5
	mov r8, r0
_08003FB0:
	ldr r0, _08004058 @ =0x03000040
	ldr r1, _0800405C @ =0x0000485E
	add r0, r0, r1
	ldrh r0, [r0]
	lsr r5, r0, #3
	mov r0, #7
	and r5, r0
	mov r4, #0
	cmp r4, r8
	bge _08004042
	ldr r3, _08004060 @ =0x02011C20
	ldr r2, _08004064 @ =0x000020D0
	mov sl, r2
	mov r6, #0xAC
	lsl r6, r6, #0xE
	ldr r7, _08004068 @ =0x08198618
	mov r9, r7
_08003FD2:
	ldr r0, _08004054 @ =0x0201F814
	ldr r2, [r0]
	lsl r2, r2, #0x10
	lsr r1, r2, #0x1D
	lsl r0, r1, #2
	add r0, r0, r1
	add r0, r0, r4
	add r0, #1
	lsl r0, r0, #2
	add r0, r0, r3
	add r0, sl
	ldrh r0, [r0]
	lsl r1, r0, #0x15
	lsr r1, r1, #0x15
	lsr r2, r2, #0x1D
	lsl r0, r2, #2
	add r0, r0, r2
	add r0, r0, r4
	add r0, #1
	lsl r0, r0, #2
	add r0, r0, r3
	add r0, sl
	ldr r0, [r0]
	lsl r0, r0, #0xA
	lsr r0, r0, #0x15
	sub r1, r1, r0
	lsr r2, r1, #0x1F
	cmp r1, #0
	ble _08004010
	mov r2, #1
	neg r2, r2
_08004010:
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #4
	add r0, #0x84
	orr r0, r6
	lsl r1, r5, #1
	add r1, r9
	ldrh r1, [r1]
	mov r7, #0x80
	lsl r7, r7, #5
	add r2, r1, r7
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r1, #0x80
	lsl r1, r1, #7
	str r3, [sp, #0]
	bl sub_080761F0
	mov r0, #0xC0
	lsl r0, r0, #0xD
	add r6, r6, r0
	add r4, #1
	ldr r3, [sp, #0]
	cmp r4, r8
	blt _08003FD2
_08004042:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08004054: .4byte 0x0201F814
_08004058: .4byte 0x03000040
_0800405C: .4byte 0x0000485E
_08004060: .4byte 0x02011C20
_08004064: .4byte 0x000020D0
_08004068: .4byte gUnk_08198618
	thumb_func_end sub_08003F88

