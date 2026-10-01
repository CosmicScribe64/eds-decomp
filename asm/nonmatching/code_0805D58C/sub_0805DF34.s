	thumb_func_start sub_0805DF34
sub_0805DF34: @ 0x0805DF34
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov sl, r0
	add r4, r1, #0
	add r0, r3, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r9, r2
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	str r1, [sp, #0]
	lsr r0, r0, #0x14
	lsl r0, r0, #5
	mov r1, #0xA0
	lsl r1, r1, #0x13
	add r0, r0, r1
	lsl r1, r4, #7
	ldr r2, _0805E040 @ =0x08608360
	add r1, r1, r2
	mov r2, #0x80
	bl sub_08075294
	lsl r0, r4, #4
	add r0, r0, r4
	lsl r0, r0, #3
	sub r0, r0, r4
	lsl r0, r0, #5
	ldr r7, _0805E044 @ =0x082A6500
	add r6, r0, r7
	mov r1, sl
	lsl r0, r1, #0xE
	ldr r2, _0805E048 @ =0x06004000
	add r0, r0, r2
	mov r7, r9
	lsl r1, r7, #5
	add r5, r0, r1
	mov r0, #0x3F
	mov ip, r0
	mov r1, #0xFC
	lsl r1, r1, #4
	mov r8, r1
	mov r2, #0xB4
	lsl r2, r2, #2
	str r2, [sp, #4]
_0805DF98:
	ldrh r2, [r6]
	ldrh r3, [r6, #2]
	ldrh r4, [r6, #4]
	add r1, r2, #0
	mov r7, ip
	and r1, r7
	add r0, r2, #0
	mov r7, r8
	and r0, r7
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
	mov r0, ip
	and r2, r0
	lsr r3, r3, #6
	mov r1, #0xF
	add r0, r4, #0
	and r0, r1
	lsl r0, r0, #2
	orr r3, r0
	lsl r3, r3, #8
	orr r2, r3
	strh r2, [r5, #4]
	lsr r4, r4, #4
	add r0, r4, #0
	mov r1, ip
	and r0, r1
	and r4, r7
	lsl r4, r4, #2
	orr r0, r4
	strh r0, [r5, #6]
	add r6, #6
	add r5, #8
	ldr r2, [sp, #4]
	sub r2, #1
	str r2, [sp, #4]
	cmp r2, #0
	bne _0805DF98
	mov r6, sl
	lsl r0, r6, #0xE
	ldr r7, _0805E048 @ =0x06004000
	add r0, r0, r7
	mov r2, r9
	lsl r1, r2, #5
	add r1, r0, r1
	mov r3, #0
	ldr r5, _0805E04C @ =0x00000B3F
	ldr r6, [sp, #0]
	lsl r0, r6, #0x18
	ldr r4, _0805E050 @ =0x00003F3F
	lsr r0, r0, #0x18
	lsl r2, r0, #8
	orr r2, r0
_0805E01C:
	add r0, r4, #0
	ldrh r7, [r1]
	and r0, r7
	add r0, r0, r2
	strh r0, [r1]
	add r1, #2
	add r3, #1
	cmp r3, r5
	bls _0805E01C
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0805E040: .4byte gUnk_08608360
_0805E044: .4byte gUnk_082A6500
_0805E048: .4byte 0x06004000
_0805E04C: .4byte 0x00000B3F
_0805E050: .4byte 0x00003F3F
	thumb_func_end sub_0805DF34

