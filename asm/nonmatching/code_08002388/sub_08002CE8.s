	thumb_func_start sub_08002CE8
sub_08002CE8: @ 0x08002CE8
	push {r4, lr}
	ldr r4, _08002D2C @ =0x0201F7E0
	ldrb r1, [r4]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1D
	mov r1, #0
	bl sub_080034B8
	mov r0, #0
	bl sub_08003174
	ldr r3, _08002D30 @ =0x0819834C
	ldrb r2, [r4]
	lsl r1, r2, #0x1D
	lsr r1, r1, #0x1D
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1D
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r0, r3
	ldrh r0, [r0]
	bl sub_080036FC
	mov r0, #4
	bl sub_08075A6C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08002D2C: .4byte 0x0201F7E0
_08002D30: .4byte gUnk_0819834C
	thumb_func_end sub_08002CE8

