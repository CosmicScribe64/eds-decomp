	thumb_func_start DeckStats_Update
DeckStats_Update: @ 0x0806D010
	push {r4, r5, r6, r7, lr}
	ldr r0, _0806D0A4 @ =0x03000040
	ldr r5, _0806D0A8 @ =0x000003FF
	ldrh r0, [r0, #6]
	and r5, r0
	ldr r4, _0806D0AC @ =0x0201E138
	add r0, r4, #0
	bl FadeTick
	bl DeckStats_DrawNumbers
	ldr r0, _0806D0B0 @ =0x00001634
	add r1, r4, r0
	ldrh r0, [r1]
	add r0, #0x80
	strh r0, [r1]
	ldr r1, _0806D0B4 @ =0x00001636
	add r2, r4, r1
	ldrh r1, [r2]
	add r1, #0x80
	strh r1, [r2]
	ldr r2, _0806D0B8 @ =0x0400001C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x18
	strh r0, [r2]
	ldr r0, _0806D0BC @ =0x0400001E
	lsl r1, r1, #0x10
	lsr r1, r1, #0x18
	strh r1, [r0]
	ldrb r0, [r4, #6]
	cmp r0, #0
	bne _0806D070
	ldr r2, _0806D0C0 @ =0x0000163B
	add r0, r4, r2
	ldrb r0, [r0]
	cmp r0, #0
	bne _0806D070
	cmp r5, #2
	bhi _0806D070
	cmp r5, #1
	bcc _0806D070
	ldr r3, _0806D0C4 @ =0x00001639
	add r1, r4, r3
	mov r0, #1
	strb r0, [r1]
	mov r0, #2
	bl PlaySE
_0806D070:
	ldr r4, _0806D0C8 @ =0x0201DB20
	ldr r0, _0806D0CC @ =0x0000061E
	add r5, r4, r0
	ldrb r0, [r5]
	cmp r0, #2
	bne _0806D0D8
	ldr r1, _0806D0D0 @ =0x00001C48
	add r2, r4, r1
	mov r0, #0x1F
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	mov r1, #8
	orr r0, r1
	strb r0, [r2]
	ldr r0, _0806D0D4 @ =0x00001C3D
	add r2, r4, r0
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	mov r0, #1
	b _0806D174
_0806D0A4: .4byte 0x03000040
_0806D0A8: .4byte 0x000003FF
_0806D0AC: .4byte 0x0201E138
_0806D0B0: .4byte 0x00001634
_0806D0B4: .4byte 0x00001636
_0806D0B8: .4byte 0x0400001C
_0806D0BC: .4byte 0x0400001E
_0806D0C0: .4byte 0x0000163B
_0806D0C4: .4byte 0x00001639
_0806D0C8: .4byte 0x0201DB20
_0806D0CC: .4byte 0x0000061E
_0806D0D0: .4byte 0x00001C48
_0806D0D4: .4byte 0x00001C3D
_0806D0D8:
	cmp r0, #3
	bne _0806D10E
	ldr r1, _0806D17C @ =0x04000050
	ldr r2, _0806D180 @ =0x00003F44
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #0x10
	bl SetBldAlpha
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0x80
	lsl r3, r3, #3
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	mov r0, #0
	strb r0, [r5]
	ldr r0, _0806D184 @ =0x00001C51
	add r1, r4, r0
	mov r0, #0xFF
	strb r0, [r1]
	ldr r2, _0806D188 @ =0x00001C52
	add r1, r4, r2
	mov r0, #1
	strb r0, [r1]
_0806D10E:
	ldr r3, _0806D184 @ =0x00001C51
	add r6, r4, r3
	ldrb r1, [r6]
	mov r0, #0
	ldsb r0, [r6, r0]
	cmp r0, #0
	beq _0806D164
	ldr r0, _0806D18C @ =0x00001C50
	add r5, r4, r0
	ldrb r2, [r5]
	add r0, r1, r2
	mov r7, #0
	strb r0, [r5]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #8
	bne _0806D132
	strb r7, [r6]
_0806D132:
	ldrb r3, [r5]
	cmp r3, #0x10
	bne _0806D15E
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _0806D190 @ =0x0000FBFF
	and r0, r1
	strh r0, [r2]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r0, #0xC3
	lsl r0, r0, #3
	add r3, r4, r0
	mov r0, #0
	mov r2, #0
	bl FadeStart
	strb r7, [r6]
	ldr r1, _0806D188 @ =0x00001C52
	add r0, r4, r1
	strb r7, [r0]
_0806D15E:
	ldrb r0, [r5]
	bl SetBldAlpha
_0806D164:
	ldr r4, _0806D194 @ =0x0201DB20
	add r0, r4, #0
	bl OamListFlush
	add r0, r4, #0
	bl OamListClear
	mov r0, #0
_0806D174:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0806D17C: .4byte 0x04000050
_0806D180: .4byte 0x00003F44
_0806D184: .4byte 0x00001C51
_0806D188: .4byte 0x00001C52
_0806D18C: .4byte 0x00001C50
_0806D190: .4byte 0x0000FBFF
_0806D194: .4byte 0x0201DB20
	thumb_func_end DeckStats_Update

