	thumb_func_start CB_Title
CB_Title: @ 0x080057BC
	push {r4, r5, lr}
	ldr r1, _08005800 @ =0x081988B0
	ldr r5, _08005804 @ =0x03000040
	ldr r0, _08005808 @ =0x00004878
	add r4, r5, r0
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08005810
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080057FC
	ldrb r0, [r4]
	add r0, #1
	mov r1, #0
	strb r0, [r4]
	ldr r2, _0800580C @ =0x00004858
	add r0, r5, r2
	strb r1, [r0]
	add r2, #1
	add r0, r5, r2
	strb r1, [r0]
	add r2, #1
	add r0, r5, r2
	strb r1, [r0]
	add r2, #1
	add r0, r5, r2
	strb r1, [r0]
_080057FC:
	mov r0, #0
	b _08005812
_08005800: .4byte gTitleSteps
_08005804: .4byte 0x03000040
_08005808: .4byte 0x00004878
_0800580C: .4byte 0x00004858
_08005810:
	mov r0, #1
_08005812:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end CB_Title

