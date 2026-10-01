	thumb_func_start sub_0804F168
sub_0804F168: @ 0x0804F168
	push {r4, r5, r6, r7, lr}
	ldr r0, _0804F184 @ =0x020192E0
	mov r1, #0xD9
	lsl r1, r1, #5
	add r6, r0, r1
	ldrb r2, [r6]
	cmp r2, #1
	beq _0804F1A4
	cmp r2, #1
	bgt _0804F188
	cmp r2, #0
	beq _0804F18E
	b _0804F278
	.align 2, 0
_0804F184: .4byte 0x020192E0
_0804F188:
	cmp r2, #2
	beq _0804F24C
	b _0804F278
_0804F18E:
	ldr r2, _0804F1A0 @ =0x00001B12
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	mov r1, #0xB
	bl sub_080240A8
	b _0804F268
_0804F1A0: .4byte 0x00001B12
_0804F1A4:
	add r7, r0, #4
	ldr r3, _0804F218 @ =0x00001B12
	add r3, r3, r0
	mov ip, r3
	ldrb r5, [r3]
	lsl r3, r5, #0x1E
	lsr r1, r3, #0x1F
	add r0, r2, #0
	and r0, r1
	ldr r4, _0804F21C @ =0x00000D64
	mul r0, r4
	add r0, r0, r7
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1C
	cmp r0, #0
	bge _0804F22C
	add r0, r2, #0
	and r0, r1
	add r1, r0, #0
	mul r1, r4
	add r1, r1, r7
	mov r0, #9
	neg r0, r0
	ldrb r4, [r1, #9]
	and r0, r4
	strb r0, [r1, #9]
	ldr r3, _0804F220 @ =0x02015EE8
	add r0, r2, #0
	ldrb r7, [r3, #1]
	and r0, r7
	cmp r0, #0
	bne _0804F202
	mov r0, #2
	mov r1, ip
	ldrb r1, [r1]
	and r0, r1
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	cmp r1, #0
	bne _0804F1FA
	ldr r0, _0804F224 @ =0x02015EF0
	strb r1, [r0]
	strb r1, [r0, #1]
_0804F1FA:
	ldrb r3, [r3, #1]
	and r2, r3
	cmp r2, #0
	beq _0804F20E
_0804F202:
	ldr r0, _0804F228 @ =0x0000F002
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
_0804F20E:
	ldr r1, _0804F220 @ =0x02015EE8
	ldrb r0, [r1]
	add r0, #5
	strb r0, [r1]
	b _0804F2FA
_0804F218: .4byte 0x00001B12
_0804F21C: .4byte 0x00000D64
_0804F220: .4byte 0x02015EE8
_0804F224: .4byte 0x02015EF0
_0804F228: .4byte 0x0000F002
_0804F22C:
	mov r0, #2
	and r0, r5
	mov r1, #1
	cmp r0, #0
	beq _0804F238
	ldr r1, _0804F248 @ =0x00008001
_0804F238:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	b _0804F268
	.align 2, 0
_0804F248: .4byte 0x00008001
_0804F24C:
	ldr r2, _0804F274 @ =0x00001B12
	add r4, r0, r2
	ldrb r3, [r4]
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	bl sub_0804EFF0
	ldrb r4, [r4]
	lsl r1, r4, #0x1E
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	bl sub_0804EFF0
_0804F268:
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
	mov r0, #0
	b _0804F2FC
	.align 2, 0
_0804F274: .4byte 0x00001B12
_0804F278:
	ldr r4, _0804F304 @ =0x020192E4
	ldr r7, _0804F308 @ =0x00001B0E
	add r6, r4, r7
	ldrb r1, [r6]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	ldr r5, _0804F30C @ =0x00000D64
	add r1, r0, #0
	mul r1, r5
	add r1, r1, r4
	mov r2, #0x11
	neg r2, r2
	add r0, r2, #0
	ldrb r3, [r1, #9]
	and r0, r3
	strb r0, [r1, #9]
	ldrb r7, [r6]
	lsl r0, r7, #0x1E
	lsr r0, r0, #0x1F
	add r1, r0, #0
	mul r1, r5
	add r1, r1, r4
	mov r3, #0x21
	neg r3, r3
	add r0, r3, #0
	ldrb r7, [r1, #9]
	and r0, r7
	strb r0, [r1, #9]
	ldrb r1, [r6]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	mul r0, r5
	add r0, r0, r4
	ldrb r7, [r0, #8]
	and r2, r7
	strb r2, [r0, #8]
	ldrb r1, [r6]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	add r1, r0, #0
	mul r1, r5
	add r1, r1, r4
	add r0, r3, #0
	ldrb r2, [r1, #8]
	and r0, r2
	strb r0, [r1, #8]
	ldrb r7, [r6]
	lsl r0, r7, #0x1E
	lsr r0, r0, #0x1F
	add r1, r0, #0
	mul r1, r5
	add r1, r1, r4
	mov r0, #9
	neg r0, r0
	ldrb r2, [r1, #0xB]
	and r0, r2
	strb r0, [r1, #0xB]
	ldrb r6, [r6]
	lsl r0, r6, #0x1E
	lsr r0, r0, #0x1F
	mul r0, r5
	add r0, r0, r4
	ldrb r4, [r0, #0xC]
	and r3, r4
	strb r3, [r0, #0xC]
_0804F2FA:
	mov r0, #1
_0804F2FC:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0804F304: .4byte 0x020192E4
_0804F308: .4byte 0x00001B0E
_0804F30C: .4byte 0x00000D64
	thumb_func_end sub_0804F168

