	arm_func_start SoundMixAll
SoundMixAll: @ 0x0807EAD0
	.syntax unified
	push {r4, r5, r6, r7, r8, sb, sl, fp, ip, lr}
	ldr r7, [pc, #0x128]
	mov r1, #0
	bl SoundMixFifo
	mov r1, #4
	bl SoundMixFifo
	pop {r4, r5, r6, r7, r8, sb, sl, fp, ip, lr}
	bx lr
	.syntax divided
	arm_func_end SoundMixAll
