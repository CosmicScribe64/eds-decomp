	thumb_func_start sub_08061D24
sub_08061D24: @ 0x08061D24
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	add r4, r0, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	lsl r0, r4, #7
	ldr r1, _08061E40 @ =0x08608360
	add r5, r0, r1
	ldr r3, _08061E44 @ =0x05000260
	mov ip, r3
	mov r0, ip
	add r1, r5, #0
	mov r2, #0x80
	bl sub_08075294
	lsl r0, r4, #4
	add r0, r0, r4
	lsl r0, r0, #3
	sub r0, r0, r4
	lsl r0, r0, #5
	ldr r6, _08061E48 @ =0x082A6500
	add r5, r0, r6
	mov r0, #4
	mov r7, #0x3F
	mov r8, r7
_08061D5E:
	mov r2, #2
	add r1, r0, #2
	str r1, [sp, #4]
	add r0, #1
	str r0, [sp, #0]
_08061D68:
	lsl r1, r2, #0x11
	lsr r1, r1, #0x10
	ldr r3, [sp, #4]
	lsl r0, r3, #0x10
	lsr r0, r0, #0xB
	add r1, r1, r0
	lsl r1, r1, #5
	ldr r6, _08061E4C @ =0x06010000
	add r6, r6, r1
	mov ip, r6
	add r6, r5, #0
	mov r5, ip
	mov r7, #0x30
	add r7, r7, r6
	mov r9, r7
	add r2, #1
	mov sl, r2
	mov r0, #7
	str r0, [sp, #8]
_08061D8E:
	ldrh r2, [r6]
	ldrh r3, [r6, #2]
	ldrh r4, [r6, #4]
	add r1, r2, #0
	mov r7, r8
	and r1, r7
	add r0, r2, #0
	mov r7, #0xFC
	lsl r7, r7, #4
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
	mov r0, r8
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
	mov r1, r8
	and r0, r1
	and r4, r7
	lsl r4, r4, #2
	orr r0, r4
	strh r0, [r5, #6]
	add r6, #6
	add r5, #8
	ldr r3, [sp, #8]
	sub r3, #1
	str r3, [sp, #8]
	cmp r3, #0
	bge _08061D8E
	mov r5, r9
	mov r1, ip
	mov r3, #0
	ldr r4, _08061E50 @ =0x00003F3F
	mov r6, #0x30
	lsl r2, r6, #8
	orr r2, r6
_08061E04:
	add r0, r4, #0
	ldrh r7, [r1]
	and r0, r7
	add r0, r0, r2
	strh r0, [r1]
	add r1, #2
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	cmp r3, #0x1F
	bls _08061E04
	mov r1, sl
	lsl r0, r1, #0x10
	lsr r2, r0, #0x10
	cmp r2, #0xA
	bls _08061D68
	ldr r3, [sp, #0]
	lsl r0, r3, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0xD
	bls _08061D5E
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08061E40: .4byte gUnk_08608360
_08061E44: .4byte 0x05000260
_08061E48: .4byte gUnk_082A6500
_08061E4C: .4byte 0x06010000
_08061E50: .4byte 0x00003F3F
	thumb_func_end sub_08061D24

