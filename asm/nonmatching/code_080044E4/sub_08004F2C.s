	thumb_func_start sub_08004F2C
sub_08004F2C: @ 0x08004F2C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r5, #0
	mov r0, #0xD0
	lsl r0, r0, #0xF
	mov r9, r0
	mov r1, #0x58
	mov r8, r1
	mov r6, #0x80
	lsl r6, r6, #0x12
	mov r7, #0x18
_08004F46:
	lsr r4, r6, #0x10
	ldr r0, _08004F9C @ =0x0201527C
	ldr r0, [r0]
	lsl r0, r0, #0xE
	lsr r0, r0, #0x1F
	cmp r0, r5
	beq _08004F5C
	add r0, r4, #0
	add r0, #0xC
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
_08004F5C:
	add r0, r7, #0
	mov r1, r9
	orr r0, r1
	ldr r1, _08004FA0 @ =0x000040C0
	add r2, r4, #0
	bl sub_080761F0
	mov r0, r8
	mov r1, r9
	orr r0, r1
	add r2, r4, #0
	add r2, #8
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r1, #0x80
	bl sub_080761F0
	mov r0, #0x70
	add r8, r0
	mov r1, #0x80
	lsl r1, r1, #0x10
	add r6, r6, r1
	add r7, #0x70
	add r5, #1
	cmp r5, #1
	ble _08004F46
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08004F9C: .4byte 0x0201527C
_08004FA0: .4byte 0x000040C0
	thumb_func_end sub_08004F2C

