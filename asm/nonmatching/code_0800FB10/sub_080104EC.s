	thumb_func_start sub_080104EC
sub_080104EC: @ 0x080104EC
	push {r4, lr}
	ldr r3, _08010524 @ =0x020185C0
	ldr r4, _08010528 @ =0x020192E4
	ldrh r0, [r3, #2]
	lsl r1, r0, #2
	ldrh r0, [r3]
	lsr r2, r0, #0xF
	ldr r0, _0801052C @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	add r1, r1, r4
	ldr r2, _08010530 @ =0x00000907
	add r1, r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r0, _08010534 @ =0x0000080D
	add r3, r3, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r3]
	and r0, r1
	strb r0, [r3]
	pop {r4}
	pop {r0}
	bx r0
_08010524: .4byte 0x020185C0
_08010528: .4byte 0x020192E4
_0801052C: .4byte 0x00000D64
_08010530: .4byte 0x00000907
_08010534: .4byte 0x0000080D
	thumb_func_end sub_080104EC

