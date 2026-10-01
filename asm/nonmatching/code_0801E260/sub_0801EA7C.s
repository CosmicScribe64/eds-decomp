	thumb_func_start sub_0801EA7C
sub_0801EA7C: @ 0x0801EA7C
	push {lr}
	ldr r0, _0801EA8C @ =0x02017A30
	ldr r0, [r0]
	cmp r0, #0
	bne _0801EA90
	mov r0, #0
	b _0801EA98
	.align 2, 0
_0801EA8C: .4byte 0x02017A30
_0801EA90:
	bl _call_via_r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
_0801EA98:
	pop {r1}
	bx r1
	thumb_func_end sub_0801EA7C

