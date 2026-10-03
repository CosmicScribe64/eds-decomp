	thumb_func_start IsLineStartForbidden
IsLineStartForbidden: @ 0x080728C0
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	add r2, r1, #0
	ldr r0, _08072900 @ =0x000082A5
	cmp r1, r0
	bne _080728CE
	b _080729EC
_080728CE:
	cmp r1, r0
	bgt _0807295C
	ldr r0, _08072904 @ =0x0000815E
	cmp r1, r0
	bgt _08072920
	sub r0, #1
	cmp r1, r0
	blt _080728E0
	b _080729EC
_080728E0:
	sub r0, #0x18
	cmp r1, r0
	bne _080728E8
	b _080729EC
_080728E8:
	cmp r1, r0
	bgt _08072908
	sub r0, #3
	cmp r1, r0
	ble _080728F4
	b _080729F4
_080728F4:
	sub r0, #1
	cmp r1, r0
	bge _080728FC
	b _080729F4
_080728FC:
	b _080729EC
	.align 2, 0
_08072900: .4byte 0x000082A5
_08072904: .4byte 0x0000815E
_08072908:
	ldr r0, _0807291C @ =0x00008148
	cmp r1, r0
	bge _08072910
	b _080729F4
_08072910:
	add r0, #1
	cmp r1, r0
	ble _080729EC
	add r0, #0x12
	b _080729D4
	.align 2, 0
_0807291C: .4byte 0x00008148
_08072920:
	ldr r0, _08072938 @ =0x0000817A
	cmp r1, r0
	beq _080729EC
	cmp r1, r0
	bgt _08072944
	sub r0, #4
	cmp r1, r0
	beq _080729EC
	cmp r1, r0
	bgt _0807293C
	sub r0, #0xC
	b _080729D4
_08072938: .4byte 0x0000817A
_0807293C:
	ldr r0, _08072940 @ =0x00008178
	b _080729D4
_08072940: .4byte 0x00008178
_08072944:
	ldr r0, _08072954 @ =0x000082A1
	cmp r1, r0
	beq _080729EC
	cmp r1, r0
	ble _080729D2
	ldr r0, _08072958 @ =0x000082A3
	b _080729D4
	.align 2, 0
_08072954: .4byte 0x000082A1
_08072958: .4byte 0x000082A3
_0807295C:
	ldr r0, _08072980 @ =0x00008344
	cmp r1, r0
	beq _080729EC
	cmp r1, r0
	bgt _080729A8
	sub r0, #0x61
	cmp r1, r0
	beq _080729EC
	cmp r1, r0
	bgt _0807298C
	sub r0, #0x22
	cmp r1, r0
	beq _080729EC
	cmp r1, r0
	bgt _08072984
	sub r0, #0x1A
	b _080729D4
	.align 2, 0
_08072980: .4byte 0x00008344
_08072984:
	ldr r0, _08072988 @ =0x000082E1
	b _080729D4
_08072988: .4byte 0x000082E1
_0807298C:
	ldr r0, _0807299C @ =0x00008340
	cmp r1, r0
	beq _080729EC
	cmp r1, r0
	bgt _080729A0
	sub r0, #0x5B
	b _080729D4
	.align 2, 0
_0807299C: .4byte 0x00008340
_080729A0:
	ldr r0, _080729A4 @ =0x00008342
	b _080729D4
_080729A4: .4byte 0x00008342
_080729A8:
	ldr r0, _080729C0 @ =0x00008383
	cmp r1, r0
	beq _080729EC
	cmp r1, r0
	bgt _080729C8
	sub r0, #0x3B
	cmp r1, r0
	beq _080729EC
	cmp r1, r0
	ble _080729D2
	ldr r0, _080729C4 @ =0x00008362
	b _080729D4
_080729C0: .4byte 0x00008383
_080729C4: .4byte 0x00008362
_080729C8:
	ldr r0, _080729DC @ =0x00008387
	cmp r1, r0
	beq _080729EC
	cmp r1, r0
	bgt _080729E0
_080729D2:
	sub r0, #2
_080729D4:
	cmp r1, r0
	beq _080729EC
	b _080729F4
	.align 2, 0
_080729DC: .4byte 0x00008387
_080729E0:
	ldr r0, _080729F0 @ =0x00008396
	cmp r2, r0
	bgt _080729F4
	sub r0, #1
	cmp r2, r0
	blt _080729F4
_080729EC:
	mov r0, #1
	b _080729F6
_080729F0: .4byte 0x00008396
_080729F4:
	mov r0, #0
_080729F6:
	bx lr
	thumb_func_end IsLineStartForbidden

