	thumb_func_start sub_0802D620
sub_0802D620: @ 0x0802D620
	push {r4, lr}
	add r4, r0, #0
	ldr r2, _0802D640 @ =0x020192E4
	ldrb r0, [r4, #2]
	lsl r3, r0, #0x1F
	lsr r1, r3, #0x1F
	ldr r0, _0802D644 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldr r1, _0802D648 @ =0x000003E7
	ldrh r0, [r0]
	cmp r0, r1
	bhi _0802D64C
	mov r0, #0
	b _0802D66C
	.align 2, 0
_0802D640: .4byte 0x020192E4
_0802D644: .4byte 0x00000D64
_0802D648: .4byte 0x000003E7
_0802D64C:
	lsr r0, r3, #0x1F
	ldr r1, _0802D674 @ =0x000007FF
	ldrh r4, [r4]
	and r1, r4
	lsl r1, r1, #1
	ldr r2, _0802D678 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	bl sub_08044224
	mov r1, #0
	cmp r0, #0
	ble _0802D66A
	mov r1, #1
_0802D66A:
	add r0, r1, #0
_0802D66C:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802D674: .4byte 0x000007FF
_0802D678: .4byte gUnk_08622AB4
	thumb_func_end sub_0802D620

