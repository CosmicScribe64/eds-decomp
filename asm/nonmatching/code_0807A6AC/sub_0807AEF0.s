	thumb_func_start sub_0807AEF0
sub_0807AEF0: @ 0x0807AEF0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r4, r0, #0
	add r7, r1, #0
	add r0, r2, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	lsl r0, r0, #0x16
	mov r1, #0x80
	lsl r1, r1, #0x10
	add r0, r0, r1
	lsr r2, r0, #0x10
	mov sl, r2
	lsr r0, r0, #0xF
	mov r1, #0xA0
	lsl r1, r1, #0x13
	add r0, r0, r1
	lsl r1, r4, #7
	ldr r2, _0807AFEC @ =0x08608360
	add r1, r1, r2
	mov r2, #0x80
	bl sub_08075294
	lsl r0, r4, #4
	add r0, r0, r4
	lsl r0, r0, #3
	sub r0, r0, r4
	lsl r0, r0, #5
	ldr r4, _0807AFF0 @ =0x082A6500
	add r6, r0, r4
	add r5, r7, #0
	mov r0, #0x3F
	mov r8, r0
	mov r1, #0xFC
	lsl r1, r1, #4
	mov r9, r1
	mov r2, #0xB4
	lsl r2, r2, #2
	mov ip, r2
_0807AF46:
	ldrh r2, [r6]
	ldrh r3, [r6, #2]
	ldrh r4, [r6, #4]
	str r4, [sp, #0]
	add r1, r2, #0
	mov r0, r8
	and r1, r0
	add r0, r2, #0
	mov r4, r9
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
	ldr r0, [sp, #0]
	and r0, r1
	lsl r0, r0, #2
	orr r3, r0
	lsl r3, r3, #8
	orr r2, r3
	strh r2, [r5, #4]
	ldr r1, [sp, #0]
	lsr r4, r1, #4
	add r0, r4, #0
	mov r2, r8
	and r0, r2
	mov r1, r9
	and r4, r1
	lsl r4, r4, #2
	orr r0, r4
	strh r0, [r5, #6]
	add r6, #6
	add r5, #8
	mov r2, #1
	neg r2, r2
	add ip, r2
	mov r4, ip
	cmp r4, #0
	bne _0807AF46
	mov r0, #0
	mov ip, r0
	ldr r3, _0807AFF4 @ =0x00000B3F
	mov r1, sl
	lsl r0, r1, #0x18
	ldr r2, _0807AFF8 @ =0x00003F3F
	lsr r0, r0, #0x18
	lsl r1, r0, #8
	orr r1, r0
_0807AFC6:
	add r0, r2, #0
	ldrh r4, [r7]
	and r0, r4
	add r0, r0, r1
	strh r0, [r7]
	add r7, #2
	mov r0, #1
	add ip, r0
	cmp ip, r3
	bls _0807AFC6
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807AFEC: .4byte gUnk_08608360
_0807AFF0: .4byte gUnk_082A6500
_0807AFF4: .4byte 0x00000B3F
_0807AFF8: .4byte 0x00003F3F
	thumb_func_end sub_0807AEF0

