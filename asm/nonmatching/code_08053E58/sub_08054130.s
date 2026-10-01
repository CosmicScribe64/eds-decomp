	thumb_func_start sub_08054130
sub_08054130: @ 0x08054130
	push {r4, r5, r6, lr}
	add r4, r0, #0
	mov r6, #0
	ldr r5, _08054184 @ =0x0000058A
	mov r0, #0
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bgt _08054190
	mov r0, #1
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bgt _08054190
	ldr r1, _08054188 @ =0x00000582
	add r0, r4, #0
	bl sub_08008794
	cmp r0, #0
	beq _0805415E
	mov r6, #1
_0805415E:
	ldr r1, _0805418C @ =0x00000584
	add r0, r4, #0
	bl sub_08008794
	cmp r0, #0
	beq _0805416C
	mov r6, #1
_0805416C:
	cmp r6, #0
	beq _08054190
	mov r1, #1
	neg r1, r1
	add r0, r4, #0
	bl sub_08008AF8
	cmp r0, #1
	ble _08054190
	mov r0, #1
	b _08054192
	.align 2, 0
_08054184: .4byte 0x0000058A
_08054188: .4byte 0x00000582
_0805418C: .4byte 0x00000584
_08054190:
	mov r0, #0
_08054192:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08054130

