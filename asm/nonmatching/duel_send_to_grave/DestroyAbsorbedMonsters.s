	thumb_func_start DestroyAbsorbedMonsters
DestroyAbsorbedMonsters: @ 0x08017F98
	push {r4, r5, r6, lr}
	mov r5, #0
	mov r2, #1
	and r2, r0
	mov r0, #0x94
	mul r1, r0
	ldr r0, _08017FEC @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08017FF0 @ =0x0201930C
	add r0, r1, r0
	add r1, r0, #0
	add r1, #0x8A
	ldrh r2, [r1]
	cmp r5, r2
	bge _08017FE4
	add r4, r0, #0
	add r6, r1, #0
_08017FBC:
	lsl r0, r5, #1
	add r1, r4, #0
	add r1, #0xA
	add r1, r1, r0
	add r2, r4, #0
	add r2, #0x4A
	add r2, r2, r0
	ldrb r0, [r1]
	ldrh r1, [r1]
	lsr r1, r1, #8
	ldrb r2, [r2]
	cmp r2, #5
	bne _08017FDC
	mov r2, #1
	bl DestroyFieldCard
_08017FDC:
	add r5, #1
	ldrh r0, [r6]
	cmp r5, r0
	blt _08017FBC
_08017FE4:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08017FEC: .4byte 0x00000D64
_08017FF0: .4byte 0x0201930C
	thumb_func_end DestroyAbsorbedMonsters

