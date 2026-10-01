	thumb_func_start sub_080225D8
sub_080225D8: @ 0x080225D8
	push {r4, r5, lr}
	sub sp, #0x14
	ldr r3, _08022658 @ =0x020192E0
	ldr r0, _0802265C @ =0x00001B50
	add r4, r3, r0
	mov r1, #2
	ldrb r2, [r4]
	orr r1, r2
	ldr r5, _08022660 @ =0x00001B62
	add r0, r3, r5
	mov r2, #0
	strb r2, [r0]
	add r5, #1
	add r0, r3, r5
	strb r2, [r0]
	mov r0, #2
	neg r0, r0
	and r1, r0
	sub r0, #7
	and r1, r0
	strb r1, [r4]
	mov r0, #4
	and r0, r1
	cmp r0, #0
	beq _08022650
	ldr r1, _08022664 @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _08022650
	ldrh r1, [r4]
	lsl r0, r1, #0x16
	lsr r1, r0, #0x1A
	cmp r1, #3
	beq _08022650
	mov r0, sp
	strh r1, [r0]
	add r0, #2
	ldr r2, _08022668 @ =0x00001B52
	add r1, r3, r2
	mov r2, #0x10
	bl sub_08075294
	ldr r0, _0802266C @ =0x0000F0A1
	mov r1, sp
	mov r2, #0x12
	bl sub_080229BC
	ldr r1, _08022670 @ =0x02017FB0
	ldr r5, _08022674 @ =0x00000307
	add r1, r1, r5
	mov r0, #0x7F
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	mov r0, #1
	ldrb r5, [r4]
	orr r0, r5
	strb r0, [r4]
_08022650:
	add sp, #0x14
	pop {r4, r5}
	pop {r0}
	bx r0
_08022658: .4byte 0x020192E0
_0802265C: .4byte 0x00001B50
_08022660: .4byte 0x00001B62
_08022664: .4byte 0x02015EE8
_08022668: .4byte 0x00001B52
_0802266C: .4byte 0x0000F0A1
_08022670: .4byte 0x02017FB0
_08022674: .4byte 0x00000307
	thumb_func_end sub_080225D8

