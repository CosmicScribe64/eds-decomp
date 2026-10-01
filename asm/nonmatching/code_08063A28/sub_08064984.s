	thumb_func_start sub_08064984
sub_08064984: @ 0x08064984
	push {r4, r5, r6, r7, lr}
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	lsl r2, r2, #0x10
	lsr r4, r2, #0x10
	mov r1, #0x62
	mul r1, r4
	lsl r1, r1, #0x10
	mov r2, #0x80
	lsl r2, r2, #0xD
	add r1, r1, r2
	lsr r4, r1, #0x10
	mov r1, #0
	lsl r7, r0, #0xB
	ldr r0, _080649D4 @ =0x0300045C
	mov ip, r0
_080649A4:
	lsl r0, r5, #1
	add r0, ip
	add r3, r7, r0
	add r5, #0x20
	add r6, r1, #1
	mov r2, #6
_080649B0:
	add r1, r4, #0
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	strh r1, [r3]
	add r3, #2
	sub r2, #1
	cmp r2, #0
	bge _080649B0
	lsl r0, r5, #0x10
	lsr r5, r0, #0x10
	add r1, r6, #0
	cmp r1, #0xD
	ble _080649A4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080649D4: .4byte 0x0300045C
	thumb_func_end sub_08064984

