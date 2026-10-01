	thumb_func_start sub_0802E058
sub_0802E058: @ 0x0802E058
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r7, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802E100
	ldr r0, _0802E090 @ =0x020192E4
	mov r9, r0
	ldrb r0, [r7, #2]
	lsl r2, r0, #0x1F
	mov r4, #1
	lsr r0, r2, #0x1F
	ldr r6, _0802E094 @ =0x00000D64
	mul r0, r6
	add r0, r9
	ldr r1, _0802E098 @ =0x000001F3
	ldrh r0, [r0]
	cmp r0, r1
	bls _0802E100
	lsr r0, r2, #0x1F
	bl sub_08047114
	cmp r0, #0
	bne _0802E0A0
	b _0802E100
	.align 2, 0
_0802E090: .4byte 0x020192E4
_0802E094: .4byte 0x00000D64
_0802E098: .4byte 0x000001F3
_0802E09C:
	mov r0, #1
	b _0802E102
_0802E0A0:
	mov r5, #0
	ldrb r2, [r7, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	and r4, r0
	add r0, r4, #0
	mul r0, r6
	add r0, r9
	ldrb r0, [r0, #2]
	cmp r5, r0
	bge _0802E100
	mov r0, #1
	mov r8, r0
_0802E0BA:
	lsl r0, r2, #0x1F
	lsr r2, r0, #0x1F
	mov r1, r8
	and r1, r2
	lsl r2, r5, #2
	mul r1, r6
	add r2, r2, r1
	ldr r1, _0802E110 @ =0x02019968
	add r2, r2, r1
	ldr r1, [r2]
	lsl r1, r1, #0x14
	lsr r4, r1, #0x14
	lsr r0, r0, #0x1F
	add r1, r4, #0
	bl sub_08054398
	cmp r0, #0
	beq _0802E0E8
	add r0, r4, #0
	bl sub_08007834
	cmp r0, #0
	beq _0802E09C
_0802E0E8:
	add r5, #1
	ldrb r2, [r7, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, r8
	and r1, r0
	add r0, r1, #0
	mul r0, r6
	add r0, r9
	ldrb r0, [r0, #2]
	cmp r5, r0
	blt _0802E0BA
_0802E100:
	mov r0, #0
_0802E102:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802E110: .4byte 0x02019968
	thumb_func_end sub_0802E058

