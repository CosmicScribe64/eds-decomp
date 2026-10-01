	thumb_func_start sub_0801F81C
sub_0801F81C: @ 0x0801F81C
	push {r4, lr}
	ldr r0, _0801F83C @ =0x03000040
	ldr r1, _0801F840 @ =0x0000488A
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1C
	sub r0, #1
	cmp r0, #0xC
	bls _0801F832
	b _0801F958
_0801F832:
	lsl r0, r0, #2
	ldr r1, _0801F844 @ =0x0801F848
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0801F83C: .4byte 0x03000040
_0801F840: .4byte 0x0000488A
_0801F844: .4byte 0x0801F848
_0801F848:
	.4byte _0801F87C
	.4byte _0801F884
	.4byte _0801F88A
	.4byte _0801F894
	.4byte _0801F89A
	.4byte _0801F8A4
	.4byte _0801F8AA
	.4byte _0801F8B4
	.4byte _0801F8BC
	.4byte _0801F8C4
	.4byte _0801F8CC
	.4byte _0801F8D2
	.4byte _0801F8DC
_0801F87C:
	ldr r4, _0801F880 @ =0x00000149
	b _0801F8DE
_0801F880: .4byte 0x00000149
_0801F884:
	mov r4, #0xA5
	lsl r4, r4, #1
	b _0801F8DE
_0801F88A:
	ldr r4, _0801F890 @ =0x0000014B
	b _0801F8DE
	.align 2, 0
_0801F890: .4byte 0x0000014B
_0801F894:
	mov r4, #0xA6
	lsl r4, r4, #1
	b _0801F8DE
_0801F89A:
	ldr r4, _0801F8A0 @ =0x0000014D
	b _0801F8DE
	.align 2, 0
_0801F8A0: .4byte 0x0000014D
_0801F8A4:
	mov r4, #0xA7
	lsl r4, r4, #1
	b _0801F8DE
_0801F8AA:
	ldr r4, _0801F8B0 @ =0x0000042D
	b _0801F8DE
	.align 2, 0
_0801F8B0: .4byte 0x0000042D
_0801F8B4:
	ldr r4, _0801F8B8 @ =0x00000465
	b _0801F8DE
_0801F8B8: .4byte 0x00000465
_0801F8BC:
	ldr r4, _0801F8C0 @ =0x00000466
	b _0801F8DE
_0801F8C0: .4byte 0x00000466
_0801F8C4:
	ldr r4, _0801F8C8 @ =0x00000467
	b _0801F8DE
_0801F8C8: .4byte 0x00000467
_0801F8CC:
	mov r4, #0x8D
	lsl r4, r4, #3
	b _0801F8DE
_0801F8D2:
	ldr r4, _0801F8D8 @ =0x00000469
	b _0801F8DE
	.align 2, 0
_0801F8D8: .4byte 0x00000469
_0801F8DC:
	ldr r4, _0801F8FC @ =0x0000046A
_0801F8DE:
	mov r0, #1
	add r1, r4, #0
	bl sub_08058EDC
	ldr r0, _0801F900 @ =0x00008061
	mov r1, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldr r0, _0801F904 @ =0x0000FFFF
	cmp r4, r0
	bne _0801F908
	mov r0, #0
	b _0801F932
_0801F8FC: .4byte 0x0000046A
_0801F900: .4byte 0x00008061
_0801F904: .4byte 0x0000FFFF
_0801F908:
	ldr r0, _0801F918 @ =0x000007CF
	cmp r4, r0
	bhi _0801F920
	lsl r0, r4, #1
	ldr r2, _0801F91C @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	b _0801F932
_0801F918: .4byte 0x000007CF
_0801F91C: .4byte gUnk_08623DF4
_0801F920:
	ldr r1, _0801F960 @ =0xFFFFF830
	add r0, r4, r1
	ldr r1, _0801F964 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0801F968 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	add r0, #1
_0801F932:
	lsl r1, r0, #0x10
	lsr r1, r1, #0x10
	mov r2, #0x85
	lsl r2, r2, #1
	ldr r0, _0801F96C @ =0x000080C5
	mov r3, #0
	bl sub_0801EC58
	ldr r0, _0801F970 @ =0x00008011
	ldr r1, _0801F974 @ =0x03000040
	ldr r2, _0801F978 @ =0x0000488A
	add r1, r1, r2
	ldrb r1, [r1]
	lsl r1, r1, #0x1C
	lsr r1, r1, #0x1C
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
_0801F958:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801F960: .4byte 0xFFFFF830
_0801F964: .4byte 0x000007FF
_0801F968: .4byte gUnk_08623DF4
_0801F96C: .4byte 0x000080C5
_0801F970: .4byte 0x00008011
_0801F974: .4byte 0x03000040
_0801F978: .4byte 0x0000488A
	thumb_func_end sub_0801F81C

