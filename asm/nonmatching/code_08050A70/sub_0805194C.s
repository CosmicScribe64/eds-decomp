	thumb_func_start sub_0805194C
sub_0805194C: @ 0x0805194C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	add r7, r1, #0
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r9, r3
	ldr r0, _08051974 @ =0x020192E0
	mov r8, r0
	ldr r5, _08051978 @ =0x00001B62
	add r5, r8
	ldrb r4, [r5]
	cmp r4, #0
	beq _0805197C
	cmp r4, #1
	beq _080519A8
	mov r0, #1
	b _08051A72
_08051974: .4byte 0x020192E0
_08051978: .4byte 0x00001B62
_0805197C:
	add r0, r6, #0
	mov r1, #0xB
	bl sub_080240A8
	ldr r0, _0805199C @ =0x02017A40
	ldr r2, _080519A0 @ =0x000004FC
	add r1, r0, r2
	strb r4, [r1]
	ldr r1, _080519A4 @ =0x000004FD
	add r0, r0, r1
	strb r7, [r0]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _08051A70
	.align 2, 0
_0805199C: .4byte 0x02017A40
_080519A0: .4byte 0x000004FC
_080519A4: .4byte 0x000004FD
_080519A8:
	cmp r6, #0
	beq _08051A44
	ldr r0, _08051A14 @ =0x02017A40
	ldr r2, _08051A18 @ =0x000004FD
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _08051A64
	mov r5, r8
	add r5, #4
	and r4, r6
	ldr r0, _08051A1C @ =0x00000D64
	mul r0, r4
	add r0, r0, r5
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _08051A64
	bl sub_08056CE8
	add r1, r0, #0
	cmp r1, #0
	bge _08051A26
	add r0, r5, #0
	mov r1, #1
	bl sub_0805664C
	add r1, r0, #0
	cmp r1, #0
	bge _08051A26
	mov r0, #1
	bl sub_0800A1C4
	add r1, r0, #0
	cmp r1, #0
	bge _08051A26
	mov r0, #1
	bl sub_0800A158
	add r1, r0, #0
	cmp r1, #0
	bge _08051A26
	ldr r4, _08051A20 @ =0x00000D6A
	add r4, r8
	ldrb r0, [r4]
	cmp r0, #2
	bls _08051A24
	bl sub_08076F9C
	ldrb r1, [r4]
	bl __modsi3
	add r1, r0, #0
	b _08051A26
	.align 2, 0
_08051A14: .4byte 0x02017A40
_08051A18: .4byte 0x000004FD
_08051A1C: .4byte 0x00000D64
_08051A20: .4byte 0x00000D6A
_08051A24:
	mov r1, #0
_08051A26:
	add r0, r6, #0
	mov r2, r9
	mov r3, #1
	bl sub_080193D4
	ldr r0, _08051A3C @ =0x02017A40
	ldr r1, _08051A40 @ =0x000004FD
	add r0, r0, r1
	ldrb r1, [r0]
	sub r1, #1
	b _08051A6E
_08051A3C: .4byte 0x02017A40
_08051A40: .4byte 0x000004FD
_08051A44:
	mov r0, #0
	mov r1, #0xB
	mov r2, #0
	bl sub_08024134
	ldr r0, _08051A80 @ =0x00000209
	ldr r1, _08051A84 @ =0x0000050E
	ldr r3, _08051A88 @ =0x08085FDC
	mov r2, #0xB
	bl sub_080602A4
	ldr r1, _08051A8C @ =0x080516D9
	ldr r2, _08051A90 @ =0x08051731
	mov r0, #5
	bl sub_08060308
_08051A64:
	ldr r0, _08051A94 @ =0x020192E0
	ldr r2, _08051A98 @ =0x00001B62
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
_08051A6E:
	strb r1, [r0]
_08051A70:
	mov r0, #0
_08051A72:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08051A80: .4byte 0x00000209
_08051A84: .4byte 0x0000050E
_08051A88: .4byte gUnk_08085FDC
_08051A8C: .4byte sub_080516D8
_08051A90: .4byte sub_08051730
_08051A94: .4byte 0x020192E0
_08051A98: .4byte 0x00001B62
	thumb_func_end sub_0805194C

