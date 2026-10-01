	thumb_func_start sub_08024CF0
sub_08024CF0: @ 0x08024CF0
	push {r4, lr}
	ldr r4, _08024D2C @ =0x02017A30
	ldrb r2, [r4, #0xB]
	cmp r2, #1
	bne _08024D0A
	ldr r0, _08024D30 @ =0x02015280
	ldr r3, _08024D34 @ =0x00000C54
	add r1, r0, r3
	strb r2, [r1]
	ldr r1, _08024D38 @ =0x00000C55
	add r0, r0, r1
	mov r1, #0x1E
	strb r1, [r0]
_08024D0A:
	ldr r1, _08024D3C @ =0x08198FAC
	ldrb r2, [r4, #0xB]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08024D40
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08024D28
	ldrb r0, [r4, #0xB]
	add r0, #1
	strb r0, [r4, #0xB]
_08024D28:
	mov r0, #0
	b _08024D42
_08024D2C: .4byte 0x02017A30
_08024D30: .4byte 0x02015280
_08024D34: .4byte 0x00000C54
_08024D38: .4byte 0x00000C55
_08024D3C: .4byte gUnk_08198FAC
_08024D40:
	mov r0, #1
_08024D42:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08024CF0

