	thumb_func_start sub_0806704C
sub_0806704C: @ 0x0806704C
	push {r4, r5, r6, r7, lr}
	add r7, r0, #0
	add r6, r1, #0
	mov ip, r2
	ldrb r0, [r7]
	ldrb r1, [r6]
	cmp r0, r1
	beq _080670DA
	ldr r3, _08067100 @ =0x0201DB20
	add r1, r0, #0
	add r1, #0xD
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r3
	ldr r1, _08067104 @ =0x00001726
	add r0, r0, r1
	mov r4, #0
	mov r5, #1
	strb r5, [r0]
	ldrb r1, [r7]
	add r1, #0xD
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r3
	ldr r2, _08067108 @ =0x00001727
	add r0, r0, r2
	strb r4, [r0]
	ldrb r1, [r7]
	add r1, #6
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r3
	ldr r1, _08067104 @ =0x00001726
	add r0, r0, r1
	strb r5, [r0]
	ldrb r1, [r7]
	add r1, #6
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r3
	add r0, r0, r2
	strb r4, [r0]
	ldrb r1, [r6]
	add r1, #0xD
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r3
	ldr r1, _08067104 @ =0x00001726
	add r0, r0, r1
	mov r2, #0xFF
	ldrb r1, [r0]
	orr r1, r2
	strb r1, [r0]
	ldrb r1, [r6]
	add r1, #6
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r3
	ldr r1, _08067104 @ =0x00001726
	add r0, r0, r1
	ldrb r1, [r0]
	orr r2, r1
	strb r2, [r0]
	ldrb r0, [r7]
	strb r0, [r6]
_080670DA:
	ldrb r1, [r7]
	add r1, #0xD
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, ip
	mov r1, #0xFF
	strb r1, [r0, #0xE]
	ldrb r1, [r7]
	add r1, #6
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, ip
	mov r1, #1
	strb r1, [r0, #0xE]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08067100: .4byte 0x0201DB20
_08067104: .4byte 0x00001726
_08067108: .4byte 0x00001727
	thumb_func_end sub_0806704C

