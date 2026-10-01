	thumb_func_start sub_08075050
sub_08075050: @ 0x08075050
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	mov r8, r1
	add r4, r3, #0
	lsl r2, r2, #0x10
	lsr r7, r2, #0x10
	lsr r6, r2, #0x18
_08075062:
	add r0, r4, #0
	mov r1, #0xA
	bl __modsi3
	ldr r1, _08075098 @ =0xFFFF824F
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	add r1, r5, #0
	mov r2, r8
	add r3, r7, #0
	bl sub_08074C80
	sub r5, r5, r6
	add r0, r4, #0
	mov r1, #0xA
	bl __divsi3
	add r4, r0, #0
	cmp r4, #0
	bne _08075062
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08075098: .4byte 0xFFFF824F
	thumb_func_end sub_08075050

