	thumb_func_start GetDialogueIndex
GetDialogueIndex: @ 0x08001B98
	push {r4, r5, lr}
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	mov r0, #0
	mov r4, #0xF5
	lsl r4, r4, #1
	ldr r1, _08001BC4 @ =0x0813ADF4
	mov r3, #0xC1
	lsl r3, r3, #2
_08001BAA:
	ldrh r5, [r1]
	cmp r5, r2
	beq _08001BBC
	add r1, r1, r3
	add r0, #1
	cmp r0, r4
	bls _08001BAA
	mov r0, #0xF5
	lsl r0, r0, #1
_08001BBC:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08001BC4: .4byte gDialogueTable
	thumb_func_end GetDialogueIndex

