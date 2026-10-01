	thumb_func_start sub_0803DDAC
sub_0803DDAC: @ 0x0803DDAC
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	add r7, r1, #0
	add r4, r2, #0
	mov r0, #0
	mov r9, r0
	add r5, r4, #0
	cmp r4, #4
	ble _0803DDCA
	mov r1, #5
	mov r9, r1
	sub r5, r4, #5
_0803DDCA:
	cmp r4, #0xA
	bne _0803DDD4
	mov r0, #0xA
	mov r9, r0
	mov r5, #0
_0803DDD4:
	ldrh r0, [r6]
	add r1, r7, #0
	add r2, r4, #0
	bl sub_0802B1B8
	cmp r0, #0
	beq _0803DE30
	mov r1, #1
	mov r8, r1
	mov r0, r8
	ldrb r1, [r6, #2]
	and r0, r1
	cmp r0, #0
	bne _0803DDF6
	mov r0, #1
	bl sub_08077AEC
_0803DDF6:
	mov r0, r8
	ldrb r1, [r6, #2]
	and r0, r1
	mov r3, #8
	cmp r0, #0
	beq _0803DE04
	ldr r3, _0803DE2C @ =0x00008008
_0803DE04:
	lsl r1, r7, #0x10
	lsr r1, r1, #0x10
	lsl r2, r5, #0x18
	lsr r2, r2, #0x10
	mov r0, r9
	orr r2, r0
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	lsl r1, r7, #0x18
	lsl r0, r4, #0x18
	lsr r1, r1, #8
	orr r1, r0
	lsr r1, r1, #0x10
	add r0, r6, #0
	bl sub_0803DD7C
	mov r0, #1
	b _0803DE32
_0803DE2C: .4byte 0x00008008
_0803DE30:
	mov r0, #0
_0803DE32:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0803DDAC
	.align 2, 0

