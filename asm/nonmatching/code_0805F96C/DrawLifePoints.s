	thumb_func_start DrawLifePoints
DrawLifePoints: @ 0x08060934
	push {lr}
	add r3, r1, #0
	cmp r0, #0
	beq _08060942
	cmp r0, #1
	beq _08060954
	b _08060960
_08060942:
	ldr r1, _08060950 @ =0x00000267
	mov r0, #3
	mov r2, #0
	bl DrawBgNumber
	b _08060960
	.align 2, 0
_08060950: .4byte 0x00000267
_08060954:
	mov r1, #0xA5
	lsl r1, r1, #2
	mov r0, #3
	mov r2, #0
	bl DrawBgNumber
_08060960:
	pop {r0}
	bx r0
	thumb_func_end DrawLifePoints

