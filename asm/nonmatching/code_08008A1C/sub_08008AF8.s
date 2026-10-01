	thumb_func_start sub_08008AF8
sub_08008AF8: @ 0x08008AF8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	mov r8, r1
	ldr r4, _08008B20 @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bgt _08008B1C
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	ble _08008B24
_08008B1C:
	mov r0, #0
	b _08008B5E
_08008B20: .4byte 0x0000058A
_08008B24:
	mov r5, #0
	mov r4, #0
	mov r0, #1
	and r0, r6
	ldr r1, _08008B68 @ =0x00000D64
	mul r1, r0
	ldr r0, _08008B6C @ =0x0201930C
	add r7, r1, r0
_08008B34:
	mov r0, #0x94
	mul r0, r4
	add r0, r7, r0
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08008B56
	cmp r4, r8
	beq _08008B56
	add r0, r6, #0
	add r1, r4, #0
	bl sub_08008A6C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08008B56
	add r5, #1
_08008B56:
	add r4, #1
	cmp r4, #4
	ble _08008B34
	add r0, r5, #0
_08008B5E:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08008B68: .4byte 0x00000D64
_08008B6C: .4byte 0x0201930C
	thumb_func_end sub_08008AF8

