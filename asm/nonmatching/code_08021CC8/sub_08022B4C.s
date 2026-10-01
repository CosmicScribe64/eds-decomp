	thumb_func_start sub_08022B4C
sub_08022B4C: @ 0x08022B4C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r7, r0, #0
	ldr r2, _08022BD8 @ =0x02017FB0
	mov r0, #0x81
	lsl r0, r0, #2
	add r1, r2, r0
	ldr r0, _08022BDC @ =0x0000F023
	strh r0, [r1]
	ldr r5, _08022BE0 @ =0x020192E4
	mov r0, #1
	and r0, r7
	ldr r1, _08022BE4 @ =0x00000D64
	mul r1, r0
	add r3, r1, r5
	lsl r0, r7, #0x18
	lsr r0, r0, #0x10
	ldrb r4, [r3, #4]
	orr r0, r4
	ldr r4, _08022BE8 @ =0x00000206
	add r2, r2, r4
	strh r0, [r2]
	mov r4, #0
	ldrb r0, [r3, #4]
	cmp r4, r0
	bge _08022BA4
	add r6, r1, #0
	ldr r2, _08022BEC @ =0x00000904
	add r2, r2, r5
	mov r8, r2
	add r5, r3, #0
_08022B8C:
	lsl r2, r4, #2
	ldr r0, _08022BF0 @ =0x020181B8
	add r0, r2, r0
	mov r3, r8
	add r1, r6, r3
	add r1, r1, r2
	bl sub_08007558
	add r4, #1
	ldrb r0, [r5, #4]
	cmp r4, r0
	blt _08022B8C
_08022BA4:
	ldr r4, _08022BF4 @ =0x020181B4
	ldr r2, _08022BE0 @ =0x020192E4
	mov r0, #1
	and r0, r7
	ldr r1, _08022BE4 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #4]
	lsl r1, r0, #2
	add r1, #4
	add r0, r4, #0
	bl sub_080723B4
	ldr r2, _08022BF8 @ =0x00000101
	add r1, r4, r2
	mov r0, #5
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08022BD8: .4byte 0x02017FB0
_08022BDC: .4byte 0x0000F023
_08022BE0: .4byte 0x020192E4
_08022BE4: .4byte 0x00000D64
_08022BE8: .4byte 0x00000206
_08022BEC: .4byte 0x00000904
_08022BF0: .4byte 0x020181B8
_08022BF4: .4byte 0x020181B4
_08022BF8: .4byte 0x00000101
	thumb_func_end sub_08022B4C

