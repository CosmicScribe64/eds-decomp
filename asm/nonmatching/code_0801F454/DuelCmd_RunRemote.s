	thumb_func_start DuelCmd_RunRemote
DuelCmd_RunRemote: @ 0x0801F628
	push {r4, r5, lr}
	ldr r0, _0801F640 @ =0x02017FB0
	ldr r1, _0801F644 @ =0x0000048C
	add r5, r0, r1
	ldrb r2, [r5]
	cmp r2, #1
	beq _0801F688
	cmp r2, #1
	bgt _0801F648
	cmp r2, #0
	beq _0801F64E
	b _0801F728
_0801F640: .4byte 0x02017FB0
_0801F644: .4byte 0x0000048C
_0801F648:
	cmp r2, #2
	beq _0801F6F4
	b _0801F728
_0801F64E:
	ldr r4, _0801F6A8 @ =0x020185C0
	ldr r2, _0801F6AC @ =0x00000484
	add r1, r0, r2
	add r0, r4, #0
	mov r2, #8
	bl MemCopy16
	ldr r0, _0801F6B0 @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x20
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	ldr r0, _0801F6B4 @ =0x0000080A
	add r1, r4, r0
	mov r0, #0x80
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r0, _0801F6B8 @ =0x0000080C
	add r4, r4, r0
	ldr r0, _0801F6BC @ =0xFFFFF01F
	ldrh r1, [r4]
	and r0, r1
	strh r0, [r4]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
_0801F688:
	bl DuelCmd_Dispatch
	ldr r0, _0801F6A8 @ =0x020185C0
	ldr r2, _0801F6B0 @ =0x0000080D
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _0801F6C8
	ldr r0, _0801F6C0 @ =0x02017FB0
	ldr r1, _0801F6C4 @ =0x0000048C
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0801F716
_0801F6A8: .4byte 0x020185C0
_0801F6AC: .4byte 0x00000484
_0801F6B0: .4byte 0x0000080D
_0801F6B4: .4byte 0x0000080A
_0801F6B8: .4byte 0x0000080C
_0801F6BC: .4byte 0xFFFFF01F
_0801F6C0: .4byte 0x02017FB0
_0801F6C4: .4byte 0x0000048C
_0801F6C8:
	ldr r1, _0801F6E8 @ =0x03000040
	ldr r2, _0801F6EC @ =0x0000485E
	add r1, r1, r2
	mov r0, #0xF
	ldrh r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0801F716
	ldr r0, _0801F6F0 @ =0x0000F042
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelLink_SendMessage
	b _0801F716
	.align 2, 0
_0801F6E8: .4byte 0x03000040
_0801F6EC: .4byte 0x0000485E
_0801F6F0: .4byte 0x0000F042
_0801F6F4:
	ldr r1, _0801F71C @ =0x020185C0
	ldr r0, _0801F720 @ =0x00000FFF
	ldrh r1, [r1]
	and r0, r1
	cmp r0, #4
	beq _0801F704
	bl PlayDuelBGM
_0801F704:
	ldr r0, _0801F724 @ =0x0000F043
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelLink_SendMessage
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
_0801F716:
	mov r0, #1
	b _0801F738
	.align 2, 0
_0801F71C: .4byte 0x020185C0
_0801F720: .4byte 0x00000FFF
_0801F724: .4byte 0x0000F043
_0801F728:
	ldr r2, _0801F740 @ =0x00000307
	add r1, r0, r2
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	mov r0, #0
_0801F738:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0801F740: .4byte 0x00000307
	thumb_func_end DuelCmd_RunRemote

