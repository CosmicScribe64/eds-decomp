	thumb_func_start PlotCardImagePixel
PlotCardImagePixel: @ 0x08061848
	push {r4, r5, lr}
	sub sp, #4
	lsl r4, r0, #0xD
	lsr r4, r4, #0x10
	lsl r3, r1, #0xD
	lsr r3, r3, #0x10
	mov r5, #7
	and r0, r5
	lsl r0, r0, #0x10
	and r1, r5
	lsl r4, r4, #0x11
	lsr r4, r4, #0x10
	add r3, #2
	lsl r3, r3, #0x10
	lsr r3, r3, #0xB
	add r4, r4, r3
	lsl r4, r4, #5
	ldr r3, _080618C0 @ =0x06010000
	add r4, r4, r3
	lsr r3, r0, #0x12
	lsl r3, r3, #2
	add r4, r4, r3
	lsl r1, r1, #3
	add r4, r4, r1
	ldr r3, [r4]
	mov r1, sp
	strb r3, [r1]
	mov r5, sp
	lsl r1, r3, #0x10
	lsr r1, r1, #0x18
	strb r1, [r5, #1]
	mov r1, sp
	lsr r3, r3, #0x10
	strb r3, [r1, #2]
	lsr r3, r3, #8
	strb r3, [r1, #3]
	mov r1, #0xC0
	lsl r1, r1, #0xA
	and r1, r0
	lsr r1, r1, #0x10
	mov r3, sp
	add r0, r3, r1
	strb r2, [r0]
	mov r2, sp
	mov r0, sp
	ldrb r1, [r0, #1]
	lsl r1, r1, #8
	ldrb r2, [r2]
	orr r1, r2
	ldrb r2, [r0, #2]
	ldrb r0, [r0, #3]
	lsl r0, r0, #8
	orr r2, r0
	lsl r2, r2, #0x10
	orr r1, r2
	str r1, [r4]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
_080618C0: .4byte 0x06010000
	thumb_func_end PlotCardImagePixel

