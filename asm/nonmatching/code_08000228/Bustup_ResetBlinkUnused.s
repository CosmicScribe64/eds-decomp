	thumb_func_start Bustup_ResetBlinkUnused
Bustup_ResetBlinkUnused: @ 0x08000A78
	push {r4, r5, lr}
	ldr r2, _08000AB8 @ =0x02013DE0
	ldr r0, _08000ABC @ =0x000009A8
	add r1, r2, r0
	mov r0, #0
	strh r0, [r1]
	ldr r3, _08000AC0 @ =0x000009A7
	add r1, r2, r3
	sub r0, #0x20
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	mov r3, #0
	add r5, r2, #0
	ldr r4, _08000AC4 @ =0x00000822
	mov r2, #0xFF
_08000A98:
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, r0, r5
	add r0, r0, r4
	ldrb r1, [r0]
	orr r1, r2
	strb r1, [r0]
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, #0x13
	bls _08000A98
	pop {r4, r5}
	pop {r0}
	bx r0
_08000AB8: .4byte 0x02013DE0
_08000ABC: .4byte 0x000009A8
_08000AC0: .4byte 0x000009A7
_08000AC4: .4byte 0x00000822
	thumb_func_end Bustup_ResetBlinkUnused

