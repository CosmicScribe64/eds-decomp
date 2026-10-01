	thumb_func_start sub_0805F00C
sub_0805F00C: @ 0x0805F00C
	push {r4, r5, r6, r7, lr}
	lsl r0, r0, #0x10
	lsr r0, r0, #0xA
	ldr r1, _0805F06C @ =0x0822C720
	add r7, r0, r1
	add r0, r7, #0
	bl sub_080753E0
	add r4, r0, #0
	mov r5, #0xC
	cmp r4, #0xF
	ble _0805F026
	mov r5, #0xA
_0805F026:
	mov r0, #0x20
	mov r1, #2
	bl sub_08074B08
	mul r4, r5
	asr r4, r4, #1
	mov r0, #0x78
	sub r0, r0, r4
	lsr r6, r5, #1
	mov r1, #9
	sub r1, r1, r6
	lsl r5, r5, #8
	mov r3, #8
	add r2, r5, #0
	orr r2, r3
	add r3, r7, #0
	bl sub_0807501C
	mov r0, #0x77
	sub r0, r0, r4
	mov r1, #8
	sub r1, r1, r6
	mov r2, #7
	orr r5, r2
	add r2, r5, #0
	add r3, r7, #0
	bl sub_0807501C
	ldr r0, _0805F070 @ =0x0201CFB8
	mov r1, #9
	bl sub_08075114
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0805F06C: .4byte gUnk_0822C720
_0805F070: .4byte 0x0201CFB8
	thumb_func_end sub_0805F00C

