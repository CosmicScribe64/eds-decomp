	thumb_func_start sub_0807E9C8
sub_0807E9C8: @ 0x0807E9C8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r3, r0, #0
	add r4, r1, #0
	ldr r5, _0807EA10 @ =0x03005210
	mov r0, #0xC4
	lsl r0, r0, #1
	add r2, r5, r0
	ldrh r1, [r2]
	mov r6, #0x80
	lsl r6, r6, #6
	add r0, r6, #0
	mov r6, #0
	mov r8, r6
	orr r0, r1
	strh r0, [r2]
	ldr r0, _0807EA14 @ =0x00000193
	add r7, r5, r0
	ldrb r6, [r7]
	cmp r3, #0
	blt _0807EA1E
	add r0, r3, #0
	bl sub_0807E674
	bl sub_0807E554
	mov r1, r8
	strb r1, [r7]
	mov r6, #0xC8
	lsl r6, r6, #1
	add r1, r5, r6
	mov r0, #0
	strh r0, [r1]
	mov r6, #0x10
	b _0807EA1E
_0807EA10: .4byte 0x03005210
_0807EA14: .4byte 0x00000193
_0807EA18:
	add r0, r5, #0
	bl sub_0807DB58
_0807EA1E:
	sub r4, #1
	cmp r4, #0
	bge _0807EA18
	ldr r1, _0807EA44 @ =0x00000193
	add r0, r5, r1
	strb r6, [r0]
	mov r6, #0xC4
	lsl r6, r6, #1
	add r2, r5, r6
	ldrh r1, [r2]
	ldr r0, _0807EA48 @ =0x0000DFFF
	and r0, r1
	strh r0, [r2]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807EA44: .4byte 0x00000193
_0807EA48: .4byte 0x0000DFFF
	thumb_func_end sub_0807E9C8

