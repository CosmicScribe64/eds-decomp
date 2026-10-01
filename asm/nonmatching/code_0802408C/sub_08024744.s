	thumb_func_start sub_08024744
sub_08024744: @ 0x08024744
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r1, _08024970 @ =0xFFFFFE80
	ldr r6, _08024974 @ =0x02015DC8
	mov r0, #0
	mov r2, #0
	add r3, r6, #0
	bl sub_080787F4
	ldr r0, _08024978 @ =0x086A12EC
	mov r1, #0xC0
	lsl r1, r1, #0x13
	mov r2, #0x96
	lsl r2, r2, #7
	bl CpuSet
	ldr r0, _0802497C @ =0x086AA8EC
	mov r1, #0xA0
	lsl r1, r1, #0x13
	mov r4, #0x80
	lsl r4, r4, #1
	add r2, r4, #0
	bl CpuSet
	ldr r0, _08024980 @ =0x086AFA28
	ldr r1, _08024984 @ =0x05000200
	mov r2, #0x10
	bl CpuSet
	mov r0, sp
	mov r5, #0
	strh r5, [r0]
	ldr r1, _08024988 @ =0x06014000
	ldr r2, _0802498C @ =0x01000010
	bl CpuSet
	ldr r0, _08024990 @ =0x086AFA48
	ldr r1, _08024994 @ =0x06014020
	add r2, r4, #0
	bl CpuSet
	ldr r0, _08024998 @ =0x086AFC48
	ldr r1, _0802499C @ =0x06014220
	add r2, r4, #0
	bl CpuSet
	ldr r0, _080249A0 @ =0x086AFE48
	ldr r1, _080249A4 @ =0x06014420
	add r2, r4, #0
	bl CpuSet
	ldr r0, _080249A8 @ =0x086B0048
	ldr r1, _080249AC @ =0x06014620
	add r2, r4, #0
	bl CpuSet
	ldr r0, _080249B0 @ =0x086B0248
	ldr r1, _080249B4 @ =0x06014820
	add r2, r4, #0
	bl CpuSet
	ldr r0, _080249B8 @ =0x086B0448
	ldr r1, _080249BC @ =0x06014A20
	add r2, r4, #0
	bl CpuSet
	ldr r0, _080249C0 @ =0x086B0648
	ldr r1, _080249C4 @ =0x06014C20
	add r2, r4, #0
	bl CpuSet
	ldr r0, _080249C8 @ =0x086B0848
	ldr r1, _080249CC @ =0x06014E20
	add r2, r4, #0
	bl CpuSet
	ldr r0, _080249D0 @ =0x086B0A48
	ldr r1, _080249D4 @ =0x06015020
	add r2, r4, #0
	bl CpuSet
	ldr r0, _080249D8 @ =0x086B0C48
	ldr r1, _080249DC @ =0x06015220
	add r2, r4, #0
	bl CpuSet
	ldr r0, _080249E0 @ =0x086B0E48
	ldr r1, _080249E4 @ =0x06015420
	add r2, r4, #0
	bl CpuSet
	ldr r0, _080249E8 @ =0x086B1048
	ldr r1, _080249EC @ =0x06015620
	add r2, r4, #0
	bl CpuSet
	ldr r0, _080249F0 @ =0x086B1248
	ldr r1, _080249F4 @ =0x06015820
	add r2, r4, #0
	bl CpuSet
	ldr r0, _080249F8 @ =0x086B1448
	ldr r1, _080249FC @ =0x06015A20
	add r2, r4, #0
	bl CpuSet
	ldr r0, _08024A00 @ =0x086B1648
	ldr r1, _08024A04 @ =0x06015C20
	add r2, r4, #0
	bl CpuSet
	ldr r0, _08024A08 @ =0x086B1848
	ldr r1, _08024A0C @ =0x06015E20
	add r2, r4, #0
	bl CpuSet
	ldr r0, _08024A10 @ =0x086B1A48
	ldr r1, _08024A14 @ =0x06016020
	add r2, r4, #0
	bl CpuSet
	ldr r0, _08024A18 @ =0x086B1C48
	ldr r1, _08024A1C @ =0x06016220
	add r2, r4, #0
	bl CpuSet
	ldr r0, _08024A20 @ =0x086B1E48
	ldr r1, _08024A24 @ =0x060164A0
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08024A28 @ =0x086B1EC8
	ldr r1, _08024A2C @ =0x06016520
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08024A30 @ =0x086B1F48
	ldr r1, _08024A34 @ =0x060165A0
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08024A38 @ =0x086B1FC8
	ldr r1, _08024A3C @ =0x06016620
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08024A40 @ =0x086B2048
	ldr r1, _08024A44 @ =0x060166A0
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08024A48 @ =0x086B20C8
	ldr r1, _08024A4C @ =0x06016720
	mov r2, #0x40
	bl CpuSet
	ldr r0, _08024A50 @ =0x086B2148
	ldr r1, _08024A54 @ =0x05000220
	mov r2, #0x10
	bl CpuSet
	mov r0, sp
	add r0, #2
	strh r5, [r0]
	ldr r1, _08024A58 @ =0x06016420
	ldr r2, _08024A5C @ =0x01000040
	bl CpuSet
	mov r2, #0
	ldr r0, _08024A60 @ =0xFFFFF4B8
	add r6, r6, r0
	mov r5, #0xC3
	lsl r5, r5, #3
	mov r3, #0
	ldr r4, _08024A64 @ =0x0000061A
_080248A6:
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #3
	add r0, r0, r6
	add r1, r0, r5
	strh r3, [r1]
	add r0, r0, r4
	strh r3, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #2
	bls _080248A6
	ldr r4, _08024A68 @ =0x02015D2C
	add r0, r4, #0
	bl sub_08024D48
	mov r2, #0
	ldr r1, _08024A6C @ =0xFFFFF554
	add r4, r4, r1
	ldr r7, _08024A70 @ =0x00000B15
	mov ip, r7
	mov r3, #0
	add r5, r4, #0
	ldr r0, _08024A74 @ =0x00000B14
	mov r8, r0
	ldr r6, _08024A78 @ =0x00000B16
_080248DC:
	lsl r0, r2, #2
	add r0, r0, r4
	mov r7, ip
	add r1, r0, r7
	strb r3, [r1]
	mov r7, r8
	add r1, r0, r7
	strb r3, [r1]
	add r0, r0, r6
	strb r3, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #3
	bls _080248DC
	mov r2, #0
	ldr r4, _08024A7C @ =0x02015280
	ldr r3, _08024A80 @ =0x00000B2C
	mov r1, #0
_08024902:
	lsl r0, r2, #2
	add r0, r0, r4
	add r0, r0, r3
	strh r1, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #1
	bls _08024902
	ldr r1, _08024A84 @ =0x00000B3C
	add r0, r5, r1
	mov r1, #0
	strb r1, [r0]
	ldr r2, _08024A88 @ =0x00000B3D
	add r0, r5, r2
	strb r1, [r0]
	ldr r7, _08024A8C @ =0x00000B24
	add r2, r5, r7
	mov r0, #0xFF
	strb r0, [r2]
	ldr r2, _08024A90 @ =0x00000B25
	add r0, r5, r2
	strb r1, [r0]
	add r7, #4
	add r0, r5, r7
	mov r3, #0
	strh r1, [r0]
	ldr r0, _08024A94 @ =0x00000B36
	add r2, r5, r0
	mov r0, #0xFA
	strb r0, [r2]
	ldr r2, _08024A98 @ =0x00000B37
	add r0, r5, r2
	strb r3, [r0]
	add r7, #0x10
	add r0, r5, r7
	strh r1, [r0]
	add r2, #3
	add r0, r5, r2
	strh r1, [r0]
	add r7, #8
	add r0, r5, r7
	strh r1, [r0]
	mov r1, #0x80
	lsl r1, r1, #0x13
	ldr r2, _08024A9C @ =0x00001F44
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #1
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08024970: .4byte 0xFFFFFE80
_08024974: .4byte 0x02015DC8
_08024978: .4byte gUnk_086A12EC
_0802497C: .4byte gUnk_086AA8EC
_08024980: .4byte gUnk_086AFA28
_08024984: .4byte 0x05000200
_08024988: .4byte 0x06014000
_0802498C: .4byte 0x01000010
_08024990: .4byte gUnk_086AFA48
_08024994: .4byte 0x06014020
_08024998: .4byte gUnk_086AFC48
_0802499C: .4byte 0x06014220
_080249A0: .4byte gUnk_086AFE48
_080249A4: .4byte 0x06014420
_080249A8: .4byte gUnk_086B0048
_080249AC: .4byte 0x06014620
_080249B0: .4byte gUnk_086B0248
_080249B4: .4byte 0x06014820
_080249B8: .4byte gUnk_086B0448
_080249BC: .4byte 0x06014A20
_080249C0: .4byte gUnk_086B0648
_080249C4: .4byte 0x06014C20
_080249C8: .4byte gUnk_086B0848
_080249CC: .4byte 0x06014E20
_080249D0: .4byte gUnk_086B0A48
_080249D4: .4byte 0x06015020
_080249D8: .4byte gUnk_086B0C48
_080249DC: .4byte 0x06015220
_080249E0: .4byte gUnk_086B0E48
_080249E4: .4byte 0x06015420
_080249E8: .4byte gUnk_086B1048
_080249EC: .4byte 0x06015620
_080249F0: .4byte gUnk_086B1248
_080249F4: .4byte 0x06015820
_080249F8: .4byte gUnk_086B1448
_080249FC: .4byte 0x06015A20
_08024A00: .4byte gUnk_086B1648
_08024A04: .4byte 0x06015C20
_08024A08: .4byte gUnk_086B1848
_08024A0C: .4byte 0x06015E20
_08024A10: .4byte gUnk_086B1A48
_08024A14: .4byte 0x06016020
_08024A18: .4byte gUnk_086B1C48
_08024A1C: .4byte 0x06016220
_08024A20: .4byte gUnk_086B1E48
_08024A24: .4byte 0x060164A0
_08024A28: .4byte gUnk_086B1EC8
_08024A2C: .4byte 0x06016520
_08024A30: .4byte gUnk_086B1F48
_08024A34: .4byte 0x060165A0
_08024A38: .4byte gUnk_086B1FC8
_08024A3C: .4byte 0x06016620
_08024A40: .4byte gUnk_086B2048
_08024A44: .4byte 0x060166A0
_08024A48: .4byte gUnk_086B20C8
_08024A4C: .4byte 0x06016720
_08024A50: .4byte gUnk_086B2148
_08024A54: .4byte 0x05000220
_08024A58: .4byte 0x06016420
_08024A5C: .4byte 0x01000040
_08024A60: .4byte 0xFFFFF4B8
_08024A64: .4byte 0x0000061A
_08024A68: .4byte 0x02015D2C
_08024A6C: .4byte 0xFFFFF554
_08024A70: .4byte 0x00000B15
_08024A74: .4byte 0x00000B14
_08024A78: .4byte 0x00000B16
_08024A7C: .4byte 0x02015280
_08024A80: .4byte 0x00000B2C
_08024A84: .4byte 0x00000B3C
_08024A88: .4byte 0x00000B3D
_08024A8C: .4byte 0x00000B24
_08024A90: .4byte 0x00000B25
_08024A94: .4byte 0x00000B36
_08024A98: .4byte 0x00000B37
_08024A9C: .4byte 0x00001F44
	thumb_func_end sub_08024744

