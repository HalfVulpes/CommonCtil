.code

;cx dx r8 r9
;void cmtSpinLockEnter(cmtUint8* value, cmtUint64 MaxSpin)
cmtSpinLockEnter proc
	mov ah, 1

	cmp rdx, 0ffffffffffffffffh
	je cSLEcmp_nocount

	cSLEcmp:
	test rdx, rdx
	jz cSLEend
	dec rdx
	
	xor al, al
	cmpxchg byte ptr [rcx], ah
	jnz cSLEcmp
	jmp cSLEend

	cSLEcmp_nocount:
	xor al, al
	cmpxchg byte ptr [rcx], ah
	jnz cSLEcmp_nocount

	cSLEend:
	ret
cmtSpinLockEnter endp

end