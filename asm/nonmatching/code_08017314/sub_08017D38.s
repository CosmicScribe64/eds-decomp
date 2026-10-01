	thumb_func_start sub_08017D38
sub_08017D38: @ 0x08017D38
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov ip, r0
	mov r8, r1
	add r7, r2, #0
	mov sl, r3
	mov r5, #0
	mov r1, #1
	and r1, r0
	mov r0, #0x94
	mov r4, r8
	mul r4, r0
	ldr r3, _08017DD8 @ =0x00000D64
	add r0, r1, #0
	mul r0, r3
	add r0, r4, r0
	ldr r2, _08017DDC @ =0x0201930C
	add r0, r0, r2
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r5, r0
	bge _08017DC8
	add r6, r1, #0
	mov r1, ip
	lsl r0, r1, #0x18
	lsr r0, r0, #0x18
	mov r9, r0
_08017D74:
	add r0, r6, #0
	mul r0, r3
	add r0, r4, r0
	add r0, r0, r2
	lsl r2, r5, #1
	add r1, r0, #0
	add r1, #0xA
	add r1, r1, r2
	add r0, #0x4A
	add r0, r0, r2
	ldrh r3, [r1]
	ldrb r2, [r1]
	lsr r1, r3, #8
	ldrb r0, [r0]
	cmp r0, #2
	bne _08017DB2
	cmp r2, r7
	bne _08017DB2
	cmp r1, sl
	bne _08017DB2
	mov r1, r8
	lsl r0, r1, #0x18
	mov r1, r9
	lsl r2, r1, #0x10
	orr r2, r0
	add r0, r7, #0
	add r1, r3, #0
	lsr r2, r2, #0x10
	mov r3, #2
	bl sub_08017ADC
_08017DB2:
	add r5, #1
	ldr r3, _08017DD8 @ =0x00000D64
	add r0, r6, #0
	mul r0, r3
	add r0, r4, r0
	ldr r2, _08017DDC @ =0x0201930C
	add r0, r0, r2
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r5, r0
	blt _08017D74
_08017DC8:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08017DD8: .4byte 0x00000D64
_08017DDC: .4byte 0x0201930C
	thumb_func_end sub_08017D38

