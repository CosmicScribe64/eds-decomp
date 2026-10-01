	thumb_func_start sub_08046808
sub_08046808: @ 0x08046808
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov sl, r0
	mov r4, #0xA4
	lsl r4, r4, #1
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bgt _08046832
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	ble _080468FA
_08046832:
	mov r2, #0
	mov r5, #0
	mov r0, #1
	mov r9, r0
	ldr r1, _0804690C @ =0x00000D64
	mov r8, r1
_0804683E:
	mov r4, #0
	add r6, r5, #1
	add r0, r5, #0
	mov r1, r9
	and r0, r1
	mov r7, r8
	mul r7, r0
_0804684C:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r7
	ldr r1, _08046910 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0804687A
	mov r0, #3
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #2
	bne _0804687A
	add r0, r5, #0
	add r1, r4, #0
	str r2, [sp, #0]
	bl sub_0800C8BC
	ldr r2, [sp, #0]
	cmp r0, #1
	bne _0804687A
	mov r2, #1
_0804687A:
	add r4, #1
	cmp r4, #4
	ble _0804684C
	add r5, r6, #0
	cmp r5, #1
	ble _0804683E
	cmp r2, #0
	beq _080468FA
	mov r2, #0x73
	mov r0, sl
	cmp r0, #0
	beq _08046894
	ldr r2, _08046914 @ =0x00008073
_08046894:
	ldr r0, _08046918 @ =0x08624084
	ldrh r1, [r0]
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	mov r5, #0
	mov r1, #1
	mov r9, r1
	ldr r0, _0804690C @ =0x00000D64
	mov r8, r0
_080468AC:
	mov r4, #0
	add r6, r5, #1
	add r0, r5, #0
	mov r1, r9
	and r0, r1
	mov r7, r8
	mul r7, r0
_080468BA:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r7
	ldr r1, _08046910 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080468EE
	mov r0, #3
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #2
	bne _080468EE
	add r0, r5, #0
	add r1, r4, #0
	bl sub_0800C8BC
	cmp r0, #1
	bne _080468EE
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #0
	mov r3, #0
	bl sub_08018ED8
_080468EE:
	add r4, #1
	cmp r4, #4
	ble _080468BA
	add r5, r6, #0
	cmp r5, #1
	ble _080468AC
_080468FA:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0804690C: .4byte 0x00000D64
_08046910: .4byte 0x0201930C
_08046914: .4byte 0x00008073
_08046918: .4byte gUnk_08624084
	thumb_func_end sub_08046808

