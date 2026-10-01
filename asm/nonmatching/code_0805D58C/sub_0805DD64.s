	thumb_func_start sub_0805DD64
sub_0805DD64: @ 0x0805DD64
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	add r5, r2, #0
	ldr r0, _0805DD88 @ =0x00002020
	mov r8, r0
	add r6, #0x14
	cmp r5, #0
	bne _0805DD8C
	lsl r0, r1, #0x10
	orr r0, r6
	mov r1, #0
	mov r2, r8
	bl sub_080761F0
	b _0805DDBA
	.align 2, 0
_0805DD88: .4byte 0x00002020
_0805DD8C:
	lsl r7, r1, #0x10
_0805DD8E:
	add r4, r6, #0
	orr r4, r7
	add r0, r5, #0
	mov r1, #0xA
	bl __modsi3
	add r2, r0, #0
	add r2, r8
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r4, #0
	mov r1, #0
	bl sub_080761F0
	add r0, r5, #0
	mov r1, #0xA
	bl __divsi3
	add r5, r0, #0
	sub r6, #4
	cmp r5, #0
	bne _0805DD8E
_0805DDBA:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_0805DD64

