	thumb_func_start Record_DrawPageArrows
Record_DrawPageArrows: @ 0x08003F34
	push {r4, lr}
	ldr r0, _08003F78 @ =0x03000040
	ldr r1, _08003F7C @ =0x0000485E
	add r0, r0, r1
	ldrh r0, [r0]
	lsr r4, r0, #3
	mov r0, #3
	and r4, r0
	bl Record_HasPrevPage
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08003F5A
	ldr r0, _08003F80 @ =0x00200010
	lsl r2, r4, #2
	add r2, #0x20
	mov r1, #0x40
	bl AddSprite
_08003F5A:
	bl Record_HasNextPage
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08003F70
	ldr r0, _08003F84 @ =0x002000E0
	lsl r2, r4, #2
	add r2, #0x22
	mov r1, #0x40
	bl AddSprite
_08003F70:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08003F78: .4byte 0x03000040
_08003F7C: .4byte 0x0000485E
_08003F80: .4byte 0x00200010
_08003F84: .4byte 0x002000E0
	thumb_func_end Record_DrawPageArrows

