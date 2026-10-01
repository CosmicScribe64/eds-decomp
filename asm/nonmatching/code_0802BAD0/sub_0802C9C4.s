	thumb_func_start sub_0802C9C4
sub_0802C9C4: @ 0x0802C9C4
	push {lr}
	add r3, r0, #0
	mov r1, #0
	ldr r0, _0802C9EC @ =0x000007FF
	ldrh r2, [r3]
	and r0, r2
	lsl r0, r0, #1
	ldr r2, _0802C9F0 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	ldr r0, _0802C9F4 @ =0x00000405
	cmp r2, r0
	beq _0802CA0C
	cmp r2, r0
	bgt _0802C9F8
	sub r0, #6
	cmp r2, r0
	beq _0802CA0C
	b _0802CA16
	.align 2, 0
_0802C9EC: .4byte 0x000007FF
_0802C9F0: .4byte gUnk_08622AB4
_0802C9F4: .4byte 0x00000405
_0802C9F8:
	ldr r0, _0802CA08 @ =0x0000042B
	cmp r2, r0
	beq _0802CA14
	add r0, #5
	cmp r2, r0
	beq _0802CA10
	b _0802CA16
	.align 2, 0
_0802CA08: .4byte 0x0000042B
_0802CA0C:
	mov r1, #1
	b _0802CA1A
_0802CA10:
	mov r1, #2
	b _0802CA1A
_0802CA14:
	mov r1, #5
_0802CA16:
	cmp r1, #0
	ble _0802CA28
_0802CA1A:
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	mov r2, #0
	mov r3, #0
	bl sub_08022758
_0802CA28:
	mov r0, #1
	pop {r1}
	bx r1
	thumb_func_end sub_0802C9C4
	.align 2, 0

