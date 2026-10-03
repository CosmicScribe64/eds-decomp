	thumb_func_start MainLoop
MainLoop: @ 0x08075D70
	push {r4, r5, lr}
	ldr r5, _08075DD8 @ =0x0300044C
_08075D74:
	ldrh r1, [r5]
	ldr r2, _08075DDC @ =0x0000FFFE
	add r0, r2, #0
	and r1, r0
	ldrh r0, [r5]
	strh r1, [r5]
	ldrh r1, [r5]
	mov r0, #1
	and r0, r1
	cmp r0, #0
	bne _08075D98
	ldr r2, _08075DD8 @ =0x0300044C
	mov r3, #1
_08075D8E:
	ldrh r1, [r2]
	add r0, r3, #0
	and r0, r1
	cmp r0, #0
	beq _08075D8E
_08075D98:
	bl FrameSyncUpdate
	ldr r4, _08075DE0 @ =0x03000040
	ldr r0, _08075DE4 @ =0x00004866
	add r1, r4, r0
	mov r0, #0
	strh r0, [r1]
	mov r1, #0x82
	lsl r1, r1, #3
	add r0, r4, r1
	ldr r0, [r0]
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08075DBE
	ldr r0, _08075DE8 @ =0x08003AA5
	bl SetMainCallback
_08075DBE:
	bl DebugHook_Nop
	ldr r2, _08075DEC @ =0x0000485E
	add r1, r4, r2
	ldrh r0, [r1]
	add r0, #1
	strh r0, [r1]
	ldr r0, _08075DF0 @ =0x00004860
	add r1, r4, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _08075D74
_08075DD8: .4byte 0x0300044C
_08075DDC: .4byte 0x0000FFFE
_08075DE0: .4byte 0x03000040
_08075DE4: .4byte 0x00004866
_08075DE8: .4byte CB_MainMenu
_08075DEC: .4byte 0x0000485E
_08075DF0: .4byte 0x00004860
	thumb_func_end MainLoop

