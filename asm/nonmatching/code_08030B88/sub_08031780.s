	thumb_func_start sub_08031780
sub_08031780: @ 0x08031780
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0x100
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	beq _08031798
	b _080318E8
_08031798:
	ldr r4, _080317B0 @ =0x02017A40
	mov r0, #0xF8
	lsl r0, r0, #2
	add r2, r4, r0
	ldrb r0, [r2]
	cmp r0, #0x7F
	beq _080317C8
	cmp r0, #0x7F
	bgt _080317B4
	cmp r0, #0x7E
	beq _08031848
	b _080318E8
_080317B0: .4byte 0x02017A40
_080317B4:
	cmp r0, #0x80
	beq _080317BA
	b _080318E8
_080317BA:
	ldr r1, _08031830 @ =0x000003E1
	add r0, r4, r1
	mov r1, #2
	strb r1, [r0]
	ldrb r0, [r2]
	sub r0, #1
	strb r0, [r2]
_080317C8:
	mov r2, #0
	ldr r4, _08031834 @ =0x020192E4
	ldrb r0, [r5, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	ldr r5, _08031838 @ =0x00000D64
	mul r1, r5
	add r1, r1, r4
	add r7, r0, #0
	ldrb r1, [r1, #3]
	cmp r2, r1
	bge _0803182A
	lsl r3, r7, #0x1F
	mov r6, #1
	ldr r0, _0803183C @ =0x000007C4
	add r0, r0, r4
	mov r9, r0
	mov r1, #0xD4
	lsl r1, r1, #1
	mov r8, r1
	mov ip, r4
	ldr r4, _08031840 @ =0x000007FF
_080317F4:
	lsr r0, r3, #0x1F
	add r1, r6, #0
	and r1, r0
	lsl r0, r2, #2
	mul r1, r5
	add r0, r0, r1
	add r0, r9
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _08031844 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r8
	beq _080318A4
	add r2, #1
	lsr r0, r3, #0x1F
	add r1, r6, #0
	and r1, r0
	add r0, r1, #0
	mul r0, r5
	add r0, ip
	ldrb r0, [r0, #3]
	cmp r2, r0
	blt _080317F4
_0803182A:
	mov r0, #1
	and r0, r7
	b _08031886
_08031830: .4byte 0x000003E1
_08031834: .4byte 0x020192E4
_08031838: .4byte 0x00000D64
_0803183C: .4byte 0x000007C4
_08031840: .4byte 0x000007FF
_08031844: .4byte gUnk_08622AB4
_08031848:
	ldr r0, _08031878 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _08031880
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0xD4
	lsl r1, r1, #1
	bl sub_0801970C
	cmp r0, #0
	beq _08031880
	ldr r0, _0803187C @ =0x000003E1
	add r1, r4, r0
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	cmp r0, #0
	beq _08031880
	mov r0, #0x7F
	b _080318EA
	.align 2, 0
_08031878: .4byte 0x0201AE60
_0803187C: .4byte 0x000003E1
_08031880:
	mov r0, #1
	ldrb r5, [r5, #2]
	and r0, r5
_08031886:
	mov r1, #0x60
	cmp r0, #0
	beq _0803188E
	ldr r1, _080318A0 @ =0x00008060
_0803188E:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x64
	b _080318EA
	.align 2, 0
_080318A0: .4byte 0x00008060
_080318A4:
	ldr r1, _080318D4 @ =0x08082AB4
	lsl r0, r0, #1
	ldr r2, _080318D8 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r0, _080318DC @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl sub_080753F4
	ldr r0, _080318E0 @ =0x00000205
	ldr r1, _080318E4 @ =0x00000914
	mov r2, #0xB
	mov r3, sp
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	mov r0, #0x7E
	b _080318EA
_080318D4: .4byte gUnk_08082AB4
_080318D8: .4byte gUnk_08623DF4
_080318DC: .4byte gUnk_0822C720
_080318E0: .4byte 0x00000205
_080318E4: .4byte 0x00000914
_080318E8:
	mov r0, #0
_080318EA:
	add sp, #0x100
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08031780

