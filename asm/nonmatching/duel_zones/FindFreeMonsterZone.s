	thumb_func_start FindFreeMonsterZone
FindFreeMonsterZone: @ 0x08008A44
	push {r4, r5, lr}
	add r5, r0, #0
	mov r4, #0
_08008A4A:
	add r0, r5, #0
	add r1, r4, #0
	bl IsMonsterZoneFree
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08008A5C
	add r0, r4, #0
	b _08008A66
_08008A5C:
	add r4, #1
	cmp r4, #4
	ble _08008A4A
	mov r0, #1
	neg r0, r0
_08008A66:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end FindFreeMonsterZone

