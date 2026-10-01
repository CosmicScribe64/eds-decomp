	thumb_func_start sub_08059780
sub_08059780: @ 0x08059780
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x18
	ldr r1, _08059894 @ =0x02015EF0
	mov r0, #0
	strb r0, [r1, #7]
	add r6, r1, #0
	mov r4, sp
	add r5, r6, #0
	mov r0, #0x94
	mov r9, r0
	ldr r1, _08059898 @ =0x0201A070
	mov r8, r1
	ldr r2, _0805989C @ =0x00000D9C
	add r2, r8
	mov sl, r2
_080597A6:
	mov r0, #0
	strb r0, [r6, #8]
_080597AA:
	ldrb r0, [r4, #2]
	mov r1, #1
	orr r0, r1
	strb r0, [r4, #2]
	ldrb r7, [r5, #8]
	mov r0, r9
	mul r0, r7
	add r0, r8
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	strh r3, [r4]
	mov r1, #0x3F
	ldrb r0, [r5, #8]
	and r1, r0
	lsl r1, r1, #4
	ldrh r0, [r4, #2]
	ldr r7, _080598A0 @ =0xFFFFFC0F
	add r2, r7, #0
	and r0, r2
	orr r0, r1
	strh r0, [r4, #2]
	ldrb r1, [r4, #3]
	mov r0, #3
	and r0, r1
	strb r0, [r4, #3]
	cmp r3, #0
	bne _080597E4
	b _0805991C
_080597E4:
	ldr r1, _080598A4 @ =0x000007FF
	add r0, r1, #0
	and r3, r0
	lsl r1, r3, #1
	ldr r2, _080598A8 @ =0x08622AB4
	add r1, r1, r2
	ldr r7, _080598AC @ =0x0819DD64
	ldrb r6, [r6, #7]
	lsl r0, r6, #1
	add r0, r0, r7
	ldrh r1, [r1]
	ldrh r0, [r0]
	cmp r1, r0
	beq _08059802
	b _0805991C
_08059802:
	ldrb r0, [r5, #8]
	mov r1, r9
	mul r1, r0
	add r1, r8
	mov r3, #2
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08059818
	b _0805991C
_08059818:
	mov r0, sp
	mov r1, #0
	mov r2, #0
	str r3, [sp, #0x14]
	bl sub_0802CE38
	cmp r0, #0
	beq _0805991C
	mov r6, #1
	ldrb r1, [r5, #7]
	lsl r0, r1, #1
	add r0, r0, r7
	ldr r1, _080598B0 @ =0x000001FF
	ldrh r0, [r0]
	cmp r0, r1
	bne _080598DC
	ldr r0, _080598B4 @ =0x000004DD
	bl sub_08059408
	lsl r0, r0, #0x10
	ldr r3, [sp, #0x14]
	cmp r0, #0
	beq _080598D0
	mov r1, #0xDA
	lsl r1, r1, #4
	add r1, r8
	ldr r2, _080598B8 @ =0xFFFFFC03
	add r0, r2, #0
	ldrh r4, [r1]
	and r0, r4
	strh r0, [r1]
	ldrb r7, [r5, #0xB]
	lsl r0, r7, #2
	ldr r1, _080598BC @ =0x0201A6CC
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	ldr r1, _080598C0 @ =0x00000D98
	add r1, r8
	strh r0, [r1]
	ldr r0, _080598C4 @ =0x00000DA3
	add r0, r8
	ldrb r1, [r0]
	orr r1, r3
	strb r1, [r0]
	ldr r1, _080598C8 @ =0x00000DA4
	add r1, r8
	ldrb r5, [r5, #0xB]
	lsl r2, r5, #1
	ldr r4, _080598CC @ =0xFFFFFE01
	add r0, r4, #0
	ldrh r7, [r1]
	and r0, r7
	orr r0, r2
	strh r0, [r1]
	mov r1, sl
	ldrb r0, [r1]
	orr r0, r3
	strb r0, [r1]
	b _0805993E
	.align 2, 0
_08059894: .4byte 0x02015EF0
_08059898: .4byte 0x0201A070
_0805989C: .4byte 0x00000D9C
_080598A0: .4byte 0xFFFFFC0F
_080598A4: .4byte 0x000007FF
_080598A8: .4byte gUnk_08622AB4
_080598AC: .4byte gUnk_0819DD64
_080598B0: .4byte 0x000001FF
_080598B4: .4byte 0x000004DD
_080598B8: .4byte 0xFFFFFC03
_080598BC: .4byte 0x0201A6CC
_080598C0: .4byte 0x00000D98
_080598C4: .4byte 0x00000DA3
_080598C8: .4byte 0x00000DA4
_080598CC: .4byte 0xFFFFFE01
_080598D0:
	mov r0, #1
	bl sub_08008860
	cmp r0, #1
	bgt _080598DC
	mov r6, #0
_080598DC:
	cmp r6, #0
	beq _0805991C
	ldrb r4, [r5, #8]
	lsl r2, r4, #8
	ldr r0, _08059914 @ =0x00008008
	mov r1, #1
	mov r3, #0
	bl sub_0801EC58
	ldrb r2, [r5, #8]
	mov r1, #0x1F
	add r0, r2, #0
	and r0, r1
	lsl r0, r0, #0x10
	mov r1, r9
	mul r1, r2
	add r1, r8
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	ldr r2, _08059918 @ =0x80400000
	orr r1, r2
	orr r0, r1
	mov r1, #0
	bl sub_0801FBCC
	mov r0, #1
	b _08059940
_08059914: .4byte 0x00008008
_08059918: .4byte 0x80400000
_0805991C:
	ldrb r0, [r5, #8]
	add r0, #1
	strb r0, [r5, #8]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	ldr r6, _08059950 @ =0x02015EF0
	cmp r0, #4
	bhi _0805992E
	b _080597AA
_0805992E:
	ldrb r0, [r5, #7]
	add r0, #1
	strb r0, [r5, #7]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #3
	bhi _0805993E
	b _080597A6
_0805993E:
	mov r0, #0
_08059940:
	add sp, #0x18
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08059950: .4byte 0x02015EF0
	thumb_func_end sub_08059780

