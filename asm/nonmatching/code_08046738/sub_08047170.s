	thumb_func_start sub_08047170
sub_08047170: @ 0x08047170
	push {r4, lr}
	add r3, r0, #0
	ldr r2, _080471CC @ =0x020192E4
	mov r0, #1
	and r0, r3
	ldr r1, _080471D0 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #7]
	lsl r0, r0, #0x1B
	cmp r0, #0
	blt _080471E0
	ldr r1, _080471D4 @ =0x00000592
	add r0, r3, #0
	bl sub_08008524
	cmp r0, #0
	bne _080471E0
	ldr r4, _080471D8 @ =0x000005E6
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bne _080471E0
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bne _080471E0
	ldr r4, _080471DC @ =0x000005F6
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bne _080471E0
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bne _080471E0
	mov r0, #1
	b _080471E2
_080471CC: .4byte 0x020192E4
_080471D0: .4byte 0x00000D64
_080471D4: .4byte 0x00000592
_080471D8: .4byte 0x000005E6
_080471DC: .4byte 0x000005F6
_080471E0:
	mov r0, #0
_080471E2:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08047170

