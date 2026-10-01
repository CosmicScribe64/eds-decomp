	thumb_func_start sub_0800A004
sub_0800A004: @ 0x0800A004
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	add r5, r0, #0
	add r7, r1, #0
	mov r4, #0
	ldr r2, _0800A080 @ =0x020192E4
	mov r0, #1
	and r0, r5
	ldr r1, _0800A084 @ =0x00000D64
	mul r0, r1
	add r1, r0, r2
	ldrb r3, [r1, #2]
	cmp r4, r3
	bge _0800A09A
	add r6, r0, #0
	ldr r0, _0800A088 @ =0x00000684
	add r0, r0, r2
	mov ip, r0
	mov r8, r2
	add r2, r1, #0
	mov r3, #0
_0800A032:
	mov r1, ip
	add r0, r6, r1
	add r0, r0, r3
	ldr r1, [r0]
	ldr r0, [r7]
	cmp r1, r0
	bne _0800A090
	ldrb r0, [r2, #2]
	sub r0, #1
	strb r0, [r2, #2]
	add r6, r4, #0
	cmp r4, r0
	bge _0800A07A
	mov r0, #1
	and r0, r5
	ldr r1, _0800A084 @ =0x00000D64
	mul r0, r1
	ldr r1, _0800A08C @ =0x02019968
	mov r5, r8
	add r2, r0, r5
	add r5, r3, #4
	add r7, r0, r1
	lsl r0, r4, #2
	add r4, r0, r7
_0800A062:
	add r1, r7, r5
	add r0, r4, #0
	str r2, [sp, #0]
	bl sub_08007558
	add r5, #4
	add r4, #4
	add r6, #1
	ldr r2, [sp, #0]
	ldrb r0, [r2, #2]
	cmp r6, r0
	blt _0800A062
_0800A07A:
	mov r0, #1
	b _0800A09C
	.align 2, 0
_0800A080: .4byte 0x020192E4
_0800A084: .4byte 0x00000D64
_0800A088: .4byte 0x00000684
_0800A08C: .4byte 0x02019968
_0800A090:
	add r3, #4
	add r4, #1
	ldrb r1, [r2, #2]
	cmp r4, r1
	blt _0800A032
_0800A09A:
	mov r0, #0
_0800A09C:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0800A004

