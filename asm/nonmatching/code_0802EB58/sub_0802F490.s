	thumb_func_start sub_0802F490
sub_0802F490: @ 0x0802F490
	push {lr}
	ldr r2, _0802F4B8 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r3, r0, #0x1F
	lsr r1, r3, #0x1F
	ldr r0, _0802F4BC @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #8]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _0802F4C0
	lsr r0, r3, #0x1F
	bl sub_08008A1C
	cmp r0, #3
	ble _0802F4C0
	mov r0, #1
	b _0802F4C2
	.align 2, 0
_0802F4B8: .4byte 0x020192E4
_0802F4BC: .4byte 0x00000D64
_0802F4C0:
	mov r0, #0
_0802F4C2:
	pop {r1}
	bx r1
	thumb_func_end sub_0802F490
	.align 2, 0

