	thumb_func_start sub_08059164
sub_08059164: @ 0x08059164
	push {r4, r5, r6, lr}
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	mov r3, #0
	ldr r4, _080591AC @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _080591B0 @ =0x00000D64
	mul r1, r0
	add r0, r1, r4
	ldrb r2, [r0, #3]
	cmp r3, r2
	bge _080591A4
	ldr r6, _080591B4 @ =0x000007C4
	add r0, r4, r6
	add r1, r1, r0
	add r6, #0x3B
	ldr r4, _080591B8 @ =0x08622AB4
_08059188:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r0, [r0]
	cmp r0, r5
	bne _0805919C
	add r3, #1
_0805919C:
	add r1, #4
	sub r2, #1
	cmp r2, #0
	bne _08059188
_080591A4:
	add r0, r3, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_080591AC: .4byte 0x020192E4
_080591B0: .4byte 0x00000D64
_080591B4: .4byte 0x000007C4
_080591B8: .4byte gUnk_08622AB4
	thumb_func_end sub_08059164

