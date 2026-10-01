	thumb_func_start sub_0802F518
sub_0802F518: @ 0x0802F518
	push {r4, lr}
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802F558
	ldr r4, _0802F550 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r2, r0, #0x1F
	lsr r0, r2, #0x1F
	mov r1, #1
	sub r0, r1, r0
	and r0, r1
	ldr r3, _0802F554 @ =0x00000D64
	mul r0, r3
	add r0, r0, r4
	ldrb r0, [r0, #2]
	cmp r0, #5
	bls _0802F558
	lsr r0, r2, #0x1F
	and r1, r0
	add r0, r1, #0
	mul r0, r3
	add r0, r0, r4
	ldrb r0, [r0, #2]
	cmp r0, #2
	bhi _0802F558
	mov r0, #1
	b _0802F55A
	.align 2, 0
_0802F550: .4byte 0x020192E4
_0802F554: .4byte 0x00000D64
_0802F558:
	mov r0, #0
_0802F55A:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0802F518

