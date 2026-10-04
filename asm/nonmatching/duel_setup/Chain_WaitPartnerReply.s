	thumb_func_start Chain_WaitPartnerReply
Chain_WaitPartnerReply: @ 0x0801FE54
	ldr r2, _0801FE6C @ =0x0201AE60
	add r3, r2, #0
	add r3, #0x22
	ldrb r1, [r3]
	add r0, r1, #0
	cmp r0, #0
	beq _0801FE70
	cmp r0, #1
	beq _0801FE88
	mov r0, #1
	b _0801FE9C
	.align 2, 0
_0801FE6C: .4byte 0x0201AE60
_0801FE70:
	ldr r0, _0801FE80 @ =0x02017FB0
	ldr r2, _0801FE84 @ =0x00000307
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1C
	cmp r0, #0
	bge _0801FE9A
	b _0801FE96
_0801FE80: .4byte 0x02017FB0
_0801FE84: .4byte 0x00000307
_0801FE88:
	add r2, #0x23
	ldrb r0, [r2]
	cmp r0, #0x3B
	bhi _0801FE96
	add r0, #1
	strb r0, [r2]
	b _0801FE9A
_0801FE96:
	add r0, r1, #1
	strb r0, [r3]
_0801FE9A:
	mov r0, #0
_0801FE9C:
	bx lr
	thumb_func_end Chain_WaitPartnerReply
	.align 2, 0

