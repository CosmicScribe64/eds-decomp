	thumb_func_start sub_08011148
sub_08011148: @ 0x08011148
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	ldr r0, _08011174 @ =0x020185C0
	ldrh r1, [r0]
	lsr r7, r1, #0xF
	ldrh r4, [r0, #2]
	ldr r2, _08011178 @ =0x0000080A
	add r2, r2, r0
	mov r8, r2
	ldrb r1, [r2]
	lsl r0, r1, #0x19
	lsr r6, r0, #0x19
	cmp r6, #1
	beq _080111A4
	cmp r6, #1
	bgt _0801117C
	cmp r6, #0
	beq _08011182
	b _08011248
	.align 2, 0
_08011174: .4byte 0x020185C0
_08011178: .4byte 0x0000080A
_0801117C:
	cmp r6, #2
	beq _0801122C
	b _08011248
_08011182:
	add r0, r7, #0
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
	b _0801125A
_080111A4:
	add r2, r7, #0
	and r2, r6
	mov r3, #2
	neg r3, r3
	ldr r0, [sp, #0]
	and r0, r3
	orr r0, r2
	mov r1, #0x1E
	orr r0, r1
	ldr r5, _08011218 @ =0xFFFFC01F
	and r0, r5
	ldr r1, _0801121C @ =0xFFFFBFFF
	mov ip, r1
	and r0, r1
	ldr r4, _08011220 @ =0xFFFF7FFF
	and r0, r4
	str r0, [sp, #0]
	ldr r0, [sp, #4]
	and r0, r3
	orr r0, r2
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	mov r1, #0x16
	orr r0, r1
	str r0, [sp, #4]
	ldr r2, _08011224 @ =0x020192E4
	and r7, r6
	ldr r1, _08011228 @ =0x00000D64
	mul r1, r7
	add r1, r1, r2
	ldrb r1, [r1, #2]
	lsl r1, r1, #5
	and r0, r5
	orr r0, r1
	mov r2, ip
	and r0, r2
	and r0, r4
	str r0, [sp, #4]
	add r2, sp, #4
	mov r0, #1
	mov r1, sp
	bl sub_080242C4
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
	b _0801125A
	.align 2, 0
_08011218: .4byte 0xFFFFC01F
_0801121C: .4byte 0xFFFFBFFF
_08011220: .4byte 0xFFFF7FFF
_08011224: .4byte 0x020192E4
_08011228: .4byte 0x00000D64
_0801122C:
	ldr r0, _08011268 @ =0x00000D64
	add r1, r7, #0
	mul r1, r0
	ldr r0, _0801126C @ =0x02019E68
	add r1, r1, r0
	lsl r0, r4, #2
	add r1, r1, r0
	add r0, r7, #0
	bl sub_08009EAC
	add r0, r7, #0
	add r1, r4, #0
	bl sub_08009D08
_08011248:
	bl sub_080611AC
	ldr r1, _08011270 @ =0x020185C0
	ldr r2, _08011274 @ =0x0000080D
	add r1, r1, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_0801125A:
	strb r0, [r1]
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08011268: .4byte 0x00000D64
_0801126C: .4byte 0x02019E68
_08011270: .4byte 0x020185C0
_08011274: .4byte 0x0000080D
	thumb_func_end sub_08011148

