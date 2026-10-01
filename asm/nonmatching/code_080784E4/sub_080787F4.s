	thumb_func_start sub_080787F4
sub_080787F4: @ 0x080787F4
	push {r4, r5, lr}
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	lsl r1, r1, #0x10
	lsl r2, r2, #0x18
	lsr r4, r2, #0x18
	strb r5, [r3]
	lsr r2, r1, #0x10
	cmp r1, #0
	blt _0807880C
	mov r0, #0
	b _08078810
_0807880C:
	mov r0, #0x80
	lsl r0, r0, #5
_08078810:
	strh r0, [r3, #2]
	strh r2, [r3, #4]
	mov r0, #1
	strb r0, [r3, #6]
	strb r4, [r3, #7]
	ldr r1, _08078834 @ =0x04000054
	ldrh r3, [r3, #2]
	lsr r0, r3, #8
	strh r0, [r1]
	mov r1, #0xBF
	cmp r5, #0
	bne _0807882A
	mov r1, #0xFF
_0807882A:
	ldr r0, _08078838 @ =0x04000050
	strh r1, [r0]
	pop {r4, r5}
	pop {r0}
	bx r0
_08078834: .4byte 0x04000054
_08078838: .4byte 0x04000050
	thumb_func_end sub_080787F4

