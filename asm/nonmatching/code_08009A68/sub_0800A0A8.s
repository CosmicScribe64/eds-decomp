	thumb_func_start sub_0800A0A8
sub_0800A0A8: @ 0x0800A0A8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	str r0, [sp, #0]
	mov r3, #0
	ldr r0, _0800A12C @ =0x020192E4
	mov r1, #1
	ldr r2, [sp, #0]
	and r1, r2
	ldr r2, _0800A130 @ =0x00000D64
	mul r1, r2
	add r2, r1, r0
	add r4, r0, #0
	ldrb r0, [r2, #2]
	cmp r3, r0
	bge _0800A148
	mov sl, r1
	mov r8, r2
	str r1, [sp, #4]
	mov r9, r3
_0800A0D6:
	ldr r0, _0800A134 @ =0x02019968
	add r0, sl
	add r0, r9
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0800A138
	mov r2, r8
	ldrb r0, [r2, #2]
	sub r0, #1
	strb r0, [r2, #2]
	add r6, r3, #0
	cmp r3, r0
	bge _0800A13E
	mov r0, #1
	ldr r1, [sp, #0]
	and r0, r1
	ldr r1, _0800A130 @ =0x00000D64
	mul r0, r1
	ldr r1, _0800A134 @ =0x02019968
	ldr r4, _0800A12C @ =0x020192E4
	add r2, r0, r4
	mov r5, r9
	add r5, #4
	add r7, r0, r1
	lsl r0, r3, #2
	add r4, r0, r7
_0800A10C:
	add r1, r7, r5
	add r0, r4, #0
	str r2, [sp, #8]
	str r3, [sp, #0xC]
	bl sub_08007558
	add r5, #4
	add r4, #4
	add r6, #1
	ldr r2, [sp, #8]
	ldr r3, [sp, #0xC]
	ldrb r0, [r2, #2]
	cmp r6, r0
	blt _0800A10C
	ldr r4, _0800A12C @ =0x020192E4
	b _0800A13E
_0800A12C: .4byte 0x020192E4
_0800A130: .4byte 0x00000D64
_0800A134: .4byte 0x02019968
_0800A138:
	mov r1, #4
	add r9, r1
	add r3, #1
_0800A13E:
	ldr r2, [sp, #4]
	add r0, r2, r4
	ldrb r0, [r0, #2]
	cmp r3, r0
	blt _0800A0D6
_0800A148:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_0800A0A8

