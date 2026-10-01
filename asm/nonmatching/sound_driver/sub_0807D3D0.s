	thumb_func_start sub_0807D3D0
sub_0807D3D0: @ 0x0807D3D0
	push {r4, r5, lr}
	sub sp, #4
	ldr r2, _0807D4C4 @ =0x04000200
	ldrh r1, [r2]
	ldr r0, _0807D4C8 @ =0x0000F9F7
	and r0, r1
	strh r0, [r2]
	ldr r1, _0807D4CC @ =0x040000BC
	ldrh r2, [r1, #0xA]
	ldr r3, _0807D4D0 @ =0x0000C5FF
	add r0, r3, #0
	and r0, r2
	strh r0, [r1, #0xA]
	ldrh r4, [r1, #0xA]
	ldr r2, _0807D4D4 @ =0x00007FFF
	add r0, r2, #0
	and r0, r4
	strh r0, [r1, #0xA]
	ldrh r0, [r1, #0xA]
	ldr r0, _0807D4D8 @ =0x040000C8
	ldrh r1, [r0, #0xA]
	and r3, r1
	strh r3, [r0, #0xA]
	ldrh r1, [r0, #0xA]
	and r2, r1
	strh r2, [r0, #0xA]
	ldrh r0, [r0, #0xA]
	ldr r0, _0807D4DC @ =0x040000C4
	mov r1, #0
	str r1, [r0]
	add r0, #0xC
	str r1, [r0]
	ldr r1, _0807D4E0 @ =0x040000D4
	ldr r0, _0807D4E4 @ =0x0807EC1C
	str r0, [r1]
	ldr r0, _0807D4E8 @ =0x03005A54
	str r0, [r1, #4]
	ldr r0, _0807D4EC @ =0x84000038
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r2, #0x80
	lsl r2, r2, #0x18
	ldr r3, _0807D4F0 @ =0x030053AC
	ldr r4, _0807D4F4 @ =0x0300540C
	ldr r5, _0807D4F8 @ =0x03005414
	cmp r0, #0
	bge _0807D438
_0807D430:
	ldr r0, [r1, #8]
	and r0, r2
	cmp r0, #0
	bne _0807D430
_0807D438:
	add r1, r3, #0
	mov r0, #6
	mov r2, #0
_0807D43E:
	strb r2, [r1, #0xE]
	strh r2, [r1, #0xC]
	add r1, #0x10
	sub r0, #1
	cmp r0, #0
	bne _0807D43E
	str r0, [r4]
	str r0, [r4, #4]
	ldr r1, _0807D4E0 @ =0x040000D4
	str r0, [sp, #0]
	mov r0, sp
	str r0, [r1]
	str r5, [r1, #4]
	ldr r0, _0807D4FC @ =0x85000190
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r2, #0x80
	lsl r2, r2, #0x18
	cmp r0, #0
	bge _0807D470
_0807D468:
	ldr r0, [r1, #8]
	and r0, r2
	cmp r0, #0
	bne _0807D468
_0807D470:
	ldr r1, _0807D500 @ =0x04000083
	mov r0, #0xBB
	strb r0, [r1]
	mov r0, #8
	ldr r3, _0807D504 @ =0x040000A0
	mov r1, #0
	ldr r2, _0807D508 @ =0x040000A4
_0807D47E:
	str r1, [r3]
	str r1, [r2]
	sub r0, #1
	cmp r0, #0
	bne _0807D47E
	ldr r1, _0807D4CC @ =0x040000BC
	str r5, [r1]
	ldr r0, _0807D504 @ =0x040000A0
	str r0, [r1, #4]
	ldr r2, _0807D50C @ =0xF6000004
	str r2, [r1, #8]
	ldr r0, [r1, #8]
	add r1, #0xC
	mov r3, #0xC8
	lsl r3, r3, #2
	add r0, r5, r3
	str r0, [r1]
	ldr r0, _0807D508 @ =0x040000A4
	str r0, [r1, #4]
	str r2, [r1, #8]
	ldr r0, [r1, #8]
	ldr r2, _0807D4C4 @ =0x04000200
	ldrh r0, [r2]
	mov r3, #0x82
	lsl r3, r3, #2
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	ldr r1, _0807D510 @ =0x04000100
	ldr r0, _0807D514 @ =0x0080FCB9
	str r0, [r1]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
_0807D4C4: .4byte 0x04000200
_0807D4C8: .4byte 0x0000F9F7
_0807D4CC: .4byte 0x040000BC
_0807D4D0: .4byte 0x0000C5FF
_0807D4D4: .4byte 0x00007FFF
_0807D4D8: .4byte 0x040000C8
_0807D4DC: .4byte 0x040000C4
_0807D4E0: .4byte 0x040000D4
_0807D4E4: .4byte sub_0807EC1C
_0807D4E8: .4byte 0x03005A54
_0807D4EC: .4byte 0x84000038
_0807D4F0: .4byte 0x030053AC
_0807D4F4: .4byte 0x0300540C
_0807D4F8: .4byte 0x03005414
_0807D4FC: .4byte 0x85000190
_0807D500: .4byte 0x04000083
_0807D504: .4byte 0x040000A0
_0807D508: .4byte 0x040000A4
_0807D50C: .4byte 0xF6000004
_0807D510: .4byte 0x04000100
_0807D514: .4byte 0x0080FCB9
	thumb_func_end sub_0807D3D0

