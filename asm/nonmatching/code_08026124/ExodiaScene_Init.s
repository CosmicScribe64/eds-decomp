	thumb_func_start ExodiaScene_Init
ExodiaScene_Init: @ 0x080262BC
	push {r4, r5, lr}
	ldr r4, _08026350 @ =0x02020310
	ldr r1, _08026354 @ =0x00001738
	add r0, r4, #0
	bl MemClear16
	ldr r1, _08026358 @ =0x03000040
	ldr r0, _0802635C @ =0x0000040E
	add r1, r1, r0
	mov r5, #0
	mov r3, #0
	mov r0, #1
	strh r0, [r1]
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08026360 @ =0x0000E0FF
	and r0, r1
	strh r0, [r2]
	ldr r0, _08026364 @ =0x04000016
	strh r3, [r0]
	sub r0, #2
	strh r3, [r0]
	add r0, #6
	strh r3, [r0]
	sub r0, #2
	strh r3, [r0]
	add r0, #6
	strh r3, [r0]
	sub r0, #2
	strh r3, [r0]
	ldr r1, _08026368 @ =0x00000AAC
	add r0, r4, r1
	bl ExodiaScene_ResetPieceState
	ldr r2, _0802636C @ =0x00000B15
	add r0, r4, r2
	strb r5, [r0]
	ldr r1, _08026370 @ =0x00000B16
	add r0, r4, r1
	strb r5, [r0]
	sub r2, #1
	add r0, r4, r2
	strb r5, [r0]
	add r0, r4, #0
	bl OamListClear
	mov r1, #0xC3
	lsl r1, r1, #3
	add r0, r4, r1
	bl ObjAffineInit
	ldr r1, _08026374 @ =0x04000050
	ldr r2, _08026378 @ =0x00003F3F
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _0802637C @ =0x00000808
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	mov r0, #0x10
	strh r0, [r1]
	ldr r1, _08026380 @ =0x00001734
	add r0, r4, r1
	strb r5, [r0]
	ldr r2, _08026384 @ =0x00001735
	add r4, r4, r2
	strb r5, [r4]
	mov r0, #1
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08026350: .4byte 0x02020310
_08026354: .4byte 0x00001738
_08026358: .4byte 0x03000040
_0802635C: .4byte 0x0000040E
_08026360: .4byte 0x0000E0FF
_08026364: .4byte 0x04000016
_08026368: .4byte 0x00000AAC
_0802636C: .4byte 0x00000B15
_08026370: .4byte 0x00000B16
_08026374: .4byte 0x04000050
_08026378: .4byte 0x00003F3F
_0802637C: .4byte 0x00000808
_08026380: .4byte 0x00001734
_08026384: .4byte 0x00001735
	thumb_func_end ExodiaScene_Init

