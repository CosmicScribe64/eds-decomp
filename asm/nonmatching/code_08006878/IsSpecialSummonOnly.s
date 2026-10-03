	thumb_func_start IsSpecialSummonOnly
IsSpecialSummonOnly: @ 0x08007834
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	ldr r2, _08007864 @ =0x000007FF
	and r2, r3
	lsl r0, r2, #2
	ldr r1, _08007868 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _08007852
	b _08007990
_08007852:
	lsl r0, r2, #1
	ldr r1, _0800786C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08007870 @ =0x00000776
	cmp r1, r0
	bne _08007874
	mov r0, #3
	b _080078D6
_08007864: .4byte 0x000007FF
_08007868: .4byte gCardStats
_0800786C: .4byte gCardIdToNumber
_08007870: .4byte 0x00000776
_08007874:
	cmp r1, r0
	blt _08007884
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08007884
	mov r0, #1
	b _080078D6
_08007884:
	ldr r0, _080078A8 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _080078AC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _080078B6
	cmp r0, #0x16
	bgt _080078B0
	cmp r0, #0x15
	beq _080078BA
	b _080078C2
	.align 2, 0
_080078A8: .4byte 0x000007FF
_080078AC: .4byte gCardStats
_080078B0:
	cmp r0, #0x17
	beq _080078BE
	b _080078C2
_080078B6:
	mov r0, #7
	b _080078D6
_080078BA:
	mov r0, #8
	b _080078D6
_080078BE:
	mov r0, #9
	b _080078D6
_080078C2:
	ldr r0, _08007918 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0800791C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_080078D6:
	cmp r0, #0
	beq _08007990
	cmp r0, #0
	blt _080078E6
	cmp r0, #3
	bgt _080078E6
	cmp r0, #2
	bge _08007982
_080078E6:
	ldr r0, _08007918 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _08007920 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #0xB6
	lsl r0, r0, #2
	cmp r1, r0
	bgt _08007940
	sub r0, #2
	cmp r1, r0
	bge _08007982
	cmp r1, #0x42
	beq _08007982
	cmp r1, #0x42
	bgt _08007924
	cmp r1, #0x37
	blt _08007990
	cmp r1, #0x38
	ble _08007982
	cmp r1, #0x3E
	beq _08007982
	b _08007990
	.align 2, 0
_08007918: .4byte 0x000007FF
_0800791C: .4byte gCardStats
_08007920: .4byte gCardIdToNumber
_08007924:
	ldr r0, _08007934 @ =0x00000175
	cmp r1, r0
	beq _08007982
	cmp r1, r0
	bgt _08007938
	sub r0, #5
	b _08007962
	.align 2, 0
_08007934: .4byte 0x00000175
_08007938:
	ldr r0, _0800793C @ =0x00000187
	b _08007962
_0800793C: .4byte 0x00000187
_08007940:
	ldr r0, _08007958 @ =0x000004B2
	cmp r1, r0
	beq _08007982
	cmp r1, r0
	bgt _0800796C
	ldr r0, _0800795C @ =0x000002FE
	cmp r1, r0
	beq _08007982
	cmp r1, r0
	bgt _08007960
	sub r0, #0x19
	b _08007962
_08007958: .4byte 0x000004B2
_0800795C: .4byte 0x000002FE
_08007960:
	ldr r0, _08007968 @ =0x0000034D
_08007962:
	cmp r1, r0
	beq _08007982
	b _08007990
_08007968: .4byte 0x0000034D
_0800796C:
	ldr r0, _08007988 @ =0x000004E9
	cmp r1, r0
	beq _08007982
	cmp r1, r0
	blt _08007990
	ldr r0, _0800798C @ =0x000005EF
	cmp r1, r0
	bgt _08007990
	sub r0, #5
	cmp r1, r0
	blt _08007990
_08007982:
	mov r0, #1
	b _08007992
	.align 2, 0
_08007988: .4byte 0x000004E9
_0800798C: .4byte 0x000005EF
_08007990:
	mov r0, #0
_08007992:
	bx lr
	thumb_func_end IsSpecialSummonOnly

