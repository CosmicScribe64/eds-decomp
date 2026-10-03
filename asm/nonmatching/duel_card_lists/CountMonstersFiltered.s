	thumb_func_start CountMonstersFiltered
CountMonstersFiltered: @ 0x080088A4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov sl, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r9, r2
	mov r6, #0
	mov r3, #0
	mov r1, #1
	mov r8, r1
	and r0, r1
	mov r1, #0x94
	mov ip, r1
	ldr r1, _08008938 @ =0x00000D64
	add r5, r0, #0
	mul r5, r1
	ldr r7, _0800893C @ =0x0201930C
_080088D0:
	mov r0, ip
	mul r0, r3
	add r0, r0, r5
	add r1, r0, r7
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0800891E
	mov r2, #0
	mov r4, #0
	mov r0, sl
	cmp r0, #0
	beq _080088F4
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080088F6
_080088F4:
	mov r2, #1
_080088F6:
	mov r1, r9
	cmp r1, #0
	beq _0800890E
	mov r1, ip
	mul r1, r3
	add r1, r1, r5
	add r1, r1, r7
	mov r0, r8
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08008910
_0800890E:
	mov r4, #1
_08008910:
	cmp r2, #0
	beq _0800891E
	cmp r4, #0
	beq _0800891E
	add r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
_0800891E:
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	cmp r3, #4
	bls _080088D0
	add r0, r6, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08008938: .4byte 0x00000D64
_0800893C: .4byte 0x0201930C
	thumb_func_end CountMonstersFiltered

