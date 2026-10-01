	thumb_func_start sub_08009AD0
sub_08009AD0: @ 0x08009AD0
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r4, r1, #0
	ldr r3, _08009B2C @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _08009B30 @ =0x00000D64
	mul r1, r0
	add r5, r1, r3
	ldrb r0, [r5, #4]
	cmp r4, r0
	bge _08009B38
	ldr r6, _08009B34 @ =0x00000904
	add r0, r3, r6
	add r7, r1, r0
	lsl r6, r4, #2
	add r0, r7, r6
	mov r9, r0
	add r0, r2, #0
	mov r1, r9
	bl sub_08007558
	ldrb r0, [r5, #4]
	sub r0, #1
	strb r0, [r5, #4]
	cmp r4, r0
	bge _08009B26
	mov r8, r5
	add r6, #4
	mov r5, r9
_08009B10:
	add r1, r7, r6
	add r0, r5, #0
	bl sub_08007558
	add r6, #4
	add r5, #4
	add r4, #1
	mov r1, r8
	ldrb r1, [r1, #4]
	cmp r4, r1
	blt _08009B10
_08009B26:
	mov r0, #1
	b _08009B3A
	.align 2, 0
_08009B2C: .4byte 0x020192E4
_08009B30: .4byte 0x00000D64
_08009B34: .4byte 0x00000904
_08009B38:
	mov r0, #0
_08009B3A:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08009AD0
	.align 2, 0

