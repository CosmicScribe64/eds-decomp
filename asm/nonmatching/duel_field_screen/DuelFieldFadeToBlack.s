	thumb_func_start DuelFieldFadeToBlack
DuelFieldFadeToBlack: @ 0x08060B6C
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r1, _08060BD0 @ =0x04000050
	ldr r2, _08060BD4 @ =0x000027E7
	add r0, r2, #0
	strh r0, [r1]
	ldr r1, _08060BD8 @ =0x03000040
	ldr r0, _08060BDC @ =0x00004832
	add r4, r1, r0
	ldrb r3, [r4]
	lsl r2, r3, #0x1A
	lsr r0, r2, #0x1A
	add r6, r1, #0
	cmp r0, #0x1E
	bhi _08060BAE
	add r1, r0, #0
	add r1, r1, r5
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
	bls _08060BAE
	and r2, r5
	mov r0, #0x1F
	orr r2, r0
	strb r2, [r4]
_08060BAE:
	ldr r2, _08060BE0 @ =0x04000054
	ldr r1, _08060BDC @ =0x00004832
	add r0, r6, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r1, r0, #0x1A
	strh r1, [r2]
	add r0, r1, #0
	cmp r0, #0x1E
	bls _08060BE8
	sub r2, #0x54
	ldrh r1, [r2]
	ldr r0, _08060BE4 @ =0x0000F8FF
	and r0, r1
	strh r0, [r2]
	mov r0, #1
	b _08060BEA
_08060BD0: .4byte 0x04000050
_08060BD4: .4byte 0x000027E7
_08060BD8: .4byte 0x03000040
_08060BDC: .4byte 0x00004832
_08060BE0: .4byte 0x04000054
_08060BE4: .4byte 0x0000F8FF
_08060BE8:
	mov r0, #0
_08060BEA:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end DuelFieldFadeToBlack

