	thumb_func_start sub_0802D4A8
sub_0802D4A8: @ 0x0802D4A8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r7, r0, #0
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08047170
	cmp r0, #0
	beq _0802D4FC
	ldrb r2, [r7, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08008A1C
	cmp r0, #0
	beq _0802D4FC
	mov r0, #0
	mov r1, #0x3D
	bl sub_08008524
	add r4, r0, #0
	mov r0, #1
	mov r1, #0x3D
	bl sub_08008524
	mov r8, r0
	ldr r5, _0802D500 @ =0x000004E1
	mov r0, #0
	add r1, r5, #0
	bl sub_08008524
	add r6, r0, #0
	mov r0, #1
	add r1, r5, #0
	bl sub_08008524
	add r4, r8
	add r4, r4, r6
	cmn r4, r0
	bne _0802D504
_0802D4FC:
	mov r0, #0
	b _0802D528
_0802D500: .4byte 0x000004E1
_0802D504:
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802D534 @ =0x000007FF
	ldrh r7, [r7]
	and r1, r7
	lsl r1, r1, #1
	ldr r2, _0802D538 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	bl sub_08044224
	mov r1, #0
	cmp r0, #0
	ble _0802D526
	mov r1, #1
_0802D526:
	add r0, r1, #0
_0802D528:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802D534: .4byte 0x000007FF
_0802D538: .4byte gUnk_08622AB4
	thumb_func_end sub_0802D4A8

