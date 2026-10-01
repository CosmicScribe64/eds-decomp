	thumb_func_start sub_08040714
sub_08040714: @ 0x08040714
	push {r4, lr}
	add r2, r0, #0
	ldr r0, _0804072C @ =0x02017A40
	ldr r1, _08040730 @ =0x000003E5
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #0
	beq _08040734
	cmp r0, #1
	beq _08040750
	mov r0, #1
	b _0804076C
_0804072C: .4byte 0x02017A40
_08040730: .4byte 0x000003E5
_08040734:
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2, #0xA]
	and r0, r1
	strb r0, [r2, #0xA]
	ldrb r2, [r2, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #8
	mov r2, #0
	mov r3, #0
	bl sub_08022678
	b _08040764
_08040750:
	ldr r0, _08040774 @ =0x020192E0
	ldr r1, _08040778 @ =0x00001B64
	add r0, r0, r1
	ldrh r1, [r0]
	add r1, #1
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r0, r2, #0
	bl sub_0803DD7C
_08040764:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	mov r0, #0
_0804076C:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08040774: .4byte 0x020192E0
_08040778: .4byte 0x00001B64
	thumb_func_end sub_08040714

