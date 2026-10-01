	thumb_func_start sub_0807E814
sub_0807E814: @ 0x0807E814
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _0807E868 @ =0x03005210
	mov ip, r0
	mov r0, #0x80
	lsl r0, r0, #8
	and r0, r4
	cmp r0, #0
	beq _0807E848
	mov r2, ip
	add r2, #0xF8
	mov r3, #6
	mov r5, #0x80
_0807E82E:
	ldrb r1, [r2, #0x13]
	add r0, r5, #0
	and r0, r1
	cmp r0, #0
	beq _0807E840
	mov r1, #0xE
	ldsh r0, [r2, r1]
	cmp r0, r4
	beq _0807E862
_0807E840:
	add r2, #0x18
	sub r3, #1
	cmp r3, #0
	bne _0807E82E
_0807E848:
	mov r0, #0xC6
	lsl r0, r0, #1
	add r0, ip
	mov r1, #0
	strh r4, [r0]
	mov r2, #0xCB
	lsl r2, r2, #1
	add r2, ip
	mov r0, #0x10
	strb r0, [r2]
	ldr r0, _0807E86C @ =0x00000197
	add r0, ip
	strb r1, [r0]
_0807E862:
	pop {r4, r5}
	pop {r0}
	bx r0
_0807E868: .4byte 0x03005210
_0807E86C: .4byte 0x00000197
	thumb_func_end sub_0807E814

