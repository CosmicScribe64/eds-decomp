	thumb_func_start SoundMain
SoundMain: @ 0x0807E554
	push {r4, lr}
	bl SoundStartPendingSE
	ldr r0, _0807E5AC @ =0x03005210
	mov ip, r0
	mov r2, #0xC5
	lsl r2, r2, #1
	add r2, ip
	mov r3, #0
	ldsh r4, [r2, r3]
	cmp r4, #0
	blt _0807E65E
	mov r0, #0xC4
	lsl r0, r0, #1
	add r0, ip
	ldrh r1, [r0]
	mov r0, #0x80
	and r0, r1
	cmp r0, #0
	beq _0807E5C2
	mov r0, #0xC7
	lsl r0, r0, #1
	add r0, ip
	mov r3, #0
	ldsh r1, [r0, r3]
	add r0, r4, #0
	cmp r1, r0
	beq _0807E5C2
	ldr r0, _0807E5B0 @ =0x00000191
	add r0, ip
	ldrb r0, [r0]
	cmp r0, #0
	beq _0807E5B8
	mov r2, #0xC9
	lsl r2, r2, #1
	add r2, ip
	mov r1, #0
	mov r0, #0x10
	strb r0, [r2]
	ldr r0, _0807E5B4 @ =0x00000193
	add r0, ip
	strb r1, [r0]
	b _0807E65E
	.align 2, 0
_0807E5AC: .4byte 0x03005210
_0807E5B0: .4byte 0x00000191
_0807E5B4: .4byte 0x00000193
_0807E5B8:
	mov r1, #0xC9
	lsl r1, r1, #1
	add r1, ip
	mov r0, #0x10
	strb r0, [r1]
_0807E5C2:
	mov r0, #0xCA
	lsl r0, r0, #1
	add r0, ip
	ldrb r0, [r0]
	cmp r0, #0
	bne _0807E5E4
	ldr r1, _0807E5DC @ =0x00000191
	add r1, ip
	mov r0, #0x10
	strb r0, [r1]
	ldr r1, _0807E5E0 @ =0x00000193
	add r1, ip
	b _0807E5F2
_0807E5DC: .4byte 0x00000191
_0807E5E0: .4byte 0x00000193
_0807E5E4:
	mov r1, #0xC9
	lsl r1, r1, #1
	add r1, ip
	strb r0, [r1]
	ldr r1, _0807E664 @ =0x00000193
	add r1, ip
	mov r0, #0x10
_0807E5F2:
	strb r0, [r1]
	mov r1, #0xC8
	lsl r1, r1, #1
	add r1, ip
	mov r0, #0
	strb r0, [r1]
	mov r2, #0xC5
	lsl r2, r2, #1
	add r2, ip
	ldrh r0, [r2]
	mov r1, #0xC7
	lsl r1, r1, #1
	add r1, ip
	strh r0, [r1]
	ldr r0, _0807E668 @ =0x0000FFFF
	strh r0, [r2]
	mov r3, ip
	add r3, #8
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #3
	ldr r1, _0807E66C @ =0x080E09D0
	add r2, r0, r1
	ldrh r1, [r2]
	ldrh r0, [r2, #2]
	lsl r0, r0, #0x10
	orr r1, r0
	mov r0, ip
	str r1, [r0, #4]
	add r2, #4
	mov r4, #0xA
	mov r1, #0xC0
_0807E632:
	strb r1, [r3, #0x10]
	ldrh r0, [r2]
	strh r0, [r3, #4]
	sub r4, #1
	add r2, #2
	add r3, #0x18
	cmp r4, #0
	bne _0807E632
	mov r1, #0xC5
	lsl r1, r1, #1
	add r1, ip
	ldr r0, _0807E668 @ =0x0000FFFF
	strh r0, [r1]
	mov r2, #0xC4
	lsl r2, r2, #1
	add r2, ip
	ldrh r1, [r2]
	ldr r0, _0807E670 @ =0x0000BFFF
	and r0, r1
	mov r1, #0x80
	orr r0, r1
	strh r0, [r2]
_0807E65E:
	pop {r4}
	pop {r0}
	bx r0
_0807E664: .4byte 0x00000193
_0807E668: .4byte 0x0000FFFF
_0807E66C: .4byte gSongTable
_0807E670: .4byte 0x0000BFFF
	thumb_func_end SoundMain

