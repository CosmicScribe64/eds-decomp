	thumb_func_start sub_0800642C
sub_0800642C: @ 0x0800642C
	push {r4, r5, lr}
	ldr r5, _08006464 @ =0x02013D90
	mov r0, #1
	ldrb r1, [r5]
	and r0, r1
	mov r4, #0
	cmp r0, #0
	beq _0800643E
	mov r4, #0x48
_0800643E:
	add r0, r4, #0
	add r0, #0x3E
	mov r1, #0x86
	lsl r1, r1, #0x10
	orr r0, r1
	mov r1, #0x80
	lsl r1, r1, #7
	ldr r2, _08006468 @ =0x0000303A
	bl sub_080761F0
	add r0, r4, #0
	add r0, #0x44
	ldr r2, [r5, #0x2C]
	mov r1, #0x86
	bl sub_080063D0
	pop {r4, r5}
	pop {r0}
	bx r0
_08006464: .4byte 0x02013D90
_08006468: .4byte 0x0000303A
	thumb_func_end sub_0800642C

