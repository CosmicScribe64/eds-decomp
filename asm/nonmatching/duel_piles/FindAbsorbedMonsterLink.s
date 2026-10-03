	thumb_func_start FindAbsorbedMonsterLink
FindAbsorbedMonsterLink: @ 0x0800A430
	push {r4, r5, lr}
	mov r3, #0
	mov r2, #1
	and r2, r0
	mov r0, #0x94
	mul r1, r0
	ldr r0, _0800A474 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0800A478 @ =0x0201930C
	add r0, r1, r0
	add r1, r0, #0
	add r1, #0x8A
	ldrh r1, [r1]
	cmp r3, r1
	bge _0800A46C
	add r4, r1, #0
	add r2, r0, #0
	add r2, #0x4A
	add r1, r0, #0
	add r1, #0xA
_0800A45A:
	ldrh r0, [r1]
	ldrb r5, [r2]
	cmp r5, #5
	beq _0800A46E
	add r2, #2
	add r1, #2
	add r3, #1
	cmp r3, r4
	blt _0800A45A
_0800A46C:
	ldr r0, _0800A47C @ =0x0000FFFF
_0800A46E:
	pop {r4, r5}
	pop {r1}
	bx r1
_0800A474: .4byte 0x00000D64
_0800A478: .4byte 0x0201930C
_0800A47C: .4byte 0x0000FFFF
	thumb_func_end FindAbsorbedMonsterLink

