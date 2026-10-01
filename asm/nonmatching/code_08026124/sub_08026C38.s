	thumb_func_start sub_08026C38
sub_08026C38: @ 0x08026C38
	push {r4, r5, lr}
	bl sub_080269FC
	ldr r3, _08026C6C @ =0x04000052
	ldr r4, _08026C70 @ =0x02020310
	ldr r1, _08026C74 @ =0x00000B1A
	add r0, r4, r1
	ldrh r0, [r0]
	lsl r2, r0, #0x10
	lsr r0, r2, #0x18
	mov r5, #0x80
	lsl r5, r5, #4
	add r1, r5, #0
	orr r0, r1
	strh r0, [r3]
	mov r0, #0xA0
	lsl r0, r0, #0x14
	cmp r2, r0
	bls _08026C7C
	ldr r1, _08026C78 @ =0x00000B18
	add r0, r4, r1
	bl sub_0807883C
	mov r0, #0
	b _08026C86
	.align 2, 0
_08026C6C: .4byte 0x04000052
_08026C70: .4byte 0x02020310
_08026C74: .4byte 0x00000B1A
_08026C78: .4byte 0x00000B18
_08026C7C:
	ldr r5, _08026C8C @ =0x00000B1E
	add r1, r4, r5
	mov r0, #0
	strb r0, [r1]
	mov r0, #1
_08026C86:
	pop {r4, r5}
	pop {r1}
	bx r1
_08026C8C: .4byte 0x00000B1E
	thumb_func_end sub_08026C38

