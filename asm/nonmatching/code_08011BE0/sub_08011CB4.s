	thumb_func_start sub_08011CB4
sub_08011CB4: @ 0x08011CB4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	ldr r1, _08011CE4 @ =0x020185C0
	ldrh r0, [r1]
	lsr r4, r0, #0xF
	ldrh r5, [r1, #2]
	ldr r2, _08011CE8 @ =0x0000080A
	add r7, r1, r2
	ldrb r2, [r7]
	lsl r0, r2, #0x19
	lsr r2, r0, #0x19
	add r3, r1, #0
	cmp r2, #1
	bne _08011CD8
	b _08011E54
_08011CD8:
	cmp r2, #1
	bgt _08011CEC
	cmp r2, #0
	beq _08011CF4
	b _08011F18
	.align 2, 0
_08011CE4: .4byte 0x020185C0
_08011CE8: .4byte 0x0000080A
_08011CEC:
	cmp r2, #3
	bne _08011CF2
	b _08011EB8
_08011CF2:
	b _08011F18
_08011CF4:
	mov r0, #1
	mov r8, r0
	add r1, r4, #0
	and r1, r0
	mov r0, #0x94
	add r2, r5, #0
	mul r2, r0
	ldr r0, _08011D1C @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r1, _08011D20 @ =0x0201930C
	mov r9, r1
	add r6, r2, r1
	ldr r0, [r6]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _08011D28
	ldr r2, _08011D24 @ =0x0000080D
	add r1, r3, r2
	b _08011F1C
_08011D1C: .4byte 0x00000D64
_08011D20: .4byte 0x0201930C
_08011D24: .4byte 0x0000080D
_08011D28:
	add r2, r6, #0
	add r2, #0x91
	mov r0, #1
	ldrb r1, [r3, #4]
	and r0, r1
	lsl r0, r0, #3
	mov ip, r0
	mov r1, #9
	neg r1, r1
	ldrb r0, [r2]
	and r1, r0
	mov r0, ip
	orr r1, r0
	strb r1, [r2]
	ldrh r0, [r3, #4]
	cmp r0, #0
	beq _08011E18
	mov r0, #0x10
	bl sub_08077AEC
	mov r1, r8
	and r4, r1
	mov r0, #2
	neg r0, r0
	ldr r1, [sp, #0]
	and r1, r0
	orr r1, r4
	mov r3, #0x1F
	neg r3, r3
	and r1, r3
	mov r0, #0xA
	orr r1, r0
	sub r0, r5, #5
	lsl r0, r0, #0x17
	lsr r0, r0, #0x12
	ldr r2, _08011DF0 @ =0xFFFFC01F
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	cmp r5, #9
	ble _08011D84
	and r1, r3
	mov r0, #0x14
	orr r1, r0
	and r1, r2
	str r1, [sp, #0]
_08011D84:
	ldrb r2, [r6, #6]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, r8
	and r0, r1
	lsl r0, r0, #0xE
	ldr r2, _08011DF4 @ =0xFFFFBFFF
	ldr r1, [sp, #0]
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldrb r6, [r6, #6]
	lsl r0, r6, #0x1E
	lsr r0, r0, #0x1F
	mov r2, r8
	and r0, r2
	lsl r0, r0, #0xF
	ldr r2, _08011DF8 @ =0xFFFF7FFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	mov r0, sp
	ldrb r1, [r0, #1]
	mov r0, #0x40
	and r0, r1
	ldr r1, _08011DFC @ =0x0868DB94
	cmp r0, #0
	beq _08011DBE
	ldr r1, _08011E00 @ =0x0868EC38
_08011DBE:
	mov r0, sp
	mov r2, #0
	mov r3, #0
	bl sub_08024380
	ldr r1, _08011E04 @ =0x02015EE8
	mov r0, r8
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _08011DE2
	ldr r1, _08011E08 @ =0x00001AE6
	add r1, r9
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08011E0C
_08011DE2:
	mov r0, #0x80
	neg r0, r0
	ldrb r1, [r7]
	and r0, r1
	mov r1, #3
	b _08011E44
	.align 2, 0
_08011DF0: .4byte 0xFFFFC01F
_08011DF4: .4byte 0xFFFFBFFF
_08011DF8: .4byte 0xFFFF7FFF
_08011DFC: .4byte gUnk_0868DB94
_08011E00: .4byte gUnk_0868EC38
_08011E04: .4byte 0x02015EE8
_08011E08: .4byte 0x00001AE6
_08011E0C:
	mov r0, #0x80
	neg r0, r0
	ldrb r2, [r7]
	and r0, r2
	mov r1, #0xA
	b _08011E44
_08011E18:
	ldr r1, _08011E4C @ =0x02015EE8
	mov r0, r8
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _08011E32
	ldr r1, _08011E50 @ =0x00001AE6
	add r1, r9
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08011F18
_08011E32:
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
_08011E44:
	orr r0, r1
	strb r0, [r7]
	b _08011F26
	.align 2, 0
_08011E4C: .4byte 0x02015EE8
_08011E50: .4byte 0x00001AE6
_08011E54:
	and r2, r4
	mov r0, #0x94
	mul r0, r5
	ldr r1, _08011E9C @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08011EA0 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08011EA4 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08011EA8 @ =0x0000042C
	ldrh r0, [r0]
	cmp r0, r1
	bne _08011E92
	add r0, r4, #0
	add r1, r5, #0
	bl sub_08009538
	lsl r1, r0, #0x10
	lsr r0, r1, #0x10
	ldr r2, _08011EAC @ =0x0000FFFF
	cmp r0, r2
	beq _08011E92
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsr r1, r1, #0x18
	bl sub_08017314
_08011E92:
	ldr r1, _08011EB0 @ =0x020185C0
	ldr r2, _08011EB4 @ =0x0000080D
	add r1, r1, r2
	b _08011F1C
	.align 2, 0
_08011E9C: .4byte 0x00000D64
_08011EA0: .4byte 0x0201930C
_08011EA4: .4byte gUnk_08622AB4
_08011EA8: .4byte 0x0000042C
_08011EAC: .4byte 0x0000FFFF
_08011EB0: .4byte 0x020185C0
_08011EB4: .4byte 0x0000080D
_08011EB8:
	mov r0, #0x94
	mul r0, r5
	ldr r1, _08011EFC @ =0x00000D64
	mul r1, r4
	add r0, r0, r1
	ldr r1, _08011F00 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08011F04 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08011F08 @ =0x0000042C
	ldrh r0, [r0]
	cmp r0, r1
	bne _08011EF4
	add r0, r4, #0
	add r1, r5, #0
	bl sub_08009538
	lsl r1, r0, #0x10
	lsr r0, r1, #0x10
	ldr r2, _08011F0C @ =0x0000FFFF
	cmp r0, r2
	beq _08011EF4
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsr r1, r1, #0x18
	bl sub_08017314
_08011EF4:
	ldr r1, _08011F10 @ =0x020185C0
	ldr r2, _08011F14 @ =0x0000080D
	add r1, r1, r2
	b _08011F1C
_08011EFC: .4byte 0x00000D64
_08011F00: .4byte 0x0201930C
_08011F04: .4byte gUnk_08622AB4
_08011F08: .4byte 0x0000042C
_08011F0C: .4byte 0x0000FFFF
_08011F10: .4byte 0x020185C0
_08011F14: .4byte 0x0000080D
_08011F18:
	ldr r0, _08011F34 @ =0x0000080D
	add r1, r3, r0
_08011F1C:
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_08011F26:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08011F34: .4byte 0x0000080D
	thumb_func_end sub_08011CB4

