	thumb_func_start sub_080729F8
sub_080729F8: @ 0x080729F8
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	ldr r0, _08072A0C @ =0x00008169
	cmp r1, r0
	beq _08072A08
	add r0, #0xC
	cmp r1, r0
	bne _08072A10
_08072A08:
	mov r0, #1
	b _08072A12
_08072A0C: .4byte 0x00008169
_08072A10:
	mov r0, #0
_08072A12:
	bx lr
	thumb_func_end sub_080729F8

