	thumb_func_start sub_08030B4C
sub_08030B4C: @ 0x08030B4C
	push {lr}
	add r2, r0, #0
	ldrb r3, [r2, #2]
	mov r0, #0xE
	and r0, r3
	cmp r0, #6
	beq _08030B66
	add r0, r2, #0
	bl sub_08030880
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08030B80
_08030B66:
	mov r0, #1
	and r0, r3
	mov r3, #0xD0
	cmp r0, #0
	beq _08030B72
	ldr r3, _08030B84 @ =0x000080D0
_08030B72:
	ldrh r1, [r2]
	add r0, r3, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0
_08030B80:
	pop {r1}
	bx r1
_08030B84: .4byte 0x000080D0
	thumb_func_end sub_08030B4C

