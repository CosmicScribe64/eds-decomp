	thumb_func_start sub_08001464
sub_08001464: @ 0x08001464
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x18
	ldr r1, _08001574 @ =0x02013DE0
	ldr r2, _08001578 @ =0x000009A4
	add r0, r1, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	mov r5, #0
	cmp r0, #0
	beq _080014A6
	mov r3, #0x86
	lsl r3, r3, #5
	add r6, r1, r3
	add r7, r1, r2
_08001484:
	lsl r4, r5, #2
	add r4, r4, r5
	lsl r4, r4, #2
	add r0, r4, r6
	bl sub_080786D0
	add r4, r6, r4
	mov r0, #0xFF
	strb r0, [r4, #0xE]
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	ldrb r1, [r7]
	lsl r0, r1, #0x1D
	lsr r0, r0, #0x1D
	cmp r5, r0
	bcc _08001484
_080014A6:
	ldr r2, _08001574 @ =0x02013DE0
	ldr r3, _0800157C @ =0x000012E9
	add r1, r2, r3
	ldrb r3, [r1]
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r1, _08001580 @ =0x000010CE
	add r0, r0, r1
	mov r1, #0
	strb r1, [r0]
	ldr r3, _08001578 @ =0x000009A4
	add r1, r2, r3
	ldrb r3, [r1]
	lsl r0, r3, #0x1D
	mov r5, #0
	cmp r0, #0
	beq _08001522
	add r6, r2, #0
	mov r0, #1
	neg r0, r0
	mov r8, r0
	add r7, r1, #0
	mov r4, #0
_080014D8:
	lsl r0, r5, #2
	add r0, r0, r5
	lsl r1, r0, #2
	add r0, r1, r6
	ldr r2, _08001580 @ =0x000010CE
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x18
	asr r0, r0, #0x18
	cmp r0, r8
	beq _08001512
	mov r3, #0x86
	lsl r3, r3, #5
	add r0, r6, r3
	add r0, r1, r0
	mov r1, #1
	str r1, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #8]
	str r4, [sp, #0xC]
	str r4, [sp, #0x10]
	ldr r2, _08001584 @ =0x00000AA8
	add r1, r6, r2
	str r1, [sp, #0x14]
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_08078534
_08001512:
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	ldrb r3, [r7]
	lsl r0, r3, #0x1D
	lsr r0, r0, #0x1D
	cmp r5, r0
	bcc _080014D8
_08001522:
	ldr r4, _08001588 @ =0x020147B4
	add r0, r4, #0
	bl sub_0807A420
	add r0, r4, #0
	bl sub_0807A420
	add r0, r4, #0
	bl sub_0807A420
	ldr r0, _0800158C @ =0x0000091A
	add r5, r4, r0
	ldrh r0, [r5]
	mov r1, #1
	bl sub_08000AC8
	ldr r1, _08001590 @ =0x0000091E
	add r0, r4, r1
	ldrb r0, [r0]
	cmp r0, #0
	bne _08001552
	mov r0, #1
	bl sub_08001374
_08001552:
	ldrh r1, [r5]
	ldr r2, _08001594 @ =0x0000099B
	add r0, r4, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x18
	asr r0, r0, #0x18
	cmp r0, #1
	beq _0800159C
	cmp r0, #2
	beq _080015AC
	ldr r0, _08001598 @ =0x0400002C
	lsl r1, r1, #1
	strh r1, [r0]
	add r0, #2
	lsr r1, r1, #0x10
	b _080015BA
	.align 2, 0
_08001574: .4byte 0x02013DE0
_08001578: .4byte 0x000009A4
_0800157C: .4byte 0x000012E9
_08001580: .4byte 0x000010CE
_08001584: .4byte 0x00000AA8
_08001588: .4byte 0x020147B4
_0800158C: .4byte 0x0000091A
_08001590: .4byte 0x0000091E
_08001594: .4byte 0x0000099B
_08001598: .4byte 0x0400002C
_0800159C:
	ldr r0, _080015A8 @ =0x04000028
	lsl r1, r1, #1
	strh r1, [r0]
	add r0, #2
	lsr r1, r1, #0x10
	b _080015BA
_080015A8: .4byte 0x04000028
_080015AC:
	ldr r0, _080015DC @ =0x04000028
	neg r1, r1
	lsl r1, r1, #1
	strh r1, [r0]
	add r0, #2
	lsl r1, r1, #4
	lsr r1, r1, #0x14
_080015BA:
	strh r1, [r0]
	ldr r0, _080015E0 @ =0x02013DE0
	ldr r3, _080015E4 @ =0x000012EE
	add r1, r0, r3
	ldrh r1, [r1]
	ldr r2, _080015E8 @ =0x0000136F
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #1
	bne _080015EC
	ldr r0, _080015DC @ =0x04000028
	lsl r1, r1, #1
	strh r1, [r0]
	add r0, #2
	lsr r1, r1, #0x10
	b _080015FA
	.align 2, 0
_080015DC: .4byte 0x04000028
_080015E0: .4byte 0x02013DE0
_080015E4: .4byte 0x000012EE
_080015E8: .4byte 0x0000136F
_080015EC:
	ldr r0, _08001640 @ =0x04000028
	neg r1, r1
	lsl r1, r1, #1
	strh r1, [r0]
	add r0, #2
	lsl r1, r1, #4
	lsr r1, r1, #0x14
_080015FA:
	strh r1, [r0]
	ldr r4, _08001644 @ =0x02014888
	add r0, r4, #0
	bl sub_0807A298
	add r0, r4, #0
	bl sub_0807A2EC
	ldr r1, _08001648 @ =0x03000040
	ldr r3, _0800164C @ =0x0000040C
	add r1, r1, r3
	ldrh r2, [r1]
	ldr r0, _08001650 @ =0x0000FFFE
	and r0, r2
	ldrh r2, [r1]
	strh r0, [r1]
	ldr r1, _08001654 @ =0x00000844
	add r0, r4, r1
	bl sub_0807883C
	ldr r2, _08001658 @ =0x0000084A
	add r0, r4, r2
	ldrb r0, [r0]
	cmp r0, #2
	bne _08001688
	ldr r3, _0800165C @ =0x00000842
	add r0, r4, r3
	ldrb r0, [r0]
	cmp r0, #4
	bne _08001664
	ldr r0, _08001660 @ =0x00000851
	add r1, r4, r0
	mov r0, #1
	b _0800166A
	.align 2, 0
_08001640: .4byte 0x04000028
_08001644: .4byte 0x02014888
_08001648: .4byte 0x03000040
_0800164C: .4byte 0x0000040C
_08001650: .4byte 0x0000FFFE
_08001654: .4byte 0x00000844
_08001658: .4byte 0x0000084A
_0800165C: .4byte 0x00000842
_08001660: .4byte 0x00000851
_08001664:
	ldr r2, _08001698 @ =0x00000851
	add r1, r4, r2
	mov r0, #0
_0800166A:
	strb r0, [r1]
	ldr r0, _0800169C @ =0x03000040
	ldr r3, _080016A0 @ =0x00004859
	add r0, r0, r3
	mov r2, #0
	mov r1, #1
	strb r1, [r0]
	ldr r0, _080016A4 @ =0x04000028
	strh r2, [r0]
	add r0, #2
	strh r2, [r0]
	add r0, #2
	strh r2, [r0]
	add r0, #2
	strh r2, [r0]
_08001688:
	mov r0, #0
	add sp, #0x18
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08001698: .4byte 0x00000851
_0800169C: .4byte 0x03000040
_080016A0: .4byte 0x00004859
_080016A4: .4byte 0x04000028
	thumb_func_end sub_08001464

