	thumb_func_start sub_0807E898
sub_0807E898: @ 0x0807E898
	push {r4, r5, lr}
	add r5, r0, #0
	ldr r3, _0807E8D8 @ =0x03005308
	mov r4, #6
_0807E8A0:
	ldrb r1, [r3, #0x13]
	mov r0, #0x80
	and r0, r1
	cmp r0, #0
	beq _0807E8CA
	mov r2, #0xE
	ldsh r0, [r3, r2]
	cmp r0, r5
	bne _0807E8CA
	mov r0, #4
	orr r1, r0
	strb r1, [r3, #0x13]
	mov r2, #1
	add r0, r1, #0
	and r0, r2
	cmp r0, #0
	beq _0807E8CA
	strh r2, [r3, #0xC]
	mov r0, #0xFE
	and r1, r0
	strb r1, [r3, #0x13]
_0807E8CA:
	add r3, #0x18
	sub r4, #1
	cmp r4, #0
	bne _0807E8A0
	pop {r4, r5}
	pop {r0}
	bx r0
_0807E8D8: .4byte 0x03005308
	thumb_func_end sub_0807E898

