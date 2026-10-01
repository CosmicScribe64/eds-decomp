	thumb_func_start sub_08040818
sub_08040818: @ 0x08040818
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _08040848 @ =0x02017A40
	ldr r1, _0804084C @ =0x000003E5
	add r5, r0, r1
	ldrb r0, [r5]
	cmp r0, #0
	bne _0804085C
	ldr r0, _08040850 @ =0x00000206
	ldr r1, _08040854 @ =0x00000712
	ldr r3, _08040858 @ =0x080847C8
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #8
	neg r0, r0
	ldrb r2, [r4, #0xA]
	and r0, r2
	strb r0, [r4, #0xA]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	b _080408A6
_08040848: .4byte 0x02017A40
_0804084C: .4byte 0x000003E5
_08040850: .4byte 0x00000206
_08040854: .4byte 0x00000712
_08040858: .4byte gUnk_080847C8
_0804085C:
	ldr r1, _08040870 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08040874
	mov r0, #0
	strb r0, [r5]
	b _080408A6
	.align 2, 0
_08040870: .4byte 0x03000040
_08040874:
	ldr r0, _08040884 @ =0x00020002
	bl sub_08052F38
	cmp r0, #0
	bne _08040888
	mov r0, #0
	b _080408A6
	.align 2, 0
_08040884: .4byte 0x00020002
_08040888:
	ldr r0, _080408AC @ =0x0201CFB0
	ldr r3, _080408B0 @ =0x00000824
	add r1, r0, r3
	ldr r1, [r1]
	add r3, #4
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r4, #0
	bl sub_0803DDAC
	mov r0, #1
_080408A6:
	pop {r4, r5}
	pop {r1}
	bx r1
_080408AC: .4byte 0x0201CFB0
_080408B0: .4byte 0x00000824
	thumb_func_end sub_08040818

