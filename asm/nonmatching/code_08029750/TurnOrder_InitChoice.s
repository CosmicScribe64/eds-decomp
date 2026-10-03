	thumb_func_start TurnOrder_InitChoice
TurnOrder_InitChoice: @ 0x08029A50
	push {r4, r5, lr}
	sub sp, #4
	mov r1, sp
	mov r0, #0
	strh r0, [r1]
	ldr r1, _08029AE4 @ =0x040000D4
	mov r0, sp
	str r0, [r1]
	ldr r2, _08029AE8 @ =0x02020310
	str r2, [r1, #4]
	ldr r0, _08029AEC @ =0x81000592
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r3, #0x80
	lsl r3, r3, #0x18
	add r5, r2, #0
	ldr r2, _08029AF0 @ =0x03000040
	cmp r0, #0
	bge _08029A80
_08029A78:
	ldr r0, [r1, #8]
	and r0, r3
	cmp r0, #0
	bne _08029A78
_08029A80:
	ldr r1, _08029AF4 @ =0x0000040E
	add r2, r2, r1
	mov r4, #0
	mov r1, #0
	mov r0, #1
	strh r0, [r2]
	ldr r0, _08029AF8 @ =0x04000016
	strh r1, [r0]
	sub r0, #2
	strh r1, [r0]
	add r0, #6
	strh r1, [r0]
	sub r0, #2
	strh r1, [r0]
	add r0, #6
	strh r1, [r0]
	sub r0, #2
	strh r1, [r0]
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08029AFC @ =0x0000E0FF
	and r0, r1
	strh r0, [r2]
	add r0, r5, #0
	bl OamListClear
	mov r0, #8
	bl SetBldAlpha
	ldr r1, _08029B00 @ =0x00000AF4
	add r0, r5, r1
	strb r4, [r0]
	ldr r0, _08029B04 @ =0x00000AF5
	add r1, r5, r0
	mov r0, #2
	strb r0, [r1]
	ldr r1, _08029B08 @ =0x00000ABF
	add r0, r5, r1
	strb r4, [r0]
	mov r1, #0xC3
	lsl r1, r1, #3
	add r0, r5, r1
	bl ObjAffineInit
	mov r0, #1
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
_08029AE4: .4byte 0x040000D4
_08029AE8: .4byte 0x02020310
_08029AEC: .4byte 0x81000592
_08029AF0: .4byte 0x03000040
_08029AF4: .4byte 0x0000040E
_08029AF8: .4byte 0x04000016
_08029AFC: .4byte 0x0000E0FF
_08029B00: .4byte 0x00000AF4
_08029B04: .4byte 0x00000AF5
_08029B08: .4byte 0x00000ABF
	thumb_func_end TurnOrder_InitChoice

