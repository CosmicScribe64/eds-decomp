	thumb_func_start sub_08008668
sub_08008668: @ 0x08008668
	push {r4, r5, r6, lr}
	mov r4, #5
	ldr r6, _080086A8 @ =0x0201930C
	mov r1, #1
	and r1, r0
	ldr r0, _080086AC @ =0x00000D64
	mul r1, r0
	ldr r5, _080086B0 @ =0x000003BA
_08008678:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r1
	add r3, r0, r6
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _080086BC
	mov r0, #2
	ldrb r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	beq _080086BC
	ldr r0, _080086B4 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r2, _080086B8 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, r5
	bne _080086BC
	mov r0, #1
	b _080086C4
_080086A8: .4byte 0x0201930C
_080086AC: .4byte 0x00000D64
_080086B0: .4byte 0x000003BA
_080086B4: .4byte 0x000007FF
_080086B8: .4byte gUnk_08622AB4
_080086BC:
	add r4, #1
	cmp r4, #9
	ble _08008678
	mov r0, #0
_080086C4:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08008668
	.align 2, 0

