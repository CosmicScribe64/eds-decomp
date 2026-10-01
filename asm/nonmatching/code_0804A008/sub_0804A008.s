	thumb_func_start sub_0804A008
sub_0804A008: @ 0x0804A008
	push {r4, r5, r6, lr}
	sub sp, #0x14
	mov r5, #1
	bl sub_0805ECFC
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r4, #0
	bl sub_0800966C
	cmp r0, #0
	beq _0804A024
	mov r0, #1
	b _0804A1BA
_0804A024:
	ldr r2, _0804A068 @ =0x020192E0
	ldr r0, _0804A06C @ =0x00001B12
	add r1, r2, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0804A080
	ldr r6, _0804A070 @ =0x0201CFB0
	ldr r1, _0804A074 @ =0x00000828
	add r0, r6, r1
	ldr r0, [r0]
	cmp r0, #5
	beq _0804A042
	b _0804A1B8
_0804A042:
	add r0, r4, #0
	bl sub_0802CD28
	cmp r0, #1
	bgt _0804A04E
	b _0804A1B8
_0804A04E:
	ldr r3, _0804A078 @ =0x00000824
	add r0, r6, r3
	ldr r0, [r0]
	cmp r0, #0
	beq _0804A05A
	b _0804A1B8
_0804A05A:
	ldr r1, _0804A07C @ =0x0000082C
	add r0, r6, r1
	ldr r2, [r0]
	add r0, r4, #0
	mov r1, #0
	b _0804A160
	.align 2, 0
_0804A068: .4byte 0x020192E0
_0804A06C: .4byte 0x00001B12
_0804A070: .4byte 0x0201CFB0
_0804A074: .4byte 0x00000828
_0804A078: .4byte 0x00000824
_0804A07C: .4byte 0x0000082C
_0804A080:
	ldr r1, _0804A09C @ =0x0201CFB0
	ldr r3, _0804A0A0 @ =0x00000828
	add r0, r1, r3
	ldr r0, [r0]
	add r3, r1, #0
	cmp r0, #0xD
	bls _0804A090
	b _0804A1B8
_0804A090:
	lsl r0, r0, #2
	ldr r1, _0804A0A4 @ =0x0804A0A8
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0804A09C: .4byte 0x0201CFB0
_0804A0A0: .4byte 0x00000828
_0804A0A4: .4byte 0x0804A0A8
_0804A0A8:
	.4byte _0804A104
	.4byte _0804A1B8
	.4byte _0804A1B8
	.4byte _0804A1B8
	.4byte _0804A1B8
	.4byte _0804A128
	.4byte _0804A1B8
	.4byte _0804A1B8
	.4byte _0804A1B8
	.4byte _0804A1B8
	.4byte _0804A148
	.4byte _0804A0E0
	.4byte _0804A18C
	.4byte _0804A170
_0804A0E0:
	cmp r4, #0
	beq _0804A14C
	ldr r1, _0804A100 @ =0x00000824
	add r0, r3, r1
	ldr r0, [r0]
	cmp r0, #0
	bne _0804A1B8
	add r1, #8
	add r0, r3, r1
	ldr r2, [r0]
	add r0, r4, #0
	mov r1, #0
	bl sub_08049514
	b _0804A164
	.align 2, 0
_0804A100: .4byte 0x00000824
_0804A104:
	cmp r4, #0
	beq _0804A14C
	ldr r1, _0804A124 @ =0x00000824
	add r0, r3, r1
	ldr r0, [r0]
	cmp r0, #0
	bne _0804A1B8
	add r1, #8
	add r0, r3, r1
	ldr r2, [r0]
	add r0, r4, #0
	mov r1, #0
	bl sub_08049B74
	b _0804A164
	.align 2, 0
_0804A124: .4byte 0x00000824
_0804A128:
	cmp r4, #0
	beq _0804A14C
	ldr r1, _0804A144 @ =0x00000824
	add r0, r3, r1
	ldr r0, [r0]
	cmp r0, #0
	bne _0804A1B8
	add r1, #8
	add r0, r3, r1
	ldr r2, [r0]
	add r0, r4, #0
	mov r1, #0
	b _0804A160
	.align 2, 0
_0804A144: .4byte 0x00000824
_0804A148:
	cmp r4, #0
	bne _0804A150
_0804A14C:
	mov r0, #0
	b _0804A1BA
_0804A150:
	ldr r1, _0804A16C @ =0x00000824
	add r0, r3, r1
	ldr r0, [r0]
	cmp r0, #0
	bne _0804A1B8
	add r0, r4, #0
	mov r1, #0
	mov r2, #5
_0804A160:
	bl sub_08049DF0
_0804A164:
	orr r5, r0
	lsl r0, r5, #0x10
	lsr r5, r0, #0x10
	b _0804A1B8
_0804A16C: .4byte 0x00000824
_0804A170:
	ldr r3, _0804A188 @ =0x00001B12
	add r0, r2, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1B
	lsr r0, r0, #0x1D
	mov r5, #0x80
	lsl r5, r5, #2
	cmp r0, #0
	bne _0804A1B8
	mov r5, #0x80
	lsl r5, r5, #1
	b _0804A1B8
_0804A188: .4byte 0x00001B12
_0804A18C:
	bl sub_080094E4
	ldr r1, _0804A1C4 @ =0x0000060B
	cmp r0, r1
	bne _0804A1B8
	mov r2, sp
	ldrb r1, [r2, #2]
	mov r0, #2
	neg r0, r0
	and r0, r1
	strb r0, [r2, #2]
	mov r0, sp
	mov r1, #0
	mov r2, #0
	bl sub_0802DEC4
	cmp r0, #0
	beq _0804A1B8
	mov r1, #0x80
	lsl r1, r1, #3
	add r0, r1, #0
	orr r5, r0
_0804A1B8:
	add r0, r5, #0
_0804A1BA:
	add sp, #0x14
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0804A1C4: .4byte 0x0000060B
	thumb_func_end sub_0804A008

