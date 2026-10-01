	thumb_func_start sub_0806ADBC
sub_0806ADBC: @ 0x0806ADBC
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r7, r0, #0
	ldr r2, _0806AE68 @ =0x0201DB20
	ldr r0, _0806AE6C @ =0x00001C5A
	add r1, r2, r0
	mov r0, #0x1C
	ldrb r1, [r1]
	and r0, r1
	add r5, r2, #0
	cmp r0, #0
	beq _0806ADDA
	b _0806AF02
_0806ADDA:
	ldr r0, _0806AE70 @ =0x03000040
	ldrh r1, [r0, #6]
	mov r0, #0x80
	lsl r0, r0, #1
	and r0, r1
	cmp r0, #0
	beq _0806AE80
	ldrb r0, [r7]
	add r0, #1
	strb r0, [r7]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #3
	bne _0806ADFA
	mov r0, #1
	strb r0, [r7]
_0806ADFA:
	ldr r1, _0806AE74 @ =0x00001C34
	add r2, r5, r1
	mov r0, #2
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	ldr r2, _0806AE78 @ =0x00001C3C
	add r6, r5, r2
	ldr r0, [r6]
	ldr r1, _0806AE7C @ =0xFFFC7FFF
	mov r9, r1
	and r0, r1
	str r0, [r6]
	add r2, #1
	add r5, r5, r2
	mov r4, #8
	neg r4, r4
	add r0, r4, #0
	ldrb r1, [r5]
	and r0, r1
	mov r2, #3
	mov r8, r2
	mov r1, r8
	orr r0, r1
	strb r0, [r5]
	mov r0, #2
	bl sub_080671E8
	ldrb r0, [r7]
	bl sub_080679A8
	ldrb r1, [r7]
	add r1, #1
	mov r0, #7
	and r1, r0
	lsl r1, r1, #0xF
	ldr r0, [r6]
	mov r2, r9
	and r0, r2
	orr r0, r1
	str r0, [r6]
	ldrb r0, [r5]
	and r4, r0
	mov r1, r8
	orr r4, r1
	strb r4, [r5]
	mov r0, #0
	bl sub_08077AEC
	b _0806AF02
	.align 2, 0
_0806AE68: .4byte 0x0201DB20
_0806AE6C: .4byte 0x00001C5A
_0806AE70: .4byte 0x03000040
_0806AE74: .4byte 0x00001C34
_0806AE78: .4byte 0x00001C3C
_0806AE7C: .4byte 0xFFFC7FFF
_0806AE80:
	mov r0, #0x80
	lsl r0, r0, #2
	and r0, r1
	cmp r0, #0
	beq _0806AF02
	ldrb r0, [r7]
	cmp r0, #1
	beq _0806AE94
	sub r0, #1
	b _0806AE96
_0806AE94:
	mov r0, #2
_0806AE96:
	strb r0, [r7]
	ldr r0, _0806AF10 @ =0x00001C34
	add r2, r5, r0
	mov r0, #2
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	ldr r2, _0806AF14 @ =0x00001C3C
	add r6, r5, r2
	ldr r0, [r6]
	ldr r1, _0806AF18 @ =0xFFFC7FFF
	mov r9, r1
	and r0, r1
	str r0, [r6]
	add r2, #1
	add r5, r5, r2
	mov r4, #8
	neg r4, r4
	add r0, r4, #0
	ldrb r1, [r5]
	and r0, r1
	mov r2, #3
	mov r8, r2
	mov r1, r8
	orr r0, r1
	strb r0, [r5]
	mov r0, #3
	bl sub_080671E8
	ldrb r0, [r7]
	bl sub_080679A8
	ldrb r1, [r7]
	add r1, #1
	mov r0, #7
	and r1, r0
	lsl r1, r1, #0xF
	ldr r0, [r6]
	mov r2, r9
	and r0, r2
	orr r0, r1
	str r0, [r6]
	ldrb r0, [r5]
	and r4, r0
	mov r1, r8
	orr r4, r1
	strb r4, [r5]
	mov r0, #0
	bl sub_08077AEC
_0806AF02:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0806AF10: .4byte 0x00001C34
_0806AF14: .4byte 0x00001C3C
_0806AF18: .4byte 0xFFFC7FFF
	thumb_func_end sub_0806ADBC

