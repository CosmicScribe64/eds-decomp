	thumb_func_start sub_08008B70
sub_08008B70: @ 0x08008B70
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r4, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov sl, r1
	lsl r2, r2, #0x10
	lsr r5, r2, #0x10
	lsl r3, r3, #0x10
	mov r0, #0
	mov r8, r0
	mov r1, #0xA
	mov r9, r1
	cmp r3, #0
	beq _08008B98
	mov r0, #0xB
	mov r9, r0
_08008B98:
	mov r2, #5
	cmp r2, r9
	bge _08008C14
	mov r0, #1
	and r0, r4
	mov r1, #0x94
	mov ip, r1
	ldr r1, _08008BD0 @ =0x00000D64
	add r4, r0, #0
	mul r4, r1
	ldr r6, _08008BD4 @ =0x0201930C
	mov r7, #2
_08008BB0:
	mov r0, ip
	mul r0, r2
	add r0, r0, r4
	add r1, r0, r6
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08008C0A
	mov r3, #0
	mov r0, sl
	cmp r0, #0
	bne _08008BD8
	cmp r5, #0
	bne _08008BE8
	b _08008BE2
	.align 2, 0
_08008BD0: .4byte 0x00000D64
_08008BD4: .4byte 0x0201930C
_08008BD8:
	add r0, r7, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08008BE4
_08008BE2:
	mov r3, #1
_08008BE4:
	cmp r5, #0
	beq _08008BFC
_08008BE8:
	mov r1, ip
	mul r1, r2
	add r1, r1, r4
	add r1, r1, r6
	add r0, r7, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08008BFC
	mov r3, #1
_08008BFC:
	cmp r3, #0
	beq _08008C0A
	mov r0, r8
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
_08008C0A:
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	cmp r2, r9
	blt _08008BB0
_08008C14:
	mov r0, r8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08008B70

