	thumb_func_start ExodiaScene_Run
ExodiaScene_Run: @ 0x08026C90
	push {r4, lr}
	ldr r1, _08026CB8 @ =0x08199DA4
	ldr r4, _08026CBC @ =0x02017A30
	ldrb r2, [r4, #0xB]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08026CC0
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08026CB2
	ldrb r0, [r4, #0xB]
	add r0, #1
	strb r0, [r4, #0xB]
_08026CB2:
	mov r0, #0
	b _08026CC2
	.align 2, 0
_08026CB8: .4byte gExodiaSceneSteps
_08026CBC: .4byte 0x02017A30
_08026CC0:
	mov r0, #1
_08026CC2:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end ExodiaScene_Run

