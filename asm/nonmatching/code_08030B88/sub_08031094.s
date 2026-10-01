	thumb_func_start sub_08031094
sub_08031094: @ 0x08031094
	push {r4, r5, r6, lr}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	beq _080310C6
	b _080310E2
_080310A4:
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r6, r0
	add r1, r4, #0
	bl sub_08030028
	ldrb r5, [r5, #2]
	lsl r1, r5, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	sub r1, r6, r1
	add r2, r4, #0
	bl sub_08046CB0
	mov r0, #0x80
	b _080310E4
_080310C6:
	mov r4, #0
	mov r6, #1
_080310CA:
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r6, r0
	add r1, r4, #0
	bl sub_0802B28C
	cmp r0, #0
	bne _080310A4
	add r4, #1
	cmp r4, #4
	ble _080310CA
_080310E2:
	mov r0, #0
_080310E4:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08031094
	.align 2, 0

