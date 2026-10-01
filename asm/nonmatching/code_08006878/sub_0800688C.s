	thumb_func_start sub_0800688C
sub_0800688C: @ 0x0800688C
	push {r4, r5, r6, lr}
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	bl sub_08006878
	ldr r0, _080068C4 @ =0x02013D90
	strh r4, [r0, #2]
	ldr r0, _080068C8 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _080068CC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080068DA
	cmp r0, #0x17
	ble _080068D0
	cmp r0, #0x18
	beq _080068D4
	b _080068DA
_080068C4: .4byte 0x02013D90
_080068C8: .4byte 0x000007FF
_080068CC: .4byte gUnk_08621DE0
_080068D0:
	mov r0, #0
	b _080068F0
_080068D4:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _080068F0
_080068DA:
	ldr r0, _08006918 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r3, _0800691C @ =0x08621DE0
	add r0, r0, r3
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_080068F0:
	ldr r2, _08006920 @ =0x02013D90
	str r0, [r2, #0x2C]
	ldr r0, _08006918 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0800691C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0800692E
	cmp r0, #0x17
	ble _08006924
	cmp r0, #0x18
	beq _08006928
	b _0800692E
	.align 2, 0
_08006918: .4byte 0x000007FF
_0800691C: .4byte gUnk_08621DE0
_08006920: .4byte 0x02013D90
_08006924:
	mov r0, #0
	b _08006944
_08006928:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08006944
_0800692E:
	ldr r0, _08006960 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r3, _08006964 @ =0x08621DE0
	add r0, r0, r3
	ldr r1, [r0]
	ldr r0, _08006968 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08006944:
	str r0, [r2, #0x30]
	strh r5, [r2, #4]
	mov r0, #1
	add r1, r6, #0
	and r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_08006960: .4byte 0x000007FF
_08006964: .4byte gUnk_08621DE0
_08006968: .4byte 0x000001FF
	thumb_func_end sub_0800688C

