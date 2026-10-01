	thumb_func_start sub_0807C374
sub_0807C374: @ 0x0807C374
	push {lr}
	bl sub_08073574
	bl sub_08073498
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r0, #0
	strh r0, [r1]
	add r1, #0xA
	ldr r2, _0807C438 @ =0x00000105
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _0807C43C @ =0x00000286
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	add r2, #0x81
	add r0, r2, #0
	strh r0, [r1]
	ldr r0, _0807C440 @ =0x03000040
	ldr r1, _0807C444 @ =0x0000040E
	add r0, r0, r1
	mov r1, #0
	mov r2, #0x43
	strh r2, [r0]
	ldr r0, _0807C448 @ =0x0400004C
	strh r1, [r0]
	add r0, #4
	strh r1, [r0]
	add r0, #4
	strh r1, [r0]
	sub r0, #0x42
	strh r1, [r0]
	sub r0, #2
	strh r1, [r0]
	add r0, #6
	strh r1, [r0]
	sub r0, #2
	strh r1, [r0]
	add r0, #6
	strh r1, [r0]
	sub r0, #2
	strh r1, [r0]
	add r0, #6
	strh r1, [r0]
	sub r0, #2
	strh r1, [r0]
	add r0, #0xC
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
	add r0, #0x12
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
	ldr r3, _0807C44C @ =0x0863862C
	mov r0, #0
	mov r1, #0x10
	mov r2, #0x20
	bl sub_080731D0
	mov r0, #0x80
	lsl r0, r0, #4
	ldr r3, _0807C450 @ =0x0863975C
	mov r1, #0x10
	mov r2, #0x88
	bl sub_080731D0
	ldr r0, _0807C454 @ =0x05000220
	ldr r1, _0807C458 @ =0x08639DFC
	mov r2, #0x20
	bl sub_08075294
	ldr r1, _0807C45C @ =0x040000D4
	ldr r0, _0807C460 @ =0x08639E1C
	str r0, [r1]
	ldr r0, _0807C464 @ =0x06010000
	str r0, [r1, #4]
	ldr r0, _0807C468 @ =0x80001600
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r2, #0x80
	lsl r2, r2, #0x18
	cmp r0, #0
	bge _0807C42C
_0807C424:
	ldr r0, [r1, #8]
	and r0, r2
	cmp r0, #0
	bne _0807C424
_0807C42C:
	bl sub_080759F4
	mov r0, #1
	pop {r1}
	bx r1
	.align 2, 0
_0807C438: .4byte 0x00000105
_0807C43C: .4byte 0x00000286
_0807C440: .4byte 0x03000040
_0807C444: .4byte 0x0000040E
_0807C448: .4byte 0x0400004C
_0807C44C: .4byte gUnk_0863862C
_0807C450: .4byte gUnk_0863975C
_0807C454: .4byte 0x05000220
_0807C458: .4byte gUnk_08639DFC
_0807C45C: .4byte 0x040000D4
_0807C460: .4byte gUnk_08639E1C
_0807C464: .4byte 0x06010000
_0807C468: .4byte 0x80001600
	thumb_func_end sub_0807C374

