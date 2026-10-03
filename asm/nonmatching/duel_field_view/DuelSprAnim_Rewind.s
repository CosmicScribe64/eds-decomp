	thumb_func_start DuelSprAnim_Rewind
DuelSprAnim_Rewind: @ 0x08024218
	push {lr}
	ldr r0, _08024224 @ =0x0201D7F8
	bl SprAnimRewind
	pop {r0}
	bx r0
_08024224: .4byte 0x0201D7F8
	thumb_func_end DuelSprAnim_Rewind

