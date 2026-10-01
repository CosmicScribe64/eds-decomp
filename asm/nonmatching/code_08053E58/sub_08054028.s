	thumb_func_start sub_08054028
sub_08054028: @ 0x08054028
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r4, r0, #0
	mov r7, #0
	mov r0, #0
	mov r9, r0
	mov sl, r0
	mov r0, #1
	mov r8, r0
	mov r6, #0
	ldr r5, _08054114 @ =0x0000058A
	mov r0, #0
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	ble _08054054
	mov r0, #0
	mov r8, r0
_08054054:
	mov r0, #1
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	ble _08054064
	mov r0, #0
	mov r8, r0
_08054064:
	cmp r4, #0
	beq _08054074
	ldr r1, _08054118 @ =0x0000034D
	add r0, r4, #0
	bl sub_0800A2A8
	cmp r0, #0
	beq _0805410E
_08054074:
	ldr r1, _0805411C @ =0x000002E1
	add r0, r4, #0
	bl sub_080086CC
	cmp r0, #0
	beq _0805408A
	mov r0, r8
	cmp r0, #0
	beq _0805408A
	mov r7, #1
	add r6, #1
_0805408A:
	ldr r1, _0805411C @ =0x000002E1
	add r0, r4, #0
	bl sub_0800A2A8
	cmp r0, #0
	beq _08054098
	mov r7, #1
_08054098:
	cmp r7, #0
	beq _0805410E
	mov r1, #0xBD
	lsl r1, r1, #2
	add r0, r4, #0
	bl sub_080086CC
	cmp r0, #0
	beq _080540B6
	mov r0, r8
	cmp r0, #0
	beq _080540B6
	mov r0, #1
	mov r9, r0
	add r6, #1
_080540B6:
	mov r1, #0xBD
	lsl r1, r1, #2
	add r0, r4, #0
	bl sub_0800A2A8
	cmp r0, #0
	beq _080540C8
	mov r0, #1
	mov r9, r0
_080540C8:
	mov r0, r9
	cmp r0, #0
	beq _0805410E
	mov r1, #0xC8
	lsl r1, r1, #2
	add r0, r4, #0
	bl sub_080086CC
	cmp r0, #0
	beq _080540E8
	mov r0, r8
	cmp r0, #0
	beq _080540E8
	mov r0, #1
	mov sl, r0
	add r6, #1
_080540E8:
	mov r1, #0xC8
	lsl r1, r1, #2
	add r0, r4, #0
	bl sub_0800A2A8
	cmp r0, #0
	beq _080540FA
	mov r0, #1
	mov sl, r0
_080540FA:
	mov r0, sl
	cmp r0, #0
	beq _0805410E
	add r0, r4, #0
	bl sub_08008A1C
	cmp r0, #0
	bne _08054120
	cmp r6, #0
	bne _08054120
_0805410E:
	mov r0, #0
	b _08054122
	.align 2, 0
_08054114: .4byte 0x0000058A
_08054118: .4byte 0x0000034D
_0805411C: .4byte 0x000002E1
_08054120:
	mov r0, #1
_08054122:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08054028

