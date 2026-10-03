	thumb_func_start CountZoneLinksFromCard
CountZoneLinksFromCard: @ 0x0800A78C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	lsl r2, r2, #0x10
	lsr r7, r2, #0x10
	mov r2, #0
	mov r9, r2
	mov sl, r2
	mov r2, #1
	and r2, r0
	mov r0, #0x94
	mul r1, r0
	ldr r3, _0800A814 @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r1, r1, r0
	ldr r2, _0800A818 @ =0x0201930C
	add r1, r1, r2
	add r0, r1, #0
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r9, r0
	bge _0800A8B2
	str r1, [sp, #4]
	str r0, [sp, #0]
	add r3, r1, #0
	add r3, #0x4A
	mov r8, r3
_0800A7CA:
	mov r6, sl
	lsl r1, r6, #1
	ldr r0, [sp, #4]
	add r0, #0xA
	add r0, r0, r1
	mov r1, r8
	ldrb r4, [r1]
	ldrh r5, [r0]
	ldrb r2, [r0]
	lsr r6, r5, #8
	add r1, r2, #0
	mov r3, #1
	and r1, r3
	mov r3, #0x94
	add r0, r6, #0
	mul r0, r3
	ldr r3, _0800A814 @ =0x00000D64
	mul r1, r3
	add r0, r0, r1
	ldr r1, _0800A818 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	mov r0, #1
	mov ip, r0
	ldr r0, _0800A81C @ =0x00000422
	cmp r7, r0
	beq _0800A826
	cmp r7, r0
	bgt _0800A820
	mov r0, #0xAE
	lsl r0, r0, #1
	cmp r7, r0
	beq _0800A826
	b _0800A84C
	.align 2, 0
_0800A814: .4byte 0x00000D64
_0800A818: .4byte 0x0201930C
_0800A81C: .4byte 0x00000422
_0800A820:
	ldr r0, _0800A860 @ =0x000004DC
	cmp r7, r0
	bne _0800A84C
_0800A826:
	mov r1, #1
	and r2, r1
	mov r1, #0x94
	add r0, r6, #0
	mul r0, r1
	ldr r6, _0800A864 @ =0x00000D64
	add r1, r2, #0
	mul r1, r6
	add r0, r0, r1
	ldr r1, _0800A868 @ =0x0201930C
	add r0, r0, r1
	add r0, #0x91
	mov r1, #8
	ldrb r0, [r0]
	and r1, r0
	cmp r1, #0
	beq _0800A84C
	mov r2, #0
	mov ip, r2
_0800A84C:
	mov r6, ip
	cmp r6, #0
	beq _0800A8A4
	cmp r4, #3
	beq _0800A890
	cmp r4, #3
	bgt _0800A86C
	cmp r4, #1
	blt _0800A8A4
	b _0800A870
_0800A860: .4byte 0x000004DC
_0800A864: .4byte 0x00000D64
_0800A868: .4byte 0x0201930C
_0800A86C:
	cmp r4, #0xA
	bne _0800A8A4
_0800A870:
	ldr r0, _0800A888 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #1
	ldr r1, _0800A88C @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r7
	bne _0800A8A4
	mov r2, #1
	add r9, r2
	b _0800A8A4
	.align 2, 0
_0800A888: .4byte 0x000007FF
_0800A88C: .4byte gCardIdToNumber
_0800A890:
	ldr r3, _0800A8C4 @ =0x000007FF
	and r5, r3
	lsl r0, r5, #1
	ldr r6, _0800A8C8 @ =0x08622AB4
	add r0, r0, r6
	ldrh r0, [r0]
	cmp r0, r7
	bne _0800A8A4
	mov r0, #1
	add r9, r0
_0800A8A4:
	mov r1, #2
	add r8, r1
	mov r2, #1
	add sl, r2
	ldr r3, [sp, #0]
	cmp sl, r3
	blt _0800A7CA
_0800A8B2:
	mov r0, r9
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0800A8C4: .4byte 0x000007FF
_0800A8C8: .4byte gCardIdToNumber
	thumb_func_end CountZoneLinksFromCard

