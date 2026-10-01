	thumb_func_start sub_0802F0AC
sub_0802F0AC: @ 0x0802F0AC
	push {r4, r5, lr}
	lsl r2, r2, #0x10
	lsr r4, r2, #0x10
	ldr r5, _0802F0E4 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	mov r3, #1
	sub r0, r3, r0
	and r0, r3
	ldr r2, _0802F0E8 @ =0x00000D64
	mul r0, r2
	add r0, r0, r5
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _0802F0EC
	lsr r1, r1, #0x1F
	add r0, r3, #0
	and r0, r1
	mul r0, r2
	add r0, r0, r5
	ldrb r0, [r0, #2]
	sub r0, r0, r4
	cmp r0, #0
	ble _0802F0EC
	mov r0, #1
	b _0802F0EE
	.align 2, 0
_0802F0E4: .4byte 0x020192E4
_0802F0E8: .4byte 0x00000D64
_0802F0EC:
	mov r0, #0
_0802F0EE:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0802F0AC

