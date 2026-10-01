	thumb_func_start sub_08019CF0
sub_08019CF0: @ 0x08019CF0
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r5, r0, #0
	add r7, r1, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r9, r2
	ldr r4, _08019D28 @ =0x00000453
	mov r0, #0
	add r1, r4, #0
	bl sub_080086CC
	cmp r0, #0
	bgt _08019D1C
	mov r0, #1
	add r1, r4, #0
	bl sub_080086CC
	cmp r0, #0
	ble _08019D2C
_08019D1C:
	add r0, r5, #0
	add r1, r7, #0
	bl sub_08019CD0
	b _08019DF4
	.align 2, 0
_08019D28: .4byte 0x00000453
_08019D2C:
	mov r0, #0x62
	cmp r5, #0
	beq _08019D34
	ldr r0, _08019D8C @ =0x00008062
_08019D34:
	lsl r1, r7, #0x10
	lsr r1, r1, #0x10
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r4, #0
	cmp r4, r7
	bge _08019DF4
	ldr r1, _08019D90 @ =0x020192E4
	mov r2, #1
	and r2, r5
	ldr r3, _08019D94 @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #3]
	cmp r4, r0
	bge _08019DF4
	add r5, r2, #0
	lsl r6, r5, #0x1F
	ldr r0, _08019D98 @ =0x0000FFFF
	mov r8, r0
_08019D62:
	lsl r1, r4, #2
	add r0, r5, #0
	mul r0, r3
	add r1, r1, r0
	ldr r0, _08019D9C @ =0x02019AA8
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	ldr r0, _08019DA0 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08019DA4 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	cmp r2, #0xC6
	beq _08019DAC
	ldr r0, _08019DA8 @ =0x000004DA
	cmp r2, r0
	beq _08019DCC
	b _08019DDE
_08019D8C: .4byte 0x00008062
_08019D90: .4byte 0x020192E4
_08019D94: .4byte 0x00000D64
_08019D98: .4byte 0x0000FFFF
_08019D9C: .4byte 0x02019AA8
_08019DA0: .4byte 0x000007FF
_08019DA4: .4byte gUnk_08622AB4
_08019DA8: .4byte 0x000004DA
_08019DAC:
	mov r0, r9
	cmp r0, #0
	beq _08019DDE
	mov r2, r8
	and r1, r2
	ldr r0, _08019DC8 @ =0x38600000
	orr r1, r0
	orr r1, r6
	add r0, r1, #0
	mov r1, #0
	bl sub_0801FBCC
	b _08019DDE
	.align 2, 0
_08019DC8: .4byte 0x38600000
_08019DCC:
	mov r0, r8
	and r1, r0
	ldr r0, _08019E00 @ =0x38600000
	orr r1, r0
	orr r1, r6
	add r0, r1, #0
	mov r1, #0
	bl sub_0801FBCC
_08019DDE:
	add r4, #1
	cmp r4, r7
	bge _08019DF4
	ldr r1, _08019E04 @ =0x020192E4
	ldr r3, _08019E08 @ =0x00000D64
	add r0, r5, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #3]
	cmp r4, r0
	blt _08019D62
_08019DF4:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08019E00: .4byte 0x38600000
_08019E04: .4byte 0x020192E4
_08019E08: .4byte 0x00000D64
	thumb_func_end sub_08019CF0

