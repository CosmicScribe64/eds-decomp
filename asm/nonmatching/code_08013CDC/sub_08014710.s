	thumb_func_start sub_08014710
sub_08014710: @ 0x08014710
	push {r4, r5, r6, lr}
	ldr r1, _08014730 @ =0x020185C0
	ldrh r0, [r1]
	lsr r5, r0, #0xF
	ldr r2, _08014734 @ =0x0000080A
	add r6, r1, r2
	ldrb r3, [r6]
	lsl r0, r3, #0x19
	lsr r0, r0, #0x19
	add r4, r1, #0
	cmp r0, #0
	beq _08014738
	cmp r0, #1
	beq _08014774
	b _080147EC
	.align 2, 0
_08014730: .4byte 0x020185C0
_08014734: .4byte 0x0000080A
_08014738:
	add r0, r5, #0
	mov r1, #0
	bl sub_080240A8
	ldr r0, _0801476C @ =0x0000080C
	add r1, r4, r0
	ldr r0, _08014770 @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldrb r2, [r6]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r6]
	mov r0, #0xF
	bl sub_08077AEC
	b _080147FA
	.align 2, 0
_0801476C: .4byte 0x0000080C
_08014770: .4byte 0xFFFFF01F
_08014774:
	ldr r3, _080147E0 @ =0x0000080C
	add r6, r4, r3
	ldrh r1, [r6]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x1F
	bgt _080147EC
	add r0, r5, #0
	mov r1, #0
	mov r2, #2
	bl sub_080623AC
	add r4, r0, #0
	add r0, r5, #0
	mov r1, #0
	mov r2, #2
	bl sub_080623EC
	lsl r2, r0, #0x10
	orr r2, r4
	ldr r1, _080147E4 @ =0x08198D8C
	ldrh r3, [r6]
	lsl r0, r3, #0x14
	lsr r0, r0, #0x19
	lsl r0, r0, #1
	add r0, r0, r1
	mov r3, #0x80
	lsl r3, r3, #3
	add r1, r3, #0
	ldrh r4, [r0]
	orr r4, r1
	mov r3, #0x80
	lsl r3, r3, #0x11
	cmp r5, #0
	beq _080147BC
	add r3, #0x40
_080147BC:
	add r0, r2, #0
	mov r1, #0x80
	add r2, r4, #0
	bl sub_08076714
	ldrh r2, [r6]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _080147E8 @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r6]
	b _080147FA
	.align 2, 0
_080147E0: .4byte 0x0000080C
_080147E4: .4byte gUnk_08198D8C
_080147E8: .4byte 0xFFFFF01F
_080147EC:
	ldr r0, _08014800 @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_080147FA:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_08014800: .4byte 0x0000080D
	thumb_func_end sub_08014710

