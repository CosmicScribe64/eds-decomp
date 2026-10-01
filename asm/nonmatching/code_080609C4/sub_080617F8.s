	thumb_func_start sub_080617F8
sub_080617F8: @ 0x080617F8
	push {r4, lr}
	mov r3, #0xC9
	lsl r3, r3, #2
	ldr r1, _08061834 @ =0x03000040
	ldr r0, _08061838 @ =0x0000485E
	add r1, r1, r0
	mov r0, #0x20
	ldrh r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08061810
	add r3, #0x20
_08061810:
	ldr r1, _0806183C @ =0x0201CFB0
	mov r0, #6
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #6
	bne _0806182E
	ldr r0, _08061840 @ =0x00400058
	ldr r1, _08061844 @ =0x000040C0
	mov r4, #0xC0
	lsl r4, r4, #7
	add r2, r4, #0
	orr r3, r2
	add r2, r3, #0
	bl sub_080761F0
_0806182E:
	pop {r4}
	pop {r0}
	bx r0
_08061834: .4byte 0x03000040
_08061838: .4byte 0x0000485E
_0806183C: .4byte 0x0201CFB0
_08061840: .4byte 0x00400058
_08061844: .4byte 0x000040C0
	thumb_func_end sub_080617F8

