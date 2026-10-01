	thumb_func_start sub_08001E24
sub_08001E24: @ 0x08001E24
	add r2, r0, #0
	mov r0, #1
	and r0, r2
	cmp r0, #0
	beq _08001E3C
	sub r2, #1
	lsl r0, r1, #0x18
	lsr r0, r0, #0x10
	ldrb r1, [r2]
	orr r0, r1
	strh r0, [r2]
	b _08001E4A
_08001E3C:
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	ldrh r3, [r2]
	lsr r0, r3, #8
	lsl r0, r0, #8
	orr r1, r0
	strh r1, [r2]
_08001E4A:
	bx lr
	thumb_func_end sub_08001E24

