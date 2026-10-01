	thumb_func_start sub_08038604
sub_08038604: @ 0x08038604
	push {lr}
	add r2, r0, #0
	mov r0, #4
	ldrb r1, [r2, #4]
	and r0, r1
	cmp r0, #0
	bne _080386A8
	ldr r0, _0803862C @ =0x02017A40
	mov r3, #0xF8
	lsl r3, r3, #2
	add r0, r0, r3
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08038648
	cmp r0, #0x7F
	bgt _08038630
	cmp r0, #0x7E
	beq _08038674
	b _080386A8
	.align 2, 0
_0803862C: .4byte 0x02017A40
_08038630:
	cmp r0, #0x80
	bne _080386A8
	ldrb r2, [r2, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #6
	mov r2, #0
	mov r3, #0
	bl sub_08022678
	mov r0, #0x7F
	b _080386AA
_08038648:
	ldr r0, _0803866C @ =0x020192E0
	ldr r1, _08038670 @ =0x00001B64
	add r0, r0, r1
	ldrh r0, [r0]
	strh r0, [r2, #0xC]
	ldrb r2, [r2, #2]
	lsl r1, r2, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #6
	mov r2, #0
	mov r3, #0
	bl sub_08022678
	mov r0, #0x7E
	b _080386AA
	.align 2, 0
_0803866C: .4byte 0x020192E0
_08038670: .4byte 0x00001B64
_08038674:
	ldr r0, _0803869C @ =0x020192E0
	ldr r3, _080386A0 @ =0x00001B64
	add r1, r0, r3
	ldrh r0, [r1]
	strh r0, [r2, #0xE]
	mov r0, #1
	ldrb r3, [r2, #2]
	and r0, r3
	mov r3, #0xC7
	cmp r0, #0
	beq _0803868C
	ldr r3, _080386A4 @ =0x000080C7
_0803868C:
	ldrh r1, [r1]
	ldrh r2, [r2, #0xC]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x64
	b _080386AA
_0803869C: .4byte 0x020192E0
_080386A0: .4byte 0x00001B64
_080386A4: .4byte 0x000080C7
_080386A8:
	mov r0, #0
_080386AA:
	pop {r1}
	bx r1
	thumb_func_end sub_08038604
	.align 2, 0

