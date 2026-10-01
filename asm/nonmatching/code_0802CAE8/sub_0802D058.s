	thumb_func_start sub_0802D058
sub_0802D058: @ 0x0802D058
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	add r4, r1, #0
	mov r3, #0
	ldr r1, _0802D0C4 @ =0x02017A40
	mov r2, #0xF0
	lsl r2, r2, #2
	add r0, r1, r2
	ldrh r0, [r0]
	add r7, r1, #0
	cmp r3, r0
	bge _0802D09A
	mov r8, r0
	ldr r6, _0802D0C8 @ =0x00000282
	mov ip, r6
_0802D07A:
	mov r0, ip
	add r2, r1, r0
	ldrb r6, [r2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r5
	bne _0802D092
	ldrh r2, [r2]
	lsl r0, r2, #0x16
	lsr r0, r0, #0x1A
	cmp r0, r4
	beq _0802D0C0
_0802D092:
	add r1, #0x14
	add r3, #1
	cmp r3, r8
	blt _0802D07A
_0802D09A:
	mov r3, #0
	mov r1, #0xF1
	lsl r1, r1, #2
	add r0, r7, r1
	ldrh r0, [r0]
	cmp r3, r0
	bge _0802D0D4
	add r2, r0, #0
	add r1, r7, #2
_0802D0AC:
	ldrb r6, [r1]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r5
	bne _0802D0CC
	ldrh r6, [r1]
	lsl r0, r6, #0x16
	lsr r0, r0, #0x1A
	cmp r0, r4
	bne _0802D0CC
_0802D0C0:
	mov r0, #1
	b _0802D0D6
_0802D0C4: .4byte 0x02017A40
_0802D0C8: .4byte 0x00000282
_0802D0CC:
	add r1, #0x14
	add r3, #1
	cmp r3, r2
	blt _0802D0AC
_0802D0D4:
	mov r0, #0
_0802D0D6:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0802D058

