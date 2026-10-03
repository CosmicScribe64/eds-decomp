	thumb_func_start Bustup_TickBlink
Bustup_TickBlink: @ 0x08000994
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r9, r0
	ldr r2, _08000A14 @ =0x02013DE0
	ldr r0, _08000A18 @ =0x000009A8
	add r6, r2, r0
	ldrh r0, [r6]
	sub r0, #1
	strh r0, [r6]
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	ldr r0, _08000A1C @ =0x0000FFFF
	cmp r5, r0
	bne _08000A08
	ldr r1, _08000A20 @ =0x08080AA8
	mov r8, r1
	ldr r3, _08000A24 @ =0x000009A7
	add r4, r2, r3
	ldrb r2, [r4]
	lsl r1, r2, #0x1B
	lsr r0, r1, #0x1B
	add r0, #1
	mov r3, #0x1F
	mov ip, r3
	mov r3, ip
	and r0, r3
	mov r7, #0x20
	neg r7, r7
	add r3, r7, #0
	and r3, r2
	orr r3, r0
	strb r3, [r4]
	lsr r1, r1, #0x1A
	add r1, r8
	ldrh r1, [r1]
	strh r1, [r6]
	add r0, r5, #0
	and r0, r1
	cmp r0, #0
	bne _08000A02
	add r2, r7, #0
	and r2, r3
	lsl r1, r2, #0x1B
	lsr r0, r1, #0x1B
	add r0, #1
	mov r3, ip
	and r0, r3
	orr r0, r2
	strb r0, [r4]
	lsr r1, r1, #0x1A
	add r1, r8
	ldrh r0, [r1]
	strh r0, [r6]
_08000A02:
	mov r0, #1
	mov r1, r9
	strb r0, [r1, #0xE]
_08000A08:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08000A14: .4byte 0x02013DE0
_08000A18: .4byte 0x000009A8
_08000A1C: .4byte 0x0000FFFF
_08000A20: .4byte gBlinkIntervals
_08000A24: .4byte 0x000009A7
	thumb_func_end Bustup_TickBlink

