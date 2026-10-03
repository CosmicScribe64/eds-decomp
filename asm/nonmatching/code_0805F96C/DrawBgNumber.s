	thumb_func_start DrawBgNumber
DrawBgNumber: @ 0x080608BC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r4, r3, #0
	lsl r1, r1, #0x10
	mov r3, #0x80
	lsl r3, r3, #0xB
	add r1, r1, r3
	lsr r6, r1, #0x10
	mov r7, #0
	ldr r1, _080608F8 @ =0x0300045C
	mov sl, r1
	lsl r0, r0, #0xB
	mov r9, r0
	lsl r0, r2, #2
	add r0, r0, r2
	lsl r0, r0, #1
	mov r8, r0
_080608E4:
	lsl r0, r6, #1
	add r0, sl
	mov r3, r9
	add r5, r3, r0
	cmp r4, #0
	bne _080608FC
	cmp r7, #0
	ble _080608FC
	strh r4, [r5]
	b _0806090C
_080608F8: .4byte 0x0300045C
_080608FC:
	add r0, r4, #0
	mov r1, #0xA
	bl __modsi3
	ldr r1, _08060930 @ =0x00003244
	add r0, r0, r1
	add r0, r8
	strh r0, [r5]
_0806090C:
	add r0, r4, #0
	mov r1, #0xA
	bl __divsi3
	add r4, r0, #0
	sub r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	add r7, #1
	cmp r7, #4
	ble _080608E4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08060930: .4byte 0x00003244
	thumb_func_end DrawBgNumber

