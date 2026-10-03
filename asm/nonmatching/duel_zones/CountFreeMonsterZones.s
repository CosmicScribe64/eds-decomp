	thumb_func_start CountFreeMonsterZones
CountFreeMonsterZones: @ 0x08008A1C
	push {r4, r5, r6, lr}
	add r6, r0, #0
	mov r5, #0
	mov r4, #0
_08008A24:
	add r0, r6, #0
	add r1, r4, #0
	bl IsMonsterZoneFree
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08008A34
	add r5, #1
_08008A34:
	add r4, #1
	cmp r4, #4
	ble _08008A24
	add r0, r5, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end CountFreeMonsterZones
	.align 2, 0

