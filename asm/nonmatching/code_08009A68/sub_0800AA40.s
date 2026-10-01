	thumb_func_start sub_0800AA40
sub_0800AA40: @ 0x0800AA40
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov ip, r2
	mov r5, #0
	mov r2, #1
	and r2, r0
	mov r0, #0x94
	mul r1, r0
	ldr r7, _0800AAB8 @ =0x00000D64
	mul r2, r7
	add r0, r1, r2
	ldr r6, _0800AABC @ =0x0201930C
	add r3, r0, r6
	add r0, r3, #0
	add r0, #0x8A
	ldrh r4, [r0]
	cmp r5, r4
	bge _0800AAF4
	mov r9, r6
	str r3, [sp, #0]
	add r0, r2, #0
	add r0, #0x4A
	add r0, r1, r0
	add r6, r0, r6
	ldr r0, _0800AAC0 @ =0x000007FF
	mov r8, r0
	mov sl, r4
_0800AA82:
	lsl r1, r5, #1
	ldr r0, [sp, #0]
	add r0, #0xA
	add r0, r0, r1
	ldrb r3, [r6]
	ldrh r4, [r0]
	lsr r2, r4, #8
	mov r1, #1
	ldrb r0, [r0]
	and r1, r0
	mov r7, #0x94
	add r0, r2, #0
	mul r0, r7
	ldr r2, _0800AAB8 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	add r0, r9
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r3, #1
	blt _0800AAEC
	cmp r3, #2
	ble _0800AAC4
	cmp r3, #3
	beq _0800AAD4
	b _0800AAEC
_0800AAB8: .4byte 0x00000D64
_0800AABC: .4byte 0x0201930C
_0800AAC0: .4byte 0x000007FF
_0800AAC4:
	mov r7, r8
	and r0, r7
	lsl r0, r0, #1
	ldr r1, _0800AAD0 @ =0x08622AB4
	add r0, r0, r1
	b _0800AADE
_0800AAD0: .4byte gUnk_08622AB4
_0800AAD4:
	mov r2, r8
	and r4, r2
	lsl r0, r4, #1
	ldr r7, _0800AAE8 @ =0x08622AB4
	add r0, r0, r7
_0800AADE:
	ldrh r0, [r0]
	cmp r0, ip
	bne _0800AAEC
	add r0, r5, #0
	b _0800AAF8
_0800AAE8: .4byte gUnk_08622AB4
_0800AAEC:
	add r6, #2
	add r5, #1
	cmp r5, sl
	blt _0800AA82
_0800AAF4:
	mov r0, #1
	neg r0, r0
_0800AAF8:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0800AA40

