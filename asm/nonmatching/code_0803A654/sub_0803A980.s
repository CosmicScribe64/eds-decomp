	thumb_func_start sub_0803A980
sub_0803A980: @ 0x0803A980
	push {r4, lr}
	mov r2, #7
	ldrb r1, [r0, #0xA]
	and r2, r1
	cmp r2, #1
	bne _0803A9B6
	ldrb r4, [r0, #0xC]
	ldrh r0, [r0, #0xC]
	lsr r3, r0, #8
	and r2, r4
	mov r0, #0x94
	mul r0, r3
	ldr r1, _0803A9C0 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803A9C4 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803A9B6
	add r0, r4, #0
	add r1, r3, #0
	mov r2, #0
	mov r3, #0
	bl sub_08018ED8
_0803A9B6:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0803A9C0: .4byte 0x00000D64
_0803A9C4: .4byte 0x0201930C
	thumb_func_end sub_0803A980

