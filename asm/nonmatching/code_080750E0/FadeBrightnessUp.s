	thumb_func_start FadeBrightnessUp
FadeBrightnessUp: @ 0x0807588C
	push {r4, r5, r6, lr}
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldr r1, _080758E4 @ =0x04000050
	strh r0, [r1]
	ldr r1, _080758E8 @ =0x03000040
	ldr r0, _080758EC @ =0x00004832
	add r4, r1, r0
	ldrb r3, [r4]
	lsl r2, r3, #0x1A
	lsr r0, r2, #0x1A
	add r6, r1, #0
	cmp r0, #0x1E
	bhi _080758CC
	add r1, r0, #0
	add r1, #2
	mov r0, #0x3F
	and r1, r0
	mov r5, #0x40
	neg r5, r5
	add r2, r5, #0
	and r2, r3
	orr r2, r1
	strb r2, [r4]
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1A
	cmp r0, #0x1F
	bls _080758CC
	and r2, r5
	mov r0, #0x1F
	orr r2, r0
	strb r2, [r4]
_080758CC:
	ldr r2, _080758F0 @ =0x04000054
	ldr r1, _080758EC @ =0x00004832
	add r0, r6, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r1, r0, #0x1A
	strh r1, [r2]
	add r0, r1, #0
	cmp r0, #0x1E
	bls _080758F4
	mov r0, #1
	b _080758F6
_080758E4: .4byte 0x04000050
_080758E8: .4byte 0x03000040
_080758EC: .4byte 0x00004832
_080758F0: .4byte 0x04000054
_080758F4:
	mov r0, #0
_080758F6:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end FadeBrightnessUp

