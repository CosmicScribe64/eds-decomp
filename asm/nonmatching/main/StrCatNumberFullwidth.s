	thumb_func_start StrCatNumberFullwidth
StrCatNumberFullwidth: @ 0x08075308
	push {r4, r5, r6, r7, lr}
	sub sp, #0x18
	add r7, r0, #0
	add r4, r1, #0
	ldr r1, _0807536C @ =0x00004F82
	add r0, sp, #0x14
_08075314:
	strh r1, [r0]
	sub r0, #2
	cmp r0, sp
	bge _08075314
	mov r1, sp
	mov r0, #0
	strh r0, [r1, #0x16]
	mov r6, #0xA
	cmp r4, #0
	ble _08075354
	add r5, sp, #0x14
_0807532A:
	add r0, r4, #0
	mov r1, #0xA
	bl __modsi3
	add r0, #0x4F
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	mov r1, #0x82
	orr r0, r1
	strh r0, [r5]
	add r0, r4, #0
	mov r1, #0xA
	bl __divsi3
	add r4, r0, #0
	sub r5, #2
	sub r6, #1
	cmp r4, #0
	ble _08075354
	cmp r6, #0
	bgt _0807532A
_08075354:
	lsl r0, r6, #1
	add r0, #2
	mov r2, sp
	add r1, r2, r0
	add r0, r7, #0
	bl StrCat
	add sp, #0x18
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807536C: .4byte 0x00004F82
	thumb_func_end StrCatNumberFullwidth

