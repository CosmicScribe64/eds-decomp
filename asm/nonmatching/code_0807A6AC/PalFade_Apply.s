	thumb_func_start PalFade_Apply
PalFade_Apply: @ 0x0807B224
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	add r6, r0, #0
	ldr r1, _0807B308 @ =0x00000C04
	add r0, r6, r1
	ldr r0, [r0]
	mov ip, r0
	ldr r5, _0807B30C @ =0x00000C08
	add r0, r6, r5
	ldrh r0, [r0]
	cmp r0, #1
	bne _0807B2F6
	mov r7, #0
	ldr r0, _0807B310 @ =0x00000C02
	mov sl, r0
	add r0, r6, r0
	ldrh r0, [r0]
	cmp r7, r0
	bcs _0807B2E2
	sub r1, #4
	add r1, r6, r1
	str r1, [sp, #0]
	mov r5, #0xF8
	lsl r5, r5, #2
	mov r9, r5
	mov r0, #0xF8
	lsl r0, r0, #7
	mov r8, r0
_0807B264:
	lsl r0, r7, #1
	add r0, r6, r0
	ldrh r0, [r0]
	mov r1, sp
	strh r0, [r1, #8]
	mov r2, #0x1F
	add r5, r0, #0
	and r2, r5
	lsl r3, r7, #2
	add r3, r6, r3
	mov r1, #0x80
	lsl r1, r1, #3
	add r0, r3, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x18
	asr r0, r0, #0x18
	ldr r5, [sp, #0]
	ldrb r4, [r5]
	mul r0, r4
	asr r0, r0, #5
	add r2, r2, r0
	mov r0, #0x1F
	and r2, r0
	mov r0, r9
	mov r1, sp
	ldrh r1, [r1, #8]
	and r0, r1
	ldr r5, _0807B314 @ =0x00000401
	add r5, r3, r5
	mov r1, #0
	ldsb r1, [r5, r1]
	add r5, r1, #0
	mul r5, r4
	add r0, r0, r5
	mov r1, r9
	and r0, r1
	orr r2, r0
	mov r1, r8
	mov r5, sp
	ldrh r5, [r5, #8]
	and r1, r5
	ldr r0, _0807B318 @ =0x00000402
	add r3, r3, r0
	mov r0, #0
	ldsb r0, [r3, r0]
	mul r0, r4
	lsl r0, r0, #5
	add r1, r1, r0
	mov r5, r8
	and r1, r5
	orr r2, r1
	mov r0, ip
	strh r2, [r0]
	mov r1, #2
	add ip, r1
	add r0, r7, #1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	mov r5, sl
	add r0, r6, r5
	ldrh r0, [r0]
	cmp r7, r0
	bcc _0807B264
_0807B2E2:
	mov r1, #0xC0
	lsl r1, r1, #4
	add r0, r6, r1
	ldrb r0, [r0]
	cmp r0, #0x20
	bne _0807B2F6
	ldr r5, _0807B30C @ =0x00000C08
	add r1, r6, r5
	mov r0, #2
	strh r0, [r1]
_0807B2F6:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807B308: .4byte 0x00000C04
_0807B30C: .4byte 0x00000C08
_0807B310: .4byte 0x00000C02
_0807B314: .4byte 0x00000401
_0807B318: .4byte 0x00000402
	thumb_func_end PalFade_Apply

