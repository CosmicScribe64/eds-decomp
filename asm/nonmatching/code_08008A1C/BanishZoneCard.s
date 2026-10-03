	thumb_func_start BanishZoneCard
BanishZoneCard: @ 0x08008E80
	push {r4, r5, lr}
	add r4, r0, #0
	add r5, r1, #0
	mov r0, #1
	and r0, r4
	ldr r1, _08008EAC @ =0x00000D64
	mul r0, r1
	ldr r1, _08008EB0 @ =0x0201930C
	add r0, r0, r1
	mov r1, #0x94
	mul r1, r5
	add r0, r0, r1
	bl AddCardToBanished
	add r0, r4, #0
	add r1, r5, #0
	bl ClearZone
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08008EAC: .4byte 0x00000D64
_08008EB0: .4byte 0x0201930C
	thumb_func_end BanishZoneCard

