	thumb_func_start sub_0807E6B0
sub_0807E6B0: @ 0x0807E6B0
	push {r4, lr}
	add r3, r0, #0
	ldr r2, _0807E6DC @ =0x03005210
	mov r1, #0xC4
	lsl r1, r1, #1
	add r0, r2, r1
	ldrh r1, [r0]
	mov r0, #0x80
	and r0, r1
	cmp r0, #0
	beq _0807E6E0
	mov r1, #0
	mov r4, #0xC7
	lsl r4, r4, #1
	add r0, r2, r4
	mov r2, #0
	ldsh r0, [r0, r2]
	cmp r0, r3
	bne _0807E6D8
	mov r1, #1
_0807E6D8:
	add r0, r1, #0
	b _0807E6E2
_0807E6DC: .4byte 0x03005210
_0807E6E0:
	mov r0, #0
_0807E6E2:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0807E6B0

