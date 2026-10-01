	thumb_func_start sub_08017DE0
sub_08017DE0: @ 0x08017DE0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov sl, r0
	mov r8, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	str r2, [sp, #0]
	mov r7, #0
	mov r1, #1
	b _08017F34
_08017DFC:
	mov r4, #1
	mov r0, sl
	and r0, r4
	mov r3, #0x94
	mov r2, r8
	mul r2, r3
	mov r1, r9
	mul r1, r0
	add r0, r1, #0
	add r2, r2, r0
	add r2, ip
	lsl r1, r7, #1
	add r0, r2, #0
	add r0, #0xA
	add r0, r0, r1
	add r2, #0x4A
	add r2, r2, r1
	ldrb r6, [r0]
	ldrh r0, [r0]
	lsr r5, r0, #8
	add r1, r6, #0
	and r1, r4
	add r0, r5, #0
	mul r0, r3
	mov r3, r9
	mul r3, r1
	add r1, r3, #0
	add r0, r0, r1
	add r0, ip
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	mov r4, #0
	ldrb r0, [r2]
	sub r0, #1
	cmp r0, #9
	bhi _08017F1A
	lsl r0, r0, #2
	ldr r1, _08017E50 @ =0x08017E54
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08017E50: .4byte 0x08017E54
_08017E54:
	.4byte _08017E7C
	.4byte _08017E82
	.4byte _08017F1A
	.4byte _08017F1A
	.4byte _08017F18
	.4byte _08017F18
	.4byte _08017F1A
	.4byte _08017F1A
	.4byte _08017F1A
	.4byte _08017E7C
_08017E7C:
	cmp r3, #0
	beq _08017F1A
	b _08017F1E
_08017E82:
	ldr r0, _08017EA4 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #1
	ldr r1, _08017EA8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08017EAC @ =0x00000447
	cmp r1, r0
	beq _08017EBC
	cmp r1, r0
	bgt _08017EB0
	mov r0, #0xAE
	lsl r0, r0, #1
	cmp r1, r0
	beq _08017EBC
	b _08017F1A
	.align 2, 0
_08017EA4: .4byte 0x000007FF
_08017EA8: .4byte gUnk_08622AB4
_08017EAC: .4byte 0x00000447
_08017EB0:
	ldr r0, _08017EE0 @ =0x000004DC
	cmp r1, r0
	beq _08017EEC
	add r0, #0xAD
	cmp r1, r0
	bne _08017F1A
_08017EBC:
	mov r2, #1
	and r2, r6
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _08017EE4 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08017EE8 @ =0x0201930C
	add r1, r1, r0
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08017F1A
	ldr r4, [sp, #0]
	b _08017F1A
_08017EE0: .4byte 0x000004DC
_08017EE4: .4byte 0x00000D64
_08017EE8: .4byte 0x0201930C
_08017EEC:
	mov r2, #1
	and r2, r6
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _08017F10 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08017F14 @ =0x0201930C
	add r1, r1, r0
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08017F1A
	b _08017F1E
	.align 2, 0
_08017F10: .4byte 0x00000D64
_08017F14: .4byte 0x0201930C
_08017F18:
	mov r4, #1
_08017F1A:
	cmp r4, #0
	beq _08017F2E
_08017F1E:
	ldr r3, [sp, #0]
	neg r2, r3
	orr r2, r3
	lsr r2, r2, #0x1F
	add r0, r6, #0
	add r1, r5, #0
	bl sub_08018544
_08017F2E:
	add r7, #1
	mov r1, #1
	mov r0, sl
_08017F34:
	and r1, r0
	mov r0, #0x94
	mov r2, r8
	mul r2, r0
	add r0, r2, #0
	ldr r3, _08017F90 @ =0x00000D64
	mov r9, r3
	mov r2, r9
	mul r2, r1
	add r1, r2, #0
	add r0, r0, r1
	ldr r3, _08017F94 @ =0x0201930C
	mov ip, r3
	add r0, ip
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r7, r0
	bge _08017F5A
	b _08017DFC
_08017F5A:
	mov r0, r8
	cmp r0, #4
	bgt _08017F7E
	mov r7, #0
_08017F62:
	mov r4, #0
	add r5, r7, #1
_08017F66:
	add r0, r7, #0
	add r1, r4, #0
	mov r2, sl
	mov r3, r8
	bl sub_08017D38
	add r4, #1
	cmp r4, #4
	ble _08017F66
	add r7, r5, #0
	cmp r7, #1
	ble _08017F62
_08017F7E:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08017F90: .4byte 0x00000D64
_08017F94: .4byte 0x0201930C
	thumb_func_end sub_08017DE0

