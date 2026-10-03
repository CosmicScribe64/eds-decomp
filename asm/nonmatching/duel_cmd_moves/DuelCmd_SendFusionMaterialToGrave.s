	thumb_func_start DuelCmd_SendFusionMaterialToGrave
DuelCmd_SendFusionMaterialToGrave: @ 0x08012FA4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r5, _080130B8 @ =0x020185C0
	ldrh r0, [r5]
	lsr r7, r0, #0xF
	ldrh r6, [r5, #2]
	ldrh r1, [r5, #4]
	mov r9, r1
	ldr r2, _080130BC @ =0x0000080A
	add r2, r2, r5
	mov sl, r2
	ldrb r3, [r2]
	lsl r0, r3, #0x19
	cmp r0, #0
	beq _08012FCC
	b _080130DC
_08012FCC:
	add r0, r7, #0
	add r1, r6, #0
	bl ClearZoneTiles
	mov r0, #1
	mov r8, r0
	add r1, r7, #0
	and r1, r0
	mov r0, #0x94
	add r2, r6, #0
	mul r2, r0
	mov ip, r2
	ldr r0, _080130C0 @ =0x00000D64
	mul r1, r0
	add r4, r2, r1
	ldr r2, _080130C4 @ =0x0201930C
	add r4, r4, r2
	mov r0, #0x10
	ldrb r3, [r4, #2]
	orr r0, r3
	strb r0, [r4, #2]
	add r1, r1, r2
	add r1, ip
	ldr r0, _080130C8 @ =0x02018DD4
	bl CopyDuelCard
	add r0, r7, #0
	add r1, r6, #0
	mov r2, r9
	bl SendZoneCardToGraveyardOrBanished
	mov r0, r8
	and r7, r0
	mov r3, #2
	neg r3, r3
	ldr r2, [sp, #0]
	and r2, r3
	orr r2, r7
	mov r1, #0x1F
	neg r1, r1
	and r2, r1
	ldr r0, _080130CC @ =0x000001FF
	and r6, r0
	lsl r0, r6, #5
	ldr r1, _080130D0 @ =0xFFFFC01F
	mov ip, r1
	and r2, r1
	orr r2, r0
	str r2, [sp, #0]
	ldrb r1, [r4, #6]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, r8
	and r0, r1
	lsl r0, r0, #0xE
	ldr r7, _080130D4 @ =0xFFFFBFFF
	add r1, r7, #0
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldrb r4, [r4, #6]
	lsl r0, r4, #0x1E
	lsr r0, r0, #0x1F
	mov r2, r8
	and r0, r2
	lsl r0, r0, #0xF
	ldr r2, _080130D8 @ =0xFFFF7FFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldr r0, _080130C8 @ =0x02018DD4
	ldr r1, [r0]
	lsl r0, r1, #0x13
	lsr r0, r0, #0x1F
	mov r2, r8
	and r0, r2
	ldr r2, [sp, #4]
	and r2, r3
	orr r2, r0
	str r2, [sp, #4]
	mov r0, #0xE
	mov r3, r9
	cmp r3, #0
	beq _08013076
	mov r0, #0xF
_08013076:
	lsl r0, r0, #1
	mov r3, #0x1F
	neg r3, r3
	and r2, r3
	orr r2, r0
	mov r0, ip
	and r2, r0
	and r2, r7
	mov r0, #0x80
	lsl r0, r0, #8
	orr r2, r0
	str r2, [sp, #4]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl DuelAnim_MoveCard
	mov r1, sl
	ldrb r2, [r1]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	mov r2, sl
	strb r0, [r2]
	b _080130EE
	.align 2, 0
_080130B8: .4byte 0x020185C0
_080130BC: .4byte 0x0000080A
_080130C0: .4byte 0x00000D64
_080130C4: .4byte 0x0201930C
_080130C8: .4byte 0x02018DD4
_080130CC: .4byte 0x000001FF
_080130D0: .4byte 0xFFFFC01F
_080130D4: .4byte 0xFFFFBFFF
_080130D8: .4byte 0xFFFF7FFF
_080130DC:
	bl DrawAllAreaTiles
	ldr r3, _08013100 @ =0x0000080D
	add r1, r5, r3
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_080130EE:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08013100: .4byte 0x0000080D
	thumb_func_end DuelCmd_SendFusionMaterialToGrave

