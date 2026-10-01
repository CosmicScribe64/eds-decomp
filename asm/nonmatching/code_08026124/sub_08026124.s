	thumb_func_start sub_08026124
sub_08026124: @ 0x08026124
	push {r4, lr}
	ldr r1, _0802614C @ =0x08199A58
	ldr r4, _08026150 @ =0x02017A30
	ldrb r2, [r4, #0xB]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08026154
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08026146
	ldrb r0, [r4, #0xB]
	add r0, #1
	strb r0, [r4, #0xB]
_08026146:
	mov r0, #0
	b _08026156
	.align 2, 0
_0802614C: .4byte gUnk_08199A58
_08026150: .4byte 0x02017A30
_08026154:
	mov r0, #1
_08026156:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08026124

