	thumb_func_start sub_080516D8
sub_080516D8: @ 0x080516D8
	push {r4, r5, r6, r7, lr}
	ldr r0, _08051720 @ =0x0201AE60
	ldrh r1, [r0, #8]
	add r1, #1
	lsl r1, r1, #3
	add r2, r0, #0
	add r2, #0x21
	ldrb r2, [r2]
	ldrh r0, [r0, #0xE]
	sub r0, r2, r0
	add r0, #2
	lsl r7, r0, #3
	mov r5, #0
	ldr r0, _08051724 @ =0x02017A40
	ldr r2, _08051728 @ =0x000004FD
	add r0, r0, r2
	ldrb r2, [r0]
	cmp r5, r2
	bge _08051718
	add r6, r0, #0
	add r4, r1, #0
_08051702:
	lsl r0, r7, #0x10
	orr r0, r4
	mov r1, #0
	ldr r2, _0805172C @ =0x0000431C
	bl sub_080761F0
	add r4, #0xA
	add r5, #1
	ldrb r0, [r6]
	cmp r5, r0
	blt _08051702
_08051718:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08051720: .4byte 0x0201AE60
_08051724: .4byte 0x02017A40
_08051728: .4byte 0x000004FD
_0805172C: .4byte 0x0000431C
	thumb_func_end sub_080516D8

