	thumb_func_start TextDrawNumber
TextDrawNumber: @ 0x080750E0
	push {r4, r5, lr}
	add r4, r0, #0
	add r5, r1, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	ldr r1, _08075100 @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _08075104
	add r0, r4, #0
	add r1, r5, #0
	bl TextDrawSjisNumber
	b _0807510C
_08075100: .4byte 0x02011C20
_08075104:
	add r0, r4, #0
	add r1, r5, #0
	bl TextDrawLatinNumber
_0807510C:
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end TextDrawNumber
	.align 2, 0

