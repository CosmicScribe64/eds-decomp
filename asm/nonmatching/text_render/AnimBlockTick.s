	thumb_func_start AnimBlockTick
AnimBlockTick: @ 0x0807871C
	push {r4, r5, lr}
	add r5, r0, #0
	mov r4, #0
	b _08078736
_08078724:
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, r5, r0
	bl AnimStateTick
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
_08078736:
	mov r1, #0xC8
	lsl r1, r1, #1
	add r0, r5, r1
	ldrh r0, [r0]
	cmp r4, r0
	bcc _08078724
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end AnimBlockTick

