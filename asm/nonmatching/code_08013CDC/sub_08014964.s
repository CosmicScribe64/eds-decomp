	thumb_func_start sub_08014964
sub_08014964: @ 0x08014964
	push {r4, r5, r6, r7, lr}
	ldr r6, _080149C8 @ =0x020192E4
	ldr r3, _080149CC @ =0x020185C0
	mov r4, #0x80
	lsl r4, r4, #8
	add r0, r4, #0
	ldrh r1, [r3]
	and r0, r1
	mov r2, #0
	cmp r0, #0
	beq _0801497C
	ldr r2, _080149D0 @ =0x00000D64
_0801497C:
	add r2, r2, r6
	mov r5, #1
	ldrb r1, [r3, #2]
	and r1, r5
	lsl r1, r1, #3
	mov r0, #9
	neg r0, r0
	ldrb r7, [r2, #7]
	and r0, r7
	orr r0, r1
	strb r0, [r2, #7]
	add r0, r4, #0
	ldrh r1, [r3]
	and r0, r1
	mov r2, #0
	cmp r0, #0
	beq _080149A0
	ldr r2, _080149D0 @ =0x00000D64
_080149A0:
	add r2, r2, r6
	ldrb r1, [r3, #4]
	and r1, r5
	lsl r1, r1, #4
	mov r0, #0x11
	neg r0, r0
	ldrb r4, [r2, #7]
	and r0, r4
	orr r0, r1
	strb r0, [r2, #7]
	ldr r7, _080149D4 @ =0x0000080D
	add r1, r3, r7
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080149C8: .4byte 0x020192E4
_080149CC: .4byte 0x020185C0
_080149D0: .4byte 0x00000D64
_080149D4: .4byte 0x0000080D
	thumb_func_end sub_08014964

