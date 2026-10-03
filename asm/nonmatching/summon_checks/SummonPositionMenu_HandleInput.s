	thumb_func_start SummonPositionMenu_HandleInput
SummonPositionMenu_HandleInput: @ 0x0805487C
	push {r4, lr}
	ldr r4, _080548AC @ =0x0201AE60
	add r3, r4, #0
	add r3, #0x22
	ldrb r2, [r3]
	add r1, r2, #0
	cmp r1, #1
	beq _080548B4
	cmp r1, #2
	beq _080548CA
	ldr r1, _080548B0 @ =0x03000040
	mov r0, #0x30
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080548CE
	mov r0, #0
	bl PlaySE
	mov r0, #1
	ldrh r1, [r4, #0x14]
	sub r0, r0, r1
	strh r0, [r4, #0x14]
	b _080548CE
_080548AC: .4byte 0x0201AE60
_080548B0: .4byte 0x03000040
_080548B4:
	add r1, r4, #0
	add r1, #0x23
	ldrb r0, [r1]
	cmp r0, #0x3B
	bhi _080548C4
	add r0, #1
	strb r0, [r1]
	b _080548F0
_080548C4:
	add r0, r2, #1
	strb r0, [r3]
	b _080548F0
_080548CA:
	mov r0, #1
	b _080548F2
_080548CE:
	ldr r1, _080548F8 @ =0x03000040
	mov r0, #1
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080548F0
	mov r0, #1
	bl PlaySE
	ldr r0, _080548FC @ =0x0201AE60
	add r3, r0, #0
	add r3, #0x22
	mov r2, #0
	mov r1, #1
	strb r1, [r3]
	add r0, #0x23
	strb r2, [r0]
_080548F0:
	mov r0, #0
_080548F2:
	pop {r4}
	pop {r1}
	bx r1
_080548F8: .4byte 0x03000040
_080548FC: .4byte 0x0201AE60
	thumb_func_end SummonPositionMenu_HandleInput

