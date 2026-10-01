	thumb_func_start sub_08076FF8
sub_08076FF8: @ 0x08076FF8
	push {lr}
	ldr r0, _0807700C @ =0x081A78A8
	ldr r1, _08077010 @ =0x02013D86
	mov r2, #8
	bl sub_08076FC4
	cmp r0, #0
	beq _08077014
	mov r0, #0
	b _08077016
_0807700C: .4byte gUnk_081A78A8
_08077010: .4byte 0x02013D86
_08077014:
	mov r0, #1
_08077016:
	pop {r1}
	bx r1
	thumb_func_end sub_08076FF8
	.align 2, 0

