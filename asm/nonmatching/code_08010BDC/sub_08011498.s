	thumb_func_start sub_08011498
sub_08011498: @ 0x08011498
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	ldr r4, _080114C8 @ =0x020185C0
	ldrh r0, [r4]
	lsr r2, r0, #0xF
	ldrh r3, [r4, #2]
	ldr r1, _080114CC @ =0x0000080A
	add r1, r1, r4
	mov sl, r1
	ldrb r5, [r1]
	lsl r0, r5, #0x19
	lsr r7, r0, #0x19
	cmp r7, #1
	beq _080114F8
	cmp r7, #1
	bgt _080114D0
	cmp r7, #0
	beq _080114D6
	b _080115DC
	.align 2, 0
_080114C8: .4byte 0x020185C0
_080114CC: .4byte 0x0000080A
_080114D0:
	cmp r7, #2
	beq _080115BC
	b _080115DC
_080114D6:
	add r0, r2, #0
	mov r1, #0xB
	bl sub_080240A8
	mov r7, sl
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r7]
	b _080115F0
_080114F8:
	ldr r0, _080115A0 @ =0x00000814
	add r0, r0, r4
	mov r9, r0
	add r0, r2, #0
	and r0, r7
	ldr r1, _080115A4 @ =0x00000D64
	add r5, r0, #0
	mul r5, r1
	ldr r6, _080115A8 @ =0x02019968
	add r1, r5, r6
	lsl r4, r3, #2
	add r1, r1, r4
	mov r0, r9
	str r2, [sp, #8]
	str r3, [sp, #0xC]
	bl sub_08007558
	add r4, r4, r5
	add r4, r4, r6
	ldr r0, _080115AC @ =0xFFFFF000
	ldrh r1, [r4]
	and r0, r1
	strh r0, [r4]
	ldr r2, [sp, #8]
	and r2, r7
	mov r5, #2
	neg r5, r5
	mov r8, r5
	ldr r0, [sp, #0]
	and r0, r5
	orr r0, r2
	mov r6, #0x1F
	neg r6, r6
	and r0, r6
	mov r1, #0x16
	orr r0, r1
	ldr r1, _080115B0 @ =0x000001FF
	ldr r3, [sp, #0xC]
	and r3, r1
	lsl r1, r3, #5
	ldr r5, _080115B4 @ =0xFFFFC01F
	and r0, r5
	orr r0, r1
	ldr r4, _080115B8 @ =0xFFFFBFFF
	and r0, r4
	mov r3, #0x80
	lsl r3, r3, #8
	orr r0, r3
	str r0, [sp, #0]
	mov r1, r9
	ldr r0, [r1]
	lsl r2, r0, #0x13
	lsr r2, r2, #0x1F
	and r2, r7
	ldr r1, [sp, #4]
	mov r7, r8
	and r1, r7
	orr r1, r2
	and r1, r6
	mov r2, #0x1C
	orr r1, r2
	and r1, r5
	and r1, r4
	orr r1, r3
	str r1, [sp, #4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl sub_080242C4
	mov r0, sl
	ldrb r2, [r0]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	mov r1, sl
	b _080115EE
_080115A0: .4byte 0x00000814
_080115A4: .4byte 0x00000D64
_080115A8: .4byte 0x02019968
_080115AC: .4byte 0xFFFFF000
_080115B0: .4byte 0x000001FF
_080115B4: .4byte 0xFFFFC01F
_080115B8: .4byte 0xFFFFBFFF
_080115BC:
	ldrh r0, [r4, #4]
	cmp r0, #0
	beq _080115C8
	add r0, r2, #0
	bl sub_0800A0A8
_080115C8:
	ldr r2, _08011600 @ =0x00000816
	add r0, r4, r2
	mov r1, #0x10
	ldrb r5, [r0]
	orr r1, r5
	strb r1, [r0]
	ldr r7, _08011604 @ =0x00000814
	add r0, r4, r7
	bl sub_080096F4
_080115DC:
	bl sub_080611AC
	ldr r1, _08011608 @ =0x020185C0
	ldr r0, _0801160C @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_080115EE:
	strb r0, [r1]
_080115F0:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08011600: .4byte 0x00000816
_08011604: .4byte 0x00000814
_08011608: .4byte 0x020185C0
_0801160C: .4byte 0x0000080D
	thumb_func_end sub_08011498

