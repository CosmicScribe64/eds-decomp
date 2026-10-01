	thumb_func_start sub_080137F8
sub_080137F8: @ 0x080137F8
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	add r5, r2, #0
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #0x14
	ldr r2, _0801381C @ =0xF3640000
	add r0, r0, r2
	lsr r0, r0, #0x10
	mov r8, r0
	cmp r5, #0
	bge _08013820
	mov r0, #0xB
	neg r5, r5
	b _08013822
_0801381C: .4byte 0xF3640000
_08013820:
	mov r0, #0xA
_08013822:
	add r6, #0x50
	cmp r5, #0
	bne _08013836
	lsl r0, r1, #0x10
	orr r0, r6
	mov r1, #0x40
	mov r2, r8
	bl sub_080761F0
	b _0801387C
_08013836:
	lsl r7, r1, #0x10
	lsl r0, r0, #2
	mov r9, r0
_0801383C:
	add r4, r6, #0
	orr r4, r7
	add r0, r5, #0
	mov r1, #0xA
	bl __modsi3
	add r2, r0, #0
	lsl r2, r2, #2
	add r2, r8
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r4, #0
	mov r1, #0x40
	bl sub_080761F0
	add r0, r5, #0
	mov r1, #0xA
	bl __divsi3
	add r5, r0, #0
	sub r6, #0x10
	cmp r5, #0
	bne _0801383C
	orr r7, r6
	mov r2, r8
	add r2, r9
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r7, #0
	mov r1, #0x40
	bl sub_080761F0
_0801387C:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_080137F8

