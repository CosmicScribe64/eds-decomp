	thumb_func_start HandHasRitualMonster
HandHasRitualMonster: @ 0x08043594
	push {r4, r5, r6, lr}
	add r5, r1, #0
	mov r3, #0
	ldr r4, _080435D8 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _080435DC @ =0x00000D64
	mul r1, r0
	add r0, r1, r4
	ldrb r2, [r0, #2]
	cmp r3, r2
	bcs _080435F6
	add r6, r1, #0
	ldr r0, _080435E0 @ =0x00000684
	add r4, r4, r0
	ldr r0, _080435E4 @ =0x0819A990
	lsl r1, r5, #2
	add r1, r1, r0
	ldrh r1, [r1]
	lsl r0, r1, #0x13
	lsr r1, r0, #0x13
_080435BE:
	lsl r0, r3, #2
	add r0, r0, r6
	add r0, r0, r4
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r5, _080435E8 @ =0x08622AB4
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, r1
	bne _080435EC
	mov r0, #1
	b _080435F8
_080435D8: .4byte 0x020192E4
_080435DC: .4byte 0x00000D64
_080435E0: .4byte 0x00000684
_080435E4: .4byte gRitualRecipes
_080435E8: .4byte gCardIdToNumber
_080435EC:
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	cmp r3, r2
	bcc _080435BE
_080435F6:
	mov r0, #0
_080435F8:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end HandHasRitualMonster
	.align 2, 0

