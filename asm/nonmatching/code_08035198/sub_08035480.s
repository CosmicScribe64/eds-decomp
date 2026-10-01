	thumb_func_start sub_08035480
sub_08035480: @ 0x08035480
	push {r4, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r2, [r4, #4]
	and r0, r2
	cmp r0, #0
	bne _08035524
	cmp r1, #0
	beq _080354D8
	ldr r0, _080354CC @ =0x000007FF
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _080354D0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08035524
	cmp r0, #0x15
	blt _08035524
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0xB0
	cmp r0, #0
	beq _080354BE
	ldr r1, _080354D4 @ =0x000080B0
_080354BE:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	b _08035524
_080354CC: .4byte 0x000007FF
_080354D0: .4byte gUnk_08621DE0
_080354D4: .4byte 0x000080B0
_080354D8:
	ldr r0, _08035510 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x80
	bne _08035518
	add r0, r4, #0
	mov r1, #0
	mov r2, #0
	bl sub_0802E4E0
	cmp r0, #0
	beq _08035524
	ldrh r1, [r4, #6]
	ldrb r0, [r4, #6]
	mov r2, #0x91
	cmp r0, #0
	beq _08035500
	ldr r2, _08035514 @ =0x00008091
_08035500:
	lsr r1, r1, #8
	add r0, r2, #0
	mov r2, #7
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x7F
	b _08035526
_08035510: .4byte 0x02017A40
_08035514: .4byte 0x00008091
_08035518:
	ldrb r0, [r4, #6]
	ldrh r4, [r4, #6]
	lsr r1, r4, #8
	mov r2, #1
	bl sub_08018544
_08035524:
	mov r0, #0
_08035526:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08035480

