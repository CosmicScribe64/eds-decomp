	thumb_func_start sub_08012B34
sub_08012B34: @ 0x08012B34
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r4, _08012C00 @ =0x020185C0
	ldrh r0, [r4]
	lsr r0, r0, #0xF
	mov sl, r0
	ldrh r1, [r4, #2]
	str r1, [sp, #4]
	ldrh r7, [r4, #4]
	ldr r2, _08012C04 @ =0x02018DCA
	ldrb r2, [r2]
	lsl r0, r2, #0x19
	cmp r0, #0
	bne _08012C24
	mov r3, #1
	mov r9, r3
	mov r0, sl
	and r0, r3
	ldr r1, _08012C08 @ =0x00000D64
	add r6, r0, #0
	mul r6, r1
	ldr r0, _08012C0C @ =0x0201930C
	mov r8, r0
	add r1, r6, r0
	mov r2, #0x94
	add r4, r7, #0
	mul r4, r2
	add r0, r1, r4
	ldr r3, [sp, #4]
	add r5, r3, #0
	mul r5, r2
	add r1, r1, r5
	bl sub_08075294
	add r4, r4, r6
	add r4, r8
	mov r0, #3
	neg r0, r0
	ldrb r1, [r4, #6]
	and r0, r1
	mov r1, #1
	orr r0, r1
	strb r0, [r4, #6]
	add r5, r5, r6
	add r5, r8
	ldr r0, _08012C10 @ =0xFFFFF000
	ldrh r2, [r5]
	and r0, r2
	strh r0, [r5]
	mov r0, #0x10
	bl sub_08077AEC
	mov r1, sl
	mov r3, r9
	and r1, r3
	mov r2, #2
	neg r2, r2
	ldr r0, [sp, #0]
	and r0, r2
	orr r0, r1
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	ldr r1, _08012C14 @ =0x000001FF
	and r7, r1
	lsl r2, r7, #5
	ldr r1, _08012C18 @ =0xFFFFC01F
	and r0, r1
	orr r0, r2
	mov r1, #0x80
	lsl r1, r1, #7
	orr r0, r1
	ldr r1, _08012C1C @ =0xFFFF7FFF
	and r0, r1
	str r0, [sp, #0]
	ldr r1, _08012C20 @ =0x0869771C
	mov r0, sp
	mov r2, #0
	mov r3, #0
	bl sub_08024380
	mov r0, sl
	ldr r1, [sp, #4]
	bl sub_08060FD0
	ldr r0, _08012C04 @ =0x02018DCA
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
	ldr r1, _08012C04 @ =0x02018DCA
	b _08012C34
_08012C00: .4byte 0x020185C0
_08012C04: .4byte 0x02018DCA
_08012C08: .4byte 0x00000D64
_08012C0C: .4byte 0x0201930C
_08012C10: .4byte 0xFFFFF000
_08012C14: .4byte 0x000001FF
_08012C18: .4byte 0xFFFFC01F
_08012C1C: .4byte 0xFFFF7FFF
_08012C20: .4byte gUnk_0869771C
_08012C24:
	bl sub_080611AC
	ldr r2, _08012C48 @ =0x0000080D
	add r1, r4, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
_08012C34:
	strb r0, [r1]
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08012C48: .4byte 0x0000080D
	thumb_func_end sub_08012B34

