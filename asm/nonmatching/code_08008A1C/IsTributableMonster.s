	thumb_func_start IsTributableMonster
IsTributableMonster: @ 0x08008A6C
	push {r4, lr}
	mov r2, #1
	and r2, r0
	mov r0, #0x94
	mul r0, r1
	ldr r1, _08008AD4 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08008AD8 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _08008AF0
	ldr r1, _08008ADC @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r2, _08008AE0 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	ldr r2, _08008AE4 @ =0xFFFFF880
	add r0, r0, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _08008AF0
	lsl r0, r1, #2
	ldr r1, _08008AE8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08008AF0
	ldr r4, _08008AEC @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _08008AF0
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _08008AF0
	mov r0, #1
	b _08008AF2
_08008AD4: .4byte 0x00000D64
_08008AD8: .4byte 0x0201930C
_08008ADC: .4byte 0x000007FF
_08008AE0: .4byte gCardIdToNumber
_08008AE4: .4byte 0xFFFFF880
_08008AE8: .4byte gCardStats
_08008AEC: .4byte 0x0000058A
_08008AF0:
	mov r0, #0
_08008AF2:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end IsTributableMonster

