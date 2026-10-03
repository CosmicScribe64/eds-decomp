	thumb_func_start EffectUmiOnFieldPrepare
EffectUmiOnFieldPrepare: @ 0x0802FB48
	push {lr}
	bl GetFaceUpFieldMagicNumber
	ldr r1, _0802FB58 @ =0x0000014D
	cmp r0, r1
	beq _0802FB5C
	mov r0, #0
	b _0802FB5E
_0802FB58: .4byte 0x0000014D
_0802FB5C:
	mov r0, #1
_0802FB5E:
	pop {r1}
	bx r1
	thumb_func_end EffectUmiOnFieldPrepare
	.align 2, 0

