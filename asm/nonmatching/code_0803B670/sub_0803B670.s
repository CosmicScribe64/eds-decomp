	thumb_func_start sub_0803B670
sub_0803B670: @ 0x0803B670
	push {r4, lr}
	add r4, r0, #0
	mov r0, #7
	ldrb r1, [r4, #0xA]
	and r0, r1
	cmp r0, #2
	bne _0803B6B4
	mov r0, #1
	ldrb r3, [r4, #2]
	and r0, r3
	mov r3, #0x87
	cmp r0, #0
	beq _0803B68C
	ldr r3, _0803B6BC @ =0x00008087
_0803B68C:
	ldrh r0, [r4, #2]
	lsl r1, r0, #0x16
	lsr r1, r1, #0x1A
	ldrh r2, [r4, #0xC]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldrb r3, [r4, #2]
	lsl r1, r3, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	ldrh r3, [r4, #2]
	lsl r2, r3, #0x16
	lsr r2, r2, #0x1A
	lsl r2, r2, #8
	orr r1, r2
	ldrh r2, [r4, #0xE]
	bl sub_08017B04
_0803B6B4:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
_0803B6BC: .4byte 0x00008087
	thumb_func_end sub_0803B670

