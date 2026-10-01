	thumb_func_start sub_080417DC
sub_080417DC: @ 0x080417DC
	push {r4, r5, lr}
	add r5, r0, #0
	ldr r0, _0804180C @ =0x02017A40
	ldr r1, _08041810 @ =0x000003E5
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #0
	bne _08041820
	ldr r0, _08041814 @ =0x00000206
	ldr r1, _08041818 @ =0x00000712
	ldr r3, _0804181C @ =0x08084BD4
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #8
	neg r0, r0
	ldrb r2, [r5, #0xA]
	and r0, r2
	strb r0, [r5, #0xA]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0804188E
	.align 2, 0
_0804180C: .4byte 0x02017A40
_08041810: .4byte 0x000003E5
_08041814: .4byte 0x00000206
_08041818: .4byte 0x00000712
_0804181C: .4byte gUnk_08084BD4
_08041820:
	mov r0, #0xE0
	lsl r0, r0, #0x10
	bl sub_08052F38
	cmp r0, #0
	beq _0804188E
	ldr r0, _08041878 @ =0x0201CFB0
	ldr r2, _0804187C @ =0x00000824
	add r1, r0, r2
	ldr r4, [r1]
	add r2, #4
	add r1, r0, r2
	add r2, #4
	add r0, r0, r2
	ldr r1, [r1]
	ldr r0, [r0]
	add r3, r1, r0
	mov r2, #1
	and r2, r4
	mov r0, #0x94
	mul r0, r3
	ldr r1, _08041880 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08041884 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08041888
	ldrh r1, [r5, #6]
	lsr r0, r1, #8
	cmp r0, r3
	beq _08041888
	add r0, r5, #0
	add r1, r4, #0
	add r2, r3, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08041888
	mov r0, #1
	b _08041890
_08041878: .4byte 0x0201CFB0
_0804187C: .4byte 0x00000824
_08041880: .4byte 0x00000D64
_08041884: .4byte 0x0201930C
_08041888:
	mov r0, #3
	bl sub_08077AEC
_0804188E:
	mov r0, #0
_08041890:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_080417DC
	.align 2, 0

