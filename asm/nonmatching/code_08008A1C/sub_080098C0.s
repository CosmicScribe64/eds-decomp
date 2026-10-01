	thumb_func_start sub_080098C0
sub_080098C0: @ 0x080098C0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	str r0, [sp, #0]
	mov sl, r1
	mov r6, #0
	ldr r7, _080099B0 @ =0x020192E4
	mov r2, #1
	and r2, r0
	ldr r1, _080099B4 @ =0x00000D64
	add r0, r2, #0
	mul r0, r1
	add r0, r0, r7
	ldrb r3, [r0, #6]
	cmp r6, r3
	bge _080099D4
	add r3, r7, #0
	mov r8, r0
_080098EA:
	lsl r0, r6, #1
	add r4, r2, #0
	mul r4, r1
	add r0, r0, r4
	ldr r1, _080099B8 @ =0x00000CC4
	add r1, r1, r3
	mov r9, r1
	add r0, r9
	ldrh r1, [r0]
	ldrb r5, [r0]
	cmp r5, #1
	bne _080099C4
	lsr r0, r1, #8
	cmp r0, sl
	bne _080099C4
	add r0, r3, #0
	add r0, #0x28
	add r0, r4, r0
	mov r1, #0x94
	mov r2, sl
	mul r2, r1
	add r1, r2, #0
	add r0, r0, r1
	ldr r1, _080099BC @ =0x00000B84
	add r2, r3, r1
	add r1, r4, r2
	lsl r7, r6, #2
	add r1, r1, r7
	str r2, [sp, #4]
	str r3, [sp, #8]
	bl sub_08007558
	ldr r3, [sp, #8]
	add r1, r4, r3
	ldrb r0, [r1, #6]
	sub r0, #1
	strb r0, [r1, #6]
	ldr r2, [sp, #4]
	cmp r6, r0
	bge _0800996C
	ldr r3, [sp, #0]
	and r5, r3
	ldr r4, _080099B4 @ =0x00000D64
	add r1, r5, #0
	mul r1, r4
	ldr r0, _080099C0 @ =0xFFFFF33C
	add r0, r9
	add r3, r1, r0
	add r5, r7, #4
	add r2, r1, r2
	add r4, r7, r2
_08009950:
	add r1, r2, r5
	add r0, r4, #0
	str r2, [sp, #4]
	str r3, [sp, #8]
	bl sub_08007558
	add r5, #4
	add r4, #4
	add r6, #1
	ldr r3, [sp, #8]
	ldr r2, [sp, #4]
	ldrb r0, [r3, #6]
	cmp r6, r0
	blt _08009950
_0800996C:
	mov r1, r8
	ldrb r3, [r1, #0xB]
	lsr r0, r3, #4
	mov r1, #1
	mov r2, r8
	ldrb r2, [r2, #0xC]
	and r1, r2
	lsl r1, r1, #4
	orr r1, r0
	mov r0, #1
	mov r4, sl
	lsl r0, r4
	bic r1, r0
	mov r0, #0xF
	add r2, r1, #0
	and r2, r0
	lsl r2, r2, #4
	and r0, r3
	orr r0, r2
	mov r2, r8
	strb r0, [r2, #0xB]
	lsr r1, r1, #4
	mov r3, #1
	and r1, r3
	and r1, r3
	mov r4, #2
	neg r4, r4
	add r0, r4, #0
	ldrb r2, [r2, #0xC]
	and r0, r2
	orr r0, r1
	mov r3, r8
	strb r0, [r3, #0xC]
	b _080099D4
_080099B0: .4byte 0x020192E4
_080099B4: .4byte 0x00000D64
_080099B8: .4byte 0x00000CC4
_080099BC: .4byte 0x00000B84
_080099C0: .4byte 0xFFFFF33C
_080099C4:
	add r6, #1
	ldr r1, _080099E4 @ =0x00000D64
	add r0, r2, #0
	mul r0, r1
	add r0, r0, r7
	ldrb r0, [r0, #6]
	cmp r6, r0
	blt _080098EA
_080099D4:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080099E4: .4byte 0x00000D64
	thumb_func_end sub_080098C0

