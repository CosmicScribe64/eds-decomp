	thumb_func_start EffectRiryokuPrepare
EffectRiryokuPrepare: @ 0x0802EFC4
	push {r4, lr}
	mov r0, #0
	mov r1, #1
	mov r2, #0
	bl CountMonstersFiltered
	add r4, r0, #0
	mov r0, #1
	mov r1, #1
	mov r2, #0
	bl CountMonstersFiltered
	add r4, r4, r0
	cmp r4, #1
	ble _0802EFE6
	mov r0, #1
	b _0802EFE8
_0802EFE6:
	mov r0, #0
_0802EFE8:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectRiryokuPrepare
	.align 2, 0

