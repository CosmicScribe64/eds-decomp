	thumb_func_start sub_08078FD4
sub_08078FD4: @ 0x08078FD4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r4, r1, #0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r2, r2, #0x10
	lsr r7, r2, #0x10
	lsl r3, r3, #0x10
	lsr r6, r3, #0x10
	bl sub_08072584
	lsl r0, r0, #3
	ldr r1, _08079064 @ =0x081C0000
	add r5, r0, r1
	mov r0, #0xF
	mov r9, r0
	mov r1, #3
	mov r8, r1
_08078FFC:
	ldrh r1, [r5]
	lsr r0, r1, #4
	mov r1, r9
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl sub_080725B0
	strh r0, [r4]
	add r4, #2
	mov r0, #0xF
	ldrh r1, [r5]
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl sub_080725B0
	strh r0, [r4]
	add r4, #2
	ldrh r1, [r5]
	lsr r0, r1, #0xC
	mov r1, r9
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl sub_080725B0
	strh r0, [r4]
	add r4, #2
	ldrh r1, [r5]
	lsr r0, r1, #8
	mov r1, r9
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl sub_080725B0
	strh r0, [r4]
	add r4, #2
	add r5, #2
	mov r0, #1
	neg r0, r0
	add r8, r0
	mov r1, r8
	cmp r1, #0
	bge _08078FFC
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08079064: .4byte gUnk_081C0000
	thumb_func_end sub_08078FD4

