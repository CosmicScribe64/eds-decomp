	thumb_func_start CardMenu_DrawCardPreview
CardMenu_DrawCardPreview: @ 0x0801D9F8
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r5, _0801DA54 @ =0x0201CFB0
	ldr r0, _0801DA58 @ =0x00000828
	add r4, r5, r0
	ldr r0, [r4]
	cmp r0, #0xD
	bgt _0801DA12
	cmp r0, #0xC
	blt _0801DA12
	b _0801DBF2
_0801DA12:
	bl DuelCursor_GetCardId
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GetCardIconObjTile
	mov r1, #0xA0
	lsl r1, r1, #5
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	ldr r0, _0801DA5C @ =0x020192E0
	ldr r2, _0801DA60 @ =0x00001B2C
	add r0, r0, r2
	mov r1, #0xF0
	lsl r1, r1, #2
	ldrh r0, [r0]
	and r1, r0
	cmp r1, #0x40
	beq _0801DA44
	mov r0, #0xE0
	lsl r0, r0, #1
	cmp r1, r0
	bne _0801DAA0
_0801DA44:
	ldr r0, [r4]
	cmp r0, #5
	beq _0801DA6A
	cmp r0, #5
	bgt _0801DA64
	cmp r0, #0
	beq _0801DA6A
	b _0801DAA0
_0801DA54: .4byte 0x0201CFB0
_0801DA58: .4byte 0x00000828
_0801DA5C: .4byte 0x020192E0
_0801DA60: .4byte 0x00001B2C
_0801DA64:
	cmp r0, #0xA
	beq _0801DA94
	b _0801DAA0
_0801DA6A:
	ldr r2, _0801DA88 @ =0x0201CFB0
	ldr r3, _0801DA8C @ =0x00000824
	add r0, r2, r3
	ldr r0, [r0]
	ldr r4, _0801DA90 @ =0x00000828
	add r1, r2, r4
	add r3, #8
	add r2, r2, r3
	ldr r1, [r1]
	ldr r2, [r2]
	add r1, r1, r2
	bl ClearZoneTiles
	b _0801DAA0
	.align 2, 0
_0801DA88: .4byte 0x0201CFB0
_0801DA8C: .4byte 0x00000824
_0801DA90: .4byte 0x00000828
_0801DA94:
	ldr r4, _0801DB14 @ =0x00000824
	add r0, r5, r4
	ldr r0, [r0]
	mov r1, #0xA
	bl ClearZoneTiles
_0801DAA0:
	ldr r0, _0801DB18 @ =0x020192E0
	ldr r1, _0801DB1C @ =0x00001B2C
	add r1, r1, r0
	mov r9, r1
	ldrh r2, [r1]
	lsl r0, r2, #0x16
	lsr r0, r0, #0x1C
	cmp r0, #7
	bls _0801DAB4
	b _0801DBE4
_0801DAB4:
	ldr r4, _0801DB20 @ =0x0201CFB0
	ldr r3, _0801DB14 @ =0x00000824
	add r5, r4, r3
	ldr r0, [r5]
	ldr r1, _0801DB24 @ =0x00000828
	add r7, r4, r1
	ldr r1, [r7]
	ldr r2, _0801DB28 @ =0x0000082C
	add r4, r4, r2
	ldr r2, [r4]
	bl GetAreaX
	add r6, r0, #0
	ldr r0, [r5]
	ldr r1, [r7]
	ldr r2, [r4]
	bl GetAreaY
	add r3, r0, #0
	mov r0, #0x68
	sub r0, r0, r6
	mov r4, r9
	ldrh r4, [r4]
	lsl r2, r4, #0x16
	lsr r1, r2, #0x1C
	mul r0, r1
	cmp r0, #0
	bge _0801DAEE
	add r0, #7
_0801DAEE:
	asr r5, r0, #3
	mov r0, #0x20
	sub r0, r0, r3
	lsr r1, r2, #0x1C
	mul r0, r1
	cmp r0, #0
	bge _0801DAFE
	add r0, #7
_0801DAFE:
	asr r4, r0, #3
	add r5, r5, r6
	add r4, r4, r3
	ldr r1, [r7]
	cmp r1, #5
	beq _0801DB30
	cmp r1, #5
	bgt _0801DB2C
	cmp r1, #0
	beq _0801DB30
	b _0801DBAC
_0801DB14: .4byte 0x00000824
_0801DB18: .4byte 0x020192E0
_0801DB1C: .4byte 0x00001B2C
_0801DB20: .4byte 0x0201CFB0
_0801DB24: .4byte 0x00000828
_0801DB28: .4byte 0x0000082C
_0801DB2C:
	cmp r1, #0xA
	bne _0801DBAC
_0801DB30:
	ldr r2, _0801DB84 @ =0x0201CFB0
	ldr r1, _0801DB88 @ =0x00000824
	add r0, r2, r1
	ldr r3, [r0]
	mov r0, #1
	and r3, r0
	add r1, #4
	add r0, r2, r1
	ldr r1, [r0]
	ldr r0, _0801DB8C @ =0x0000082C
	add r2, r2, r0
	ldr r0, [r2]
	add r1, r1, r0
	mov r0, #0x94
	mul r1, r0
	ldr r0, _0801DB90 @ =0x00000D64
	mul r0, r3
	add r1, r1, r0
	ldr r2, _0801DB94 @ =0x0201930C
	add r1, r1, r2
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0801DBAC
	mov r1, #0xD8
	lsl r1, r1, #5
	add r0, r2, r1
	ldrh r0, [r0]
	lsl r0, r0, #0x16
	lsr r2, r0, #0x1C
	cmp r2, #0
	blt _0801DBAC
	cmp r2, #2
	bgt _0801DB98
	add r0, r2, #0
	lsl r0, r0, #0x14
	mov r2, #0x88
	lsl r2, r2, #0x13
	add r0, r0, r2
	b _0801DBA8
	.align 2, 0
_0801DB84: .4byte 0x0201CFB0
_0801DB88: .4byte 0x00000824
_0801DB8C: .4byte 0x0000082C
_0801DB90: .4byte 0x00000D64
_0801DB94: .4byte 0x0201930C
_0801DB98:
	cmp r2, #4
	bgt _0801DBAC
	lsr r1, r0, #0x1C
	mov r0, #5
	sub r0, r0, r1
	lsl r0, r0, #4
	add r0, r8
	lsl r0, r0, #0x10
_0801DBA8:
	lsr r0, r0, #0x10
	mov r8, r0
_0801DBAC:
	cmp r4, #0x9F
	bhi _0801DBF2
	lsl r0, r4, #0x10
	orr r5, r0
	ldr r1, _0801DBD8 @ =0x081A4444
	ldr r0, _0801DBDC @ =0x020192E0
	ldr r3, _0801DBE0 @ =0x00001B2C
	add r0, r0, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x16
	lsr r0, r0, #0x1C
	lsl r0, r0, #1
	add r0, r0, r1
	ldrh r0, [r0]
	lsl r3, r0, #0x10
	add r0, r5, #0
	mov r1, #0x80
	mov r2, r8
	bl AddAffineSprite
	b _0801DBF2
	.align 2, 0
_0801DBD8: .4byte gShrinkScaleSteps
_0801DBDC: .4byte 0x020192E0
_0801DBE0: .4byte 0x00001B2C
_0801DBE4:
	ldr r0, _0801DC00 @ =0x00200068
	mov r3, #0x80
	lsl r3, r3, #0x10
	mov r1, #0x80
	mov r2, r8
	bl AddAffineSprite
_0801DBF2:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801DC00: .4byte 0x00200068
	thumb_func_end CardMenu_DrawCardPreview

