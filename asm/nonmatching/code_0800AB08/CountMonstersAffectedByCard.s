	thumb_func_start CountMonstersAffectedByCard
CountMonstersAffectedByCard: @ 0x0800AB08
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	mov r6, #0
	mov r4, #0
	mov r0, #1
	and r0, r5
	ldr r1, _0800AB64 @ =0x00000D64
	add r7, r0, #0
	mul r7, r1
_0800AB24:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r7
	ldr r1, _0800AB68 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0800AB50
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0800AB50
	add r0, r5, #0
	add r1, r4, #0
	mov r2, r8
	bl CountZoneLinksFromCard
	cmp r0, #0
	beq _0800AB50
	add r6, #1
_0800AB50:
	add r4, #1
	cmp r4, #4
	ble _0800AB24
	add r0, r6, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0800AB64: .4byte 0x00000D64
_0800AB68: .4byte 0x0201930C
	thumb_func_end CountMonstersAffectedByCard

