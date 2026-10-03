	thumb_func_start CardMenu_FusionSummon
CardMenu_FusionSummon: @ 0x08049450
	push {r4, lr}
	sub sp, #0x14
	ldr r1, _08049478 @ =0x020192E0
	ldr r0, _0804947C @ =0x00001B30
	add r4, r1, r0
	ldrh r3, [r4]
	lsl r2, r3, #0x16
	lsr r0, r2, #0x18
	cmp r0, #0
	beq _08049484
	cmp r0, #1
	beq _080494A2
	ldr r2, _08049480 @ =0x00001B2C
	add r1, r1, r2
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	b _080494F6
_08049478: .4byte 0x020192E0
_0804947C: .4byte 0x00001B30
_08049480: .4byte 0x00001B2C
_08049484:
	ldr r0, _08049500 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	mov r1, #0x80
	strb r1, [r0]
	lsr r1, r2, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #2
	ldr r0, _08049504 @ =0xFFFFFC03
	and r0, r3
	orr r0, r1
	strh r0, [r4]
_080494A2:
	ldr r0, _08049508 @ =0x08624A0A
	ldrh r1, [r0]
	mov r0, sp
	strh r1, [r0]
	mov r2, sp
	ldrb r1, [r2, #2]
	mov r0, #2
	neg r0, r0
	and r0, r1
	strb r0, [r2, #2]
	mov r1, sp
	mov r0, #5
	neg r0, r0
	ldrb r2, [r1, #4]
	and r0, r2
	strb r0, [r1, #4]
	mov r0, sp
	mov r1, #0
	bl EffectPolymerizationResolve
	ldr r1, _08049500 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r1, r1, r2
	strb r0, [r1]
	lsl r0, r0, #0x18
	cmp r0, #0
	bne _080494F6
	ldr r2, _0804950C @ =0x020192E0
	ldr r0, _08049510 @ =0x00001B30
	add r2, r2, r0
	ldrh r3, [r2]
	lsl r1, r3, #0x16
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #2
	ldr r0, _08049504 @ =0xFFFFFC03
	and r0, r3
	orr r0, r1
	strh r0, [r2]
_080494F6:
	add sp, #0x14
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08049500: .4byte 0x02017A40
_08049504: .4byte 0xFFFFFC03
_08049508: .4byte gUnk_08624A0A
_0804950C: .4byte 0x020192E0
_08049510: .4byte 0x00001B30
	thumb_func_end CardMenu_FusionSummon

