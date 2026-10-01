	thumb_func_start sub_08046BE0
sub_08046BE0: @ 0x08046BE0
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r4, r1, #0
	ldr r6, _08046C18 @ =0x00000476
	add r1, r6, #0
	bl sub_08008524
	mul r4, r0
	cmp r4, #0
	ble _08046C10
	lsl r0, r6, #1
	ldr r1, _08046C1C @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r5, #0
	bl sub_080197E0
	mov r0, #1
	sub r0, r0, r5
	add r1, r4, #0
	mov r2, #0
	mov r3, #1
	bl sub_0802272C
_08046C10:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08046C18: .4byte 0x00000476
_08046C1C: .4byte gUnk_08623DF4
	thumb_func_end sub_08046BE0

