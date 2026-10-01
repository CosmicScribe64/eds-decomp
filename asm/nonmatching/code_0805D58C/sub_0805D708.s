	thumb_func_start sub_0805D708
sub_0805D708: @ 0x0805D708
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r0, _0805D754 @ =0x0201CFB0
	mov r8, r0
	ldr r0, _0805D758 @ =0x00000834
	add r0, r8
	ldrb r7, [r0]
	ldrb r6, [r0, #1]
	ldrb r1, [r0, #2]
	str r1, [sp, #0]
	ldrh r0, [r0, #2]
	lsr r0, r0, #8
	mov r9, r0
	mov r0, #1
	and r0, r7
	ldr r1, _0805D75C @ =0x00000D64
	mul r0, r1
	ldr r1, _0805D760 @ =0x0201930C
	add r0, r0, r1
	mov r1, #0x94
	mul r1, r6
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	str r0, [sp, #4]
	ldr r5, _0805D764 @ =0x00000838
	add r5, r8
	ldrb r4, [r5]
	cmp r4, #0
	beq _0805D768
	cmp r4, #1
	beq _0805D782
	b _0805D816
_0805D754: .4byte 0x0201CFB0
_0805D758: .4byte 0x00000834
_0805D75C: .4byte 0x00000D64
_0805D760: .4byte 0x0201930C
_0805D764: .4byte 0x00000838
_0805D768:
	mov r0, #6
	bl sub_08077AEC
	add r0, r7, #0
	add r1, r6, #0
	bl sub_08060FD0
	ldr r0, _0805D838 @ =0x00000839
	add r0, r8
	strb r4, [r0]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
_0805D782:
	ldr r0, _0805D83C @ =0x0201CFB0
	ldr r2, _0805D838 @ =0x00000839
	add r2, r2, r0
	mov r8, r2
	ldrb r3, [r2]
	cmp r3, #0x17
	bhi _0805D816
	add r0, r7, #0
	mov r1, #0
	add r2, r6, #0
	bl sub_080623AC
	mov sl, r0
	add r0, r7, #0
	mov r1, #0
	add r2, r6, #0
	bl sub_080623EC
	add r6, r0, #0
	ldr r2, _0805D840 @ =0x081A4474
	mov r0, r8
	ldrb r0, [r0]
	lsl r1, r0, #1
	mov r3, r9
	lsl r0, r3, #1
	add r0, r9
	lsl r0, r0, #4
	add r1, r1, r0
	add r1, r1, r2
	ldrh r4, [r1]
	mov r5, #0x80
	lsl r5, r5, #5
	add r0, r4, #0
	and r0, r5
	cmp r0, #0
	beq _0805D7E0
	ldr r0, _0805D844 @ =0x0000EFFF
	and r4, r0
	ldr r0, [sp, #4]
	bl sub_08062140
	add r0, r0, r5
	lsl r1, r4, #0x10
	asr r1, r1, #0x10
	add r1, r1, r0
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
_0805D7E0:
	lsl r1, r6, #0x10
	mov r0, sl
	orr r1, r0
	mov r2, #0x80
	lsl r2, r2, #3
	add r0, r2, #0
	orr r4, r0
	lsl r0, r4, #0x10
	lsr r2, r0, #0x10
	mov r3, #0x80
	lsl r3, r3, #0x11
	ldr r0, [sp, #0]
	cmp r0, #0
	beq _0805D7FE
	add r3, #0x20
_0805D7FE:
	add r0, r1, #0
	mov r1, #0x80
	bl sub_08076714
	mov r1, r8
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x17
	bls _0805D828
_0805D816:
	ldr r1, _0805D83C @ =0x0201CFB0
	mov r2, #0x83
	lsl r2, r2, #4
	add r1, r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
_0805D828:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0805D838: .4byte 0x00000839
_0805D83C: .4byte 0x0201CFB0
_0805D840: .4byte gUnk_081A4474
_0805D844: .4byte 0x0000EFFF
	thumb_func_end sub_0805D708

