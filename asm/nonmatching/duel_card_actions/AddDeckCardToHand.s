	thumb_func_start AddDeckCardToHand
AddDeckCardToHand: @ 0x0801970C
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	mov r3, #0
	ldr r2, _08019764 @ =0x020192E4
	mov r0, #1
	and r0, r4
	ldr r1, _08019768 @ =0x00000D64
	mul r0, r1
	add r1, r0, r2
	ldrb r5, [r1, #3]
	cmp r3, r5
	bge _08019780
	ldr r5, _0801976C @ =0x000007C4
	add r5, r5, r2
	mov ip, r5
	add r6, r0, #0
	add r5, r1, #0
_08019732:
	mov r1, ip
	add r0, r6, r1
	lsl r1, r3, #2
	add r0, r0, r1
	ldr r2, [r0]
	lsl r0, r2, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08019770 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r7
	bne _08019778
	mov r0, #0x64
	cmp r4, #0
	beq _08019752
	ldr r0, _08019774 @ =0x00008064
_08019752:
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	lsr r2, r2, #0x10
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #1
	b _08019782
	.align 2, 0
_08019764: .4byte 0x020192E4
_08019768: .4byte 0x00000D64
_0801976C: .4byte 0x000007C4
_08019770: .4byte gCardIdToNumber
_08019774: .4byte 0x00008064
_08019778:
	add r3, #1
	ldrb r0, [r5, #3]
	cmp r3, r0
	blt _08019732
_08019780:
	mov r0, #0
_08019782:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end AddDeckCardToHand

