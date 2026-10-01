	thumb_func_start sub_0803A81C
sub_0803A81C: @ 0x0803A81C
	push {r4, r5, lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _0803A880
	mov r0, #0xFC
	ldrb r2, [r1, #3]
	and r0, r2
	cmp r0, #0x20
	bne _0803A880
	ldrb r3, [r1, #6]
	ldrh r0, [r1, #6]
	lsr r5, r0, #8
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r3, r0
	beq _0803A880
	mov r4, #1
	add r1, r3, #0
	and r1, r4
	mov r0, #0x94
	add r2, r5, #0
	mul r2, r0
	ldr r0, _0803A888 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0803A88C @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803A880
	ldrb r2, [r2, #6]
	add r0, r4, #0
	and r0, r2
	cmp r0, #0
	beq _0803A880
	mov r0, #2
	and r0, r2
	cmp r0, #0
	bne _0803A880
	add r0, r3, #0
	add r1, r5, #0
	mov r2, #1
	mov r3, #0
	bl sub_08018ED8
_0803A880:
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
_0803A888: .4byte 0x00000D64
_0803A88C: .4byte 0x0201930C
	thumb_func_end sub_0803A81C

