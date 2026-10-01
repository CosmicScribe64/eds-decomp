	thumb_func_start sub_08006AE8
sub_08006AE8: @ 0x08006AE8
	push {lr}
	bl sub_080064AC
	ldr r1, _08006B04 @ =0x03000040
	mov r0, #3
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08006B08
	mov r0, #2
	bl sub_08077AEC
	mov r0, #1
	b _08006B76
_08006B04: .4byte 0x03000040
_08006B08:
	ldr r0, _08006B20 @ =0x02013D90
	ldrh r1, [r0, #4]
	add r2, r0, #0
	cmp r1, #0
	beq _08006B24
	sub r0, r1, #1
	strh r0, [r2, #4]
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08006B24
	mov r0, #1
	b _08006B76
_08006B20: .4byte 0x02013D90
_08006B24:
	ldr r1, _08006B64 @ =0x03000040
	mov r0, #0x40
	ldrh r3, [r1, #6]
	and r0, r3
	add r3, r1, #0
	cmp r0, #0
	beq _08006B3C
	ldr r0, [r2, #0x38]
	cmp r0, #0
	ble _08006B3C
	mov r0, #0
	str r0, [r2, #0x38]
_08006B3C:
	mov r0, #0x80
	ldrh r1, [r3, #6]
	and r0, r1
	cmp r0, #0
	beq _08006B54
	ldr r0, [r2, #0x3C]
	add r1, r0, #0
	sub r1, #0x70
	ldr r0, [r2, #0x38]
	cmp r0, r1
	bge _08006B54
	str r1, [r2, #0x38]
_08006B54:
	ldr r0, [r2, #0x34]
	ldr r1, [r2, #0x38]
	cmp r0, r1
	beq _08006B6C
	cmp r0, r1
	bge _08006B68
	add r0, #2
	b _08006B6A
_08006B64: .4byte 0x03000040
_08006B68:
	sub r0, #2
_08006B6A:
	str r0, [r2, #0x34]
_08006B6C:
	ldr r1, [r2, #0x34]
	ldr r2, _08006B7C @ =0x00004426
	add r0, r3, r2
	strh r1, [r0]
	mov r0, #0
_08006B76:
	pop {r1}
	bx r1
	.align 2, 0
_08006B7C: .4byte 0x00004426
	thumb_func_end sub_08006AE8

