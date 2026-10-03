	thumb_func_start ProhibitCardSelect_SwitchScreen
ProhibitCardSelect_SwitchScreen: @ 0x0807093C
	push {r4, lr}
	ldr r0, _0807096C @ =0x03000040
	ldr r1, _08070970 @ =0x0000485A
	add r0, r0, r1
	mov r1, #0
	strb r1, [r0]
	ldr r2, _08070974 @ =0x02017A40
	ldr r3, _08070978 @ =0x000003E7
	add r0, r2, r3
	strb r1, [r0]
	ldr r1, _0807097C @ =0x0201DB20
	ldr r4, _08070980 @ =0x00001C48
	add r0, r1, r4
	ldrb r0, [r0]
	lsl r0, r0, #0x1B
	lsr r0, r0, #0x1C
	add r3, r1, #0
	cmp r0, #4
	bhi _08070A14
	lsl r0, r0, #2
	ldr r1, _08070984 @ =0x08070988
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0807096C: .4byte 0x03000040
_08070970: .4byte 0x0000485A
_08070974: .4byte 0x02017A40
_08070978: .4byte 0x000003E7
_0807097C: .4byte 0x0201DB20
_08070980: .4byte 0x00001C48
_08070984: .4byte 0x08070988
_08070988:
	.4byte _08070A14
	.4byte _0807099C
	.4byte _080709A8
	.4byte _080709F8
	.4byte _08070A04
_0807099C:
	ldr r0, _080709A4 @ =0x000003E6
	add r1, r2, r0
	mov r0, #5
	b _08070A0A
_080709A4: .4byte 0x000003E6
_080709A8:
	ldr r4, _080709EC @ =0x000003E6
	add r1, r2, r4
	mov r0, #0xD
	strb r0, [r1]
	ldr r1, _080709F0 @ =0x02013D90
	mov r0, #1
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldr r4, _080709F4 @ =0x00001C1C
	add r0, r3, r4
	ldrb r0, [r0]
	mov r2, #0xA5
	lsl r2, r2, #5
	add r1, r3, r2
	add r1, r0, r1
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r4, #0xC4
	lsl r4, r4, #3
	add r3, r3, r4
	add r2, r2, r3
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, #0
	mov r2, #0
	bl CardDetail_Init
	mov r0, #0
	b _08070A16
	.align 2, 0
_080709EC: .4byte 0x000003E6
_080709F0: .4byte 0x02013D90
_080709F4: .4byte 0x00001C1C
_080709F8:
	ldr r0, _08070A00 @ =0x000003E6
	add r1, r2, r0
	mov r0, #9
	b _08070A0A
_08070A00: .4byte 0x000003E6
_08070A04:
	ldr r3, _08070A10 @ =0x000003E6
	add r1, r2, r3
	mov r0, #1
_08070A0A:
	strb r0, [r1]
	mov r0, #0
	b _08070A16
_08070A10: .4byte 0x000003E6
_08070A14:
	mov r0, #1
_08070A16:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end ProhibitCardSelect_SwitchScreen

