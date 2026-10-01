	thumb_func_start sub_0804E31C
sub_0804E31C: @ 0x0804E31C
	push {r4, r5, lr}
	add r2, r0, #0
	ldr r3, _0804E374 @ =0x0819D1D8
	ldr r4, _0804E378 @ =0x020192E0
	ldr r0, _0804E37C @ =0x00001B14
	add r5, r4, r0
	ldr r0, [r5]
	lsl r1, r0, #0xF
	lsr r0, r1, #0x18
	lsl r0, r0, #2
	add r0, r0, r3
	ldr r0, [r0]
	cmp r0, #0
	beq _0804E38C
	lsr r0, r1, #0x18
	lsl r0, r0, #2
	add r0, r0, r3
	ldr r1, [r0]
	add r0, r2, #0
	bl _call_via_r1
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0804E36E
	ldr r2, _0804E380 @ =0x00001B16
	add r1, r4, r2
	ldr r0, _0804E384 @ =0xFFFFFE01
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldr r2, [r5]
	lsl r1, r2, #0xF
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #9
	ldr r0, _0804E388 @ =0xFFFE01FF
	and r0, r2
	orr r0, r1
	str r0, [r5]
_0804E36E:
	mov r0, #0
	b _0804E3AC
	.align 2, 0
_0804E374: .4byte gUnk_0819D1D8
_0804E378: .4byte 0x020192E0
_0804E37C: .4byte 0x00001B14
_0804E380: .4byte 0x00001B16
_0804E384: .4byte 0xFFFFFE01
_0804E388: .4byte 0xFFFE01FF
_0804E38C:
	ldr r0, _0804E3B4 @ =0x00001B12
	add r1, r4, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	mov r1, #0x54
	cmp r0, #0
	beq _0804E39E
	ldr r1, _0804E3B8 @ =0x00008054
_0804E39E:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #1
_0804E3AC:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0804E3B4: .4byte 0x00001B12
_0804E3B8: .4byte 0x00008054
	thumb_func_end sub_0804E31C

