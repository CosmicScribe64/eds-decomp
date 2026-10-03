	thumb_func_start GetDuelistName
GetDuelistName: @ 0x08000228
	push {r4, r5, r6, lr}
	add r6, r0, #0
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	mov r3, #1
	ldr r0, _08000250 @ =0x08139F64
	add r4, r0, #0
	add r4, #0xC8
	add r1, r0, #0
	add r1, #0x88
	add r2, r0, #0
	add r2, #0x84
_08000240:
	ldr r0, [r2]
	cmp r0, r6
	bne _08000258
	cmp r5, #0
	beq _08000254
	add r0, r1, #0
	b _08000266
	.align 2, 0
_08000250: .4byte gDuelists
_08000254:
	add r0, r4, #0
	b _08000266
_08000258:
	add r4, #0x84
	add r1, #0x84
	add r2, #0x84
	add r3, #1
	cmp r3, #0x1B
	bls _08000240
	ldr r0, _0800026C @ =0x08139F68
_08000266:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0800026C: .4byte gUnk_08139F68
	thumb_func_end GetDuelistName

