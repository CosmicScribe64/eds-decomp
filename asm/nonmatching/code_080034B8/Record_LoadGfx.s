	thumb_func_start Record_LoadGfx
Record_LoadGfx: @ 0x08003B64
	push {r4, lr}
	sub sp, #4
	ldr r0, _08003C14 @ =0x05000220
	ldr r1, _08003C18 @ =0x087E2878
	mov r2, #0x20
	bl MemCopy16
	ldr r0, _08003C1C @ =0x06010000
	ldr r1, _08003C20 @ =0x087E2C78
	mov r4, #0x80
	lsl r4, r4, #2
	add r2, r4, #0
	bl MemCopy16
	ldr r0, _08003C24 @ =0x05000200
	ldr r1, _08003C28 @ =0x087E2A78
	mov r2, #0x20
	bl MemCopy16
	ldr r0, _08003C2C @ =0x06010400
	ldr r1, _08003C30 @ =0x087E2E78
	add r2, r4, #0
	bl MemCopy16
	ldr r0, _08003C34 @ =0x06010800
	ldr r1, _08003C38 @ =0x087E3078
	add r2, r4, #0
	bl MemCopy16
	mov r0, #0xA0
	lsl r0, r0, #0x13
	ldr r1, _08003C3C @ =0x087E3278
	mov r2, #0x20
	bl MemCopy16
	ldr r0, _08003C40 @ =0x06004080
	ldr r1, _08003C44 @ =0x087E3478
	mov r2, #0xA0
	lsl r2, r2, #1
	bl MemCopy16
	ldr r0, _08003C48 @ =0x087E4280
	str r0, [sp, #0]
	mov r0, #0
	mov r1, #0
	mov r2, #0x10
	mov r3, #0x10
	bl LoadBgImage4bppToMap
	ldr r0, _08003C4C @ =0x087E35B8
	str r0, [sp, #0]
	mov r0, #5
	mov r1, #0
	mov r2, #0x20
	mov r3, #0x80
	bl LoadBgImage4bppToMap
	mov r0, #0
	mov r1, #0
	bl Record_DrawPage
	mov r0, #1
	mov r1, #1
	bl Record_DrawPage
	ldr r2, _08003C50 @ =0x0201F814
	mov r0, #2
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #7
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	ldr r0, _08003C54 @ =0xFFFFE007
	ldrh r1, [r2]
	and r0, r1
	strh r0, [r2]
	mov r0, #0x1F
	ldrb r1, [r2, #1]
	and r0, r1
	strb r0, [r2, #1]
	mov r0, #1
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08003C14: .4byte 0x05000220
_08003C18: .4byte gRecordMarkerObjPal
_08003C1C: .4byte 0x06010000
_08003C20: .4byte gRecordMarkerObjGfx
_08003C24: .4byte 0x05000200
_08003C28: .4byte gRecordArrowObjPal
_08003C2C: .4byte 0x06010400
_08003C30: .4byte gRecordArrowObjGfxTop
_08003C34: .4byte 0x06010800
_08003C38: .4byte gRecordArrowObjGfxBottom
_08003C3C: .4byte gRecordDigitPal
_08003C40: .4byte 0x06004080
_08003C44: .4byte gRecordDigitGfx
_08003C48: .4byte gRecordFrameImage
_08003C4C: .4byte gRecordBgPatternImage
_08003C50: .4byte 0x0201F814
_08003C54: .4byte 0xFFFFE007
	thumb_func_end Record_LoadGfx

