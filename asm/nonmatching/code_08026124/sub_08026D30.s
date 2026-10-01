	thumb_func_start sub_08026D30
sub_08026D30: @ 0x08026D30
	ldr r1, _08026D54 @ =0x02020310
	ldr r0, _08026D58 @ =0x00000B07
	add r2, r1, r0
	ldrb r0, [r2]
	sub r0, #1
	strb r0, [r2]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0xFF
	bne _08026D52
	mov r0, #8
	strb r0, [r2]
	ldr r0, _08026D5C @ =0x00000B06
	add r1, r1, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_08026D52:
	bx lr
_08026D54: .4byte 0x02020310
_08026D58: .4byte 0x00000B07
_08026D5C: .4byte 0x00000B06
	thumb_func_end sub_08026D30

