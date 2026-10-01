	arm_func_start sub_0807EAD0
sub_0807EAD0: @ 0x0807EAD0
	.syntax unified
	push {r4, r5, r6, r7, r8, sb, sl, fp, ip, lr}
	ldr r7, [pc, #0x128]
	mov r1, #0
	bl sub_0807EAF0
	mov r1, #4
	bl sub_0807EAF0
	pop {r4, r5, r6, r7, r8, sb, sl, fp, ip, lr}
	bx lr
	.syntax divided
	arm_func_end sub_0807EAD0
