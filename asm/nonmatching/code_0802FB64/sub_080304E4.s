	thumb_func_start sub_080304E4
sub_080304E4: @ 0x080304E4
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	mov r2, #7
	ldrb r0, [r4, #0xA]
	and r2, r0
	cmp r2, #1
	bne _08030570
	ldrb r6, [r4, #0xC]
	ldrh r1, [r4, #0xC]
	lsr r5, r1, #8
	and r2, r6
	mov r0, #0x94
	mul r0, r5
	ldr r1, _0803052C @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08030530 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08030570
	mov r7, #0
	ldr r0, _08030534 @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08030538 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	cmp r1, #0x58
	beq _08030540
	ldr r0, _0803053C @ =0x000001FF
	cmp r1, r0
	beq _08030550
	b _08030554
_0803052C: .4byte 0x00000D64
_08030530: .4byte 0x0201930C
_08030534: .4byte 0x000007FF
_08030538: .4byte gUnk_08622AB4
_0803053C: .4byte 0x000001FF
_08030540:
	add r0, r6, #0
	add r1, r5, #0
	bl sub_0800C894
	bl sub_0807548C
	add r7, r0, #0
	b _08030554
_08030550:
	mov r7, #0xFA
	lsl r7, r7, #1
_08030554:
	add r0, r6, #0
	add r1, r5, #0
	bl sub_08017FF4
	cmp r0, #0
	beq _08030570
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	add r1, r7, #0
	bl sub_08019860
_08030570:
	mov r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_080304E4

