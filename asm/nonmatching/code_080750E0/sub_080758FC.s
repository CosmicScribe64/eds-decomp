	thumb_func_start sub_080758FC
sub_080758FC: @ 0x080758FC
	push {r4, r5, r6, lr}
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	ldr r1, _08075928 @ =0x03000040
	ldr r0, _0807592C @ =0x00004832
	add r4, r1, r0
	ldrb r2, [r4]
	lsl r3, r2, #0x1A
	lsr r0, r3, #0x1A
	add r5, r1, #0
	cmp r0, #2
	bls _08075930
	sub r0, #2
	mov r1, #0x3F
	and r0, r1
	mov r1, #0x40
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r4]
	b _08075938
	.align 2, 0
_08075928: .4byte 0x03000040
_0807592C: .4byte 0x00004832
_08075930:
	mov r0, #0x40
	neg r0, r0
	and r0, r2
	strb r0, [r4]
_08075938:
	ldr r1, _08075950 @ =0x00004832
	add r0, r5, r1
	ldrb r1, [r0]
	mov r0, #0x3F
	and r0, r1
	cmp r0, #0
	bne _08075954
	bl sub_080757F4
	mov r0, #1
	b _0807596C
	.align 2, 0
_08075950: .4byte 0x00004832
_08075954:
	ldr r3, _08075974 @ =0x04000052
	lsl r1, r1, #0x1A
	lsr r2, r1, #0x1A
	add r1, r2, #0
	mov r0, #0x1F
	sub r0, r0, r1
	lsl r0, r0, #8
	add r2, r2, r0
	strh r2, [r3]
	ldr r0, _08075978 @ =0x04000050
	strh r6, [r0]
	mov r0, #0
_0807596C:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08075974: .4byte 0x04000052
_08075978: .4byte 0x04000050
	thumb_func_end sub_080758FC

