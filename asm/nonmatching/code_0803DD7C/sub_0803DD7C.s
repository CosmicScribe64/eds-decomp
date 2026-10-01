	thumb_func_start sub_0803DD7C
sub_0803DD7C: @ 0x0803DD7C
	add r3, r0, #0
	lsl r1, r1, #0x10
	lsr r2, r1, #0x10
	cmp r3, #0
	beq _0803DDAA
	ldrb r0, [r3, #0xA]
	lsl r1, r0, #0x1D
	lsr r1, r1, #0x1C
	add r0, r3, #0
	add r0, #0xC
	add r0, r0, r1
	strh r2, [r0]
	ldrb r2, [r3, #0xA]
	lsl r1, r2, #0x1D
	lsr r1, r1, #0x1D
	add r1, #1
	mov r0, #7
	and r1, r0
	mov r0, #8
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #0xA]
_0803DDAA:
	bx lr
	thumb_func_end sub_0803DD7C

