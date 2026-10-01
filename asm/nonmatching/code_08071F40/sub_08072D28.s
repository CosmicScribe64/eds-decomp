	thumb_func_start sub_08072D28
sub_08072D28: @ 0x08072D28
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r4, [sp, #0x28]
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r8, r2
	lsl r3, r3, #0x10
	lsl r4, r4, #0x10
	mov r2, #0xE0
	lsl r2, r2, #0xB
	and r2, r0
	lsr r2, r2, #5
	ldr r0, _08072E7C @ =0x0300045C
	add r5, r2, r0
	lsr r1, r1, #0xF
	add r5, r5, r1
	mov r7, #0
	lsr r6, r3, #0x11
	lsr r0, r4, #0x14
	mov ip, r0
	mov r1, r8
	lsl r1, r1, #7
	mov r9, r1
	lsr r3, r3, #0xB
	str r3, [sp, #4]
	mov r0, r8
	lsl r0, r0, #4
	mov sl, r0
	lsl r4, r4, #8
	str r4, [sp, #0]
_08072D70:
	mov r3, #0
	add r4, r5, #0
	add r4, #0x40
_08072D76:
	lsl r2, r3, #1
	add r2, r2, r5
	add r1, r6, #0
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	strh r1, [r2]
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	cmp r3, #8
	bls _08072D76
	add r5, r4, #0
	add r0, r7, #1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	cmp r7, #9
	bls _08072D70
	mov r1, ip
	lsl r0, r1, #5
	mov r1, #0xA0
	lsl r1, r1, #0x13
	add r0, r0, r1
	ldr r1, _08072E80 @ =0x08608360
	add r1, r9
	mov r2, #0x80
	bl sub_08075294
	mov r0, sl
	add r0, r8
	lsl r0, r0, #3
	mov r4, r8
	sub r0, r0, r4
	lsl r0, r0, #5
	ldr r1, _08072E84 @ =0x082A6500
	add r6, r0, r1
	ldr r4, [sp, #4]
	ldr r0, _08072E88 @ =0x06004000
	add r5, r4, r0
	mov r7, #0
	mov r1, #0x3F
	mov r8, r1
	mov r4, #0xFC
	lsl r4, r4, #4
	mov ip, r4
	ldr r0, _08072E8C @ =0x000002CF
	mov r9, r0
_08072DD4:
	ldrh r2, [r6]
	ldrh r3, [r6, #2]
	ldrh r1, [r6, #4]
	mov sl, r1
	add r1, r2, #0
	mov r4, r8
	and r1, r4
	add r0, r2, #0
	mov r4, ip
	and r0, r4
	lsl r0, r0, #2
	orr r1, r0
	strh r1, [r5]
	lsr r2, r2, #0xC
	mov r1, #3
	add r0, r3, #0
	and r0, r1
	lsl r0, r0, #4
	orr r2, r0
	mov r0, #0xFC
	and r0, r3
	lsl r0, r0, #6
	orr r2, r0
	strh r2, [r5, #2]
	lsr r3, r3, #8
	add r2, r3, #0
	mov r0, r8
	and r2, r0
	lsr r3, r3, #6
	mov r1, #0xF
	mov r0, sl
	and r0, r1
	lsl r0, r0, #2
	orr r3, r0
	lsl r3, r3, #8
	orr r2, r3
	strh r2, [r5, #4]
	mov r1, sl
	lsr r4, r1, #4
	add r0, r4, #0
	mov r1, r8
	and r0, r1
	mov r1, ip
	and r4, r1
	lsl r4, r4, #2
	orr r0, r4
	strh r0, [r5, #6]
	add r6, #6
	add r5, #8
	add r0, r7, #1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	cmp r7, r9
	bls _08072DD4
	ldr r4, [sp, #4]
	ldr r0, _08072E88 @ =0x06004000
	add r5, r4, r0
	mov r7, #0
	ldr r3, _08072E90 @ =0x00003F3F
	ldr r1, [sp, #0]
	lsr r0, r1, #0x18
	lsl r1, r0, #8
	orr r1, r0
	ldr r2, _08072E94 @ =0x00000B3F
_08072E54:
	add r0, r3, #0
	ldrh r4, [r5]
	and r0, r4
	add r0, r0, r1
	strh r0, [r5]
	add r5, #2
	add r0, r7, #1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	cmp r7, r2
	bls _08072E54
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08072E7C: .4byte 0x0300045C
_08072E80: .4byte gUnk_08608360
_08072E84: .4byte gUnk_082A6500
_08072E88: .4byte 0x06004000
_08072E8C: .4byte 0x000002CF
_08072E90: .4byte 0x00003F3F
_08072E94: .4byte 0x00000B3F
	thumb_func_end sub_08072D28

