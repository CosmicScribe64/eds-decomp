	thumb_func_start sub_0807A960
sub_0807A960: @ 0x0807A960
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r7, r0, #0
	add r6, r1, #0
	ldr r0, [sp, #0x20]
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r8, r3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov sl, r0
	mov r4, #0
	cmp r4, r8
	bcs _0807A9AC
	lsl r5, r2, #1
	lsr r0, r5, #1
	mov r9, r0
_0807A98C:
	add r0, r7, #0
	add r1, r6, #0
	mov r2, r9
	ldr r3, _0807A9BC @ =0x001FFFFF
	and r2, r3
	bl CpuSet
	add r7, r7, r5
	mov r1, sl
	lsl r0, r1, #1
	add r6, r6, r0
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, r8
	bcc _0807A98C
_0807A9AC:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807A9BC: .4byte 0x001FFFFF
	thumb_func_end sub_0807A960

