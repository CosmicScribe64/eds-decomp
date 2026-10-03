	thumb_func_start SaveGame
SaveGame: @ 0x080754BC
	push {r4, r5, r6, r7, lr}
	bl UpdateSaveChecksum
	mov r4, #0
	ldr r7, _080754F0 @ =0x02011C20
	mov r6, #0xE0
	lsl r6, r6, #0x14
	ldr r5, _080754F4 @ =0x00002170
_080754CC:
	add r0, r7, #0
	add r1, r6, #0
	add r2, r5, #0
	bl WriteSram
	add r0, r7, #0
	add r1, r6, #0
	add r2, r5, #0
	bl VerifySram
	cmp r0, #0
	beq _080754EA
	add r4, #1
	cmp r4, #0x1F
	ble _080754CC
_080754EA:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080754F0: .4byte 0x02011C20
_080754F4: .4byte 0x00002170
	thumb_func_end SaveGame

