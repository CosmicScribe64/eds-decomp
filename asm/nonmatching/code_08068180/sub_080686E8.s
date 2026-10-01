	thumb_func_start sub_080686E8
sub_080686E8: @ 0x080686E8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	mov r4, #0
	ldr r0, _0806872C @ =0x0201DB20
	mov r8, r0
	ldr r3, _08068730 @ =0x03000040
	ldr r2, _08068734 @ =0x00001494
	add r2, r8
	mov r1, #0
_08068702:
	lsl r0, r4, #1
	add r0, r0, r2
	strh r1, [r0]
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, #2
	bls _08068702
	ldr r1, _08068738 @ =0x00004874
	add r0, r3, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1E
	cmp r0, #1
	beq _08068744
	cmp r0, #1
	bgt _0806873C
	cmp r0, #0
	bne _0806872A
	b _08068838
_0806872A:
	b _08068962
_0806872C: .4byte 0x0201DB20
_08068730: .4byte 0x03000040
_08068734: .4byte 0x00001494
_08068738: .4byte 0x00004874
_0806873C:
	cmp r0, #2
	bne _08068742
	b _08068924
_08068742:
	b _08068962
_08068744:
	mov r4, #1
	ldr r2, _080687B8 @ =0x02011C20
	mov sl, r2
	ldr r6, _080687BC @ =0x0201E164
	ldr r5, _080687C0 @ =0x00000E52
	add r3, r6, r5
	ldr r7, _080687C4 @ =0x0000070A
	add r7, r7, r6
	mov ip, r7
	mov r0, #0xE5
	lsl r0, r0, #4
	add r0, r0, r6
	mov r9, r0
_0806875E:
	ldr r0, _080687C8 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _080687CC @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r2, _080687D0 @ =0xFFFFFB46
	add r0, r1, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldr r5, _080687D4 @ =0x00000315
	cmp r0, r5
	bhi _08068784
	ldr r7, _080687D8 @ =0xFFFFF894
	add r0, r1, r7
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x13
	bhi _08068820
_08068784:
	lsl r0, r4, #2
	mov r1, sl
	add r2, r0, r1
	ldrh r5, [r2, #8]
	lsl r1, r5, #0x16
	add r5, r0, #0
	cmp r1, #0
	beq _080687A4
	mov r7, r9
	ldrh r0, [r7]
	add r1, r0, #1
	strh r1, [r7]
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	add r0, r0, r6
	strh r4, [r0]
_080687A4:
	ldr r0, _080687DC @ =0x00001C5A
	add r0, r8
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	cmp r0, #0
	bge _080687E0
	ldrb r2, [r2, #9]
	lsl r0, r2, #0x1C
	lsr r0, r0, #0x1E
	b _080687EC
_080687B8: .4byte 0x02011C20
_080687BC: .4byte 0x0201E164
_080687C0: .4byte 0x00000E52
_080687C4: .4byte 0x0000070A
_080687C8: .4byte 0x000007FF
_080687CC: .4byte gUnk_08622AB4
_080687D0: .4byte 0xFFFFFB46
_080687D4: .4byte 0x00000315
_080687D8: .4byte 0xFFFFF894
_080687DC: .4byte 0x00001C5A
_080687E0:
	ldrb r1, [r2, #9]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1E
	cmp r0, #0
	bne _080687F0
	lsr r0, r1, #6
_080687EC:
	cmp r0, #0
	beq _08068802
_080687F0:
	ldrh r0, [r3]
	add r1, r0, #1
	strh r1, [r3]
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	ldr r1, _08068830 @ =0x00000CAE
	add r1, r8
	add r0, r0, r1
	strh r4, [r0]
_08068802:
	mov r1, sl
	add r0, r5, r1
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1E
	cmp r0, #0
	beq _08068820
	ldr r2, _08068834 @ =0x0201EFB8
	ldrh r0, [r2]
	add r1, r0, #1
	strh r1, [r2]
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	add r0, ip
	strh r4, [r0]
_08068820:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	mov r0, #0xCD
	lsl r0, r0, #2
	cmp r4, r0
	bls _0806875E
	b _08068962
_08068830: .4byte 0x00000CAE
_08068834: .4byte 0x0201EFB8
_08068838:
	mov r4, #1
	ldr r3, _080688AC @ =0x02011C20
	mov sl, r3
	ldr r7, _080688B0 @ =0x0201EFB4
	add r6, r7, #2
	ldr r5, _080688B4 @ =0xFFFFF8BA
	add r5, r5, r7
	mov ip, r5
	add r0, r7, #4
	mov r9, r0
_0806884C:
	ldr r0, _080688B8 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _080688BC @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r2, _080688C0 @ =0xFFFFF880
	add r0, r0, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _08068910
	lsl r0, r4, #2
	mov r3, sl
	add r2, r0, r3
	ldrh r5, [r2, #8]
	lsl r1, r5, #0x16
	add r5, r0, #0
	cmp r1, #0
	beq _08068884
	ldrh r0, [r7]
	add r1, r0, #1
	strh r1, [r7]
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	ldr r1, _080688C4 @ =0x0201E164
	add r0, r0, r1
	strh r4, [r0]
_08068884:
	mov r3, r8
	ldr r1, _080688C8 @ =0x00001C5A
	add r0, r3, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	cmp r0, #0
	bge _080688D0
	ldrb r2, [r2, #9]
	lsl r0, r2, #0x1C
	lsr r0, r0, #0x1E
	cmp r0, #0
	beq _080688F2
	ldrh r0, [r6]
	add r1, r0, #1
	strh r1, [r6]
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	ldr r2, _080688CC @ =0x00000CAE
	add r1, r3, r2
	b _080688EE
_080688AC: .4byte 0x02011C20
_080688B0: .4byte 0x0201EFB4
_080688B4: .4byte 0xFFFFF8BA
_080688B8: .4byte 0x000007FF
_080688BC: .4byte gUnk_08622AB4
_080688C0: .4byte 0xFFFFF880
_080688C4: .4byte 0x0201E164
_080688C8: .4byte 0x00001C5A
_080688CC: .4byte 0x00000CAE
_080688D0:
	ldrb r1, [r2, #9]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1E
	cmp r0, #0
	bne _080688E0
	lsr r0, r1, #6
	cmp r0, #0
	beq _080688F2
_080688E0:
	ldrh r0, [r6]
	add r1, r0, #1
	strh r1, [r6]
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	ldr r1, _08068920 @ =0x00000CAE
	add r1, r8
_080688EE:
	add r0, r0, r1
	strh r4, [r0]
_080688F2:
	mov r3, sl
	add r0, r5, r3
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1E
	cmp r0, #0
	beq _08068910
	mov r5, r9
	ldrh r0, [r5]
	add r1, r0, #1
	strh r1, [r5]
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	add r0, ip
	strh r4, [r0]
_08068910:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	mov r0, #0xCD
	lsl r0, r0, #2
	cmp r4, r0
	bls _0806884C
	b _08068962
_08068920: .4byte 0x00000CAE
_08068924:
	mov r4, #1
	ldr r6, _080689B4 @ =0x000007FF
	ldr r2, _080689B8 @ =0x0201EFB4
	ldr r7, _080689BC @ =0xFFFFF1B0
	add r5, r2, r7
	mov r3, #0xCD
	lsl r3, r3, #2
_08068932:
	add r0, r4, #0
	and r0, r6
	lsl r0, r0, #1
	ldr r1, _080689C0 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r7, _080689C4 @ =0xFFFFF894
	add r0, r0, r7
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x63
	bls _08068958
	ldrh r0, [r2]
	add r1, r0, #1
	strh r1, [r2]
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	add r0, r0, r5
	strh r4, [r0]
_08068958:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, r3
	bls _08068932
_08068962:
	mov r4, #0
_08068964:
	ldr r0, _080689C8 @ =0x0201DB20
	mov r1, #0xA5
	lsl r1, r1, #5
	add r2, r0, r1
	add r0, r4, r2
	add r3, r4, #1
	str r3, [sp, #4]
	ldrb r0, [r0]
	cmp r0, #1
	beq _0806897A
	b _08068B70
_0806897A:
	mov r6, #0
	mov r7, #0
	lsl r0, r4, #1
	ldr r5, _080689CC @ =0x0201EFBA
	add r1, r0, r5
	str r0, [sp, #8]
	ldrh r1, [r1]
	cmp r6, r1
	bcc _0806898E
	b _08068B68
_0806898E:
	ldr r0, _080689D0 @ =0x02011C20
	mov sl, r0
	ldr r1, _080689D4 @ =0xFFFFF1A4
	add r1, r1, r2
	mov r9, r1
	ldr r3, _080689D8 @ =0xFFFFF80E
	add r3, r3, r2
	mov r8, r3
	ldr r5, _080689DC @ =0xFFFFF8AE
	add r5, r5, r2
	mov ip, r5
_080689A4:
	cmp r4, #1
	beq _08068A44
	cmp r4, #1
	bgt _080689E0
	cmp r4, #0
	beq _080689E8
	b _08068B54
	.align 2, 0
_080689B4: .4byte 0x000007FF
_080689B8: .4byte 0x0201EFB4
_080689BC: .4byte 0xFFFFF1B0
_080689C0: .4byte gUnk_08622AB4
_080689C4: .4byte 0xFFFFF894
_080689C8: .4byte 0x0201DB20
_080689CC: .4byte 0x0201EFBA
_080689D0: .4byte 0x02011C20
_080689D4: .4byte 0xFFFFF1A4
_080689D8: .4byte 0xFFFFF80E
_080689DC: .4byte 0xFFFFF8AE
_080689E0:
	cmp r4, #2
	bne _080689E6
	b _08068AF4
_080689E6:
	b _08068B54
_080689E8:
	lsl r0, r7, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	add r0, r0, r1
	add r0, r9
	ldrh r1, [r0]
	lsl r0, r1, #2
	add r0, sl
	ldrh r0, [r0, #8]
	lsl r0, r0, #0x16
	cmp r0, #0
	bne _08068A02
	b _08068B54
_08068A02:
	add r3, r1, #0
	mov r1, #0
	add r2, r6, #0
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	cmp r1, #1
	beq _08068A2E
	cmp r1, #1
	bgt _08068A1C
	cmp r1, #0
	beq _08068A22
	b _08068B54
_08068A1C:
	cmp r1, #2
	beq _08068A3A
	b _08068B54
_08068A22:
	lsl r0, r2, #1
	mov r2, #0xE5
	lsl r2, r2, #3
	add r0, r0, r2
	add r0, r9
	b _08068B52
_08068A2E:
	lsl r0, r2, #1
	mov r5, #0xE5
	lsl r5, r5, #3
	add r0, r0, r5
	add r0, r8
	b _08068B52
_08068A3A:
	lsl r0, r2, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	add r0, r0, r1
	b _08068B50
_08068A44:
	lsl r1, r7, #1
	mov r2, #0xE5
	lsl r2, r2, #3
	add r0, r1, r2
	add r0, r8
	ldrh r2, [r0]
	lsl r0, r2, #2
	add r0, sl
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1E
	lsl r5, r4, #0x18
	cmp r0, #0
	bne _08068A6E
	lsl r0, r2, #2
	ldr r3, _08068A80 @ =0x02011C20
	add r0, r3, r0
	ldrb r0, [r0, #9]
	lsr r0, r0, #6
	cmp r0, #0
	beq _08068B54
_08068A6E:
	lsr r0, r5, #0x18
	add r2, r0, #0
	cmp r0, #1
	beq _08068A94
	cmp r0, #1
	bgt _08068A84
	cmp r0, #0
	beq _08068A8A
	b _08068AAC
_08068A80: .4byte 0x02011C20
_08068A84:
	cmp r2, #2
	beq _08068A9E
	b _08068AAC
_08068A8A:
	mov r2, #0xE5
	lsl r2, r2, #3
	add r0, r1, r2
	add r0, r9
	b _08068AA6
_08068A94:
	mov r2, #0xE5
	lsl r2, r2, #3
	add r0, r1, r2
	add r0, r8
	b _08068AA6
_08068A9E:
	mov r2, #0xE5
	lsl r2, r2, #3
	add r0, r1, r2
	add r0, ip
_08068AA6:
	ldrh r0, [r0]
	mov r3, sp
	strh r0, [r3]
_08068AAC:
	mov r1, sp
	ldrh r1, [r1]
	lsl r0, r1, #0x10
	lsr r3, r0, #0x10
	lsr r1, r5, #0x18
	add r2, r6, #0
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	cmp r1, #1
	beq _08068ADE
	cmp r1, #1
	bgt _08068ACC
	cmp r1, #0
	beq _08068AD2
	b _08068B54
_08068ACC:
	cmp r1, #2
	beq _08068AEA
	b _08068B54
_08068AD2:
	lsl r0, r2, #1
	mov r2, #0xE5
	lsl r2, r2, #3
	add r0, r0, r2
	add r0, r9
	b _08068B52
_08068ADE:
	lsl r0, r2, #1
	mov r5, #0xE5
	lsl r5, r5, #3
	add r0, r0, r5
	add r0, r8
	b _08068B52
_08068AEA:
	lsl r0, r2, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	add r0, r0, r1
	b _08068B50
_08068AF4:
	lsl r0, r7, #1
	mov r2, #0xE5
	lsl r2, r2, #3
	add r0, r0, r2
	add r0, ip
	ldrh r1, [r0]
	lsl r0, r1, #2
	add r0, sl
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1E
	cmp r0, #0
	beq _08068B54
	lsl r5, r4, #0x18
	add r3, r1, #0
	lsr r1, r5, #0x18
	add r2, r6, #0
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	cmp r1, #1
	beq _08068B3C
	cmp r1, #1
	bgt _08068B2A
	cmp r1, #0
	beq _08068B30
	b _08068B54
_08068B2A:
	cmp r1, #2
	beq _08068B48
	b _08068B54
_08068B30:
	lsl r0, r2, #1
	mov r5, #0xE5
	lsl r5, r5, #3
	add r0, r0, r5
	add r0, r9
	b _08068B52
_08068B3C:
	lsl r0, r2, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	add r0, r0, r1
	add r0, r8
	b _08068B52
_08068B48:
	lsl r0, r2, #1
	mov r2, #0xE5
	lsl r2, r2, #3
	add r0, r0, r2
_08068B50:
	add r0, ip
_08068B52:
	strh r3, [r0]
_08068B54:
	add r0, r7, #1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	ldr r3, [sp, #8]
	ldr r5, _08068B8C @ =0x0201EFBA
	add r0, r3, r5
	ldrh r0, [r0]
	cmp r7, r0
	bcs _08068B68
	b _080689A4
_08068B68:
	ldr r7, [sp, #8]
	ldr r1, _08068B8C @ =0x0201EFBA
	add r0, r7, r1
	strh r6, [r0]
_08068B70:
	ldr r2, [sp, #4]
	lsl r0, r2, #0x10
	lsr r4, r0, #0x10
	cmp r4, #2
	bhi _08068B7C
	b _08068964
_08068B7C:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08068B8C: .4byte 0x0201EFBA
	thumb_func_end sub_080686E8

