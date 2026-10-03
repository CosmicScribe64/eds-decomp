	thumb_func_start CanNormalSummon
CanNormalSummon: @ 0x08047114
	push {r4, lr}
	add r3, r0, #0
	ldr r2, _08047158 @ =0x020192E4
	mov r0, #1
	and r0, r3
	ldr r1, _0804715C @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #7]
	lsl r0, r0, #0x1C
	cmp r0, #0
	blt _08047168
	ldr r1, _08047160 @ =0x00000592
	add r0, r3, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08047168
	ldr r4, _08047164 @ =0x000005F6
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08047168
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08047168
	mov r0, #1
	b _0804716A
	.align 2, 0
_08047158: .4byte 0x020192E4
_0804715C: .4byte 0x00000D64
_08047160: .4byte 0x00000592
_08047164: .4byte 0x000005F6
_08047168:
	mov r0, #0
_0804716A:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end CanNormalSummon

