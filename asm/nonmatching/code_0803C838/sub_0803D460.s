	thumb_func_start sub_0803D460
sub_0803D460: @ 0x0803D460
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	ldr r1, _0803D4C0 @ =0x02017A40
	ldr r0, _0803D4C4 @ =0x00000502
	add r2, r1, r0
	ldrb r3, [r2]
	lsl r0, r3, #0x1E
	mov r4, #0
	cmp r0, #0
	beq _0803D4EC
	ldr r7, _0803D4C8 @ =0x000007FF
	ldr r6, _0803D4CC @ =0x08622AB4
	mov r0, r8
	and r0, r7
	lsl r0, r0, #1
	add r0, r0, r6
	mov ip, r0
	ldr r0, _0803D4D0 @ =0x00000504
	add r3, r1, r0
	ldr r5, _0803D4D4 @ =0xFFFFF830
	mov r9, r2
_0803D494:
	ldrh r1, [r3]
	cmp r1, #0
	beq _0803D4DC
	add r0, r7, #0
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r6
	ldrh r1, [r0]
	mov r2, ip
	ldrh r0, [r2]
	ldr r2, _0803D4D8 @ =0x000007CF
	cmp r0, r2
	ble _0803D4B0
	add r0, r0, r5
_0803D4B0:
	cmp r1, r2
	ble _0803D4B6
	add r1, r1, r5
_0803D4B6:
	cmp r1, r0
	bne _0803D4DC
	mov r0, #0
	strh r0, [r3]
	b _0803D566
_0803D4C0: .4byte 0x02017A40
_0803D4C4: .4byte 0x00000502
_0803D4C8: .4byte 0x000007FF
_0803D4CC: .4byte gUnk_08622AB4
_0803D4D0: .4byte 0x00000504
_0803D4D4: .4byte 0xFFFFF830
_0803D4D8: .4byte 0x000007CF
_0803D4DC:
	add r3, #2
	add r4, #1
	mov r1, r9
	ldrb r1, [r1]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1E
	cmp r4, r0
	blt _0803D494
_0803D4EC:
	ldr r0, _0803D540 @ =0x000007FF
	mov r2, r8
	and r0, r2
	lsl r0, r0, #1
	ldr r3, _0803D544 @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803D566
	ldr r2, _0803D548 @ =0x02017A40
	ldr r1, _0803D54C @ =0x00000502
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	mov r4, #0
	cmp r0, #0
	beq _0803D566
_0803D514:
	lsl r1, r4, #1
	ldr r3, _0803D550 @ =0x00000504
	add r0, r2, r3
	add r5, r1, r0
	ldrh r1, [r5]
	cmp r1, #0
	beq _0803D554
	ldr r2, _0803D540 @ =0x000007FF
	add r0, r2, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _0803D544 @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803D554
	mov r0, #0
	strh r0, [r5]
	b _0803D566
_0803D540: .4byte 0x000007FF
_0803D544: .4byte gUnk_08622AB4
_0803D548: .4byte 0x02017A40
_0803D54C: .4byte 0x00000502
_0803D550: .4byte 0x00000504
_0803D554:
	add r4, #1
	ldr r2, _0803D574 @ =0x02017A40
	ldr r1, _0803D578 @ =0x00000502
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1E
	cmp r4, r0
	blt _0803D514
_0803D566:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0803D574: .4byte 0x02017A40
_0803D578: .4byte 0x00000502
	thumb_func_end sub_0803D460

