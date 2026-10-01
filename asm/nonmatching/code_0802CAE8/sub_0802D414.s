	thumb_func_start sub_0802D414
sub_0802D414: @ 0x0802D414
	push {r4, r5, r6, lr}
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802D472
	ldrb r1, [r0, #2]
	lsl r4, r1, #0x1F
	lsr r2, r4, #0x1F
	ldrh r0, [r0, #2]
	lsl r0, r0, #0x16
	lsr r0, r0, #0x1A
	mov r1, #0x94
	mul r1, r0
	ldr r6, _0802D444 @ =0x00000D64
	add r0, r2, #0
	mul r0, r6
	add r1, r1, r0
	ldr r5, _0802D448 @ =0x0201930C
	add r1, r1, r5
	mov r0, #0x20
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	bne _0802D450
	b _0802D472
_0802D444: .4byte 0x00000D64
_0802D448: .4byte 0x0201930C
_0802D44C:
	mov r0, #1
	b _0802D474
_0802D450:
	mov r3, #0
	mov r2, #1
	mov r1, #0
_0802D456:
	lsr r0, r4, #0x1F
	sub r0, r2, r0
	and r0, r2
	mul r0, r6
	add r0, r1, r0
	add r0, r0, r5
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0802D44C
	add r1, #0x94
	add r3, #1
	cmp r3, #4
	ble _0802D456
_0802D472:
	mov r0, #0
_0802D474:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0802D414
	.align 2, 0

