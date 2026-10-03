	thumb_func_start DuelPrompt_BanishRandom
DuelPrompt_BanishRandom: @ 0x08051BBC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	mov r8, r2
	ldr r2, _08051C04 @ =0x020192E4
	mov r0, #1
	and r0, r6
	ldr r1, _08051C08 @ =0x00000D64
	mul r0, r1
	add r7, r0, r2
	ldrb r0, [r7, #2]
	cmp r0, #0
	beq _08051CCC
	ldr r0, _08051C0C @ =0x00001B5E
	add r5, r2, r0
	ldrb r4, [r5]
	cmp r4, #0
	bne _08051C1C
	add r0, r6, #0
	mov r1, #0xB
	bl DuelScreen_ScrollToZone
	ldr r0, _08051C10 @ =0x02017A40
	ldr r2, _08051C14 @ =0x000004FC
	add r1, r0, r2
	strb r4, [r1]
	ldr r1, _08051C18 @ =0x000004FD
	add r0, r0, r1
	mov r2, r8
	strb r2, [r0]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	b _08051CCE
_08051C04: .4byte 0x020192E4
_08051C08: .4byte 0x00000D64
_08051C0C: .4byte 0x00001B5E
_08051C10: .4byte 0x02017A40
_08051C14: .4byte 0x000004FC
_08051C18: .4byte 0x000004FD
_08051C1C:
	ldr r1, _08051C64 @ =0x02017A40
	ldr r0, _08051C68 @ =0x000004FD
	add r5, r1, r0
	ldrb r0, [r5]
	cmp r0, #0
	beq _08051CCC
	ldr r2, _08051C6C @ =0x000004FC
	add r2, r2, r1
	mov r8, r2
	ldrb r0, [r2]
	cmp r0, #9
	bhi _08051C78
	ldr r1, _08051C70 @ =0x0201CFB0
	ldr r2, _08051C74 @ =0x00000808
	add r1, r1, r2
	mov r0, #8
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	bl Random
	ldrb r1, [r7, #2]
	bl __modsi3
	add r2, r0, #0
	add r0, r6, #0
	mov r1, #0xB
	bl DuelCursor_Select
	mov r1, r8
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	mov r0, #0
	b _08051CCE
	.align 2, 0
_08051C64: .4byte 0x02017A40
_08051C68: .4byte 0x000004FD
_08051C6C: .4byte 0x000004FC
_08051C70: .4byte 0x0201CFB0
_08051C74: .4byte 0x00000808
_08051C78:
	mov r3, #8
	cmp r6, #0
	beq _08051C80
	ldr r3, _08051CBC @ =0x00008008
_08051C80:
	lsl r1, r6, #0x10
	lsr r1, r1, #0x10
	ldr r0, _08051CC0 @ =0x0201CFB0
	ldr r2, _08051CC4 @ =0x0000082C
	add r4, r0, r2
	ldrb r0, [r4]
	lsl r2, r0, #8
	mov r0, #0xB
	orr r2, r0
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0xC1
	cmp r6, #0
	beq _08051CA2
	ldr r0, _08051CC8 @ =0x000080C1
_08051CA2:
	ldrh r1, [r4]
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldrb r0, [r5]
	sub r0, #1
	mov r1, #0
	strb r0, [r5]
	mov r2, r8
	strb r1, [r2]
	mov r0, #0
	b _08051CCE
_08051CBC: .4byte 0x00008008
_08051CC0: .4byte 0x0201CFB0
_08051CC4: .4byte 0x0000082C
_08051CC8: .4byte 0x000080C1
_08051CCC:
	mov r0, #1
_08051CCE:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end DuelPrompt_BanishRandom

