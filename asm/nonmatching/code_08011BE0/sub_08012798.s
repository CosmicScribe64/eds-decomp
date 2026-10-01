	thumb_func_start sub_08012798
sub_08012798: @ 0x08012798
	push {r4, lr}
	ldr r2, _080127D0 @ =0x020185C0
	ldrh r0, [r2]
	lsr r3, r0, #0xF
	mov r0, #0x94
	ldrh r4, [r2, #2]
	add r1, r4, #0
	mul r1, r0
	ldr r0, _080127D4 @ =0x00000D64
	mul r0, r3
	add r1, r1, r0
	ldr r0, _080127D8 @ =0x0201930C
	add r1, r1, r0
	add r1, #0x8C
	mov r0, #0x20
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	ldr r4, _080127DC @ =0x0000080D
	add r2, r2, r4
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	strb r0, [r2]
	pop {r4}
	pop {r0}
	bx r0
_080127D0: .4byte 0x020185C0
_080127D4: .4byte 0x00000D64
_080127D8: .4byte 0x0201930C
_080127DC: .4byte 0x0000080D
	thumb_func_end sub_08012798

