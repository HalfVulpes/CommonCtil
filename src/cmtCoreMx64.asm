;cmtCore assembly support
;amd64 version for masm
.code

;cx dx r8 r9
;void cmtSpinLockEnter(cmtUint8* value, cmtUint64 MaxSpin)
cmtSpinLockEnter proc
	;ah=1
	xor ah, ah
	inc ah

	;检查MaxSpin是否为-1
	cmp rdx, 0ffffffffffffffffh
	je cSLE_NoCount

	cSLE_SpinStart:
	;减法模式，控制循环次数
	test rdx, rdx
	;自旋数到达上限
	jz cSLE_EndMaxSpin
	dec rdx
	
	xor al, al
	;cmpxchg：
	;if(op1==ax)
	;{
	;	op1=op2;
	;	rflags.zf=1;
	;}
	;else
	;{
	;	ax=op1;
	;	rflags.zf=0;
	;}
	lock cmpxchg byte ptr [rcx], ah
	;MaxSpin=0：允许进入
	jz cSLE_EndNormal
	;MaxSpin!=0：继续自旋
	jmp cSLE_SpinStart

	cSLE_NoCount:
	xor al, al
	cmpxchg byte ptr [rcx], ah
	jz cSLE_EndNormal
	jmp cSLE_NoCount

	cSLE_EndMaxSpin:
	xor rax, rax
	inc rax
	ret

	cSLE_EndNormal:
	xor rax, rax
	ret
cmtSpinLockEnter endp

end