.code

cmtSpinLockEnter proc
	xor al, al
	mov ah, 1
	cSLEcmp:
	cmpxchg byte ptr [rcx], ah
	jnz cSLEcmp
	ret
cmtSpinLockEnter endp

cmtSpinLockLeave proc
	dec byte ptr [rcx]
	ret
cmtSpinLockLeave endp

end