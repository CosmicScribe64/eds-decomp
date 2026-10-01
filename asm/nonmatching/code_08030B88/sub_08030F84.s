	thumb_func_start sub_08030F84
sub_08030F84: @ 0x08030F84
	push {r4, r5, r6, lr}
	add r4, r0, #0
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	mov r6, #1
	lsr r0, r0, #0x1F
	ldr r1, _08030FE8 @ =0x00000D64
	mul r0, r1
	ldr r1, _08030FEC @ =0x020198D4
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08030FDE
	add r0, r6, #0
	and r0, r2
	mov r5, #0x11
	cmp r0, #0
	beq _08030FAC
	ldr r5, _08030FF0 @ =0x00008011
_08030FAC:
	ldr r0, _08030FF4 @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08030FF8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	bl sub_08008300
	add r1, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r0, r5, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r6, r0
	mov r1, #0x18
	mov r2, #0
	bl sub_08042AB0
_08030FDE:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08030FE8: .4byte 0x00000D64
_08030FEC: .4byte 0x020198D4
_08030FF0: .4byte 0x00008011
_08030FF4: .4byte 0x000007FF
_08030FF8: .4byte gUnk_08622AB4
	thumb_func_end sub_08030F84

