	thumb_func_start sub_08002940
sub_08002940: @ 0x08002940
	push {r4, lr}
	ldr r1, _0800296C @ =0x08198338
	ldr r0, _08002970 @ =0x03000040
	ldr r2, _08002974 @ =0x0000485B
	add r4, r0, r2
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08002978
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08002966
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08002966:
	mov r0, #0
	b _0800297A
	.align 2, 0
_0800296C: .4byte gUnk_08198338
_08002970: .4byte 0x03000040
_08002974: .4byte 0x0000485B
_08002978:
	mov r0, #1
_0800297A:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08002940

