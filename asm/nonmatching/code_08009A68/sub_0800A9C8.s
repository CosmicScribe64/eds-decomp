	thumb_func_start sub_0800A9C8
sub_0800A9C8: @ 0x0800A9C8
	push {r4, r5, r6, r7, lr}
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	mov r4, #0
	mov r2, #1
	and r2, r0
	mov r0, #0x94
	mul r1, r0
	ldr r0, _0800AA20 @ =0x00000D64
	mul r2, r0
	add r0, r1, r2
	ldr r3, _0800AA24 @ =0x0201930C
	add r0, r0, r3
	add r0, #0x8A
	ldrh r5, [r0]
	cmp r4, r5
	bge _0800AA38
	add r0, r2, r3
	add r0, r0, r1
	mov ip, r0
	add r0, r3, #0
	add r0, #0x4A
	add r0, r2, r0
	add r2, r0, r1
	ldr r7, _0800AA28 @ =0x000007FF
	add r3, r5, #0
_0800A9FC:
	lsl r1, r4, #1
	mov r0, ip
	add r0, #0xA
	add r0, r0, r1
	ldrh r0, [r0]
	ldrh r1, [r2]
	cmp r1, #3
	bne _0800AA30
	and r0, r7
	lsl r0, r0, #1
	ldr r1, _0800AA2C @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r6
	bne _0800AA30
	mov r0, #1
	b _0800AA3A
	.align 2, 0
_0800AA20: .4byte 0x00000D64
_0800AA24: .4byte 0x0201930C
_0800AA28: .4byte 0x000007FF
_0800AA2C: .4byte gUnk_08622AB4
_0800AA30:
	add r2, #2
	add r4, #1
	cmp r4, r3
	blt _0800A9FC
_0800AA38:
	mov r0, #0
_0800AA3A:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0800A9C8

