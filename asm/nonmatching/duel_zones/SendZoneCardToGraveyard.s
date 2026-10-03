	thumb_func_start SendZoneCardToGraveyard
SendZoneCardToGraveyard: @ 0x08008E44
	push {r4, r5, lr}
	add r4, r0, #0
	add r5, r1, #0
	mov r0, #1
	and r0, r4
	ldr r1, _08008E78 @ =0x00000D64
	mul r0, r1
	ldr r1, _08008E7C @ =0x0201930C
	add r0, r0, r1
	mov r1, #0x94
	mul r1, r5
	add r0, r0, r1
	bl AddCardToGraveyard
	add r0, r4, #0
	add r1, r5, #0
	bl RemoveLinksToZone
	add r0, r4, #0
	add r1, r5, #0
	bl ClearZone
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08008E78: .4byte 0x00000D64
_08008E7C: .4byte 0x0201930C
	thumb_func_end SendZoneCardToGraveyard

