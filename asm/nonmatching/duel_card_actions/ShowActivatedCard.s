	thumb_func_start ShowActivatedCard
ShowActivatedCard: @ 0x080197C0
	push {lr}
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r2, #0x72
	cmp r0, #0
	beq _080197CE
	ldr r2, _080197DC @ =0x00008072
_080197CE:
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	pop {r0}
	bx r0
_080197DC: .4byte 0x00008072
	thumb_func_end ShowActivatedCard

