	thumb_func_start sub_08002048
sub_08002048: @ 0x08002048
	ldr r3, _0800207C @ =0x0201F7D0
	ldrb r2, [r3, #8]
	lsl r0, r2, #0x1C
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	mov r0, #1
	and r1, r0
	lsl r1, r1, #3
	mov r0, #9
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #8]
	mov r1, #8
	and r0, r1
	cmp r0, #0
	beq _08002080
	mov r0, #0x80
	lsl r0, r0, #0x13
	ldrh r1, [r0]
	mov r2, #0x10
	orr r1, r2
	strh r1, [r0]
	b _0800208C
	.align 2, 0
_0800207C: .4byte 0x0201F7D0
_08002080:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08002090 @ =0x0000FFEF
	and r0, r1
	strh r0, [r2]
_0800208C:
	bx lr
	.align 2, 0
_08002090: .4byte 0x0000FFEF
	thumb_func_end sub_08002048

