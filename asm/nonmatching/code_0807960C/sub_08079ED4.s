	thumb_func_start sub_08079ED4
sub_08079ED4: @ 0x08079ED4
	add r2, r0, #0
	ldrb r0, [r2]
	sub r0, #0x30
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #9
	bhi _08079EEE
	ldrb r1, [r2]
	sub r1, #0x30
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r1, r0, #1
	b _08079EF0
_08079EEE:
	mov r1, #0
_08079EF0:
	ldrb r0, [r2, #1]
	sub r0, #0x30
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #9
	bhi _08079F08
	add r0, r1, #0
	sub r0, #0x30
	ldrb r2, [r2, #1]
	add r0, r2, r0
	lsl r0, r0, #0x18
	b _08079F0A
_08079F08:
	lsl r0, r1, #0x18
_08079F0A:
	lsr r0, r0, #0x18
	bx lr
	thumb_func_end sub_08079ED4
	.align 2, 0

