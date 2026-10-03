	thumb_func_start CoinToss_Init
CoinToss_Init: @ 0x080246A8
	push {r4, r5, r6, lr}
	ldr r5, _08024728 @ =0x02015280
	ldr r1, _0802472C @ =0x00000C58
	add r0, r5, #0
	bl MemClear16
	ldr r1, _08024730 @ =0x03000040
	ldr r0, _08024734 @ =0x0000040E
	add r1, r1, r0
	mov r6, #0
	mov r4, #0
	mov r0, #1
	strh r0, [r1]
	ldr r0, _08024738 @ =0x04000016
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	add r0, #6
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	add r0, #6
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _0802473C @ =0x0000E0FF
	and r0, r1
	strh r0, [r2]
	add r0, r5, #0
	bl OamListClear
	mov r1, #0xC3
	lsl r1, r1, #3
	add r0, r5, r1
	bl ObjAffineInit
	mov r0, #8
	bl SetBldAlpha
	mov r1, #0xB4
	lsl r1, r1, #4
	add r0, r5, r1
	strh r4, [r0]
	add r1, #2
	add r0, r5, r1
	strb r6, [r0]
	ldr r0, _08024740 @ =0x00000B27
	add r1, r5, r0
	mov r0, #0xFF
	strb r0, [r1]
	mov r1, #0xB5
	lsl r1, r1, #4
	add r5, r5, r1
	add r0, r5, #0
	bl CoinToss_ClearSparkles
	mov r0, #1
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08024728: .4byte 0x02015280
_0802472C: .4byte 0x00000C58
_08024730: .4byte 0x03000040
_08024734: .4byte 0x0000040E
_08024738: .4byte 0x04000016
_0802473C: .4byte 0x0000E0FF
_08024740: .4byte 0x00000B27
	thumb_func_end CoinToss_Init

