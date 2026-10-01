	thumb_func_start sub_08012264
sub_08012264: @ 0x08012264
	push {r4, r5, lr}
	ldr r3, _080122A4 @ =0x020185C0
	ldrh r0, [r3]
	lsr r1, r0, #0xF
	mov r0, #0x94
	ldrh r2, [r3, #2]
	add r4, r2, #0
	mul r4, r0
	ldr r0, _080122A8 @ =0x00000D64
	mul r0, r1
	add r4, r4, r0
	ldr r0, _080122AC @ =0x0201930C
	add r4, r4, r0
	mov r1, #1
	ldrb r5, [r3, #4]
	and r1, r5
	lsl r1, r1, #5
	mov r2, #0x21
	neg r2, r2
	add r0, r2, #0
	ldrb r5, [r4, #7]
	and r0, r5
	orr r0, r1
	strb r0, [r4, #7]
	ldr r0, _080122B0 @ =0x0000080D
	add r3, r3, r0
	ldrb r5, [r3]
	and r2, r5
	strb r2, [r3]
	pop {r4, r5}
	pop {r0}
	bx r0
_080122A4: .4byte 0x020185C0
_080122A8: .4byte 0x00000D64
_080122AC: .4byte 0x0201930C
_080122B0: .4byte 0x0000080D
	thumb_func_end sub_08012264

