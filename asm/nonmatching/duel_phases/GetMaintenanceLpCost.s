	thumb_func_start GetMaintenanceLpCost
GetMaintenanceLpCost: @ 0x0804F654
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	add r2, r1, #0
	ldr r0, _0804F674 @ =0x00000482
	cmp r1, r0
	beq _0804F692
	cmp r1, r0
	bgt _0804F678
	sub r0, #0xC8
	cmp r1, r0
	beq _0804F69E
	add r0, #0x91
	cmp r1, r0
	beq _0804F68C
	b _0804F6A4
	.align 2, 0
_0804F674: .4byte 0x00000482
_0804F678:
	ldr r0, _0804F688 @ =0x0000058C
	cmp r2, r0
	beq _0804F698
	add r0, #4
	cmp r2, r0
	beq _0804F69E
	b _0804F6A4
	.align 2, 0
_0804F688: .4byte 0x0000058C
_0804F68C:
	mov r0, #0xFA
	lsl r0, r0, #3
	b _0804F6A6
_0804F692:
	mov r0, #0xAF
	lsl r0, r0, #2
	b _0804F6A6
_0804F698:
	mov r0, #0xFA
	lsl r0, r0, #2
	b _0804F6A6
_0804F69E:
	mov r0, #0xFA
	lsl r0, r0, #1
	b _0804F6A6
_0804F6A4:
	mov r0, #0
_0804F6A6:
	bx lr
	thumb_func_end GetMaintenanceLpCost

