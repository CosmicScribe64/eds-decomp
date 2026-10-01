	thumb_func_start sub_080515A4
sub_080515A4: @ 0x080515A4
	push {r4, r5, r6, lr}
	add r3, r0, #0
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	mov r5, #0
	cmp r4, #0
	bne _080515CC
	ldr r2, _080515C4 @ =0x020192E4
	mov r0, #1
	and r0, r3
	ldr r1, _080515C8 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	b _08051620
	.align 2, 0
_080515C4: .4byte 0x020192E4
_080515C8: .4byte 0x00000D64
_080515CC:
	ldr r6, _08051628 @ =0x020192E4
	mov r0, #1
	and r0, r3
	ldr r1, _0805162C @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r0, r2, r6
	ldrb r1, [r0, #2]
	cmp r5, r1
	bge _0805161E
	ldr r3, _08051630 @ =0x00000684
	add r0, r6, r3
	add r2, r2, r0
	ldr r6, _08051634 @ =0x000007FF
	add r3, r1, #0
_080515EA:
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _08051638 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _0805160A
	cmp r4, #0
	bne _08051616
_0805160A:
	mov r0, #6
	ldrb r1, [r2, #2]
	and r0, r1
	cmp r0, #0
	bne _08051616
	add r5, #1
_08051616:
	add r2, #4
	sub r3, #1
	cmp r3, #0
	bne _080515EA
_0805161E:
	add r0, r5, #0
_08051620:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08051628: .4byte 0x020192E4
_0805162C: .4byte 0x00000D64
_08051630: .4byte 0x00000684
_08051634: .4byte 0x000007FF
_08051638: .4byte gUnk_08621DE0
	thumb_func_end sub_080515A4

