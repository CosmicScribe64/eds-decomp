	thumb_func_start sub_0803266C
sub_0803266C: @ 0x0803266C
	push {r4, r5, r6, lr}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _080326B2
	mov r4, #5
	mov r5, #1
_0803267E:
	ldrb r0, [r6, #2]
	lsl r3, r0, #0x1F
	lsr r1, r3, #0x1F
	sub r1, r5, r1
	and r1, r5
	mov r0, #0x94
	add r2, r4, #0
	mul r2, r0
	ldr r0, _080326BC @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _080326C0 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080326AC
	lsr r0, r3, #0x1F
	sub r0, r5, r0
	add r1, r4, #0
	mov r2, #5
	bl sub_08018544
_080326AC:
	add r4, #1
	cmp r4, #0xA
	ble _0803267E
_080326B2:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_080326BC: .4byte 0x00000D64
_080326C0: .4byte 0x0201930C
	thumb_func_end sub_0803266C

