	thumb_func_start DuelCursor_IsValidTarget
DuelCursor_IsValidTarget: @ 0x08052908
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	add r6, r0, #0
	add r4, r1, #0
	str r2, [sp, #0]
	mov r0, #1
	mov sl, r0
	add r0, r6, #0
	mov r1, sl
	and r0, r1
	ldr r2, _08052964 @ =0x00000D64
	mov ip, r2
	mov r5, ip
	mul r5, r0
	ldr r0, _08052968 @ =0x0201930C
	mov r9, r0
	add r2, r5, r0
	ldr r0, [sp, #0]
	add r1, r4, r0
	mov r0, #0x94
	mul r0, r1
	add r2, r2, r0
	ldr r0, [r2]
	lsl r7, r0, #0x14
	lsl r0, r7, #1
	lsr r0, r0, #0x13
	ldr r1, _0805296C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	mov r8, r0
	cmp r4, #5
	beq _08052A1A
	cmp r4, #5
	bgt _08052970
	cmp r4, #0
	beq _0805299C
	b _08052B64
	.align 2, 0
_08052964: .4byte 0x00000D64
_08052968: .4byte 0x0201930C
_0805296C: .4byte gCardStats
_08052970:
	cmp r4, #0xA
	bne _08052976
	b _08052B20
_08052976:
	cmp r4, #0xB
	beq _0805297C
	b _08052B64
_0805297C:
	lsl r1, r6, #4
	mov r0, sl
	lsl r0, r1
	and r0, r3
	cmp r0, #0
	bne _0805298A
	b _08052B64
_0805298A:
	mov r0, r9
	sub r0, #0x28
	add r0, r5, r0
	ldr r2, [sp, #0]
	ldrb r0, [r0, #2]
	cmp r2, r0
	bge _0805299A
	b _08052B60
_0805299A:
	b _08052B64
_0805299C:
	lsl r1, r6, #4
	mov r0, #0xF0
	lsl r0, r1
	and r0, r3
	cmp r0, #0
	bne _080529AA
	b _08052B64
_080529AA:
	mov r5, #0
	mov r4, #0
	cmp r7, #0
	bne _080529B4
	b _08052B64
_080529B4:
	mov r0, #0x20
	lsl r0, r1
	and r0, r3
	cmp r0, #0
	beq _080529CA
	mov r0, #2
	ldrb r6, [r2, #6]
	and r0, r6
	cmp r0, #0
	beq _080529CA
	mov r5, #1
_080529CA:
	mov r0, #0x10
	lsl r0, r1
	and r0, r3
	cmp r0, #0
	beq _080529E0
	mov r0, #2
	ldrb r6, [r2, #6]
	and r0, r6
	cmp r0, #0
	bne _080529E0
	mov r5, #1
_080529E0:
	mov r0, #0x80
	lsl r0, r1
	and r0, r3
	cmp r0, #0
	beq _080529F6
	mov r0, #1
	ldrb r6, [r2, #6]
	and r0, r6
	cmp r0, #0
	beq _080529F6
	mov r4, #1
_080529F6:
	mov r0, #0x40
	lsl r0, r1
	and r0, r3
	cmp r0, #0
	beq _08052A0C
	mov r0, #1
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _08052A0C
	mov r4, #1
_08052A0C:
	cmp r4, #0
	bne _08052A12
	b _08052B64
_08052A12:
	cmp r5, #0
	beq _08052A18
	b _08052B60
_08052A18:
	b _08052B64
_08052A1A:
	lsl r4, r6, #4
	mov r5, #0xE
	lsl r5, r4
	and r5, r3
	cmp r5, #0
	bne _08052A28
	b _08052B64
_08052A28:
	cmp r7, #0
	bne _08052A2E
	b _08052B64
_08052A2E:
	mov r1, #2
	add r0, r1, #0
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _08052A48
	add r0, r1, #0
_08052A3C:
	lsl r0, r4
	and r0, r3
	cmp r0, #0
	beq _08052A46
	b _08052B60
_08052A46:
	b _08052B64
_08052A48:
	add r0, r1, #0
	lsl r0, r4
	cmp r5, r0
	bne _08052A52
	b _08052B64
_08052A52:
	mov r0, r8
	cmp r0, #0x15
	beq _08052A60
	cmp r0, #0x16
	bne _08052A64
	mov r0, #4
	b _08052A3C
_08052A60:
	mov r0, #8
	b _08052A3C
_08052A64:
	mov r1, #0
	mov sl, r1
	lsl r0, r6, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #4]
_08052A6E:
	mov r2, #0
	mov r9, r2
	mov r0, sl
	mov r3, #1
	and r0, r3
	ldr r6, _08052B18 @ =0x00000D64
	add r1, r0, #0
	mul r1, r6
	mov r8, r2
	ldr r0, _08052B1C @ =0x0201930C
	add r0, #0x8A
	add r7, r1, r0
	ldr r0, _08052B1C @ =0x0201930C
	add r5, r1, r0
_08052A8A:
	ldr r2, _08052B1C @ =0x0201930C
	ldr r0, [r5]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08052AFC
	mov r0, #2
	ldrb r1, [r5, #6]
	and r0, r1
	cmp r0, #0
	beq _08052AFC
	mov r4, #0
	ldrh r3, [r7]
	cmp r4, r3
	bge _08052AFC
	mov r0, #1
	mov r6, sl
	and r0, r6
	ldr r1, _08052B18 @ =0x00000D64
	mul r0, r1
	mov r3, r8
	add r1, r3, r0
	add r6, r1, r2
	mov ip, r6
	add r0, #0x4A
	add r0, r8
	add r3, r0, r2
	ldr r0, [sp, #0]
	add r0, #5
	lsl r0, r0, #0x18
	lsr r2, r0, #0x10
	ldr r0, [sp, #4]
	orr r2, r0
	add r6, r1, #0
_08052ACC:
	lsl r1, r4, #1
	mov r0, ip
	add r0, #0xA
	add r0, r0, r1
	ldrh r1, [r0]
	ldrh r0, [r3]
	cmp r0, #1
	beq _08052AE8
	cmp r0, #1
	blt _08052AEC
	cmp r0, #6
	bgt _08052AEC
	cmp r0, #5
	blt _08052AEC
_08052AE8:
	cmp r1, r2
	beq _08052B60
_08052AEC:
	add r3, #2
	add r4, #1
	ldr r1, _08052B1C @ =0x0201930C
	add r0, r6, r1
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r4, r0
	blt _08052ACC
_08052AFC:
	mov r2, #0x94
	add r8, r2
	add r7, #0x94
	add r5, #0x94
	mov r3, #1
	add r9, r3
	mov r6, r9
	cmp r6, #4
	ble _08052A8A
	add sl, r3
	mov r0, sl
	cmp r0, #1
	ble _08052A6E
	b _08052B64
_08052B18: .4byte 0x00000D64
_08052B1C: .4byte 0x0201930C
_08052B20:
	mov r0, #0xB9
	lsl r0, r0, #3
	add r0, r9
	add r2, r5, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08052B64
	mov r4, #0
	mov r5, #2
	add r0, r5, #0
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _08052B4E
	lsl r0, r6, #4
	mov r1, #4
	lsl r1, r0
	and r1, r3
	neg r0, r1
	orr r0, r1
	lsr r4, r0, #0x1F
	b _08052B5C
_08052B4E:
	lsl r1, r6, #4
	add r0, r5, #0
	lsl r0, r1
	and r0, r3
	cmp r0, #0
	beq _08052B5C
	mov r4, #1
_08052B5C:
	cmp r4, #0
	beq _08052B64
_08052B60:
	mov r0, #1
	b _08052B66
_08052B64:
	mov r0, #0
_08052B66:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end DuelCursor_IsValidTarget
	.align 2, 0

