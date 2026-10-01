	thumb_func_start sub_08043758
sub_08043758: @ 0x08043758
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r0, _080437BC @ =0x0201AE60
	ldrh r1, [r0, #8]
	add r1, #1
	lsl r2, r1, #3
	add r1, r0, #0
	add r1, #0x21
	ldrh r3, [r0, #0xE]
	ldrb r1, [r1]
	sub r1, r3, r1
	ldrh r0, [r0, #0xA]
	sub r1, r0, r1
	lsl r6, r1, #3
	add r6, #8
	mov r5, #0
	ldr r0, _080437C0 @ =0x02017A40
	mov r3, #0xA2
	lsl r3, r3, #3
	add r1, r0, r3
	ldrb r3, [r1]
	cmp r5, r3
	bge _080437B2
	mov r8, r0
	add r7, r1, #0
	add r4, r2, #0
_0804378E:
	lsl r1, r6, #0x10
	orr r1, r4
	ldr r0, _080437C4 @ =0x000003E1
	add r0, r8
	ldr r2, _080437C8 @ =0x0000431D
	ldrb r0, [r0]
	cmp r5, r0
	bge _080437A0
	add r2, #1
_080437A0:
	add r0, r1, #0
	mov r1, #0
	bl sub_080761F0
	add r4, #0xA
	add r5, #1
	ldrb r0, [r7]
	cmp r5, r0
	blt _0804378E
_080437B2:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080437BC: .4byte 0x0201AE60
_080437C0: .4byte 0x02017A40
_080437C4: .4byte 0x000003E1
_080437C8: .4byte 0x0000431D
	thumb_func_end sub_08043758

