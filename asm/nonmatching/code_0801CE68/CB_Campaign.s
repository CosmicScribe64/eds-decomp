	thumb_func_start CB_Campaign
CB_Campaign: @ 0x0801D1E8
	push {r4, r5, lr}
	ldr r1, _0801D238 @ =0x08198EAC
	ldr r5, _0801D23C @ =0x03000040
	ldr r0, _0801D240 @ =0x00004857
	add r4, r5, r0
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0801D254
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0801D234
	ldrb r0, [r4]
	add r0, #1
	mov r2, #0
	strb r0, [r4]
	ldr r3, _0801D244 @ =0x0000488A
	add r1, r5, r3
	ldr r0, _0801D248 @ =0xFFFFF00F
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	ldr r1, _0801D24C @ =0x00004858
	add r0, r5, r1
	strb r2, [r0]
	ldr r3, _0801D250 @ =0x00004859
	add r0, r5, r3
	strb r2, [r0]
	add r1, #2
	add r0, r5, r1
	strb r2, [r0]
	add r3, #2
	add r0, r5, r3
	strb r2, [r0]
_0801D234:
	mov r0, #0
	b _0801D25E
_0801D238: .4byte gCampaignSteps
_0801D23C: .4byte 0x03000040
_0801D240: .4byte 0x00004857
_0801D244: .4byte 0x0000488A
_0801D248: .4byte 0xFFFFF00F
_0801D24C: .4byte 0x00004858
_0801D250: .4byte 0x00004859
_0801D254:
	mov r0, #8
	bl FadeToBlack
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
_0801D25E:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end CB_Campaign

