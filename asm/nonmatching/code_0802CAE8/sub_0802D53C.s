	thumb_func_start sub_0802D53C
sub_0802D53C: @ 0x0802D53C
	push {r4, r5, r6, lr}
	mov r3, #0
	mov r6, #1
	ldr r5, _0802D560 @ =0x00000D64
	ldr r4, _0802D564 @ =0x0201930C
_0802D546:
	mov r2, #0
	add r0, r3, #0
	and r0, r6
	add r1, r0, #0
	mul r1, r5
_0802D550:
	add r0, r1, r4
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802D568
	mov r0, #1
	b _0802D578
	.align 2, 0
_0802D560: .4byte 0x00000D64
_0802D564: .4byte 0x0201930C
_0802D568:
	add r1, #0x94
	add r2, #1
	cmp r2, #4
	ble _0802D550
	add r3, #1
	cmp r3, #1
	ble _0802D546
	mov r0, #0
_0802D578:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0802D53C
	.align 2, 0

