	thumb_func_start CardDetail_Run
CardDetail_Run: @ 0x08006D08
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r7, _08006D24 @ =0x02013D90
	ldrh r0, [r7]
	lsr r6, r0, #1
	cmp r6, #1
	beq _08006D48
	cmp r6, #1
	bgt _08006D28
	cmp r6, #0
	beq _08006D36
	b _08006E84
_08006D24: .4byte 0x02013D90
_08006D28:
	cmp r6, #2
	bne _08006D2E
	b _08006E60
_08006D2E:
	cmp r6, #3
	bne _08006D34
	b _08006E66
_08006D34:
	b _08006E84
_08006D36:
	bl CardDetail_InitVideo
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08006D42
	b _08006E80
_08006D42:
	bl CardDetail_DrawCard
	b _08006E70
_08006D48:
	add r0, r6, #0
	ldrb r1, [r7]
	and r0, r1
	cmp r0, #0
	bne _08006D60
	bl CardDetail_FadeIn
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08006D5E
	b _08006E80
_08006D5E:
	b _08006E32
_08006D60:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0xF8
	lsl r3, r3, #5
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	ldr r0, _08006DE0 @ =0x03000040
	mov r9, r0
	ldr r1, _08006DE4 @ =0x00004834
	add r1, r9
	mov r8, r1
	mov r2, #0
	ldsh r0, [r1, r2]
	cmp r0, #0x9F
	bgt _08006DF8
	add r5, r0, #0
	add r4, r5, #0
	add r4, #0x20
	add r0, r4, #0
	mov r1, #0xC
	bl __divsi3
	mov r1, #0xF
	sub r3, r1, r0
	cmp r4, #0
	bge _08006D9A
	add r4, #7
_08006D9A:
	asr r1, r4, #3
	mov r0, #0x18
	sub r0, r0, r1
	ldr r1, _08006DE8 @ =0x04000054
	strh r0, [r1]
	ldr r2, _08006DEC @ =0x0400004C
	mov r0, #0xF
	add r1, r3, #0
	and r1, r0
	lsl r0, r1, #4
	orr r0, r1
	strh r0, [r2]
	ldr r2, _08006DF0 @ =0x00004836
	add r2, r9
	mov r1, r8
	mov r3, #0
	ldsh r0, [r1, r3]
	add r1, r0, #0
	add r1, #0x20
	cmp r1, #0
	bge _08006DC6
	add r1, #7
_08006DC6:
	asr r1, r1, #3
	lsl r1, r1, #5
	ldr r0, _08006DF4 @ =0x08198A50
	add r1, r1, r0
	add r0, r2, #0
	mov r2, #0x20
	bl MemCopy16
	mov r1, r8
	ldrh r0, [r1]
	add r0, #0x10
	strh r0, [r1]
	b _08006E80
_08006DE0: .4byte 0x03000040
_08006DE4: .4byte 0x00004834
_08006DE8: .4byte 0x04000054
_08006DEC: .4byte 0x0400004C
_08006DF0: .4byte 0x00004836
_08006DF4: .4byte gCardDetailWaveTable
_08006DF8:
	ldr r2, _08006E44 @ =0x04000008
	ldrh r1, [r2]
	ldr r0, _08006E48 @ =0x0000FFBF
	and r0, r1
	strh r0, [r2]
	ldr r0, _08006E4C @ =0x0400004C
	mov r4, #0
	strh r4, [r0]
	bl ClearBlend
	bl ResetBgScroll
	ldr r5, _08006E50 @ =0x04000208
	strh r4, [r5]
	ldr r2, _08006E54 @ =0x04000200
	ldrh r3, [r2]
	ldr r1, _08006E58 @ =0x0000FFFD
	add r0, r1, #0
	and r0, r3
	strh r0, [r2]
	strh r6, [r5]
	strh r4, [r5]
	ldrh r0, [r2]
	and r1, r0
	strh r1, [r2]
	ldr r1, _08006E5C @ =0x03000000
	mov r0, #0
	str r0, [r1, #4]
	strh r6, [r5]
_08006E32:
	ldrh r1, [r7]
	lsr r0, r1, #1
	add r0, #1
	lsl r0, r0, #1
	and r6, r1
	orr r6, r0
	strh r6, [r7]
	b _08006E80
	.align 2, 0
_08006E44: .4byte 0x04000008
_08006E48: .4byte 0x0000FFBF
_08006E4C: .4byte 0x0400004C
_08006E50: .4byte 0x04000208
_08006E54: .4byte 0x04000200
_08006E58: .4byte 0x0000FFFD
_08006E5C: .4byte 0x03000000
_08006E60:
	bl CardDetail_HandleInput
	b _08006E6A
_08006E66:
	bl CardDetail_FadeOut
_08006E6A:
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08006E80
_08006E70:
	ldrh r2, [r7]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strh r0, [r7]
_08006E80:
	mov r0, #0
	b _08006E86
_08006E84:
	mov r0, #1
_08006E86:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end CardDetail_Run
	.align 2, 0

