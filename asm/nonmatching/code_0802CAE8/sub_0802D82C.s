	thumb_func_start sub_0802D82C
sub_0802D82C: @ 0x0802D82C
	push {r4, r5, lr}
	add r1, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802D886
	mov r0, #0xFC
	ldrb r2, [r1, #3]
	and r0, r2
	ldrb r5, [r1, #2]
	cmp r0, #0x40
	bne _0802D876
	ldrb r3, [r1, #6]
	ldrh r1, [r1, #6]
	lsr r2, r1, #8
	mov r4, #1
	add r1, r3, #0
	and r1, r4
	mov r0, #0x94
	mul r2, r0
	ldr r0, _0802D88C @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802D890 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802D876
	add r0, r4, #0
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _0802D876
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r3
	bne _0802D894
_0802D876:
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	mov r2, #0
	bl sub_080088A4
	cmp r0, #0
	bgt _0802D894
_0802D886:
	mov r0, #0
	b _0802D896
	.align 2, 0
_0802D88C: .4byte 0x00000D64
_0802D890: .4byte 0x0201930C
_0802D894:
	mov r0, #1
_0802D896:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0802D82C

