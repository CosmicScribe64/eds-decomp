	thumb_func_start EffectTimeMachinePrepare
EffectTimeMachinePrepare: @ 0x0802DC58
	add r1, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802DC6E
	mov r0, #0xFC
	ldrb r1, [r1, #3]
	and r0, r1
	cmp r0, #0x4C
	bne _0802DC6E
	mov r0, #1
	b _0802DC70
_0802DC6E:
	mov r0, #0
_0802DC70:
	bx lr
	thumb_func_end EffectTimeMachinePrepare
	.align 2, 0

