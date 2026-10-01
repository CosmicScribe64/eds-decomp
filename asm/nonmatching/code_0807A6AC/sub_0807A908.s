	thumb_func_start sub_0807A908
sub_0807A908: @ 0x0807A908
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r6, r0, #0
	add r5, r1, #0
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r8, r3
	mov r4, #0
	cmp r4, r8
	bcs _0807A94C
	lsl r7, r2, #1
	lsr r0, r7, #1
	mov r9, r0
	ldr r3, _0807A95C @ =0x001FFFFF
	mov sl, r3
_0807A930:
	add r0, r6, #0
	add r1, r5, #0
	mov r2, r9
	mov r3, sl
	and r2, r3
	bl CpuSet
	add r6, r6, r7
	add r5, #0x40
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, r8
	bcc _0807A930
_0807A94C:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807A95C: .4byte 0x001FFFFF
	thumb_func_end sub_0807A908

