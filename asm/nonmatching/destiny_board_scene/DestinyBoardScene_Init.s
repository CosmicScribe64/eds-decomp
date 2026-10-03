	thumb_func_start DestinyBoardScene_Init
DestinyBoardScene_Init: @ 0x08027670
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	add r1, sp, #4
	mov r0, #0
	strh r0, [r1]
	ldr r5, _08027728 @ =0x02020310
	ldr r2, _0802772C @ =0x01000592
	add r0, r1, #0
	add r1, r5, #0
	bl CpuSet
	ldr r0, _08027730 @ =0x03000040
	ldr r1, _08027734 @ =0x0000040E
	add r0, r0, r1
	mov r2, #0
	mov r1, #1
	strh r1, [r0]
	ldr r0, _08027738 @ =0x04000012
	strh r2, [r0]
	sub r0, #2
	strh r2, [r0]
	add r0, #6
	strh r2, [r0]
	sub r0, #2
	strh r2, [r0]
	add r0, #6
	strh r2, [r0]
	sub r0, #2
	strh r2, [r0]
	add r0, #6
	strh r2, [r0]
	sub r0, #2
	strh r2, [r0]
	add r0, #0xC
	strh r2, [r0]
	add r0, #2
	strh r2, [r0]
	add r0, #0x12
	strh r2, [r0]
	add r0, #2
	strh r2, [r0]
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _0802773C @ =0x0000E0FF
	and r0, r1
	strh r0, [r2]
	add r0, r5, #0
	bl OamListClear
	ldr r0, _08027740 @ =0x0819A698
	ldr r2, _08027744 @ =0x00000918
	add r1, r5, r2
	bl AnimBlockInit
	ldr r0, _08027748 @ =0x02020928
	bl ObjAffineInit
	mov r4, #0
	ldr r5, _0802774C @ =0x08199DCC
	mov r6, #3
_080276EE:
	ldr r0, [r5]
	ldr r1, [r5, #4]
	mov r3, #8
	ldsh r2, [r5, r3]
	mov r7, #0xA
	ldsh r3, [r5, r7]
	ldr r7, _08027750 @ =0x02020DC4
	mov r8, r7
	add r7, r4, r7
	str r7, [sp, #0]
	bl ScrollLayer_Init
	add r4, #0x14
	add r5, #0xC
	sub r6, #1
	cmp r6, #0
	bge _080276EE
	mov r1, r8
	add r1, #0x51
	mov r0, #0x62
	strb r0, [r1]
	mov r0, #1
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08027728: .4byte 0x02020310
_0802772C: .4byte 0x01000592
_08027730: .4byte 0x03000040
_08027734: .4byte 0x0000040E
_08027738: .4byte 0x04000012
_0802773C: .4byte 0x0000E0FF
_08027740: .4byte gDestinyBoardAnimList
_08027744: .4byte 0x00000918
_08027748: .4byte 0x02020928
_0802774C: .4byte gDestinyBoardLayerInit
_08027750: .4byte 0x02020DC4
	thumb_func_end DestinyBoardScene_Init

