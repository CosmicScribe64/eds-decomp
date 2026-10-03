	thumb_func_start EffectTailorOfTheFickleCheck
EffectTailorOfTheFickleCheck: @ 0x0802C080
	push {r4, r5, lr}
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r5, r0, #0x18
	lsr r4, r1, #0x18
	cmp r4, #4
	bgt _0802C090
	b _0802C226
_0802C090:
	mov r1, #1
	and r1, r5
	mov r0, #0x94
	add r2, r4, #0
	mul r2, r0
	ldr r0, _0802C0FC @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r1, _0802C100 @ =0x0201930C
	add r2, r2, r1
	mov r0, #2
	ldrb r3, [r2, #6]
	and r0, r3
	add r3, r1, #0
	cmp r0, #0
	bne _0802C0B2
	b _0802C226
_0802C0B2:
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	bne _0802C0BE
	b _0802C226
_0802C0BE:
	ldr r0, _0802C104 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r1, _0802C108 @ =0x08622AB4
	add r0, r0, r1
	ldrh r2, [r0]
	ldr r0, _0802C10C @ =0x000003F5
	cmp r2, r0
	bgt _0802C154
	sub r0, #1
	cmp r2, r0
	blt _0802C0D8
	b _0802C1C2
_0802C0D8:
	ldr r0, _0802C110 @ =0x0000028B
	cmp r2, r0
	bgt _0802C12C
	sub r0, #1
	cmp r2, r0
	bge _0802C1C2
	mov r0, #0x9F
	lsl r0, r0, #1
	cmp r2, r0
	beq _0802C1C2
	cmp r2, r0
	bgt _0802C114
	sub r0, #2
	cmp r2, r0
	ble _0802C0F8
	b _0802C226
_0802C0F8:
	sub r0, #0x10
	b _0802C11E
_0802C0FC: .4byte 0x00000D64
_0802C100: .4byte 0x0201930C
_0802C104: .4byte 0x000007FF
_0802C108: .4byte gCardIdToNumber
_0802C10C: .4byte 0x000003F5
_0802C110: .4byte 0x0000028B
_0802C114:
	ldr r0, _0802C128 @ =0x00000147
	cmp r2, r0
	ble _0802C11C
	b _0802C226
_0802C11C:
	sub r0, #7
_0802C11E:
	cmp r2, r0
	bge _0802C124
	b _0802C226
_0802C124:
	b _0802C1C2
	.align 2, 0
_0802C128: .4byte 0x00000147
_0802C12C:
	ldr r0, _0802C13C @ =0x00000291
	cmp r2, r0
	bgt _0802C140
	sub r0, #1
	cmp r2, r0
	bge _0802C1C2
	sub r0, #3
	b _0802C1BE
_0802C13C: .4byte 0x00000291
_0802C140:
	ldr r0, _0802C14C @ =0x0000029B
	cmp r2, r0
	beq _0802C1C2
	ldr r0, _0802C150 @ =0x000003C2
	b _0802C1BE
	.align 2, 0
_0802C14C: .4byte 0x0000029B
_0802C150: .4byte 0x000003C2
_0802C154:
	ldr r0, _0802C170 @ =0x0000058C
	cmp r2, r0
	bgt _0802C194
	sub r0, #1
	cmp r2, r0
	bge _0802C1C2
	ldr r0, _0802C174 @ =0x00000417
	cmp r2, r0
	bgt _0802C178
	sub r0, #1
	cmp r2, r0
	bge _0802C1C2
	sub r0, #4
	b _0802C1BE
_0802C170: .4byte 0x0000058C
_0802C174: .4byte 0x00000417
_0802C178:
	ldr r0, _0802C188 @ =0x00000424
	cmp r2, r0
	beq _0802C1C2
	cmp r2, r0
	bgt _0802C18C
	sub r0, #2
	b _0802C1BE
	.align 2, 0
_0802C188: .4byte 0x00000424
_0802C18C:
	ldr r0, _0802C190 @ =0x0000049E
	b _0802C1BE
_0802C190: .4byte 0x0000049E
_0802C194:
	ldr r0, _0802C1A4 @ =0x000005AA
	cmp r2, r0
	bgt _0802C1A8
	sub r0, #2
	cmp r2, r0
	bge _0802C1C2
	sub r0, #0x1A
	b _0802C1BE
_0802C1A4: .4byte 0x000005AA
_0802C1A8:
	ldr r0, _0802C1B8 @ =0x0000060C
	cmp r2, r0
	beq _0802C1C2
	cmp r2, r0
	bgt _0802C1BC
	sub r0, #8
	b _0802C1BE
	.align 2, 0
_0802C1B8: .4byte 0x0000060C
_0802C1BC:
	ldr r0, _0802C1F8 @ =0x0000060E
_0802C1BE:
	cmp r2, r0
	bne _0802C226
_0802C1C2:
	mov r2, #1
	and r2, r5
	mov r0, #0x94
	mul r0, r4
	ldr r1, _0802C1FC @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	add r0, r0, r3
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x13
	ldr r3, _0802C200 @ =0x08621DE0
	add r0, r0, r3
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _0802C204
	cmp r0, #0x15
	blt _0802C204
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _0802C206
_0802C1F8: .4byte 0x0000060E
_0802C1FC: .4byte 0x00000D64
_0802C200: .4byte gCardStats
_0802C204:
	mov r0, #0
_0802C206:
	cmp r0, #3
	bne _0802C226
	add r0, r5, #0
	add r1, r4, #0
	bl IsCardLinkedToMonster
	cmp r0, #0
	beq _0802C226
	add r0, r5, #0
	add r1, r4, #0
	bl CountValidEquipTargets
	cmp r0, #1
	ble _0802C226
	mov r0, #1
	b _0802C228
_0802C226:
	mov r0, #0
_0802C228:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectTailorOfTheFickleCheck
	.align 2, 0

