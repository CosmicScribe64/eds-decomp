	thumb_func_start LinkBattle_Init
LinkBattle_Init: @ 0x0801A7F4
	push {r4, r5, lr}
	ldr r0, _0801A810 @ =0x03000040
	ldr r1, _0801A814 @ =0x00004859
	add r4, r0, r1
	ldrb r2, [r4]
	add r5, r0, #0
	cmp r2, #0
	beq _0801A818
	cmp r2, #1
	beq _0801A83E
	bl LinkShutdown
	mov r0, #1
	b _0801A882
_0801A810: .4byte 0x03000040
_0801A814: .4byte 0x00004859
_0801A818:
	bl LoadPlayerDeckFromSave
	ldr r0, _0801A830 @ =0x020192E4
	ldrb r0, [r0, #3]
	cmp r0, #0x27
	bhi _0801A838
	ldr r2, _0801A834 @ =0x00004858
	add r1, r5, r2
	mov r0, #0xA
	strb r0, [r1]
	b _0801A880
	.align 2, 0
_0801A830: .4byte 0x020192E4
_0801A834: .4byte 0x00004858
_0801A838:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0801A83E:
	ldr r0, _0801A888 @ =0x0000488A
	add r1, r5, r0
	mov r0, #0x10
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	bl LinkInit
	ldr r0, _0801A88C @ =0x02017FB0
	ldr r1, _0801A890 @ =0x00000494
	bl MemClear16
	ldr r1, _0801A894 @ =0x020192E0
	ldr r0, _0801A898 @ =0x00001B12
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r1, _0801A89C @ =0x00004859
	add r0, r5, r1
	ldrb r1, [r0]
	add r1, #1
	mov r2, #0
	strb r1, [r0]
	ldr r1, _0801A8A0 @ =0x0000485A
	add r0, r5, r1
	strb r2, [r0]
	add r1, #1
	add r0, r5, r1
	strb r2, [r0]
_0801A880:
	mov r0, #0
_0801A882:
	pop {r4, r5}
	pop {r1}
	bx r1
_0801A888: .4byte 0x0000488A
_0801A88C: .4byte 0x02017FB0
_0801A890: .4byte 0x00000494
_0801A894: .4byte 0x020192E0
_0801A898: .4byte 0x00001B12
_0801A89C: .4byte 0x00004859
_0801A8A0: .4byte 0x0000485A
	thumb_func_end LinkBattle_Init

