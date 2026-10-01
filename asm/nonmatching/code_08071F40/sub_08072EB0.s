	thumb_func_start sub_08072EB0
sub_08072EB0: @ 0x08072EB0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov ip, r3
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0]
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov sl, r2
	mov r0, ip
	str r0, [sp, #4]
	ldrh r1, [r0]
	lsl r0, r1, #1
	add r1, r0, #0
	add r1, #8
	ldr r2, [sp, #4]
	add r1, r1, r2
	mov r9, r1
	add r0, #0x10
	add r2, r2, r0
	mov r3, sl
	lsl r0, r3, #5
	ldr r7, _08072FA4 @ =0x06004000
	add r5, r0, r7
	ldrh r1, [r1]
	lsl r0, r1, #6
	add r0, r0, r2
	mov r8, r0
	mov r4, r8
	add r4, #8
	mov r3, #0
	cmp r1, #0
	beq _08072F38
_08072EFE:
	ldrh r0, [r2]
	add r1, r0, #0
	mov r7, #0xFF
	lsl r7, r7, #8
	and r0, r7
	cmp r0, #0
	beq _08072F14
	lsl r0, r6, #8
	add r0, r1, r0
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_08072F14:
	mov r0, #0xFF
	and r0, r1
	cmp r0, #0
	beq _08072F22
	add r0, r1, r6
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_08072F22:
	strh r1, [r5]
	add r5, #2
	add r2, #2
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	mov r1, r9
	ldrh r1, [r1]
	lsl r0, r1, #5
	cmp r3, r0
	blt _08072EFE
_08072F38:
	lsl r0, r6, #1
	mov r1, #0xA0
	lsl r1, r1, #0x13
	add r0, r0, r1
	mov r1, ip
	add r1, #8
	ldr r3, [sp, #4]
	ldrh r3, [r3]
	lsl r2, r3, #1
	bl sub_08075294
	mov r3, #0
	mov r5, r8
	ldrh r5, [r5]
	cmp r3, r5
	bcs _08072F90
	mov r6, #0xFF
	lsl r6, r6, #8
	ldr r5, _08072FA8 @ =0x03000C5C
_08072F5E:
	ldrh r1, [r4]
	add r4, #2
	ldrh r2, [r4]
	add r4, #2
	mov r0, #0x3F
	and r0, r1
	and r1, r6
	lsr r1, r1, #3
	orr r0, r1
	ldr r7, [sp, #0]
	add r0, r0, r7
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	add r0, r0, r5
	mov r7, sl
	lsr r1, r7, #1
	add r2, r2, r1
	strh r2, [r0]
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	mov r0, r8
	ldrh r0, [r0]
	cmp r3, r0
	bcc _08072F5E
_08072F90:
	mov r1, r9
	ldrh r0, [r1]
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08072FA4: .4byte 0x06004000
_08072FA8: .4byte 0x03000C5C
	thumb_func_end sub_08072EB0

