	thumb_func_start AiPickWeakestHandCard
AiPickWeakestHandCard: @ 0x0805664C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	str r0, [sp, #0]
	str r1, [sp, #4]
	mov r0, #1
	neg r0, r0
	str r0, [sp, #8]
	ldr r1, _080566D0 @ =0x0000270F
	mov sl, r1
	mov r7, #0
	ldr r1, _080566D4 @ =0x020192E4
	mov r2, #1
	ldr r3, [sp, #4]
	and r2, r3
	ldr r3, _080566D8 @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp r7, r0
	blt _08056680
	b _0805680E
_08056680:
	mov r9, r2
	ldr r4, _080566DC @ =0x000007FF
	add r6, r4, #0
	mov r0, #0xF8
	lsl r0, r0, #0x11
	mov r8, r0
_0805668C:
	lsl r1, r7, #2
	mov r0, r9
	mul r0, r3
	add r1, r1, r0
	ldr r0, _080566E0 @ =0x02019968
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	cmp r5, #0
	bne _080566A4
	b _080567FA
_080566A4:
	add r1, r5, #0
	and r1, r6
	lsl r0, r1, #2
	ldr r2, _080566E4 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r3, r8
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _080566BC
	b _080567FA
_080566BC:
	lsl r0, r1, #1
	ldr r4, _080566E8 @ =0x08622AB4
	add r0, r0, r4
	ldrh r1, [r0]
	ldr r0, _080566EC @ =0x00000776
	cmp r1, r0
	bne _080566F0
	mov r0, #3
	b _0805674A
	.align 2, 0
_080566D0: .4byte 0x0000270F
_080566D4: .4byte 0x020192E4
_080566D8: .4byte 0x00000D64
_080566DC: .4byte 0x000007FF
_080566E0: .4byte 0x02019968
_080566E4: .4byte gCardStats
_080566E8: .4byte gCardIdToNumber
_080566EC: .4byte 0x00000776
_080566F0:
	cmp r1, r0
	blt _08056700
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08056700
	mov r0, #1
	b _0805674A
_08056700:
	add r0, r5, #0
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _08056720 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r2, r8
	and r0, r2
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0805672A
	cmp r0, #0x16
	bgt _08056724
	cmp r0, #0x15
	beq _0805672E
	b _08056736
_08056720: .4byte gCardStats
_08056724:
	cmp r0, #0x17
	beq _08056732
	b _08056736
_0805672A:
	mov r0, #7
	b _0805674A
_0805672E:
	mov r0, #8
	b _0805674A
_08056732:
	mov r0, #9
	b _0805674A
_08056736:
	add r0, r5, #0
	and r0, r6
	lsl r0, r0, #2
	ldr r3, _08056784 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0805674A:
	cmp r0, #0
	bne _080567FA
	add r4, r5, #0
	and r4, r6
	lsl r0, r4, #1
	ldr r1, _08056788 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #1
	bl AiIsKeyCard
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _080567FA
	lsl r0, r4, #2
	ldr r2, _08056784 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r3, r8
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08056796
	cmp r0, #0x17
	ble _0805678C
	cmp r0, #0x18
	beq _08056790
	b _08056796
	.align 2, 0
_08056784: .4byte gCardStats
_08056788: .4byte gCardIdToNumber
_0805678C:
	mov r2, #0
	b _080567AC
_08056790:
	mov r2, #0xFA
	lsl r2, r2, #4
	b _080567AC
_08056796:
	add r0, r5, #0
	and r0, r6
	lsl r0, r0, #2
	ldr r4, _080567CC @ =0x08621DE0
	add r0, r0, r4
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r2, r0, #1
_080567AC:
	add r0, r5, #0
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _080567CC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r3, r8
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080567DA
	cmp r0, #0x17
	ble _080567D0
	cmp r0, #0x18
	beq _080567D4
	b _080567DA
_080567CC: .4byte gCardStats
_080567D0:
	mov r0, #0
	b _080567F0
_080567D4:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _080567F0
_080567DA:
	and r5, r6
	lsl r0, r5, #2
	ldr r4, _08056818 @ =0x08621DE0
	add r0, r0, r4
	ldr r1, [r0]
	ldr r3, _0805681C @ =0x000001FF
	add r0, r3, #0
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_080567F0:
	add r0, r0, r2
	cmp sl, r0
	ble _080567FA
	mov sl, r0
	str r7, [sp, #8]
_080567FA:
	add r7, #1
	ldr r1, _08056820 @ =0x020192E4
	ldr r3, _08056824 @ =0x00000D64
	mov r0, r9
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp r7, r0
	bge _0805680E
	b _0805668C
_0805680E:
	ldr r4, [sp, #8]
	cmp r4, #0
	blt _08056828
	add r0, r4, #0
	b _08056A72
_08056818: .4byte gCardStats
_0805681C: .4byte 0x000001FF
_08056820: .4byte 0x020192E4
_08056824: .4byte 0x00000D64
_08056828:
	ldr r0, _080568AC @ =0x0000270F
	mov sl, r0
	mov r7, #0
	mov r1, #1
	ldr r2, [sp, #4]
	and r1, r2
	ldr r2, _080568B0 @ =0x00000D64
	add r0, r1, #0
	mul r0, r2
	ldr r3, _080568B4 @ =0x020192E4
	add r0, r0, r3
	ldrb r0, [r0, #2]
	cmp r7, r0
	blt _08056846
	b _0805694E
_08056846:
	mov r8, r1
	ldr r4, _080568B8 @ =0x000007FF
	add r3, r4, #0
_0805684C:
	mov r0, r8
	mul r0, r2
	ldr r1, [sp, #0]
	add r0, r1, r0
	lsl r1, r7, #2
	ldr r2, _080568BC @ =0x00000684
	add r1, r1, r2
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	cmp r4, #0
	beq _0805693A
	add r1, r4, #0
	and r1, r3
	lsl r0, r1, #2
	ldr r2, _080568C0 @ =0x08621DE0
	add r5, r0, r2
	ldr r0, [r5]
	mov r6, #0xF8
	lsl r6, r6, #0x11
	and r0, r6
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0805693A
	lsl r0, r1, #1
	ldr r1, _080568C4 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #1
	str r3, [sp, #0xC]
	bl AiIsKeyCard
	lsl r0, r0, #0x10
	ldr r3, [sp, #0xC]
	cmp r0, #0
	bne _0805693A
	ldr r0, [r5]
	and r0, r6
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080568D2
	cmp r0, #0x17
	ble _080568C8
	cmp r0, #0x18
	beq _080568CC
	b _080568D2
	.align 2, 0
_080568AC: .4byte 0x0000270F
_080568B0: .4byte 0x00000D64
_080568B4: .4byte 0x020192E4
_080568B8: .4byte 0x000007FF
_080568BC: .4byte 0x00000684
_080568C0: .4byte gCardStats
_080568C4: .4byte gCardIdToNumber
_080568C8:
	mov r2, #0
	b _080568E8
_080568CC:
	mov r2, #0xFA
	lsl r2, r2, #4
	b _080568E8
_080568D2:
	add r0, r4, #0
	and r0, r3
	lsl r0, r0, #2
	ldr r2, _0805690C @ =0x08621DE0
	add r0, r0, r2
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r2, r0, #1
_080568E8:
	add r0, r4, #0
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0805690C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805691A
	cmp r0, #0x17
	ble _08056910
	cmp r0, #0x18
	beq _08056914
	b _0805691A
	.align 2, 0
_0805690C: .4byte gCardStats
_08056910:
	mov r0, #0
	b _08056930
_08056914:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08056930
_0805691A:
	and r4, r3
	lsl r0, r4, #2
	ldr r4, _080569CC @ =0x08621DE0
	add r0, r0, r4
	ldr r1, [r0]
	ldr r4, _080569D0 @ =0x000001FF
	add r0, r4, #0
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08056930:
	add r0, r0, r2
	cmp sl, r0
	ble _0805693A
	mov sl, r0
	str r7, [sp, #8]
_0805693A:
	add r7, #1
	ldr r1, _080569D4 @ =0x020192E4
	ldr r2, _080569D8 @ =0x00000D64
	mov r0, r8
	mul r0, r2
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp r7, r0
	bge _0805694E
	b _0805684C
_0805694E:
	ldr r0, [sp, #8]
	cmp r0, #0
	blt _08056956
	b _08056A72
_08056956:
	ldr r1, _080569DC @ =0x0000270F
	mov sl, r1
	mov r7, #0
	mov r1, #1
	ldr r2, [sp, #4]
	and r1, r2
	ldr r2, _080569D8 @ =0x00000D64
	add r0, r1, #0
	mul r0, r2
	ldr r3, _080569D4 @ =0x020192E4
	add r0, r0, r3
	ldrb r0, [r0, #2]
	cmp r7, r0
	blt _08056974
	b _08056A70
_08056974:
	add r3, r1, #0
	ldr r4, _080569E0 @ =0x000007FF
	add r6, r4, #0
_0805697A:
	add r1, r3, #0
	mul r1, r2
	ldr r0, [sp, #0]
	add r1, r0, r1
	lsl r0, r7, #2
	ldr r2, _080569E4 @ =0x00000684
	add r0, r0, r2
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	add r4, r5, #0
	and r4, r6
	lsl r0, r4, #1
	ldr r1, _080569E8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #1
	str r3, [sp, #0xC]
	bl AiIsKeyCard
	lsl r0, r0, #0x10
	ldr r3, [sp, #0xC]
	cmp r0, #0
	bne _08056A5E
	lsl r0, r4, #2
	ldr r2, _080569CC @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080569F6
	cmp r0, #0x17
	ble _080569EC
	cmp r0, #0x18
	beq _080569F0
	b _080569F6
	.align 2, 0
_080569CC: .4byte gCardStats
_080569D0: .4byte 0x000001FF
_080569D4: .4byte 0x020192E4
_080569D8: .4byte 0x00000D64
_080569DC: .4byte 0x0000270F
_080569E0: .4byte 0x000007FF
_080569E4: .4byte 0x00000684
_080569E8: .4byte gCardIdToNumber
_080569EC:
	mov r2, #0
	b _08056A0C
_080569F0:
	mov r2, #0xFA
	lsl r2, r2, #4
	b _08056A0C
_080569F6:
	add r0, r5, #0
	and r0, r6
	lsl r0, r0, #2
	ldr r4, _08056A30 @ =0x08621DE0
	add r0, r0, r4
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r2, r0, #1
_08056A0C:
	add r0, r5, #0
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _08056A30 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08056A3E
	cmp r0, #0x17
	ble _08056A34
	cmp r0, #0x18
	beq _08056A38
	b _08056A3E
	.align 2, 0
_08056A30: .4byte gCardStats
_08056A34:
	mov r0, #0
	b _08056A54
_08056A38:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08056A54
_08056A3E:
	and r5, r6
	lsl r0, r5, #2
	ldr r4, _08056A84 @ =0x08621DE0
	add r0, r0, r4
	ldr r1, [r0]
	ldr r4, _08056A88 @ =0x000001FF
	add r0, r4, #0
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08056A54:
	add r0, r0, r2
	cmp sl, r0
	ble _08056A5E
	mov sl, r0
	str r7, [sp, #8]
_08056A5E:
	add r7, #1
	ldr r1, _08056A8C @ =0x020192E4
	ldr r2, _08056A90 @ =0x00000D64
	add r0, r3, #0
	mul r0, r2
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp r7, r0
	blt _0805697A
_08056A70:
	ldr r0, [sp, #8]
_08056A72:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08056A84: .4byte gCardStats
_08056A88: .4byte 0x000001FF
_08056A8C: .4byte 0x020192E4
_08056A90: .4byte 0x00000D64
	thumb_func_end AiPickWeakestHandCard

