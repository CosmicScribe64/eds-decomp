	thumb_func_start DrawZoneTiles
DrawZoneTiles: @ 0x08060ECC
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	add r4, r1, #0
	mov r0, #1
	mov r9, r0
	add r0, r6, #0
	mov r1, r9
	and r0, r1
	ldr r1, _08060F70 @ =0x00000D64
	mul r1, r0
	ldr r0, _08060F74 @ =0x0201930C
	add r1, r1, r0
	mov r0, #0x94
	mul r0, r4
	add r5, r1, r0
	ldr r0, [r5]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	mov ip, r2
	ldr r3, _08060F78 @ =0x081A42A4
	lsl r1, r4, #3
	lsl r0, r6, #7
	add r1, r1, r0
	add r0, r1, r3
	ldr r0, [r0]
	cmp r0, #0
	bge _08060F0A
	add r0, #7
_08060F0A:
	asr r0, r0, #3
	mov r8, r0
	add r0, r3, #4
	add r0, r1, r0
	ldr r0, [r0]
	cmp r0, #0
	bge _08060F1A
	add r0, #7
_08060F1A:
	asr r7, r0, #3
	cmp r2, #0
	bne _08060F88
	mov r0, r8
	add r1, r7, #0
	bl ClearTileBlock4x4
	cmp r4, #4
	bgt _08060FBE
	add r0, r6, #0
	add r1, r4, #0
	bl IsMonsterZoneFree
	cmp r0, #0
	bne _08060FBE
	mov r1, r8
	add r1, #1
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r0, r7, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xB
	add r1, r1, r0
	lsl r1, r1, #1
	ldr r0, _08060F7C @ =0x03001C5C
	add r1, r1, r0
	mov r2, #0x92
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
	ldr r3, _08060F80 @ =0x00001241
	add r0, r3, #0
	strh r0, [r1, #2]
	add r2, r1, #0
	add r2, #0x40
	add r3, #1
	add r0, r3, #0
	strh r0, [r2]
	add r1, #0x42
	ldr r2, _08060F84 @ =0x00001243
	add r0, r2, #0
	strh r0, [r1]
	b _08060FBE
_08060F70: .4byte 0x00000D64
_08060F74: .4byte 0x0201930C
_08060F78: .4byte gDuelZonePositions
_08060F7C: .4byte 0x03001C5C
_08060F80: .4byte 0x00001241
_08060F84: .4byte 0x00001243
_08060F88:
	ldr r2, _08060FCC @ =0x00001070
	mov r0, #2
	ldrb r3, [r5, #6]
	and r0, r3
	cmp r0, #0
	beq _08060FA4
	mov r0, ip
	bl GetCardIconBgTile
	mov r1, #0x80
	lsl r1, r1, #6
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
_08060FA4:
	mov r0, r9
	ldrb r5, [r5, #6]
	and r0, r5
	cmp r0, #0
	beq _08060FB6
	add r0, r2, #0
	add r0, #0x30
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
_08060FB6:
	mov r0, r8
	add r1, r7, #0
	bl FillTileBlock4x4
_08060FBE:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08060FCC: .4byte 0x00001070
	thumb_func_end DrawZoneTiles

