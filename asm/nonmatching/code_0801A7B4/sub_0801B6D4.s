	thumb_func_start sub_0801B6D4
sub_0801B6D4: @ 0x0801B6D4
	push {lr}
_0801B6D6:
	bl sub_08076F9C
	mov r1, #0x3C
	bl __umodsi3
	ldr r1, _0801B6F4 @ =0x08081A6C
	lsl r0, r0, #1
	add r0, r0, r1
	ldrh r1, [r0]
	add r2, r1, #0
	ldr r0, _0801B6F8 @ =0x0000FFFF
	cmp r1, r0
	bne _0801B6FC
	mov r0, #0
	b _0801B732
_0801B6F4: .4byte gUnk_08081A6C
_0801B6F8: .4byte 0x0000FFFF
_0801B6FC:
	ldr r0, _0801B714 @ =0x000007CF
	cmp r1, r0
	bhi _0801B720
	ldr r2, _0801B718 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _0801B71C @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _0801B732
	.align 2, 0
_0801B714: .4byte 0x000007CF
_0801B718: .4byte 0x000007FF
_0801B71C: .4byte gUnk_08623DF4
_0801B720:
	ldr r1, _0801B74C @ =0xFFFFF830
	add r0, r2, r1
	ldr r1, _0801B750 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0801B754 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	add r0, #1
_0801B732:
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldr r1, _0801B758 @ =0x02011C20
	lsl r0, r2, #2
	add r0, r0, r1
	ldrh r0, [r0, #8]
	lsl r0, r0, #0x16
	cmp r0, #0
	beq _0801B6D6
	add r0, r2, #0
	pop {r1}
	bx r1
	.align 2, 0
_0801B74C: .4byte 0xFFFFF830
_0801B750: .4byte 0x000007FF
_0801B754: .4byte gUnk_08623DF4
_0801B758: .4byte 0x02011C20
	thumb_func_end sub_0801B6D4

