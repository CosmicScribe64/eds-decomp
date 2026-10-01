	thumb_func_start sub_0800FF04
sub_0800FF04: @ 0x0800FF04
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	ldr r5, _0800FF58 @ =0x020185C0
	ldrh r0, [r5]
	lsr r6, r0, #0xF
	ldrh r1, [r5, #2]
	ldr r2, _0800FF5C @ =0x0000080A
	add r2, r2, r5
	mov r8, r2
	ldrb r3, [r2]
	lsl r0, r3, #0x19
	lsr r4, r0, #0x19
	cmp r4, #0
	beq _0800FF68
	cmp r4, #1
	beq _0800FF8A
	ldr r0, _0800FF60 @ =0x00000814
	add r4, r5, r0
	add r0, r4, #0
	bl sub_0800743C
	add r0, r6, #0
	add r1, r4, #0
	bl sub_08007CE0
	bl sub_080611AC
	add r0, r6, #0
	mov r1, #0xD
	mov r2, #0
	bl sub_08024134
	ldr r2, _0800FF64 @ =0x0000080D
	add r1, r5, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	b _08010024
	.align 2, 0
_0800FF58: .4byte 0x020185C0
_0800FF5C: .4byte 0x0000080A
_0800FF60: .4byte 0x00000814
_0800FF64: .4byte 0x0000080D
_0800FF68:
	add r0, r6, #0
	mov r1, #0xB
	bl sub_080240A8
	mov r0, r8
	ldrb r2, [r0]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	mov r1, r8
	b _08010024
_0800FF8A:
	ldr r2, _08010008 @ =0x00000814
	add r7, r5, r2
	add r0, r6, #0
	add r2, r7, #0
	bl sub_08009C08
	cmp r0, #0
	beq _08010018
	add r0, r6, #0
	add r1, r7, #0
	bl sub_08009B48
	and r6, r4
	mov r5, #2
	neg r5, r5
	ldr r0, [sp, #0]
	and r0, r5
	orr r0, r6
	mov r4, #0x1F
	neg r4, r4
	and r0, r4
	mov r1, #0x1C
	orr r0, r1
	ldr r3, _0801000C @ =0xFFFFC01F
	and r0, r3
	ldr r2, _08010010 @ =0xFFFFBFFF
	and r0, r2
	mov r1, #0x80
	lsl r1, r1, #8
	orr r0, r1
	str r0, [sp, #0]
	ldr r0, [sp, #4]
	and r0, r5
	orr r0, r6
	and r0, r4
	mov r1, #0x1A
	orr r0, r1
	and r0, r3
	and r0, r2
	ldr r1, _08010014 @ =0xFFFF7FFF
	and r0, r1
	str r0, [sp, #4]
	ldr r0, [r7]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl sub_080242C4
	mov r3, r8
	ldrb r2, [r3]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _08010026
	.align 2, 0
_08010008: .4byte 0x00000814
_0801000C: .4byte 0xFFFFC01F
_08010010: .4byte 0xFFFFBFFF
_08010014: .4byte 0xFFFF7FFF
_08010018:
	ldr r0, _08010034 @ =0x0000080D
	add r1, r5, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_08010024:
	strb r0, [r1]
_08010026:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08010034: .4byte 0x0000080D
	thumb_func_end sub_0800FF04

