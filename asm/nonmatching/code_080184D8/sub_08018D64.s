	thumb_func_start sub_08018D64
sub_08018D64: @ 0x08018D64
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r7, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	mov r1, #0
_08018D74:
	cmp r1, #0
	bne _08018D7E
	mov r0, #1
	sub r5, r0, r7
	b _08018D80
_08018D7E:
	add r5, r7, #0
_08018D80:
	mov r4, #0
	add r6, r1, #1
_08018D84:
	add r0, r5, #0
	add r1, r4, #0
	bl sub_08018C3C
	add r4, #1
	cmp r4, #4
	ble _08018D84
	mov r0, r8
	cmp r0, #0
	beq _08018DB4
	add r0, r5, #0
	bl sub_08008860
	cmp r0, #0
	ble _08018DB4
	mov r0, #0x60
	cmp r5, #0
	beq _08018DAA
	ldr r0, _08018DC4 @ =0x00008060
_08018DAA:
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08018DB4:
	add r1, r6, #0
	cmp r1, #1
	ble _08018D74
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08018DC4: .4byte 0x00008060
	thumb_func_end sub_08018D64

