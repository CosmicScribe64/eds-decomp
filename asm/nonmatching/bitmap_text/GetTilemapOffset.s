	thumb_func_start GetTilemapOffset
GetTilemapOffset: @ 0x0807A490
	push {r4, r5, lr}
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	lsl r1, r1, #0x10
	lsr r3, r1, #0x10
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov r1, #0
	cmp r4, #0xFF
	bls _0807A4B0
	mov r1, #0x80
	lsl r1, r1, #4
	add r0, r4, #0
	sub r0, #0x20
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
_0807A4B0:
	cmp r3, #0xFF
	bls _0807A4C6
	mov r5, #0x80
	lsl r5, r5, #4
	add r0, r1, r5
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	add r0, r3, #0
	sub r0, #0x20
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
_0807A4C6:
	cmp r2, #3
	bne _0807A4D4
	mov r2, #0x80
	lsl r2, r2, #4
	add r0, r1, r2
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_0807A4D4:
	lsl r0, r3, #5
	add r0, r4, r0
	lsl r0, r0, #1
	add r0, r1, r0
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	add r0, r1, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end GetTilemapOffset

