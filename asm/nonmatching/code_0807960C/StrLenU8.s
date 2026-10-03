	thumb_func_start StrLenU8
StrLenU8: @ 0x0807A190
	add r1, r0, #0
	mov r2, #0
	b _0807A19C
_0807A196:
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
_0807A19C:
	ldrb r0, [r1]
	add r1, #1
	cmp r0, #0
	bne _0807A196
	add r0, r2, #0
	bx lr
	thumb_func_end StrLenU8

