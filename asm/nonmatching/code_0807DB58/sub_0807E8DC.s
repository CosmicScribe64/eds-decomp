	thumb_func_start sub_0807E8DC
sub_0807E8DC: @ 0x0807E8DC
	push {r4, r5, r6, lr}
	ldr r3, _0807E914 @ =0x03005308
	mov r4, #6
	mov r6, #0x80
	mov r5, #4
_0807E8E6:
	ldrb r1, [r3, #0x13]
	add r0, r6, #0
	and r0, r1
	cmp r0, #0
	beq _0807E906
	orr r1, r5
	strb r1, [r3, #0x13]
	mov r2, #1
	add r0, r1, #0
	and r0, r2
	cmp r0, #0
	beq _0807E906
	strh r2, [r3, #0xC]
	mov r0, #0xFE
	and r1, r0
	strb r1, [r3, #0x13]
_0807E906:
	add r3, #0x18
	sub r4, #1
	cmp r4, #0
	bne _0807E8E6
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0807E914: .4byte 0x03005308
	thumb_func_end sub_0807E8DC

