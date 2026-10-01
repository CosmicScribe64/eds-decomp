	thumb_func_start sub_08056300
sub_08056300: @ 0x08056300
	add r2, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	cmp r1, #0x14
	bgt _0805630E
	cmp r1, #0x10
	bge _080563A8
_0805630E:
	cmp r2, #0
	ble _0805639A
	ldr r0, _08056338 @ =0x000003F2
	cmp r1, r0
	beq _080563A8
	cmp r1, r0
	bgt _0805635E
	ldr r0, _0805633C @ =0x0000029F
	cmp r1, r0
	beq _080563A8
	cmp r1, r0
	bgt _08056344
	ldr r0, _08056340 @ =0x0000014F
	cmp r1, r0
	blt _0805639A
	add r0, #1
	cmp r1, r0
	ble _080563A8
	add r0, #0xB
	b _08056396
	.align 2, 0
_08056338: .4byte 0x000003F2
_0805633C: .4byte 0x0000029F
_08056340: .4byte 0x0000014F
_08056344:
	ldr r0, _08056354 @ =0x000003BA
	cmp r1, r0
	beq _080563A8
	cmp r1, r0
	bgt _08056358
	sub r0, #0xE0
	b _08056396
	.align 2, 0
_08056354: .4byte 0x000003BA
_08056358:
	mov r0, #0xFC
	lsl r0, r0, #2
	b _08056396
_0805635E:
	mov r0, #0x84
	lsl r0, r0, #3
	cmp r1, r0
	beq _080563A8
	cmp r1, r0
	bgt _08056382
	sub r0, #0x1D
	cmp r1, r0
	beq _080563A8
	cmp r1, r0
	blt _0805639A
	add r0, #3
	cmp r1, r0
	bgt _0805639A
	sub r0, #1
	cmp r1, r0
	blt _0805639A
	b _080563A8
_08056382:
	ldr r0, _08056390 @ =0x0000042C
	cmp r1, r0
	beq _080563A8
	cmp r1, r0
	bgt _08056394
	sub r0, #7
	b _08056396
_08056390: .4byte 0x0000042C
_08056394:
	ldr r0, _080563AC @ =0x0000049B
_08056396:
	cmp r1, r0
	beq _080563A8
_0805639A:
	cmp r2, #1
	ble _080563B4
	cmp r1, #0x39
	beq _080563A8
	ldr r0, _080563B0 @ =0x000001A3
	cmp r1, r0
	bne _080563B4
_080563A8:
	mov r0, #1
	b _080563B6
_080563AC: .4byte 0x0000049B
_080563B0: .4byte 0x000001A3
_080563B4:
	mov r0, #0
_080563B6:
	bx lr
	thumb_func_end sub_08056300

