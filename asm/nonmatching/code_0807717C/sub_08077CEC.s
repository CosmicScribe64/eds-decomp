	thumb_func_start sub_08077CEC
sub_08077CEC: @ 0x08077CEC
	push {r4, r5, r6, lr}
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r4, r0, #0
	add r5, r1, #0
	cmp r2, #0x10
	beq _08077D0E
	mov r0, #0x80
	lsl r0, r0, #1
	cmp r2, r0
	bne _08077D32
	mov r2, #0x80
	lsl r2, r2, #6
	add r0, r4, #0
	bl CpuSet
	b _08077D32
_08077D0E:
	mov r6, #0
_08077D10:
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0x80
	lsl r2, r2, #1
	bl CpuSet
	mov r0, #0x80
	lsl r0, r0, #2
	add r4, r4, r0
	mov r0, #0x80
	lsl r0, r0, #3
	add r5, r5, r0
	add r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	cmp r6, #0xF
	bls _08077D10
_08077D32:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	thumb_func_end sub_08077CEC

