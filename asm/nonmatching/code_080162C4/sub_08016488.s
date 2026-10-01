	thumb_func_start sub_08016488
sub_08016488: @ 0x08016488
	push {r4, lr}
	ldr r4, _080164C0 @ =0x020185C0
	ldrh r0, [r4, #2]
	bl sub_0806044C
	ldr r2, _080164C4 @ =0x020192E0
	ldr r0, _080164C8 @ =0x00001ACC
	add r2, r2, r0
	mov r1, #0xF
	ldrb r3, [r4, #2]
	and r1, r3
	mov r0, #0x10
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	ldr r0, _080164CC @ =0x0000080D
	add r4, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080164C0: .4byte 0x020185C0
_080164C4: .4byte 0x020192E0
_080164C8: .4byte 0x00001ACC
_080164CC: .4byte 0x0000080D
	thumb_func_end sub_08016488

