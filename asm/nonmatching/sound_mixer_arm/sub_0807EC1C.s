	arm_func_start sub_0807EC1C
sub_0807EC1C: @ 0x0807EC1C
	.syntax unified
	stmdb sp!, {lr}
	cmp r2, #0x800
	beq _0807ECB4
	mov r0, #0
_0807EC2C:
	ldrsb r1, [r5, r0]!
	mul r1, r8, r1
	asr r1, r1, #4
_0807EC38:
	subs r4, r4, #1
	bmi _0807EC80
	ldrsb r0, [r6]
	add r0, r0, r1
	strb r0, [r6], #1
	add r3, r3, r2
	lsrs r0, r3, #0xc
	beq _0807EC38
	lsl r3, r3, #0x14
	lsr r3, r3, #0x14
	subs ip, ip, r0
	bgt _0807EC2C
	sub r1, pc, #0x48
_0807EC6C:
	ldrb r0, [r7, #0xe]
	lsrs r0, r0, #7
	bhs _0807EC84
	mov r0, #0
	strb r0, [r7, #0xe]
_0807EC80:
	ldm sp!, {pc}
_0807EC84:
	ldr r5, [pc, #0x64]
	ldrsh r0, [r7, #0xc]
	lsls r0, r0, #0x11
	ldrhs r5, [pc, #0x5c]
	add r5, r5, r0, lsr #15
	ldr r5, [r5]
	ldr ip, [r5, #4]
	ldr r0, [r5, #8]
	sub ip, ip, r0
	add r5, r5, #0xc
	add r5, r5, r0
	bx r1
_0807ECB4:
	subs r4, r4, #2
	bmi _0807EC80
	ldrsb r1, [r5], #1
	mul r1, r8, r1
	asr r1, r1, #4
	ldrsb r0, [r6]
	add r0, r0, r1
	strb r0, [r6], #1
	ldrsb r0, [r6]
	add r0, r0, r1
	strb r0, [r6], #1
	subs ip, ip, #1
	bgt _0807ECB4
	sub r1, pc, #0x3c
	b _0807EC6C
	.4byte 0x0811B420
	.4byte 0x08088A20
	.syntax divided
	arm_func_end sub_0807EC1C
