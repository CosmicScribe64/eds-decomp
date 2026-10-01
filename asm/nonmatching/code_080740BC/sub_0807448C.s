	thumb_func_start sub_0807448C
sub_0807448C: @ 0x0807448C
	push {r4, r5, lr}
	bl sub_08063BAC
	cmp r0, #0
	bne _080744AE
	mov r5, #0
_08074498:
	add r4, r5, #1
	add r0, r4, #0
	bl sub_08077948
	add r0, r4, #0
	bl sub_08077948
	add r5, r4, #0
	cmp r5, #4
	ble _08074498
	b _0807454C
_080744AE:
	bl sub_08063C14
	cmp r0, #0
	bne _080744D4
	mov r5, #0
_080744B8:
	add r4, r5, #6
	add r0, r4, #0
	bl sub_08077948
	add r0, r4, #0
	bl sub_08077948
	add r0, r4, #0
	bl sub_08077948
	add r5, #1
	cmp r5, #4
	ble _080744B8
	b _0807454C
_080744D4:
	bl sub_08063C7C
	cmp r0, #0
	bne _08074502
	mov r5, #0
_080744DE:
	add r4, r5, #0
	add r4, #0xB
	add r0, r4, #0
	bl sub_08077948
	add r0, r4, #0
	bl sub_08077948
	add r0, r4, #0
	bl sub_08077948
	add r0, r4, #0
	bl sub_08077948
	add r5, #1
	cmp r5, #4
	ble _080744DE
	b _0807454C
_08074502:
	bl sub_08063CE4
	cmp r0, #0
	bne _08074536
	mov r5, #0
_0807450C:
	add r4, r5, #0
	add r4, #0x10
	add r0, r4, #0
	bl sub_08077948
	add r0, r4, #0
	bl sub_08077948
	add r0, r4, #0
	bl sub_08077948
	add r0, r4, #0
	bl sub_08077948
	add r0, r4, #0
	bl sub_08077948
	add r5, #1
	cmp r5, #4
	ble _0807450C
	b _0807454C
_08074536:
	mov r5, #0
_08074538:
	add r5, #1
	mov r4, #0x13
_0807453C:
	add r0, r5, #0
	bl sub_08077948
	sub r4, #1
	cmp r4, #0
	bge _0807453C
	cmp r5, #0x13
	ble _08074538
_0807454C:
	mov r0, #1
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0807448C

