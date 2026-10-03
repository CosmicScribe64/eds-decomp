	thumb_func_start BanishFieldCard
BanishFieldCard: @ 0x080189FC
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	add r7, r1, #0
	lsl r2, r2, #0x10
	lsr r3, r2, #0x10
	mov r2, #1
	and r2, r5
	mov r0, #0x94
	mul r0, r7
	ldr r1, _08018A3C @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08018A40 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	cmp r6, #0
	beq _08018AC0
	cmp r3, #0
	beq _08018A48
	mov r0, #0x7B
	cmp r5, #0
	beq _08018A2E
	ldr r0, _08018A44 @ =0x0000807B
_08018A2E:
	lsl r4, r7, #0x10
	lsr r1, r4, #0x10
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	b _08018A5C
_08018A3C: .4byte 0x00000D64
_08018A40: .4byte 0x0201930C
_08018A44: .4byte 0x0000807B
_08018A48:
	mov r0, #0x7A
	cmp r5, #0
	beq _08018A50
	ldr r0, _08018AC8 @ =0x0000807A
_08018A50:
	lsl r4, r7, #0x10
	lsr r1, r4, #0x10
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_08018A5C:
	cmp r7, #4
	bgt _08018A6A
	add r0, r5, #0
	add r1, r7, #0
	mov r2, #1
	bl DestroyLinkedCards
_08018A6A:
	ldr r0, _08018ACC @ =0x000007FF
	and r6, r0
	lsl r0, r6, #1
	ldr r1, _08018AD0 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08018AD4 @ =0x0000048A
	ldrh r0, [r0]
	cmp r0, r1
	bne _08018A8E
	mov r0, #0x8F
	cmp r5, #0
	beq _08018A84
	ldr r0, _08018AD8 @ =0x0000808F
_08018A84:
	lsr r1, r4, #0x10
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08018A8E:
	cmp r7, #0xA
	bne _08018AC0
	mov r0, #1
	and r0, r5
	ldr r1, _08018ADC @ =0x00000D64
	mul r1, r0
	mov r0, #0xB9
	lsl r0, r0, #3
	add r1, r1, r0
	ldr r0, _08018AE0 @ =0x0201930C
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08018AC0
	mov r0, #0x11
	cmp r5, #0
	beq _08018AB6
	ldr r0, _08018AE4 @ =0x00008011
_08018AB6:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08018AC0:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08018AC8: .4byte 0x0000807A
_08018ACC: .4byte 0x000007FF
_08018AD0: .4byte gCardIdToNumber
_08018AD4: .4byte 0x0000048A
_08018AD8: .4byte 0x0000808F
_08018ADC: .4byte 0x00000D64
_08018AE0: .4byte 0x0201930C
_08018AE4: .4byte 0x00008011
	thumb_func_end BanishFieldCard

