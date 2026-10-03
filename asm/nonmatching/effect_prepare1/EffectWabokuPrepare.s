	thumb_func_start EffectWabokuPrepare
EffectWabokuPrepare: @ 0x0802E754
	add r3, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802E780
	ldr r0, _0802E778 @ =0x020192E0
	ldr r1, _0802E77C @ =0x00001B12
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r1, r0, #0x1E
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r1, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r1, r0
	beq _0802E780
	mov r0, #1
	b _0802E782
	.align 2, 0
_0802E778: .4byte 0x020192E0
_0802E77C: .4byte 0x00001B12
_0802E780:
	mov r0, #0
_0802E782:
	bx lr
	thumb_func_end EffectWabokuPrepare

