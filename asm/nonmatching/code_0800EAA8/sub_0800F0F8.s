	thumb_func_start sub_0800F0F8
sub_0800F0F8: @ 0x0800F0F8
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	ldr r4, _0800F124 @ =0x020185C0
	ldrh r0, [r4]
	lsr r6, r0, #0xF
	ldr r1, _0800F128 @ =0x0000080A
	add r1, r1, r4
	mov r9, r1
	ldrb r2, [r1]
	lsl r0, r2, #0x19
	lsr r7, r0, #0x19
	cmp r7, #1
	beq _0800F180
	cmp r7, #1
	bgt _0800F12C
	cmp r7, #0
	beq _0800F132
	b _0800F24C
	.align 2, 0
_0800F124: .4byte 0x020185C0
_0800F128: .4byte 0x0000080A
_0800F12C:
	cmp r7, #2
	beq _0800F208
	b _0800F24C
_0800F132:
	add r0, r6, #0
	mov r1, #0xB
	bl sub_080240A8
	mov r0, r9
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
	mov r1, r9
	strb r0, [r1]
	ldr r1, _0800F174 @ =0x020192E4
	ldr r0, _0800F178 @ =0x00000D64
	mul r0, r6
	add r1, r0, r1
	ldrb r0, [r1, #3]
	cmp r0, #0
	beq _0800F164
	b _0800F274
_0800F164:
	mov r0, #1
	ldrb r2, [r1, #7]
	orr r0, r2
	strb r0, [r1, #7]
	ldr r0, _0800F17C @ =0x0000080D
	add r1, r4, r0
	b _0800F26A
	.align 2, 0
_0800F174: .4byte 0x020192E4
_0800F178: .4byte 0x00000D64
_0800F17C: .4byte 0x0000080D
_0800F180:
	add r2, r6, #0
	and r2, r7
	mov r4, #2
	neg r4, r4
	ldr r0, [sp, #0]
	and r0, r4
	orr r0, r2
	mov r3, #0x1F
	neg r3, r3
	and r0, r3
	mov r1, #0x1A
	orr r0, r1
	ldr r1, _0800F1F8 @ =0xFFFFC01F
	mov ip, r1
	and r0, r1
	sub r1, #0x20
	mov r8, r1
	and r0, r1
	ldr r5, _0800F1FC @ =0xFFFF7FFF
	and r0, r5
	str r0, [sp, #0]
	ldr r0, [sp, #4]
	and r0, r4
	orr r0, r2
	and r0, r3
	mov r1, #0x16
	orr r0, r1
	str r0, [sp, #4]
	ldr r2, _0800F200 @ =0x020192E4
	and r6, r7
	ldr r1, _0800F204 @ =0x00000D64
	mul r1, r6
	add r1, r1, r2
	ldrb r1, [r1, #2]
	lsl r1, r1, #5
	mov r2, ip
	and r0, r2
	orr r0, r1
	mov r1, r8
	and r0, r1
	and r0, r5
	str r0, [sp, #4]
	add r2, sp, #4
	mov r0, #0
	mov r1, sp
	bl sub_080242C4
	mov r0, r9
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
	mov r1, r9
	b _0800F272
_0800F1F8: .4byte 0xFFFFC01F
_0800F1FC: .4byte 0xFFFF7FFF
_0800F200: .4byte 0x020192E4
_0800F204: .4byte 0x00000D64
_0800F208:
	ldrh r1, [r4, #2]
	add r0, r6, #0
	bl sub_080080B4
	ldrh r0, [r4, #4]
	sub r0, #1
	strh r0, [r4, #4]
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0800F24C
	ldr r2, _0800F244 @ =0x0000080C
	add r1, r4, r2
	ldr r0, _0800F248 @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	mov r0, r9
	ldrb r2, [r0]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	sub r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	mov r1, r9
	b _0800F272
	.align 2, 0
_0800F244: .4byte 0x0000080C
_0800F248: .4byte 0xFFFFF01F
_0800F24C:
	ldr r1, _0800F284 @ =0x020192E4
	ldr r0, _0800F288 @ =0x00000D64
	mul r0, r6
	add r0, r0, r1
	ldrb r2, [r0, #2]
	sub r2, #1
	add r0, r6, #0
	mov r1, #0xB
	bl sub_08024134
	bl sub_080611AC
	ldr r1, _0800F28C @ =0x020185C0
	ldr r2, _0800F290 @ =0x0000080D
	add r1, r1, r2
_0800F26A:
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_0800F272:
	strb r0, [r1]
_0800F274:
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800F284: .4byte 0x020192E4
_0800F288: .4byte 0x00000D64
_0800F28C: .4byte 0x020185C0
_0800F290: .4byte 0x0000080D
	thumb_func_end sub_0800F0F8

