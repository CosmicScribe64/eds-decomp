	thumb_func_start sub_080611AC
sub_080611AC: @ 0x080611AC
	push {r4, r5, r6, lr}
	mov r5, #0
_080611B0:
	mov r4, #0
	add r6, r5, #1
_080611B4:
	add r0, r5, #0
	mov r1, #0
	add r2, r4, #0
	bl sub_08061004
	add r0, r5, #0
	mov r1, #5
	add r2, r4, #0
	bl sub_08061004
	add r4, #1
	cmp r4, #4
	ble _080611B4
	add r0, r5, #0
	mov r1, #0xA
	mov r2, #0
	bl sub_08061004
	add r0, r5, #0
	mov r1, #0xC
	mov r2, #0
	bl sub_08061004
	add r0, r5, #0
	mov r1, #0xD
	mov r2, #0
	bl sub_08061004
	add r0, r5, #0
	mov r1, #0xE
	mov r2, #0
	bl sub_08061004
	add r0, r5, #0
	mov r1, #0xF
	mov r2, #0
	bl sub_08061004
	add r5, r6, #0
	cmp r5, #1
	ble _080611B0
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	thumb_func_end sub_080611AC

