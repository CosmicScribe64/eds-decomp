	thumb_func_start sub_08063EDC
sub_08063EDC: @ 0x08063EDC
	push {r4, lr}
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	add r2, r1, #0
	cmp r1, #0x17
	bne _08063EEA
	b _08064220
_08063EEA:
	cmp r1, #0x17
	bgt _08063F34
	cmp r1, #6
	bne _08063EF4
	b _08064064
_08063EF4:
	cmp r1, #6
	bgt _08063F0E
	cmp r1, #4
	bne _08063EFE
	b _08063FFC
_08063EFE:
	cmp r1, #4
	ble _08063F04
	b _08064020
_08063F04:
	cmp r1, #1
	bge _08063F0A
	b _0806425C
_08063F0A:
	mov r0, #1
	b _0806425E
_08063F0E:
	cmp r1, #0xC
	bne _08063F14
	b _080641B0
_08063F14:
	cmp r1, #0xC
	bgt _08063F26
	cmp r1, #7
	bne _08063F1E
	b _080641D4
_08063F1E:
	cmp r1, #0xB
	bne _08063F24
	b _0806419C
_08063F24:
	b _0806425C
_08063F26:
	cmp r1, #0x15
	bne _08063F2C
	b _08064088
_08063F2C:
	cmp r1, #0x16
	bne _08063F32
	b _080641F8
_08063F32:
	b _0806425C
_08063F34:
	mov r0, #0xFC
	lsl r0, r0, #1
	cmp r1, r0
	bne _08063F3E
	b _080640EC
_08063F3E:
	cmp r1, r0
	bgt _08063F6A
	sub r0, #3
	cmp r1, r0
	beq _08063FB6
	cmp r1, r0
	bgt _08063F58
	cmp r1, #0x21
	beq _08063FAC
	cmp r1, #0x29
	bne _08063F56
	b _080640CC
_08063F56:
	b _0806425C
_08063F58:
	mov r0, #0xFB
	lsl r0, r0, #1
	cmp r1, r0
	beq _08063FA0
	add r0, #1
	cmp r1, r0
	bne _08063F68
	b _080641C0
_08063F68:
	b _0806425C
_08063F6A:
	ldr r0, _08063F88 @ =0x000001FB
	cmp r1, r0
	beq _08063FA6
	cmp r1, r0
	bgt _08063F8C
	sub r0, #2
	cmp r1, r0
	bne _08063F7C
	b _08064140
_08063F7C:
	add r0, #1
	cmp r1, r0
	bne _08063F84
	b _0806420C
_08063F84:
	b _0806425C
	.align 2, 0
_08063F88: .4byte 0x000001FB
_08063F8C:
	mov r0, #0xFE
	lsl r0, r0, #1
	cmp r2, r0
	bne _08063F96
	b _080641E8
_08063F96:
	add r0, #1
	cmp r2, r0
	bne _08063F9E
	b _08064240
_08063F9E:
	b _0806425C
_08063FA0:
	bl sub_08063BAC
	b _08063FB0
_08063FA6:
	bl sub_08063C14
	b _08063FB0
_08063FAC:
	bl sub_08063C7C
_08063FB0:
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0806425E
_08063FB6:
	mov r3, #0
	ldr r2, _08063FEC @ =0x02011C20
	ldr r1, _08063FF0 @ =0x000020D4
	add r0, r2, r1
	ldrh r0, [r0]
	lsl r1, r0, #0x15
	lsr r1, r1, #0x15
	ldr r4, _08063FF4 @ =0x000020D8
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	add r1, r1, r0
	add r4, #4
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	add r1, r1, r0
	add r4, #4
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	add r1, r1, r0
	ldr r0, _08063FF8 @ =0x000020E4
	b _08064120
_08063FEC: .4byte 0x02011C20
_08063FF0: .4byte 0x000020D4
_08063FF4: .4byte 0x000020D8
_08063FF8: .4byte 0x000020E4
_08063FFC:
	mov r2, #0
	ldr r1, _08064014 @ =0x02011C20
	ldr r3, _08064018 @ =0x000020D4
	add r0, r1, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #9
	bgt _08064010
	b _0806418C
_08064010:
	ldr r4, _0806401C @ =0x000020D8
	b _08064154
_08064014: .4byte 0x02011C20
_08064018: .4byte 0x000020D4
_0806401C: .4byte 0x000020D8
_08064020:
	mov r3, #0
	ldr r2, _08064058 @ =0x02011C20
	ldr r4, _0806405C @ =0x000020E8
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r1, r0, #0x15
	lsr r1, r1, #0x15
	add r4, #4
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	add r1, r1, r0
	add r4, #4
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	add r1, r1, r0
	add r4, #4
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	add r1, r1, r0
	ldr r0, _08064060 @ =0x000020F8
	b _08064120
	.align 2, 0
_08064058: .4byte 0x02011C20
_0806405C: .4byte 0x000020E8
_08064060: .4byte 0x000020F8
_08064064:
	mov r2, #0
	ldr r1, _0806407C @ =0x02011C20
	ldr r3, _08064080 @ =0x000020E8
	add r0, r1, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #9
	bgt _08064078
	b _0806418C
_08064078:
	ldr r4, _08064084 @ =0x000020EC
	b _08064154
_0806407C: .4byte 0x02011C20
_08064080: .4byte 0x000020E8
_08064084: .4byte 0x000020EC
_08064088:
	mov r3, #0
	ldr r2, _080640C0 @ =0x02011C20
	ldr r4, _080640C4 @ =0x000020FC
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r1, r0, #0x15
	lsr r1, r1, #0x15
	add r4, #4
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	add r1, r1, r0
	add r4, #4
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	add r1, r1, r0
	add r4, #4
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	add r1, r1, r0
	ldr r0, _080640C8 @ =0x0000210C
	b _08064120
	.align 2, 0
_080640C0: .4byte 0x02011C20
_080640C4: .4byte 0x000020FC
_080640C8: .4byte 0x0000210C
_080640CC:
	mov r2, #0
	ldr r1, _080640E4 @ =0x02011C20
	ldr r3, _080640E8 @ =0x000020FC
	add r0, r1, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #9
	ble _0806418C
	mov r4, #0x84
	lsl r4, r4, #6
	b _08064154
_080640E4: .4byte 0x02011C20
_080640E8: .4byte 0x000020FC
_080640EC:
	mov r3, #0
	ldr r2, _08064134 @ =0x02011C20
	ldr r4, _08064138 @ =0x00002110
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r1, r0, #0x15
	lsr r1, r1, #0x15
	add r4, #4
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	add r1, r1, r0
	add r4, #4
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	add r1, r1, r0
	add r4, #4
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	add r1, r1, r0
	ldr r0, _0806413C @ =0x00002120
_08064120:
	add r2, r2, r0
	ldrh r2, [r2]
	lsl r0, r2, #0x15
	lsr r0, r0, #0x15
	add r1, r1, r0
	cmp r1, #9
	ble _08064130
	mov r3, #1
_08064130:
	add r0, r3, #0
	b _0806425E
_08064134: .4byte 0x02011C20
_08064138: .4byte 0x00002110
_0806413C: .4byte 0x00002120
_08064140:
	mov r2, #0
	ldr r1, _08064190 @ =0x02011C20
	ldr r3, _08064194 @ =0x00002110
	add r0, r1, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #9
	ble _0806418C
	ldr r4, _08064198 @ =0x00002114
_08064154:
	add r0, r1, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #9
	ble _0806418C
	add r3, #8
	add r0, r1, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #9
	ble _0806418C
	add r4, #8
	add r0, r1, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #9
	ble _0806418C
	add r3, #8
	add r0, r1, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #9
	ble _0806418C
	mov r2, #1
_0806418C:
	add r0, r2, #0
	b _0806425E
_08064190: .4byte 0x02011C20
_08064194: .4byte 0x00002110
_08064198: .4byte 0x00002114
_0806419C:
	mov r1, #0
	ldr r0, _080641A8 @ =0x02011C20
	ldr r4, _080641AC @ =0x000020D4
	add r0, r0, r4
	b _08064228
	.align 2, 0
_080641A8: .4byte 0x02011C20
_080641AC: .4byte 0x000020D4
_080641B0:
	mov r1, #0
	ldr r0, _080641B8 @ =0x02011C20
	ldr r2, _080641BC @ =0x000020DC
	b _08064226
_080641B8: .4byte 0x02011C20
_080641BC: .4byte 0x000020DC
_080641C0:
	mov r1, #0
	ldr r0, _080641CC @ =0x02011C20
	ldr r3, _080641D0 @ =0x000020F4
	add r0, r0, r3
	b _08064228
	.align 2, 0
_080641CC: .4byte 0x02011C20
_080641D0: .4byte 0x000020F4
_080641D4:
	mov r1, #0
	ldr r0, _080641E0 @ =0x02011C20
	ldr r4, _080641E4 @ =0x000020F8
	add r0, r0, r4
	b _08064228
	.align 2, 0
_080641E0: .4byte 0x02011C20
_080641E4: .4byte 0x000020F8
_080641E8:
	mov r1, #0
	ldr r0, _080641F0 @ =0x02011C20
	ldr r2, _080641F4 @ =0x00002108
	b _08064226
_080641F0: .4byte 0x02011C20
_080641F4: .4byte 0x00002108
_080641F8:
	mov r1, #0
	ldr r0, _08064204 @ =0x02011C20
	ldr r3, _08064208 @ =0x0000210C
	add r0, r0, r3
	b _08064228
	.align 2, 0
_08064204: .4byte 0x02011C20
_08064208: .4byte 0x0000210C
_0806420C:
	mov r1, #0
	ldr r0, _08064218 @ =0x02011C20
	ldr r4, _0806421C @ =0x00002110
	add r0, r0, r4
	b _08064228
	.align 2, 0
_08064218: .4byte 0x02011C20
_0806421C: .4byte 0x00002110
_08064220:
	mov r1, #0
	ldr r0, _08064238 @ =0x02011C20
	ldr r2, _0806423C @ =0x00002120
_08064226:
	add r0, r0, r2
_08064228:
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #0x13
	ble _08064234
	mov r1, #1
_08064234:
	add r0, r1, #0
	b _0806425E
_08064238: .4byte 0x02011C20
_0806423C: .4byte 0x00002120
_08064240:
	ldr r0, _08064254 @ =0x02011C20
	ldr r3, _08064258 @ =0x00002128
	add r0, r0, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #0
	beq _0806425E
	mov r0, #1
	b _0806425E
_08064254: .4byte 0x02011C20
_08064258: .4byte 0x00002128
_0806425C:
	mov r0, #0
_0806425E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08063EDC

