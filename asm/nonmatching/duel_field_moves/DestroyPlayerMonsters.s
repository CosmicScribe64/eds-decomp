	thumb_func_start DestroyPlayerMonsters
DestroyPlayerMonsters: @ 0x08018620
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	mov r4, #0
	mov r0, #1
	and r0, r5
	ldr r1, _0801865C @ =0x00000D64
	add r6, r0, #0
	mul r6, r1
_08018634:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _08018660 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08018650
	add r0, r5, #0
	add r1, r4, #0
	add r2, r7, #0
	bl DestroyFieldCard
_08018650:
	add r4, #1
	cmp r4, #4
	ble _08018634
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0801865C: .4byte 0x00000D64
_08018660: .4byte 0x0201930C
	thumb_func_end DestroyPlayerMonsters

