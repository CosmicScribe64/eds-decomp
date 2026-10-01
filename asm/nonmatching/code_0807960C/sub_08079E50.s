	thumb_func_start sub_08079E50
sub_08079E50: @ 0x08079E50
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0x14
	mov r8, r0
	mov r9, r3
	ldr r0, [sp, #0x30]
	ldr r3, [sp, #0x34]
	ldr r4, [sp, #0x38]
	ldr r5, [sp, #0x3C]
	ldr r6, [sp, #0x40]
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov ip, r2
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	lsl r5, r5, #0x10
	lsr r5, r5, #0x10
	lsl r6, r6, #0x18
	lsr r6, r6, #0x18
	ldr r1, _08079EAC @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _08079EB0
	str r2, [sp, #0]
	str r3, [sp, #4]
	str r4, [sp, #8]
	str r5, [sp, #0xC]
	str r6, [sp, #0x10]
	mov r0, r8
	add r1, r7, #0
	mov r2, ip
	mov r3, r9
	bl sub_08079D88
	b _08079EC6
	.align 2, 0
_08079EAC: .4byte 0x02011C20
_08079EB0:
	str r2, [sp, #0]
	str r3, [sp, #4]
	str r4, [sp, #8]
	str r5, [sp, #0xC]
	str r6, [sp, #0x10]
	mov r0, r8
	add r1, r7, #0
	mov r2, ip
	mov r3, r9
	bl sub_08079CC8
_08079EC6:
	add sp, #0x14
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08079E50

