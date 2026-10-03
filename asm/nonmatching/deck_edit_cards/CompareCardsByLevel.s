	thumb_func_start CompareCardsByLevel
CompareCardsByLevel: @ 0x08069014
	lsl r1, r1, #0x10
	lsr r3, r1, #0x10
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldr r0, _08069040 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08069044 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08069050
	cmp r0, #0x17
	ble _08069048
	cmp r0, #0x18
	beq _0806904C
	b _08069050
	.align 2, 0
_08069040: .4byte 0x000007FF
_08069044: .4byte gCardStats
_08069048:
	mov r2, #0
	b _08069064
_0806904C:
	mov r2, #0xA
	b _08069064
_08069050:
	ldr r0, _08069088 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _0806908C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r2, r0, #0x19
_08069064:
	ldr r0, _08069088 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0806908C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08069098
	cmp r0, #0x17
	ble _08069090
	cmp r0, #0x18
	beq _08069094
	b _08069098
	.align 2, 0
_08069088: .4byte 0x000007FF
_0806908C: .4byte gCardStats
_08069090:
	mov r0, #0
	b _080690AC
_08069094:
	mov r0, #0xA
	b _080690AC
_08069098:
	ldr r0, _080690BC @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _080690C0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_080690AC:
	mov r1, #0
	sub r0, r2, r0
	cmp r0, #0
	ble _080690B6
	mov r1, #1
_080690B6:
	add r0, r1, #0
	bx lr
	.align 2, 0
_080690BC: .4byte 0x000007FF
_080690C0: .4byte gCardStats
	thumb_func_end CompareCardsByLevel

