	thumb_func_start sub_0802E114
sub_0802E114: @ 0x0802E114
	push {r4, r5, lr}
	add r4, r0, #0
	add r3, r1, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	beq _0802E122
	b _0802E3C0
_0802E122:
	cmp r3, #0
	bne _0802E128
	b _0802E3C0
_0802E128:
	mov r0, #1
	add r1, r0, #0
	ldrb r2, [r3, #2]
	and r1, r2
	ldrb r2, [r4, #2]
	and r0, r2
	cmp r1, r0
	bne _0802E13A
	b _0802E3C0
_0802E13A:
	ldr r2, _0802E164 @ =0x000007FF
	add r0, r2, #0
	ldrh r4, [r4]
	and r0, r4
	lsl r0, r0, #1
	ldr r4, _0802E168 @ =0x08622AB4
	add r0, r0, r4
	ldrh r1, [r0]
	ldr r0, _0802E16C @ =0x000003FE
	cmp r1, r0
	beq _0802E1BA
	cmp r1, r0
	bgt _0802E170
	sub r0, #3
	cmp r1, r0
	beq _0802E194
	add r0, #2
	cmp r1, r0
	beq _0802E1A8
	b _0802E3C0
	.align 2, 0
_0802E164: .4byte 0x000007FF
_0802E168: .4byte gUnk_08622AB4
_0802E16C: .4byte 0x000003FE
_0802E170:
	ldr r0, _0802E184 @ =0x000004DF
	cmp r1, r0
	beq _0802E1FC
	cmp r1, r0
	bgt _0802E188
	sub r0, #0xB9
	cmp r1, r0
	beq _0802E1DC
	b _0802E3C0
	.align 2, 0
_0802E184: .4byte 0x000004DF
_0802E188:
	ldr r0, _0802E190 @ =0x000005FB
	cmp r1, r0
	beq _0802E268
	b _0802E3C0
_0802E190: .4byte 0x000005FB
_0802E194:
	mov r5, #0
	add r0, r2, #0
	ldrh r3, [r3]
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r4
	ldr r1, _0802E1A4 @ =0x0000014F
	b _0802E1EA
_0802E1A4: .4byte 0x0000014F
_0802E1A8:
	mov r5, #0
	add r0, r2, #0
	ldrh r3, [r3]
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r4
	mov r1, #0xFC
	lsl r1, r1, #2
	b _0802E1EA
_0802E1BA:
	mov r4, #0
	add r0, r2, #0
	ldrh r3, [r3]
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _0802E1D8 @ =0x08622AB4
	add r0, r0, r1
	mov r1, #0xA8
	lsl r1, r1, #1
	ldrh r0, [r0]
	cmp r0, r1
	bne _0802E1D4
	mov r4, #1
_0802E1D4:
	add r0, r4, #0
	b _0802E3C2
_0802E1D8: .4byte gUnk_08622AB4
_0802E1DC:
	mov r5, #0
	add r0, r2, #0
	ldrh r3, [r3]
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r4
	ldr r1, _0802E1F8 @ =0x0000029F
_0802E1EA:
	ldrh r0, [r0]
	cmp r0, r1
	bne _0802E1F2
	mov r5, #1
_0802E1F2:
	add r0, r5, #0
	b _0802E3C2
	.align 2, 0
_0802E1F8: .4byte 0x0000029F
_0802E1FC:
	add r0, r2, #0
	ldrh r3, [r3]
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r1, [r0]
	ldr r0, _0802E22C @ =0x000003F2
	cmp r1, r0
	bne _0802E210
	b _0802E3B8
_0802E210:
	cmp r1, r0
	bgt _0802E23C
	sub r0, #0x2A
	cmp r1, r0
	bne _0802E21C
	b _0802E3B8
_0802E21C:
	cmp r1, r0
	bgt _0802E234
	ldr r0, _0802E230 @ =0x00000159
	cmp r1, r0
	ble _0802E228
	b _0802E3C0
_0802E228:
	sub r0, #8
	b _0802E344
_0802E22C: .4byte 0x000003F2
_0802E230: .4byte 0x00000159
_0802E234:
	ldr r0, _0802E238 @ =0x000003EF
	b _0802E33E
_0802E238: .4byte 0x000003EF
_0802E23C:
	ldr r0, _0802E25C @ =0x0000042F
	cmp r1, r0
	bgt _0802E260
	sub r0, #1
	cmp r1, r0
	blt _0802E24A
	b _0802E3B8
_0802E24A:
	sub r0, #0x2D
	cmp r1, r0
	bne _0802E252
	b _0802E3B8
_0802E252:
	add r0, #0xE
_0802E254:
	cmp r1, r0
	bne _0802E25A
	b _0802E3B8
_0802E25A:
	b _0802E3C0
_0802E25C: .4byte 0x0000042F
_0802E260:
	ldr r0, _0802E264 @ =0x00000439
	b _0802E254
_0802E264: .4byte 0x00000439
_0802E268:
	add r0, r2, #0
	ldrh r3, [r3]
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r1, [r0]
	ldr r0, _0802E2A8 @ =0x00000424
	cmp r1, r0
	bne _0802E27C
	b _0802E3B8
_0802E27C:
	cmp r1, r0
	bgt _0802E314
	sub r0, #0x34
	cmp r1, r0
	bne _0802E288
	b _0802E3B8
_0802E288:
	cmp r1, r0
	bgt _0802E2D8
	ldr r0, _0802E2AC @ =0x00000147
	cmp r1, r0
	bgt _0802E2B0
	sub r0, #9
	cmp r1, r0
	blt _0802E29A
	b _0802E3B8
_0802E29A:
	sub r0, #2
	cmp r1, r0
	ble _0802E2A2
	b _0802E3C0
_0802E2A2:
	sub r0, #0x10
	b _0802E344
	.align 2, 0
_0802E2A8: .4byte 0x00000424
_0802E2AC: .4byte 0x00000147
_0802E2B0:
	ldr r0, _0802E2C0 @ =0x0000028D
	cmp r1, r0
	bne _0802E2B8
	b _0802E3B8
_0802E2B8:
	cmp r1, r0
	bgt _0802E2C4
	sub r0, #2
	b _0802E3A0
_0802E2C0: .4byte 0x0000028D
_0802E2C4:
	mov r0, #0xA4
	lsl r0, r0, #2
	cmp r1, r0
	bne _0802E2CE
	b _0802E3B8
_0802E2CE:
	ldr r0, _0802E2D4 @ =0x000003C2
	b _0802E3A0
	.align 2, 0
_0802E2D4: .4byte 0x000003C2
_0802E2D8:
	ldr r0, _0802E2F4 @ =0x00000413
	cmp r1, r0
	bgt _0802E300
	sub r0, #1
	cmp r1, r0
	bge _0802E3B8
	sub r0, #0x13
	cmp r1, r0
	beq _0802E3B8
	cmp r1, r0
	bgt _0802E2F8
	sub r0, #0xA
	b _0802E33E
	.align 2, 0
_0802E2F4: .4byte 0x00000413
_0802E2F8:
	ldr r0, _0802E2FC @ =0x00000403
	b _0802E3A0
_0802E2FC: .4byte 0x00000403
_0802E300:
	ldr r0, _0802E310 @ =0x00000416
	cmp r1, r0
	blt _0802E3C0
	add r0, #1
	cmp r1, r0
	ble _0802E3B8
	add r0, #0xB
	b _0802E3A0
_0802E310: .4byte 0x00000416
_0802E314:
	ldr r0, _0802E338 @ =0x00000521
	cmp r1, r0
	beq _0802E3B8
	cmp r1, r0
	bgt _0802E374
	sub r0, #0x9C
	cmp r1, r0
	beq _0802E3B8
	cmp r1, r0
	bgt _0802E350
	sub r0, #0x55
	cmp r1, r0
	beq _0802E3B8
	cmp r1, r0
	bgt _0802E33C
	sub r0, #4
	b _0802E3A0
	.align 2, 0
_0802E338: .4byte 0x00000521
_0802E33C:
	ldr r0, _0802E34C @ =0x00000434
_0802E33E:
	cmp r1, r0
	bgt _0802E3C0
	sub r0, #1
_0802E344:
	cmp r1, r0
	blt _0802E3C0
	b _0802E3B8
	.align 2, 0
_0802E34C: .4byte 0x00000434
_0802E350:
	ldr r0, _0802E360 @ =0x0000049E
	cmp r1, r0
	beq _0802E3B8
	cmp r1, r0
	bgt _0802E364
	sub r0, #0x16
	b _0802E3A0
	.align 2, 0
_0802E360: .4byte 0x0000049E
_0802E364:
	ldr r0, _0802E370 @ =0x000004BB
	cmp r1, r0
	beq _0802E3B8
	add r0, #9
	b _0802E3A0
	.align 2, 0
_0802E370: .4byte 0x000004BB
_0802E374:
	ldr r0, _0802E390 @ =0x000005AB
	cmp r1, r0
	bgt _0802E394
	sub r0, #3
	cmp r1, r0
	bge _0802E3B8
	sub r0, #0x1D
	cmp r1, r0
	blt _0802E3C0
	add r0, #1
	cmp r1, r0
	ble _0802E3B8
	add r0, #2
	b _0802E3A0
_0802E390: .4byte 0x000005AB
_0802E394:
	ldr r0, _0802E3A8 @ =0x0000060A
	cmp r1, r0
	beq _0802E3B8
	cmp r1, r0
	bgt _0802E3AC
	sub r0, #6
_0802E3A0:
	cmp r1, r0
	beq _0802E3B8
	b _0802E3C0
	.align 2, 0
_0802E3A8: .4byte 0x0000060A
_0802E3AC:
	ldr r0, _0802E3BC @ =0x0000060C
	cmp r1, r0
	beq _0802E3B8
	add r0, #2
	cmp r1, r0
	bne _0802E3C0
_0802E3B8:
	mov r0, #1
	b _0802E3C2
_0802E3BC: .4byte 0x0000060C
_0802E3C0:
	mov r0, #0
_0802E3C2:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0802E114

