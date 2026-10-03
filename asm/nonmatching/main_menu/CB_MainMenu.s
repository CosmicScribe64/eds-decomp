	thumb_func_start CB_MainMenu
CB_MainMenu: @ 0x08003AA4
	push {r4, r5, lr}
	ldr r1, _08003ADC @ =0x081984F4
	ldr r5, _08003AE0 @ =0x03000040
	ldr r0, _08003AE4 @ =0x00004859
	add r4, r5, r0
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08003AEC
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08003AD8
	ldrb r0, [r4]
	add r0, #1
	mov r1, #0
	strb r0, [r4]
	ldr r2, _08003AE8 @ =0x0000485A
	add r0, r5, r2
	strb r1, [r0]
	add r2, #1
	add r0, r5, r2
	strb r1, [r0]
_08003AD8:
	mov r0, #0
	b _08003AEE
_08003ADC: .4byte gMainMenuSteps
_08003AE0: .4byte 0x03000040
_08003AE4: .4byte 0x00004859
_08003AE8: .4byte 0x0000485A
_08003AEC:
	mov r0, #1
_08003AEE:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end CB_MainMenu

