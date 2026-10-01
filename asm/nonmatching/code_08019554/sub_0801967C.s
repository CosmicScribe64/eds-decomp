	thumb_func_start sub_0801967C
sub_0801967C: @ 0x0801967C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	mov ip, r2
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	mov r5, #0
	ldr r3, _080196E0 @ =0x020192E4
	mov r0, #1
	and r0, r6
	ldr r1, _080196E4 @ =0x00000D64
	mul r0, r1
	add r1, r0, r3
	ldrb r2, [r1, #3]
	cmp r5, r2
	bge _080196FC
	ldr r2, _080196E8 @ =0x000007C4
	add r2, r2, r3
	mov r8, r2
	add r2, r0, #0
	add r3, r1, #0
_080196A8:
	mov r0, r8
	add r1, r2, r0
	lsl r0, r5, #2
	add r4, r1, r0
	ldr r0, [r4]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _080196EC @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r7
	bne _080196F4
	mov r0, ip
	add r1, r4, #0
	bl sub_08007558
	mov r0, #0x65
	cmp r6, #0
	beq _080196D0
	ldr r0, _080196F0 @ =0x00008065
_080196D0:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	b _08019700
	.align 2, 0
_080196E0: .4byte 0x020192E4
_080196E4: .4byte 0x00000D64
_080196E8: .4byte 0x000007C4
_080196EC: .4byte gUnk_08622AB4
_080196F0: .4byte 0x00008065
_080196F4:
	add r5, #1
	ldrb r0, [r3, #3]
	cmp r5, r0
	blt _080196A8
_080196FC:
	mov r0, #1
	neg r0, r0
_08019700:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0801967C
	.align 2, 0

