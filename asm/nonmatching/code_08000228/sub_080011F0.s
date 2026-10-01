	thumb_func_start sub_080011F0
sub_080011F0: @ 0x080011F0
	push {r4, r5, r6, lr}
	sub sp, #4
	ldr r4, _0800130C @ =0x02013DE0
	mov r1, #0x9C
	lsl r1, r1, #5
	add r0, r4, #0
	bl sub_08075278
	ldr r1, _08001310 @ =0x03000040
	ldr r0, _08001314 @ =0x0000040E
	add r1, r1, r0
	mov r5, #0
	mov r2, #0
	mov r0, #1
	strh r0, [r1]
	mov r3, #0x80
	lsl r3, r3, #0x13
	ldrh r1, [r3]
	ldr r0, _08001318 @ =0x0000E0FF
	and r0, r1
	strh r0, [r3]
	ldr r0, _0800131C @ =0x04000016
	strh r2, [r0]
	sub r0, #2
	strh r2, [r0]
	add r0, #6
	strh r2, [r0]
	sub r0, #2
	strh r2, [r0]
	add r0, #6
	strh r2, [r0]
	sub r0, #2
	strh r2, [r0]
	add r0, #0xC
	strh r2, [r0]
	add r0, #2
	strh r2, [r0]
	add r0, #2
	strh r2, [r0]
	add r0, #2
	strh r2, [r0]
	ldr r1, _08001320 @ =0x000009CC
	add r0, r4, r1
	strb r5, [r0]
	ldr r3, _08001324 @ =0x000012E9
	add r0, r4, r3
	strb r5, [r0]
	ldr r1, _08001328 @ =0x000012EA
	add r0, r4, r1
	strb r5, [r0]
	sub r3, #1
	add r0, r4, r3
	strb r5, [r0]
	add r1, #0x82
	add r0, r4, r1
	strh r2, [r0]
	ldr r2, _0800132C @ =0x00000AA8
	add r0, r4, r2
	bl sub_0807A2EC
	ldr r3, _08001330 @ =0x000009E8
	add r1, r4, r3
	mov r0, #0xFF
	strb r0, [r1]
	mov r2, #0
	ldr r6, _08001334 @ =0x000009EC
	mov r3, #0xFF
	add r5, r4, #0
_08001278:
	lsl r0, r2, #2
	add r0, r0, r4
	add r0, r0, r6
	ldrb r1, [r0]
	orr r1, r3
	strb r1, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #0x1D
	bls _08001278
	ldr r1, _08001338 @ =0x08080A48
	ldr r4, _08001324 @ =0x000012E9
	add r0, r5, r4
	ldrb r0, [r0]
	lsl r0, r0, #2
	add r0, r0, r1
	ldrb r2, [r0]
	ldrb r3, [r0, #1]
	ldr r1, _0800133C @ =0x000009D4
	add r0, r5, r1
	str r0, [sp, #0]
	add r0, r2, #0
	add r1, r3, #0
	bl sub_0807A398
	ldr r2, _08001340 @ =0x000012FA
	add r1, r5, r2
	mov r2, #0
	mov r0, #5
	strb r0, [r1]
	ldr r3, _08001344 @ =0x000012F9
	add r0, r5, r3
	strb r2, [r0]
	add r4, #0x85
	add r3, r5, r4
	ldr r1, _08001310 @ =0x03000040
	ldr r0, _08001348 @ =0x00004884
	add r2, r1, r0
	ldrb r4, [r2]
	lsl r0, r4, #0x1C
	lsr r0, r0, #0x1F
	strb r0, [r3]
	ldrh r2, [r2]
	lsr r2, r2, #4
	ldr r3, _0800134C @ =0x000012EB
	add r0, r5, r3
	strb r2, [r0]
	ldr r4, _08001350 @ =0x00004886
	add r1, r1, r4
	ldrh r1, [r1]
	ldr r2, _08001354 @ =0x0000136C
	add r0, r5, r2
	mov r2, #0
	strh r1, [r0]
	add r3, #0x84
	add r0, r5, r3
	strb r2, [r0]
	ldr r4, _08001358 @ =0x00001370
	add r0, r5, r4
	strb r2, [r0]
	ldr r1, _0800135C @ =0x00001374
	add r0, r5, r1
	bl sub_0807B0C0
	ldr r2, _08001360 @ =0x00001378
	add r0, r5, r2
	bl sub_0807B0C0
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800130C: .4byte 0x02013DE0
_08001310: .4byte 0x03000040
_08001314: .4byte 0x0000040E
_08001318: .4byte 0x0000E0FF
_0800131C: .4byte 0x04000016
_08001320: .4byte 0x000009CC
_08001324: .4byte 0x000012E9
_08001328: .4byte 0x000012EA
_0800132C: .4byte 0x00000AA8
_08001330: .4byte 0x000009E8
_08001334: .4byte 0x000009EC
_08001338: .4byte gUnk_08080A48
_0800133C: .4byte 0x000009D4
_08001340: .4byte 0x000012FA
_08001344: .4byte 0x000012F9
_08001348: .4byte 0x00004884
_0800134C: .4byte 0x000012EB
_08001350: .4byte 0x00004886
_08001354: .4byte 0x0000136C
_08001358: .4byte 0x00001370
_0800135C: .4byte 0x00001374
_08001360: .4byte 0x00001378
	thumb_func_end sub_080011F0

