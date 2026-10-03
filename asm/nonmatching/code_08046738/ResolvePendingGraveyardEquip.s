	thumb_func_start ResolvePendingGraveyardEquip
ResolvePendingGraveyardEquip: @ 0x08046C6C
	push {lr}
	lsl r2, r2, #0x10
	cmp r2, #0
	beq _08046C90
	mov r2, #0xD9
	cmp r0, #0
	beq _08046C7C
	ldr r2, _08046C8C @ =0x000080D9
_08046C7C:
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	b _08046CA6
_08046C8C: .4byte 0x000080D9
_08046C90:
	mov r2, #0xD8
	cmp r0, #0
	beq _08046C98
	ldr r2, _08046CAC @ =0x000080D8
_08046C98:
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08046CA6:
	pop {r0}
	bx r0
	.align 2, 0
_08046CAC: .4byte 0x000080D8
	thumb_func_end ResolvePendingGraveyardEquip

