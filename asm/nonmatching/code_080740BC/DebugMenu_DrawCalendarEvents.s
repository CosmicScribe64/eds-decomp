	thumb_func_start DebugMenu_DrawCalendarEvents
DebugMenu_DrawCalendarEvents: @ 0x08074638
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	mov r0, #2
	mov r8, r0
	mov r7, #0xE8
	lsl r7, r7, #2
	mov r0, sp
	bl GetCurrentDate
	ldr r2, [sp, #0]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	lsl r1, r2, #0x10
	lsr r1, r1, #0x1C
	lsl r2, r2, #0xB
	lsr r2, r2, #0x1B
	bl GetCalendarEvents
	mov r9, r0
	ldr r0, _080746D8 @ =0x02011C20
	ldr r2, _080746DC @ =0x00002150
	add r1, r0, r2
	ldrh r0, [r1]
	cmp r0, #0
	beq _0807468E
	mov r1, #0x3C
	bl __umodsi3
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0807468E
	ldr r1, _080746E0 @ =0x00000802
	ldr r3, _080746E4 @ =0x08087B40
	mov r0, #0x42
	add r2, r7, #0
	bl DrawBgString
	mov r0, #3
	mov r8, r0
	add r7, #0x20
_0807468E:
	mov r6, #0
	ldr r0, _080746E8 @ =0x08087720
	add r5, r0, #4
	add r4, r0, #0
_08074696:
	ldr r0, [r4]
	mov r1, r9
	and r0, r1
	cmp r0, #0
	beq _080746BE
	mov r2, r8
	lsl r0, r2, #0x10
	lsr r0, r0, #0xB
	add r0, #2
	ldr r1, _080746EC @ =0x00000804
	add r2, r7, #0
	add r3, r5, #0
	bl DrawBgString
	mov r0, #1
	add r8, r0
	add r0, r7, #0
	add r0, #0x20
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
_080746BE:
	add r5, #0x24
	add r4, #0x24
	add r6, #1
	cmp r6, #0x1C
	bls _08074696
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080746D8: .4byte 0x02011C20
_080746DC: .4byte 0x00002150
_080746E0: .4byte 0x00000802
_080746E4: .4byte gStrRareHunterComing
_080746E8: .4byte gCalendarEventNames
_080746EC: .4byte 0x00000804
	thumb_func_end DebugMenu_DrawCalendarEvents

