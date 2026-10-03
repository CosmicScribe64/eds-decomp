	thumb_func_start SideDeckSwap_EnterListView
SideDeckSwap_EnterListView: @ 0x0806AD10
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	bl DeckEdit_InitListView
	ldr r5, _0806ADA8 @ =0x0201DB20
	ldr r0, _0806ADAC @ =0x00001C1C
	add r7, r5, r0
	ldrb r0, [r7]
	add r0, #1
	strb r0, [r7]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #3
	bne _0806AD34
	mov r0, #1
	strb r0, [r7]
_0806AD34:
	ldr r1, _0806ADB0 @ =0x00001C34
	add r2, r5, r1
	mov r0, #2
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	ldr r2, _0806ADB4 @ =0x00001C3C
	add r6, r5, r2
	ldr r0, [r6]
	ldr r1, _0806ADB8 @ =0xFFFC7FFF
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
	bl DeckEdit_StartListSlide
	ldrb r0, [r7]
	bl DeckEdit_DrawStatementLabels
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
	mov r0, #1
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0806ADA8: .4byte 0x0201DB20
_0806ADAC: .4byte 0x00001C1C
_0806ADB0: .4byte 0x00001C34
_0806ADB4: .4byte 0x00001C3C
_0806ADB8: .4byte 0xFFFC7FFF
	thumb_func_end SideDeckSwap_EnterListView

