	thumb_func_start sub_08026AE0
sub_08026AE0: @ 0x08026AE0
	push {r4, r5, r6, lr}
	ldr r4, _08026AF8 @ =0x02020E28
	add r0, r4, #0
	bl sub_0807883C
	ldrb r0, [r4, #6]
	cmp r0, #2
	bne _08026B00
	mov r0, #0
	strb r0, [r4, #6]
	ldr r3, _08026AFC @ =0x04000208
	b _08026B84
_08026AF8: .4byte 0x02020E28
_08026AFC: .4byte 0x04000208
_08026B00:
	add r6, r4, #0
	add r6, #8
	ldrb r2, [r4, #8]
	cmp r2, #2
	bne _08026B24
	mov r1, #0x80
	lsl r1, r1, #1
	mov r0, #0
	mov r2, #0
	add r3, r4, #0
	bl sub_080787F4
	mov r0, #0
	strb r0, [r4, #8]
	ldr r3, _08026B74 @ =0xFFFFFE22
	add r1, r4, r3
	mov r0, #0xFF
	strb r0, [r1]
_08026B24:
	bl sub_080269FC
	sub r5, r4, #2
	ldrb r0, [r5]
	cmp r0, #0x4F
	bls _08026B5E
	add r0, r4, #0
	add r0, #0x10
	bl sub_0807B224
	mov r0, #0xF8
	lsl r0, r0, #5
	ldr r2, _08026B78 @ =0x08087BA4
	ldrb r1, [r5]
	sub r1, #0x10
	lsl r1, r1, #1
	add r1, r1, r2
	mov r3, #0
	ldsh r2, [r1, r3]
	mov r1, #0x80
	lsl r1, r1, #1
	sub r1, r1, r2
	bl sub_0807B4D0
	asr r0, r0, #8
	mov r2, #0xC1
	lsl r2, r2, #4
	add r1, r4, r2
	strb r0, [r1]
_08026B5E:
	add r0, r6, #0
	bl sub_0807B0D0
	ldr r1, _08026B7C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08026B80
	mov r0, #0
	b _08026B96
_08026B74: .4byte 0xFFFFFE22
_08026B78: .4byte gUnk_08087BA4
_08026B7C: .4byte 0x03000040
_08026B80:
	ldr r3, _08026B9C @ =0x04000208
	mov r0, #0
_08026B84:
	strh r0, [r3]
	ldr r2, _08026BA0 @ =0x04000200
	ldrh r1, [r2]
	ldr r0, _08026BA4 @ =0x0000FFFD
	and r0, r1
	strh r0, [r2]
	mov r0, #1
	strh r0, [r3]
	mov r0, #1
_08026B96:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08026B9C: .4byte 0x04000208
_08026BA0: .4byte 0x04000200
_08026BA4: .4byte 0x0000FFFD
	thumb_func_end sub_08026AE0

