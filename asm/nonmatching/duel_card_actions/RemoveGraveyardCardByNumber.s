	thumb_func_start RemoveGraveyardCardByNumber
RemoveGraveyardCardByNumber: @ 0x080195D0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	mov ip, r2
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	mov r3, #0
	ldr r4, _08019634 @ =0x020192E4
	mov r0, #1
	and r0, r5
	ldr r1, _08019638 @ =0x00000D64
	mul r0, r1
	add r1, r0, r4
	ldrb r2, [r1, #4]
	cmp r3, r2
	bge _08019650
	ldr r2, _0801963C @ =0x00000904
	add r2, r2, r4
	mov r8, r2
	add r6, r0, #0
	add r2, r1, #0
_080195FC:
	mov r0, r8
	add r1, r6, r0
	lsl r0, r3, #2
	add r4, r1, r0
	ldr r0, [r4]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08019640 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r7
	bne _08019648
	mov r0, ip
	add r1, r4, #0
	bl CopyDuelCard
	mov r0, #0xD3
	cmp r5, #0
	beq _08019624
	ldr r0, _08019644 @ =0x000080D3
_08019624:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #1
	b _08019652
	.align 2, 0
_08019634: .4byte 0x020192E4
_08019638: .4byte 0x00000D64
_0801963C: .4byte 0x00000904
_08019640: .4byte gCardIdToNumber
_08019644: .4byte 0x000080D3
_08019648:
	add r3, #1
	ldrb r0, [r2, #4]
	cmp r3, r0
	blt _080195FC
_08019650:
	mov r0, #0
_08019652:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end RemoveGraveyardCardByNumber

