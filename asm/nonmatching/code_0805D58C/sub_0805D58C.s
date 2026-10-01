	thumb_func_start sub_0805D58C
sub_0805D58C: @ 0x0805D58C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	ldr r0, _0805D5E8 @ =0x0201CFB0
	mov r8, r0
	ldr r2, _0805D5EC @ =0x00000834
	add r2, r8
	ldrb r7, [r2]
	ldrb r6, [r2, #1]
	ldrb r1, [r2, #2]
	str r1, [sp, #0]
	mov r1, #1
	and r1, r7
	mov r0, #0x94
	add r4, r6, #0
	mul r4, r0
	ldr r0, _0805D5F0 @ =0x00000D64
	mul r1, r0
	add r0, r4, r1
	ldr r3, _0805D5F4 @ =0x0201930C
	add r0, r0, r3
	ldrb r0, [r0, #6]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	mov r9, r0
	ldrh r2, [r2, #2]
	lsr r2, r2, #8
	str r2, [sp, #4]
	add r1, r1, r3
	add r1, r1, r4
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	str r0, [sp, #8]
	ldr r5, _0805D5F8 @ =0x00000838
	add r5, r8
	ldrb r4, [r5]
	cmp r4, #0
	beq _0805D5FC
	cmp r4, #1
	beq _0805D616
	b _0805D6D2
	.align 2, 0
_0805D5E8: .4byte 0x0201CFB0
_0805D5EC: .4byte 0x00000834
_0805D5F0: .4byte 0x00000D64
_0805D5F4: .4byte 0x0201930C
_0805D5F8: .4byte 0x00000838
_0805D5FC:
	mov r0, #6
	bl sub_08077AEC
	add r0, r7, #0
	add r1, r6, #0
	bl sub_08060FD0
	ldr r0, _0805D6F4 @ =0x00000839
	add r0, r8
	strb r4, [r0]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
_0805D616:
	ldr r0, _0805D6F8 @ =0x0201CFB0
	ldr r2, _0805D6F4 @ =0x00000839
	add r2, r2, r0
	mov r8, r2
	ldrb r5, [r2]
	cmp r5, #9
	bhi _0805D6D2
	add r0, r7, #0
	mov r1, #0
	add r2, r6, #0
	bl sub_080623AC
	mov sl, r0
	add r0, r7, #0
	mov r1, #0
	add r2, r6, #0
	bl sub_080623EC
	add r7, r0, #0
	ldr r6, _0805D6FC @ =0x081A4474
	mov r1, r9
	lsl r0, r1, #1
	add r0, r9
	lsl r5, r0, #4
	add r0, r5, r6
	ldrh r4, [r0]
	ldr r2, [sp, #4]
	cmp r2, #0
	beq _0805D66C
	mov r1, r8
	ldrb r1, [r1]
	lsl r0, r1, #1
	mov r2, r8
	ldrb r2, [r2]
	add r0, r0, r2
	lsl r0, r0, #3
	mov r1, #0xA
	bl __divsi3
	lsl r0, r0, #1
	add r0, r0, r5
	add r0, r0, r6
	ldrh r4, [r0]
_0805D66C:
	mov r5, #0x80
	lsl r5, r5, #5
	add r0, r4, #0
	and r0, r5
	cmp r0, #0
	beq _0805D68E
	ldr r0, _0805D700 @ =0x0000EFFF
	and r4, r0
	ldr r0, [sp, #8]
	bl sub_08062140
	add r0, r0, r5
	lsl r1, r4, #0x10
	asr r1, r1, #0x10
	add r1, r1, r0
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
_0805D68E:
	lsl r0, r7, #0x10
	mov r5, sl
	orr r0, r5
	mov r2, #0x80
	lsl r2, r2, #3
	add r1, r2, #0
	orr r4, r1
	lsl r2, r4, #0x10
	lsr r2, r2, #0x10
	ldr r3, _0805D704 @ =0x081A44D4
	mov r5, r8
	ldrb r5, [r5]
	lsl r4, r5, #1
	ldr r5, [sp, #0]
	lsl r1, r5, #2
	add r1, r1, r5
	lsl r1, r1, #2
	add r4, r4, r1
	add r4, r4, r3
	mov r3, #0x80
	lsl r3, r3, #0x11
	ldrh r4, [r4]
	orr r3, r4
	mov r1, #0x80
	bl sub_08076714
	mov r1, r8
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #9
	bls _0805D6E4
_0805D6D2:
	ldr r1, _0805D6F8 @ =0x0201CFB0
	mov r2, #0x83
	lsl r2, r2, #4
	add r1, r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r5, [r1]
	and r0, r5
	strb r0, [r1]
_0805D6E4:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0805D6F4: .4byte 0x00000839
_0805D6F8: .4byte 0x0201CFB0
_0805D6FC: .4byte gUnk_081A4474
_0805D700: .4byte 0x0000EFFF
_0805D704: .4byte gUnk_081A44D4
	thumb_func_end sub_0805D58C

