	thumb_func_start QuickSortS16
QuickSortS16: @ 0x080690C4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	add r7, r1, #0
	mov sl, r2
	mov r1, #0
	ldr r2, _08069168 @ =0x02030000
	strh r1, [r2]
	sub r0, #1
	strh r0, [r2, #2]
	mov r3, #1
	str r3, [sp, #4]
_080690E2:
	ldr r4, [sp, #4]
	lsl r0, r4, #0x10
	ldr r1, _0806916C @ =0xFFFF0000
	add r0, r0, r1
	lsr r2, r0, #0x10
	str r2, [sp, #4]
	asr r0, r0, #0xE
	ldr r3, _08069168 @ =0x02030000
	add r0, r0, r3
	ldrh r4, [r0]
	str r4, [sp, #0]
	ldrh r0, [r0, #2]
	mov r8, r0
	lsl r6, r4, #0x10
	asr r2, r6, #0x10
	lsl r4, r0, #0x10
	asr r3, r4, #0x10
	cmp r2, r3
	blt _0806910A
	b _0806926A
_0806910A:
	sub r0, r3, r2
	cmp r0, #0x14
	ble _08069204
	sub r0, r2, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r1, r2, r3
	lsr r0, r1, #0x1F
	add r1, r1, r0
	asr r1, r1, #1
	lsl r1, r1, #1
	add r1, r1, r7
	lsl r2, r2, #1
	add r2, r2, r7
	lsl r0, r3, #1
	add r0, r0, r7
	ldrh r3, [r0]
	ldrh r1, [r1]
	lsl r1, r1, #0x10
	ldrh r2, [r2]
	lsl r0, r2, #0x10
	cmp r1, r0
	bge _08069142
	lsr r1, r1, #0x10
	mov r9, r1
_08069142:
	lsr r6, r0, #0x10
	mov r1, r9
	lsl r0, r1, #0x10
	lsr r2, r0, #0x10
	lsl r1, r6, #0x10
	lsl r0, r3, #0x10
	cmp r1, r0
	ble _08069164
	lsr r1, r1, #0x10
	mov r9, r1
	add r1, r0, #0
	lsl r0, r2, #0x10
	cmp r1, r0
	ble _08069162
	lsr r1, r1, #0x10
	mov r9, r1
_08069162:
	lsr r6, r0, #0x10
_08069164:
	lsl r6, r6, #0x10
	b _08069182
_08069168: .4byte 0x02030000
_0806916C: .4byte 0xFFFF0000
_08069170:
	lsl r2, r3, #1
	add r2, r2, r7
	ldrh r3, [r2]
	mov r9, r3
	lsl r1, r1, #1
	add r1, r1, r7
	ldrh r0, [r1]
	strh r0, [r2]
	strh r3, [r1]
_08069182:
	lsl r0, r5, #0x10
	mov r1, #0x80
	lsl r1, r1, #9
	add r0, r0, r1
	lsr r5, r0, #0x10
	asr r0, r0, #0xF
	add r0, r0, r7
	mov r2, #0
	ldsh r0, [r0, r2]
	asr r1, r6, #0x10
	bl _call_via_sl
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08069182
_080691A0:
	lsl r0, r4, #0x10
	ldr r3, _080691FC @ =0xFFFF0000
	add r0, r0, r3
	lsr r4, r0, #0x10
	asr r0, r0, #0xF
	add r0, r0, r7
	mov r2, #0
	ldsh r1, [r0, r2]
	asr r0, r6, #0x10
	bl _call_via_sl
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _080691A0
	lsl r0, r5, #0x10
	asr r3, r0, #0x10
	lsl r0, r4, #0x10
	asr r1, r0, #0x10
	cmp r3, r1
	blt _08069170
	ldr r4, [sp, #4]
	lsl r0, r4, #0x10
	asr r0, r0, #0x10
	lsl r2, r0, #2
	ldr r4, _08069200 @ =0x02030000
	add r2, r2, r4
	add r1, #1
	strh r1, [r2]
	mov r1, r8
	strh r1, [r2, #2]
	add r0, #1
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	lsl r2, r0, #2
	add r2, r2, r4
	mov r4, sp
	ldrh r4, [r4]
	strh r4, [r2]
	sub r1, r3, #1
	strh r1, [r2, #2]
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #4]
	b _0806926A
	.align 2, 0
_080691FC: .4byte 0xFFFF0000
_08069200: .4byte 0x02030000
_08069204:
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	lsl r1, r5, #0x10
	asr r0, r1, #0x10
	cmp r0, r3
	bgt _0806926A
	str r6, [sp, #0x10]
	mov r8, r4
_08069216:
	asr r1, r1, #0x10
	lsl r0, r1, #1
	add r0, r0, r7
	ldrh r6, [r0]
	sub r1, #1
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	lsl r2, r5, #0x10
	b _08069232
_08069228:
	ldrh r0, [r4]
	strh r0, [r4, #2]
	sub r0, r5, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
_08069232:
	lsl r0, r4, #0x10
	asr r5, r0, #0x10
	ldr r1, [sp, #0x10]
	cmp r0, r1
	blt _08069256
	lsl r0, r6, #0x10
	asr r0, r0, #0x10
	lsl r1, r5, #1
	add r4, r1, r7
	mov r3, #0
	ldsh r1, [r4, r3]
	str r2, [sp, #8]
	bl _call_via_sl
	lsl r0, r0, #0x10
	ldr r2, [sp, #8]
	cmp r0, #0
	bne _08069228
_08069256:
	lsl r0, r5, #1
	add r0, r0, r7
	strh r6, [r0, #2]
	mov r4, #0x80
	lsl r4, r4, #9
	add r0, r2, r4
	lsr r5, r0, #0x10
	lsl r1, r5, #0x10
	cmp r1, r8
	ble _08069216
_0806926A:
	ldr r1, [sp, #4]
	lsl r0, r1, #0x10
	cmp r0, #0
	ble _08069274
	b _080690E2
_08069274:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end QuickSortS16

