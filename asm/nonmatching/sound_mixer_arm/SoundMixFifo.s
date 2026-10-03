	arm_func_start SoundMixFifo
SoundMixFifo: @ 0x0807EAF0
	.syntax unified
	stmdb sp!, {lr}
	sub sp, sp, #0xc
	ldr r6, [pc, #0x108]
	ldrh r2, [r6, r1]!
	ldrh r3, [r6, #2]
	strh r2, [r6, #2]
	mov sb, #0
	str sb, [sp, #8]
	ldr r6, [pc, #0xf4]
	mov r0, #0xc8
	mul r1, r0, r1
	add sl, r6, r1
	add r6, sl, r3
	subs r4, r2, r3
	bgt _0807EB64
	movs sb, r2
	beq _0807EB60
	ldr r2, [pc, #0xd4]
	add r0, sp, #8
	str r0, [r2]
	str sl, [r2, #4]
	mov r0, #0x85000000
	orr r0, r0, sb, lsr #2
	str r0, [r2, #8]
	ldr r0, [r2, #8]
_0807EB54:
	ldr r0, [r2, #8]
	lsls r0, r0, #1
	bhs _0807EB54
_0807EB60:
	rsb r4, r3, #0x2c0
_0807EB64:
	ldr r2, [pc, #0xa4]
	add r0, sp, #8
	str r0, [r2]
	str r6, [r2, #4]
	mov r0, #0x85000000
	orr r0, r0, r4, lsr #2
	str r0, [r2, #8]
	str r4, [sp]
	str r6, [sp, #4]
_0807EB88:
	ldr r0, [r2, #8]
	lsls r0, r0, #1
	bhs _0807EB88
	mov fp, #3
_0807EB98:
	ldrh r1, [r7, #0xe]
	lsrs r1, r1, #8
	blo _0807EBF0
	beq _0807EBF0
	add r8, r1, #1
	ldr r5, [r7]
	ldr ip, [r7, #4]
	ldrh r2, [r7, #8]
	ldrh r3, [r7, #0xa]
	ldr r0, [pc, #0x50]
	mov lr, pc
	bx r0
	mov r6, sl
	mov r4, sb
	ldr r0, [pc, #0x40]
	mov lr, pc
	bx r0
	str r5, [r7]
	str ip, [r7, #4]
	strh r3, [r7, #0xa]
	ldr r4, [sp]
	ldr r6, [sp, #4]
_0807EBF0:
	add r7, r7, #0x10
	subs fp, fp, #1
	bne _0807EB98
	add sp, sp, #0xc
	ldm sp!, {pc}
	.4byte 0x030053AC
	.4byte 0x0300540C
	.4byte 0x03005414
	.4byte 0x040000B0
	.4byte 0x03005A54
	.4byte 0x03005A54
	.syntax divided
	arm_func_end SoundMixFifo
