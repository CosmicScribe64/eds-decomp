	thumb_func_start sub_0805146C
sub_0805146C: @ 0x0805146C
	push {r4, r5, r6, lr}
	ldr r4, _080514A4 @ =0x020192E0
	ldr r1, _080514A8 @ =0x00001B12
	add r0, r4, r1
	mov r1, #2
	ldrb r2, [r0]
	orr r1, r2
	strb r1, [r0]
	ldr r2, _080514AC @ =0x02015EE8
	mov r6, #1
	add r0, r6, #0
	ldrb r3, [r2, #1]
	and r0, r3
	cmp r0, #0
	beq _0805155C
	ldr r0, _080514B0 @ =0x02017FB0
	ldr r1, _080514B4 @ =0x00000306
	add r0, r0, r1
	ldrb r1, [r0]
	lsl r0, r1, #0x1C
	cmp r0, #0
	bge _080514B8
	ldrb r0, [r2]
	add r0, #1
	strb r0, [r2]
	mov r0, #1
	b _0805158E
	.align 2, 0
_080514A4: .4byte 0x020192E0
_080514A8: .4byte 0x00001B12
_080514AC: .4byte 0x02015EE8
_080514B0: .4byte 0x02017FB0
_080514B4: .4byte 0x00000306
_080514B8:
	lsl r0, r1, #0x1A
	cmp r0, #0
	blt _0805158C
	bl sub_080512F0
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0805158C
	ldr r2, _08051510 @ =0x00001B14
	add r4, r4, r2
	mov r5, #2
	add r0, r5, #0
	ldrb r3, [r4]
	and r0, r3
	cmp r0, #0
	beq _0805151C
	bl sub_0804A1C8
	cmp r0, #0
	bne _0805158C
	ldr r1, _08051514 @ =0x03000040
	add r0, r5, #0
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08051534
	mov r0, #3
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	mov r0, #3
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r0, _08051518 @ =0x0000F006
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	b _08051534
_08051510: .4byte 0x00001B14
_08051514: .4byte 0x03000040
_08051518: .4byte 0x0000F006
_0805151C:
	ldr r1, _0805154C @ =0x03000040
	add r0, r6, #0
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08051534
	ldr r0, _08051550 @ =0x0000F004
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
_08051534:
	ldr r0, _08051554 @ =0x02017FB0
	ldr r3, _08051558 @ =0x00000306
	add r2, r0, r3
	ldrb r1, [r2]
	lsl r0, r1, #0x1D
	cmp r0, #0
	bge _0805158C
	mov r0, #5
	neg r0, r0
	and r0, r1
	strb r0, [r2]
	b _08051564
_0805154C: .4byte 0x03000040
_08051550: .4byte 0x0000F004
_08051554: .4byte 0x02017FB0
_08051558: .4byte 0x00000306
_0805155C:
	bl sub_0801E944
	cmp r0, #0
	beq _0805158C
_08051564:
	ldr r1, _08051594 @ =0x020192E0
	ldr r0, _08051598 @ =0x00001B12
	add r2, r1, r0
	mov r0, #3
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	strb r0, [r2]
	ldr r0, _0805159C @ =0x02015EE8
	ldrb r2, [r0]
	sub r2, #6
	mov r3, #0
	strb r2, [r0]
	mov r2, #0xD9
	lsl r2, r2, #5
	add r0, r1, r2
	strb r3, [r0]
	ldr r0, _080515A0 @ =0x00001B21
	add r1, r1, r0
	strb r3, [r1]
_0805158C:
	mov r0, #0
_0805158E:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08051594: .4byte 0x020192E0
_08051598: .4byte 0x00001B12
_0805159C: .4byte 0x02015EE8
_080515A0: .4byte 0x00001B21
	thumb_func_end sub_0805146C

