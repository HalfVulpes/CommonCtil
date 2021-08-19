.code

;cx dx
cmtSpinLockEnter proc
	xor al, al
	mov ah, 1
	cSLEcmp:
	cmpxchg byte ptr [rcx], ah
	jnz cSLEcmp
	ret
cmtSpinLockEnter endp

end