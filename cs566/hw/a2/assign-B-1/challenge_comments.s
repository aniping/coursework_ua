
challenge:     file format elf64-x86-64


Disassembly of section .init:

00000000004004d8 <_init>:
  4004d8:	48 83 ec 08          	sub    $0x8,%rsp
  4004dc:	e8 ab 00 00 00       	callq  40058c <call_gmon_start>
  4004e1:	48 83 c4 08          	add    $0x8,%rsp
  4004e5:	c3                   	retq   

Disassembly of section .plt:

00000000004004f0 <__stack_chk_fail@plt-0x10>:
  4004f0:	ff 35 fa 1a 20 00    	pushq  0x201afa(%rip)        # 601ff0 <_GLOBAL_OFFSET_TABLE_+0x8>
  4004f6:	ff 25 fc 1a 20 00    	jmpq   *0x201afc(%rip)        # 601ff8 <_GLOBAL_OFFSET_TABLE_+0x10>
  4004fc:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400500 <__stack_chk_fail@plt>:
  400500:	ff 25 fa 1a 20 00    	jmpq   *0x201afa(%rip)        # 602000 <_GLOBAL_OFFSET_TABLE_+0x18>
  400506:	68 00 00 00 00       	pushq  $0x0
  40050b:	e9 e0 ff ff ff       	jmpq   4004f0 <_init+0x18>

0000000000400510 <printf@plt>:
  400510:	ff 25 f2 1a 20 00    	jmpq   *0x201af2(%rip)        # 602008 <_GLOBAL_OFFSET_TABLE_+0x20>
  400516:	68 01 00 00 00       	pushq  $0x1
  40051b:	e9 d0 ff ff ff       	jmpq   4004f0 <_init+0x18>

0000000000400520 <gettimeofday@plt>:
  400520:	ff 25 ea 1a 20 00    	jmpq   *0x201aea(%rip)        # 602010 <_GLOBAL_OFFSET_TABLE_+0x28>
  400526:	68 02 00 00 00       	pushq  $0x2
  40052b:	e9 c0 ff ff ff       	jmpq   4004f0 <_init+0x18>

0000000000400530 <__libc_start_main@plt>:
  400530:	ff 25 e2 1a 20 00    	jmpq   *0x201ae2(%rip)        # 602018 <_GLOBAL_OFFSET_TABLE_+0x30>
  400536:	68 03 00 00 00       	pushq  $0x3
  40053b:	e9 b0 ff ff ff       	jmpq   4004f0 <_init+0x18>

0000000000400540 <strtoul@plt>:
  400540:	ff 25 da 1a 20 00    	jmpq   *0x201ada(%rip)        # 602020 <_GLOBAL_OFFSET_TABLE_+0x38>
  400546:	68 04 00 00 00       	pushq  $0x4
  40054b:	e9 a0 ff ff ff       	jmpq   4004f0 <_init+0x18>

0000000000400550 <exit@plt>:
  400550:	ff 25 d2 1a 20 00    	jmpq   *0x201ad2(%rip)        # 602028 <_GLOBAL_OFFSET_TABLE_+0x40>
  400556:	68 05 00 00 00       	pushq  $0x5
  40055b:	e9 90 ff ff ff       	jmpq   4004f0 <_init+0x18>

Disassembly of section .text:

0000000000400560 <_start>:
  400560:	31 ed                	xor    %ebp,%ebp
  400562:	49 89 d1             	mov    %rdx,%r9
  400565:	5e                   	pop    %rsi
  400566:	48 89 e2             	mov    %rsp,%rdx
  400569:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  40056d:	50                   	push   %rax
  40056e:	54                   	push   %rsp
  40056f:	49 c7 c0 d0 18 40 00 	mov    $0x4018d0,%r8
  400576:	48 c7 c1 40 18 40 00 	mov    $0x401840,%rcx
  40057d:	48 c7 c7 7f 06 40 00 	mov    $0x40067f,%rdi
  400584:	e8 a7 ff ff ff       	callq  400530 <__libc_start_main@plt>
  400589:	f4                   	hlt    
  40058a:	90                   	nop
  40058b:	90                   	nop

000000000040058c <call_gmon_start>:
  40058c:	48 83 ec 08          	sub    $0x8,%rsp
  400590:	48 8b 05 49 1a 20 00 	mov    0x201a49(%rip),%rax        # 601fe0 <_DYNAMIC+0x1d0>
  400597:	48 85 c0             	test   %rax,%rax
  40059a:	74 02                	je     40059e <call_gmon_start+0x12>
  40059c:	ff d0                	callq  *%rax
  40059e:	48 83 c4 08          	add    $0x8,%rsp
  4005a2:	c3                   	retq   
  4005a3:	90                   	nop
  4005a4:	90                   	nop
  4005a5:	90                   	nop
  4005a6:	90                   	nop
  4005a7:	90                   	nop
  4005a8:	90                   	nop
  4005a9:	90                   	nop
  4005aa:	90                   	nop
  4005ab:	90                   	nop
  4005ac:	90                   	nop
  4005ad:	90                   	nop
  4005ae:	90                   	nop
  4005af:	90                   	nop

00000000004005b0 <deregister_tm_clones>:
  4005b0:	b8 c7 d8 60 00       	mov    $0x60d8c7,%eax
  4005b5:	55                   	push   %rbp
  4005b6:	48 2d c0 d8 60 00    	sub    $0x60d8c0,%rax
  4005bc:	48 83 f8 0e          	cmp    $0xe,%rax
  4005c0:	48 89 e5             	mov    %rsp,%rbp
  4005c3:	76 1b                	jbe    4005e0 <deregister_tm_clones+0x30>
  4005c5:	b8 00 00 00 00       	mov    $0x0,%eax
  4005ca:	48 85 c0             	test   %rax,%rax
  4005cd:	74 11                	je     4005e0 <deregister_tm_clones+0x30>
  4005cf:	5d                   	pop    %rbp
  4005d0:	bf c0 d8 60 00       	mov    $0x60d8c0,%edi
  4005d5:	ff e0                	jmpq   *%rax
  4005d7:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
  4005de:	00 00 
  4005e0:	5d                   	pop    %rbp
  4005e1:	c3                   	retq   
  4005e2:	66 66 66 66 66 2e 0f 	data16 data16 data16 data16 nopw %cs:0x0(%rax,%rax,1)
  4005e9:	1f 84 00 00 00 00 00 

00000000004005f0 <register_tm_clones>:
  4005f0:	be c0 d8 60 00       	mov    $0x60d8c0,%esi
  4005f5:	55                   	push   %rbp
  4005f6:	48 81 ee c0 d8 60 00 	sub    $0x60d8c0,%rsi
  4005fd:	48 c1 fe 03          	sar    $0x3,%rsi
  400601:	48 89 e5             	mov    %rsp,%rbp
  400604:	48 89 f0             	mov    %rsi,%rax
  400607:	48 c1 e8 3f          	shr    $0x3f,%rax
  40060b:	48 01 c6             	add    %rax,%rsi
  40060e:	48 d1 fe             	sar    %rsi
  400611:	74 15                	je     400628 <register_tm_clones+0x38>
  400613:	b8 00 00 00 00       	mov    $0x0,%eax
  400618:	48 85 c0             	test   %rax,%rax
  40061b:	74 0b                	je     400628 <register_tm_clones+0x38>
  40061d:	5d                   	pop    %rbp
  40061e:	bf c0 d8 60 00       	mov    $0x60d8c0,%edi
  400623:	ff e0                	jmpq   *%rax
  400625:	0f 1f 00             	nopl   (%rax)
  400628:	5d                   	pop    %rbp
  400629:	c3                   	retq   
  40062a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000400630 <__do_global_dtors_aux>:
  400630:	80 3d 89 d2 20 00 00 	cmpb   $0x0,0x20d289(%rip)        # 60d8c0 <__bss_start>
  400637:	75 11                	jne    40064a <__do_global_dtors_aux+0x1a>
  400639:	55                   	push   %rbp
  40063a:	48 89 e5             	mov    %rsp,%rbp
  40063d:	e8 6e ff ff ff       	callq  4005b0 <deregister_tm_clones>
  400642:	5d                   	pop    %rbp
  400643:	c6 05 76 d2 20 00 01 	movb   $0x1,0x20d276(%rip)        # 60d8c0 <__bss_start>
  40064a:	f3 c3                	repz retq 
  40064c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400650 <frame_dummy>:
  400650:	bf 08 1e 60 00       	mov    $0x601e08,%edi
  400655:	48 83 3f 00          	cmpq   $0x0,(%rdi)
  400659:	75 05                	jne    400660 <frame_dummy+0x10>
  40065b:	eb 93                	jmp    4005f0 <register_tm_clones>
  40065d:	0f 1f 00             	nopl   (%rax)
  400660:	b8 00 00 00 00       	mov    $0x0,%eax
  400665:	48 85 c0             	test   %rax,%rax
  400668:	74 f1                	je     40065b <frame_dummy+0xb>
  40066a:	55                   	push   %rbp
  40066b:	48 89 e5             	mov    %rsp,%rbp
  40066e:	ff d0                	callq  *%rax
  400670:	5d                   	pop    %rbp
  400671:	e9 7a ff ff ff       	jmpq   4005f0 <register_tm_clones>
  400676:	90                   	nop
  400677:	90                   	nop

0000000000400678 <megaInit>:
  400678:	55                   	push   %rbp
  400679:	48 89 e5             	mov    %rsp,%rbp
  40067c:	90                   	nop
  40067d:	5d                   	pop    %rbp
  40067e:	c3                   	retq   

000000000040067f <main>:
  40067f:	55                   	push   %rbp
  400680:	48 89 e5             	mov    %rsp,%rbp
  400683:	48 83 ec 40          	sub    $0x40,%rsp
  400687:	89 7d cc             	mov    %edi,-0x34(%rbp)
  40068a:	48 89 75 c0          	mov    %rsi,-0x40(%rbp)
  40068e:	e8 8f 11 00 00       	callq  401822 <obfmegaInit>
  400693:	48 8b 45 c0          	mov    -0x40(%rbp),%rax
  400697:	89 c2                	mov    %eax,%edx
  400699:	8b 05 25 d2 20 00    	mov    0x20d225(%rip),%eax        # 60d8c4 <_obf_1_main_entropy>
  40069f:	21 d0                	and    %edx,%eax
  4006a1:	89 05 1d d2 20 00    	mov    %eax,0x20d21d(%rip)        # 60d8c4 <_obf_1_main_entropy>
  4006a7:	8b 05 17 d2 20 00    	mov    0x20d217(%rip),%eax        # 60d8c4 <_obf_1_main_entropy>
  4006ad:	0f af 45 cc          	imul   -0x34(%rbp),%eax
  4006b1:	89 05 0d d2 20 00    	mov    %eax,0x20d20d(%rip)        # 60d8c4 <_obf_1_main_entropy>
  4006b7:	e8 bc ff ff ff       	callq  400678 <megaInit>
  4006bc:	83 7d cc 02          	cmpl   $0x2,-0x34(%rbp)
  4006c0:	74 1e                	je     4006e0 <main+0x61>
  4006c2:	be 01 00 00 00       	mov    $0x1,%esi
  4006c7:	bf f0 18 40 00       	mov    $0x4018f0,%edi
  4006cc:	b8 00 00 00 00       	mov    $0x0,%eax
  4006d1:	e8 3a fe ff ff       	callq  400510 <printf@plt>
  4006d6:	bf ff ff ff ff       	mov    $0xffffffff,%edi
  4006db:	e8 70 fe ff ff       	callq  400550 <exit@plt>
  4006e0:	c7 45 d8 00 00 00 00 	movl   $0x0,-0x28(%rbp)
  4006e7:	eb 43                	jmp    40072c <main+0xad>
  4006e9:	8b 45 d8             	mov    -0x28(%rbp),%eax
  4006ec:	48 98                	cltq   
  4006ee:	48 83 c0 01          	add    $0x1,%rax
  4006f2:	48 8d 14 c5 00 00 00 	lea    0x0(,%rax,8),%rdx
  4006f9:	00 
  4006fa:	48 8b 45 c0          	mov    -0x40(%rbp),%rax
  4006fe:	48 01 d0             	add    %rdx,%rax
  400701:	48 8b 00             	mov    (%rax),%rax
  400704:	ba 0a 00 00 00       	mov    $0xa,%edx
  400709:	be 00 00 00 00       	mov    $0x0,%esi
  40070e:	48 89 c7             	mov    %rax,%rdi
  400711:	e8 2a fe ff ff       	callq  400540 <strtoul@plt>
  400716:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  40071a:	8b 45 d8             	mov    -0x28(%rbp),%eax
  40071d:	48 98                	cltq   
  40071f:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
  400723:	48 89 54 c5 e0       	mov    %rdx,-0x20(%rbp,%rax,8)
  400728:	83 45 d8 01          	addl   $0x1,-0x28(%rbp)
  40072c:	83 7d d8 00          	cmpl   $0x0,-0x28(%rbp)
  400730:	7e b7                	jle    4006e9 <main+0x6a>
  400732:	8b 05 8c d1 20 00    	mov    0x20d18c(%rip),%eax        # 60d8c4 <_obf_1_main_entropy>
  400738:	8b 55 d8             	mov    -0x28(%rbp),%edx
  40073b:	89 c1                	mov    %eax,%ecx
  40073d:	d3 e2                	shl    %cl,%edx
  40073f:	89 d0                	mov    %edx,%eax
  400741:	89 05 7d d1 20 00    	mov    %eax,0x20d17d(%rip)        # 60d8c4 <_obf_1_main_entropy>
  400747:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40074b:	89 c2                	mov    %eax,%edx
  40074d:	8b 05 71 d1 20 00    	mov    0x20d171(%rip),%eax        # 60d8c4 <_obf_1_main_entropy>
  400753:	21 d0                	and    %edx,%eax
  400755:	89 05 69 d1 20 00    	mov    %eax,0x20d169(%rip)        # 60d8c4 <_obf_1_main_entropy>
  40075b:	48 8d 55 f0          	lea    -0x10(%rbp),%rdx
  40075f:	48 8d 45 e0          	lea    -0x20(%rbp),%rax
  400763:	48 89 d6             	mov    %rdx,%rsi
  400766:	48 89 c7             	mov    %rax,%rdi
  400769:	e8 45 00 00 00       	callq  4007b3 <SECRET>
  40076e:	c7 45 dc 00 00 00 00 	movl   $0x0,-0x24(%rbp)
  400775:	eb 20                	jmp    400797 <main+0x118>
  400777:	8b 45 dc             	mov    -0x24(%rbp),%eax
  40077a:	48 98                	cltq   
  40077c:	48 8b 44 c5 f0       	mov    -0x10(%rbp,%rax,8),%rax
  400781:	48 89 c6             	mov    %rax,%rsi
  400784:	bf 15 19 40 00       	mov    $0x401915,%edi
  400789:	b8 00 00 00 00       	mov    $0x0,%eax
  40078e:	e8 7d fd ff ff       	callq  400510 <printf@plt>
  400793:	83 45 dc 01          	addl   $0x1,-0x24(%rbp)
  400797:	83 7d dc 00          	cmpl   $0x0,-0x24(%rbp)
  40079b:	7e da                	jle    400777 <main+0xf8>
  40079d:	8b 05 21 d1 20 00    	mov    0x20d121(%rip),%eax        # 60d8c4 <_obf_1_main_entropy>
  4007a3:	33 45 dc             	xor    -0x24(%rbp),%eax
  4007a6:	89 05 18 d1 20 00    	mov    %eax,0x20d118(%rip)        # 60d8c4 <_obf_1_main_entropy>
  4007ac:	b8 00 00 00 00       	mov    $0x0,%eax
  4007b1:	c9                   	leaveq 
  4007b2:	c3                   	retq   

00000000004007b3 <SECRET>:
  4007b3:	55                   	push   %rbp
  4007b4:	48 89 e5             	mov    %rsp,%rbp
  4007b7:	53                   	push   %rbx
  4007b8:	48 81 ec b8 01 00 00 	sub    $0x1b8,%rsp
  4007bf:	48 89 bd 48 fe ff ff 	mov    %rdi,-0x1b8(%rbp)
  4007c6:	48 89 b5 40 fe ff ff 	mov    %rsi,-0x1c0(%rbp)
  4007cd:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  4007d4:	00 00 
  4007d6:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
  4007da:	31 c0                	xor    %eax,%eax
  4007dc:	48 8d 85 70 fe ff ff 	lea    -0x190(%rbp),%rax
  4007e3:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  4007ea:	48 c7 85 60 fe ff ff 	movq   $0x602100,-0x1a0(%rbp)
  4007f1:	00 21 60 00 
  4007f5:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4007fc:	48 8b 00             	mov    (%rax),%rax
  4007ff:	eb 33                	jmp    400834 <SECRET+0x81>
  400801:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400808:	48 83 c0 08          	add    $0x8,%rax
  40080c:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400813:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40081a:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  400821:	0f b7 12             	movzwl (%rdx),%edx
  400824:	0f b7 d2             	movzwl %dx,%edx
  400827:	89 10                	mov    %edx,(%rax)
  400829:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400830:	48 8b 00             	mov    (%rax),%rax
  400833:	90                   	nop
  400834:	ff e0                	jmpq   *%rax
  400836:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40083d:	48 83 c0 08          	add    $0x8,%rax
  400841:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400848:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40084f:	48 8d 50 08          	lea    0x8(%rax),%rdx
  400853:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40085a:	8b 00                	mov    (%rax),%eax
  40085c:	89 02                	mov    %eax,(%rdx)
  40085e:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400865:	48 83 c0 08          	add    $0x8,%rax
  400869:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400870:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400877:	48 83 c0 08          	add    $0x8,%rax
  40087b:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400882:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400889:	48 8b 00             	mov    (%rax),%rax
  40088c:	eb a6                	jmp    400834 <SECRET+0x81>
  40088e:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400895:	48 83 c0 08          	add    $0x8,%rax
  400899:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  4008a0:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4008a7:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  4008ab:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4008b2:	48 83 e8 08          	sub    $0x8,%rax
  4008b6:	8b 08                	mov    (%rax),%ecx
  4008b8:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4008bf:	8b 00                	mov    (%rax),%eax
  4008c1:	39 c1                	cmp    %eax,%ecx
  4008c3:	0f 95 c0             	setne  %al
  4008c6:	0f b6 c0             	movzbl %al,%eax
  4008c9:	89 02                	mov    %eax,(%rdx)
  4008cb:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4008d2:	48 83 e8 08          	sub    $0x8,%rax
  4008d6:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  4008dd:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4008e4:	48 8b 00             	mov    (%rax),%rax
  4008e7:	e9 48 ff ff ff       	jmpq   400834 <SECRET+0x81>
  4008ec:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4008f3:	48 83 c0 08          	add    $0x8,%rax
  4008f7:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  4008fe:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400905:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  40090c:	8b 12                	mov    (%rdx),%edx
  40090e:	48 63 d2             	movslq %edx,%rdx
  400911:	48 89 10             	mov    %rdx,(%rax)
  400914:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40091b:	48 8b 00             	mov    (%rax),%rax
  40091e:	e9 11 ff ff ff       	jmpq   400834 <SECRET+0x81>
  400923:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40092a:	48 83 c0 08          	add    $0x8,%rax
  40092e:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400935:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40093c:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  400940:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400947:	48 8b 08             	mov    (%rax),%rcx
  40094a:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400951:	48 83 e8 08          	sub    $0x8,%rax
  400955:	48 8b 00             	mov    (%rax),%rax
  400958:	48 21 c8             	and    %rcx,%rax
  40095b:	48 89 02             	mov    %rax,(%rdx)
  40095e:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400965:	48 83 e8 08          	sub    $0x8,%rax
  400969:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400970:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400977:	48 8b 00             	mov    (%rax),%rax
  40097a:	e9 b5 fe ff ff       	jmpq   400834 <SECRET+0x81>
  40097f:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400986:	48 83 c0 08          	add    $0x8,%rax
  40098a:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400991:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400998:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  40099f:	48 8b 12             	mov    (%rdx),%rdx
  4009a2:	48 8b 12             	mov    (%rdx),%rdx
  4009a5:	48 89 10             	mov    %rdx,(%rax)
  4009a8:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4009af:	48 8b 00             	mov    (%rax),%rax
  4009b2:	e9 7d fe ff ff       	jmpq   400834 <SECRET+0x81>
  4009b7:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4009be:	48 83 c0 08          	add    $0x8,%rax
  4009c2:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  4009c9:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4009d0:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  4009d4:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4009db:	48 83 e8 08          	sub    $0x8,%rax
  4009df:	48 8b 30             	mov    (%rax),%rsi
  4009e2:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4009e9:	48 8b 00             	mov    (%rax),%rax
  4009ec:	89 c1                	mov    %eax,%ecx
  4009ee:	48 d3 e6             	shl    %cl,%rsi
  4009f1:	48 89 f0             	mov    %rsi,%rax
  4009f4:	48 89 02             	mov    %rax,(%rdx)
  4009f7:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4009fe:	48 83 e8 08          	sub    $0x8,%rax
  400a02:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400a09:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400a10:	48 8b 00             	mov    (%rax),%rax
  400a13:	e9 1c fe ff ff       	jmpq   400834 <SECRET+0x81>
  400a18:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400a1f:	48 83 c0 08          	add    $0x8,%rax
  400a23:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400a2a:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400a31:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  400a38:	48 8b 12             	mov    (%rdx),%rdx
  400a3b:	48 8b 12             	mov    (%rdx),%rdx
  400a3e:	48 89 10             	mov    %rdx,(%rax)
  400a41:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400a48:	48 8b 00             	mov    (%rax),%rax
  400a4b:	e9 e4 fd ff ff       	jmpq   400834 <SECRET+0x81>
  400a50:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400a57:	48 83 c0 08          	add    $0x8,%rax
  400a5b:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400a62:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400a69:	48 8d 50 08          	lea    0x8(%rax),%rdx
  400a6d:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400a74:	0f b7 00             	movzwl (%rax),%eax
  400a77:	66 89 02             	mov    %ax,(%rdx)
  400a7a:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400a81:	48 83 c0 08          	add    $0x8,%rax
  400a85:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400a8c:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400a93:	48 83 c0 08          	add    $0x8,%rax
  400a97:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400a9e:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400aa5:	48 8b 00             	mov    (%rax),%rax
  400aa8:	e9 87 fd ff ff       	jmpq   400834 <SECRET+0x81>
  400aad:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400ab4:	48 83 c0 08          	add    $0x8,%rax
  400ab8:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400abf:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400ac6:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  400aca:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400ad1:	8b 08                	mov    (%rax),%ecx
  400ad3:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400ada:	48 83 e8 08          	sub    $0x8,%rax
  400ade:	8b 00                	mov    (%rax),%eax
  400ae0:	0f af c1             	imul   %ecx,%eax
  400ae3:	89 02                	mov    %eax,(%rdx)
  400ae5:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400aec:	48 83 e8 08          	sub    $0x8,%rax
  400af0:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400af7:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400afe:	48 8b 00             	mov    (%rax),%rax
  400b01:	e9 2e fd ff ff       	jmpq   400834 <SECRET+0x81>
  400b06:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400b0d:	48 83 c0 08          	add    $0x8,%rax
  400b11:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400b18:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400b1f:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  400b23:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400b2a:	48 8b 08             	mov    (%rax),%rcx
  400b2d:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400b34:	48 83 e8 08          	sub    $0x8,%rax
  400b38:	48 8b 00             	mov    (%rax),%rax
  400b3b:	48 09 c8             	or     %rcx,%rax
  400b3e:	48 89 02             	mov    %rax,(%rdx)
  400b41:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400b48:	48 83 e8 08          	sub    $0x8,%rax
  400b4c:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400b53:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400b5a:	48 8b 00             	mov    (%rax),%rax
  400b5d:	e9 d2 fc ff ff       	jmpq   400834 <SECRET+0x81>
  400b62:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400b69:	48 83 c0 08          	add    $0x8,%rax
  400b6d:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400b74:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400b7b:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  400b7f:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400b86:	48 83 e8 08          	sub    $0x8,%rax
  400b8a:	48 8b 08             	mov    (%rax),%rcx
  400b8d:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400b94:	48 8b 00             	mov    (%rax),%rax
  400b97:	48 39 c1             	cmp    %rax,%rcx
  400b9a:	0f 97 c0             	seta   %al
  400b9d:	0f b6 c0             	movzbl %al,%eax
  400ba0:	89 02                	mov    %eax,(%rdx)
  400ba2:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400ba9:	48 83 e8 08          	sub    $0x8,%rax
  400bad:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400bb4:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400bbb:	48 8b 00             	mov    (%rax),%rax
  400bbe:	e9 71 fc ff ff       	jmpq   400834 <SECRET+0x81>
  400bc3:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400bca:	48 83 c0 08          	add    $0x8,%rax
  400bce:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400bd5:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400bdc:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  400be0:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400be7:	8b 08                	mov    (%rax),%ecx
  400be9:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400bf0:	48 83 e8 08          	sub    $0x8,%rax
  400bf4:	8b 00                	mov    (%rax),%eax
  400bf6:	09 c8                	or     %ecx,%eax
  400bf8:	89 02                	mov    %eax,(%rdx)
  400bfa:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400c01:	48 83 e8 08          	sub    $0x8,%rax
  400c05:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400c0c:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400c13:	48 8b 00             	mov    (%rax),%rax
  400c16:	e9 19 fc ff ff       	jmpq   400834 <SECRET+0x81>
  400c1b:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400c22:	48 83 c0 08          	add    $0x8,%rax
  400c26:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400c2d:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400c34:	48 8d 50 08          	lea    0x8(%rax),%rdx
  400c38:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400c3f:	8b 00                	mov    (%rax),%eax
  400c41:	48 98                	cltq   
  400c43:	48 8d 8d 70 ff ff ff 	lea    -0x90(%rbp),%rcx
  400c4a:	48 01 c8             	add    %rcx,%rax
  400c4d:	48 89 02             	mov    %rax,(%rdx)
  400c50:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400c57:	48 83 c0 08          	add    $0x8,%rax
  400c5b:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400c62:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400c69:	48 83 c0 08          	add    $0x8,%rax
  400c6d:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400c74:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400c7b:	48 8b 00             	mov    (%rax),%rax
  400c7e:	e9 b1 fb ff ff       	jmpq   400834 <SECRET+0x81>
  400c83:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400c8a:	48 83 c0 08          	add    $0x8,%rax
  400c8e:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400c95:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400c9c:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  400ca0:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400ca7:	48 83 e8 08          	sub    $0x8,%rax
  400cab:	48 8b 08             	mov    (%rax),%rcx
  400cae:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400cb5:	48 8b 00             	mov    (%rax),%rax
  400cb8:	48 39 c1             	cmp    %rax,%rcx
  400cbb:	0f 92 c0             	setb   %al
  400cbe:	0f b6 c0             	movzbl %al,%eax
  400cc1:	89 02                	mov    %eax,(%rdx)
  400cc3:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400cca:	48 83 e8 08          	sub    $0x8,%rax
  400cce:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400cd5:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400cdc:	48 8b 00             	mov    (%rax),%rax
  400cdf:	e9 50 fb ff ff       	jmpq   400834 <SECRET+0x81>
  400ce4:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400ceb:	48 83 c0 08          	add    $0x8,%rax
  400cef:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400cf6:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400cfd:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  400d04:	48 8b 12             	mov    (%rdx),%rdx
  400d07:	0f b7 12             	movzwl (%rdx),%edx
  400d0a:	66 89 10             	mov    %dx,(%rax)
  400d0d:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400d14:	48 8b 00             	mov    (%rax),%rax
  400d17:	e9 18 fb ff ff       	jmpq   400834 <SECRET+0x81>
  400d1c:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400d23:	48 83 c0 08          	add    $0x8,%rax
  400d27:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400d2e:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400d35:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  400d39:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400d40:	48 8b 08             	mov    (%rax),%rcx
  400d43:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400d4a:	48 83 e8 08          	sub    $0x8,%rax
  400d4e:	48 8b 00             	mov    (%rax),%rax
  400d51:	48 01 c8             	add    %rcx,%rax
  400d54:	48 89 02             	mov    %rax,(%rdx)
  400d57:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400d5e:	48 83 e8 08          	sub    $0x8,%rax
  400d62:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400d69:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400d70:	48 8b 00             	mov    (%rax),%rax
  400d73:	e9 bc fa ff ff       	jmpq   400834 <SECRET+0x81>
  400d78:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400d7f:	48 83 c0 08          	add    $0x8,%rax
  400d83:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400d8a:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400d91:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  400d95:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400d9c:	8b 08                	mov    (%rax),%ecx
  400d9e:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400da5:	48 83 e8 08          	sub    $0x8,%rax
  400da9:	8b 00                	mov    (%rax),%eax
  400dab:	39 c1                	cmp    %eax,%ecx
  400dad:	0f 94 c0             	sete   %al
  400db0:	0f b6 c0             	movzbl %al,%eax
  400db3:	89 02                	mov    %eax,(%rdx)
  400db5:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400dbc:	48 83 e8 08          	sub    $0x8,%rax
  400dc0:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400dc7:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400dce:	48 8b 00             	mov    (%rax),%rax
  400dd1:	e9 5e fa ff ff       	jmpq   400834 <SECRET+0x81>
  400dd6:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400ddd:	48 83 c0 08          	add    $0x8,%rax
  400de1:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400de8:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400def:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  400df6:	48 8b 12             	mov    (%rdx),%rdx
  400df9:	48 89 10             	mov    %rdx,(%rax)
  400dfc:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400e03:	48 8b 00             	mov    (%rax),%rax
  400e06:	e9 29 fa ff ff       	jmpq   400834 <SECRET+0x81>
  400e0b:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400e12:	48 83 c0 08          	add    $0x8,%rax
  400e16:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400e1d:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400e24:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  400e28:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400e2f:	8b 08                	mov    (%rax),%ecx
  400e31:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400e38:	48 83 e8 08          	sub    $0x8,%rax
  400e3c:	8b 00                	mov    (%rax),%eax
  400e3e:	21 c8                	and    %ecx,%eax
  400e40:	89 02                	mov    %eax,(%rdx)
  400e42:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400e49:	48 83 e8 08          	sub    $0x8,%rax
  400e4d:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400e54:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400e5b:	48 8b 00             	mov    (%rax),%rax
  400e5e:	e9 d1 f9 ff ff       	jmpq   400834 <SECRET+0x81>
  400e63:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400e6a:	48 83 c0 08          	add    $0x8,%rax
  400e6e:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400e75:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400e7c:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  400e80:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400e87:	48 8b 08             	mov    (%rax),%rcx
  400e8a:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400e91:	48 83 e8 08          	sub    $0x8,%rax
  400e95:	48 8b 00             	mov    (%rax),%rax
  400e98:	48 01 c8             	add    %rcx,%rax
  400e9b:	48 89 02             	mov    %rax,(%rdx)
  400e9e:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400ea5:	48 83 e8 08          	sub    $0x8,%rax
  400ea9:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400eb0:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400eb7:	48 8b 00             	mov    (%rax),%rax
  400eba:	e9 75 f9 ff ff       	jmpq   400834 <SECRET+0x81>
  400ebf:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400ec6:	48 83 c0 08          	add    $0x8,%rax
  400eca:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400ed1:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400ed8:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  400edc:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400ee3:	48 83 e8 08          	sub    $0x8,%rax
  400ee7:	8b 08                	mov    (%rax),%ecx
  400ee9:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400ef0:	8b 00                	mov    (%rax),%eax
  400ef2:	39 c1                	cmp    %eax,%ecx
  400ef4:	0f 9d c0             	setge  %al
  400ef7:	0f b6 c0             	movzbl %al,%eax
  400efa:	89 02                	mov    %eax,(%rdx)
  400efc:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400f03:	48 83 e8 08          	sub    $0x8,%rax
  400f07:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400f0e:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400f15:	48 8b 00             	mov    (%rax),%rax
  400f18:	e9 17 f9 ff ff       	jmpq   400834 <SECRET+0x81>
  400f1d:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400f24:	48 83 c0 08          	add    $0x8,%rax
  400f28:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400f2f:	90                   	nop
  400f30:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  400f34:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  400f3b:	00 00 
  400f3d:	0f 84 d5 08 00 00    	je     401818 <SECRET+0x1065>
  400f43:	e9 cb 08 00 00       	jmpq   401813 <SECRET+0x1060>
  400f48:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400f4f:	48 83 c0 08          	add    $0x8,%rax
  400f53:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400f5a:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400f61:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  400f68:	48 8b 12             	mov    (%rdx),%rdx
  400f6b:	48 89 10             	mov    %rdx,(%rax)
  400f6e:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400f75:	48 8b 00             	mov    (%rax),%rax
  400f78:	e9 b7 f8 ff ff       	jmpq   400834 <SECRET+0x81>
  400f7d:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400f84:	48 83 c0 08          	add    $0x8,%rax
  400f88:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400f8f:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400f96:	48 83 e8 08          	sub    $0x8,%rax
  400f9a:	48 8b 00             	mov    (%rax),%rax
  400f9d:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  400fa4:	48 8b 12             	mov    (%rdx),%rdx
  400fa7:	48 89 10             	mov    %rdx,(%rax)
  400faa:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400fb1:	48 83 e8 10          	sub    $0x10,%rax
  400fb5:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  400fbc:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400fc3:	48 8b 00             	mov    (%rax),%rax
  400fc6:	e9 69 f8 ff ff       	jmpq   400834 <SECRET+0x81>
  400fcb:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  400fd2:	48 83 c0 08          	add    $0x8,%rax
  400fd6:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  400fdd:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400fe4:	48 8b 00             	mov    (%rax),%rax
  400fe7:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  400fee:	48 83 ea 08          	sub    $0x8,%rdx
  400ff2:	48 8b 12             	mov    (%rdx),%rdx
  400ff5:	48 89 10             	mov    %rdx,(%rax)
  400ff8:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  400fff:	48 83 e8 10          	sub    $0x10,%rax
  401003:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  40100a:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401011:	48 8b 00             	mov    (%rax),%rax
  401014:	e9 1b f8 ff ff       	jmpq   400834 <SECRET+0x81>
  401019:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401020:	48 83 c0 08          	add    $0x8,%rax
  401024:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  40102b:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401032:	8b 00                	mov    (%rax),%eax
  401034:	85 c0                	test   %eax,%eax
  401036:	74 07                	je     40103f <SECRET+0x88c>
  401038:	83 f8 01             	cmp    $0x1,%eax
  40103b:	74 16                	je     401053 <SECRET+0x8a0>
  40103d:	eb 27                	jmp    401066 <SECRET+0x8b3>
  40103f:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401046:	48 83 c0 08          	add    $0x8,%rax
  40104a:	48 c7 00 80 20 60 00 	movq   $0x602080,(%rax)
  401051:	eb 13                	jmp    401066 <SECRET+0x8b3>
  401053:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40105a:	48 83 c0 08          	add    $0x8,%rax
  40105e:	48 c7 00 c4 d8 60 00 	movq   $0x60d8c4,(%rax)
  401065:	90                   	nop
  401066:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40106d:	48 83 c0 08          	add    $0x8,%rax
  401071:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  401078:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40107f:	48 83 c0 08          	add    $0x8,%rax
  401083:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  40108a:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401091:	48 8b 00             	mov    (%rax),%rax
  401094:	e9 9b f7 ff ff       	jmpq   400834 <SECRET+0x81>
  401099:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4010a0:	48 83 c0 08          	add    $0x8,%rax
  4010a4:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  4010ab:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4010b2:	8b 00                	mov    (%rax),%eax
  4010b4:	83 f8 01             	cmp    $0x1,%eax
  4010b7:	74 07                	je     4010c0 <SECRET+0x90d>
  4010b9:	83 f8 02             	cmp    $0x2,%eax
  4010bc:	74 3d                	je     4010fb <SECRET+0x948>
  4010be:	eb 63                	jmp    401123 <SECRET+0x970>
  4010c0:	48 8d 9d 70 ff ff ff 	lea    -0x90(%rbp),%rbx
  4010c7:	48 83 c3 4c          	add    $0x4c,%rbx
  4010cb:	48 8d 85 70 ff ff ff 	lea    -0x90(%rbp),%rax
  4010d2:	48 83 c0 40          	add    $0x40,%rax
  4010d6:	48 8b 10             	mov    (%rax),%rdx
  4010d9:	48 8d 85 70 ff ff ff 	lea    -0x90(%rbp),%rax
  4010e0:	48 83 c0 28          	add    $0x28,%rax
  4010e4:	48 8b 00             	mov    (%rax),%rax
  4010e7:	48 89 d6             	mov    %rdx,%rsi
  4010ea:	48 89 c7             	mov    %rax,%rdi
  4010ed:	b8 00 00 00 00       	mov    $0x0,%eax
  4010f2:	e8 29 f4 ff ff       	callq  400520 <gettimeofday@plt>
  4010f7:	89 03                	mov    %eax,(%rbx)
  4010f9:	eb 28                	jmp    401123 <SECRET+0x970>
  4010fb:	48 8d 85 70 ff ff ff 	lea    -0x90(%rbp),%rax
  401102:	48 83 c0 70          	add    $0x70,%rax
  401106:	48 8b 10             	mov    (%rax),%rdx
  401109:	48 8d 85 70 ff ff ff 	lea    -0x90(%rbp),%rax
  401110:	48 83 c0 64          	add    $0x64,%rax
  401114:	48 8b 00             	mov    (%rax),%rax
  401117:	48 89 d6             	mov    %rdx,%rsi
  40111a:	48 89 c7             	mov    %rax,%rdi
  40111d:	e8 91 f6 ff ff       	callq  4007b3 <SECRET>
  401122:	90                   	nop
  401123:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax // move the value stored at (%rbp-0x1a0) to %rax
  40112a:	48 83 c0 08          	add    $0x8,%rax
  40112e:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp) // store added value in %rax back to address: (%rbp-0x1a0)
  401135:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax // move it back, unnecessary step
  40113c:	48 8b 00             	mov    (%rax),%rax // %rax=6300312, then move the data at the address of 6300312 to %rax, now $rax=4197403 or 0x400c1b, which is the address of the instruction: mov    -0x1a0(%rbp),%rax
  40113f:	e9 f0 f6 ff ff       	jmpq   400834 <SECRET+0x81>
  401144:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40114b:	48 83 c0 08          	add    $0x8,%rax
  40114f:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  401156:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40115d:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  401164:	48 8b 12             	mov    (%rdx),%rdx
  401167:	48 89 10             	mov    %rdx,(%rax)
  40116a:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401171:	48 8b 00             	mov    (%rax),%rax
  401174:	e9 bb f6 ff ff       	jmpq   400834 <SECRET+0x81>
  401179:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401180:	48 83 c0 08          	add    $0x8,%rax
  401184:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  40118b:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401192:	48 8d 48 f8          	lea    -0x8(%rax),%rcx
  401196:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40119d:	48 83 e8 08          	sub    $0x8,%rax
  4011a1:	8b 00                	mov    (%rax),%eax
  4011a3:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  4011aa:	8b 32                	mov    (%rdx),%esi
  4011ac:	ba 00 00 00 00       	mov    $0x0,%edx
  4011b1:	f7 f6                	div    %esi
  4011b3:	89 d0                	mov    %edx,%eax
  4011b5:	66 89 01             	mov    %ax,(%rcx)
  4011b8:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4011bf:	48 83 e8 08          	sub    $0x8,%rax
  4011c3:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  4011ca:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4011d1:	48 8b 00             	mov    (%rax),%rax
  4011d4:	e9 5b f6 ff ff       	jmpq   400834 <SECRET+0x81>
  4011d9:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4011e0:	48 83 c0 08          	add    $0x8,%rax
  4011e4:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  4011eb:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4011f2:	8b 00                	mov    (%rax),%eax
  4011f4:	85 c0                	test   %eax,%eax
  4011f6:	74 1c                	je     401214 <SECRET+0xa61>
  4011f8:	83 f8 01             	cmp    $0x1,%eax
  4011fb:	75 2d                	jne    40122a <SECRET+0xa77>
  4011fd:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401204:	48 8d 50 08          	lea    0x8(%rax),%rdx
  401208:	48 8d 85 40 fe ff ff 	lea    -0x1c0(%rbp),%rax
  40120f:	48 89 02             	mov    %rax,(%rdx)
  401212:	eb 16                	jmp    40122a <SECRET+0xa77>
  401214:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40121b:	48 8d 50 08          	lea    0x8(%rax),%rdx
  40121f:	48 8d 85 48 fe ff ff 	lea    -0x1b8(%rbp),%rax
  401226:	48 89 02             	mov    %rax,(%rdx)
  401229:	90                   	nop
  40122a:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401231:	48 83 c0 08          	add    $0x8,%rax
  401235:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  40123c:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401243:	48 83 c0 08          	add    $0x8,%rax
  401247:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  40124e:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401255:	48 8b 00             	mov    (%rax),%rax
  401258:	e9 d7 f5 ff ff       	jmpq   400834 <SECRET+0x81>
  40125d:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401264:	48 83 c0 08          	add    $0x8,%rax
  401268:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  40126f:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401276:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  40127d:	48 8b 12             	mov    (%rdx),%rdx
  401280:	8b 12                	mov    (%rdx),%edx
  401282:	89 10                	mov    %edx,(%rax)
  401284:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40128b:	48 8b 00             	mov    (%rax),%rax
  40128e:	e9 a1 f5 ff ff       	jmpq   400834 <SECRET+0x81>
  401293:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40129a:	48 83 c0 08          	add    $0x8,%rax
  40129e:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  4012a5:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4012ac:	48 8b 00             	mov    (%rax),%rax
  4012af:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  4012b6:	48 83 ea 08          	sub    $0x8,%rdx
  4012ba:	48 8b 12             	mov    (%rdx),%rdx
  4012bd:	48 89 10             	mov    %rdx,(%rax)
  4012c0:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4012c7:	48 83 e8 10          	sub    $0x10,%rax
  4012cb:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  4012d2:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4012d9:	48 8b 00             	mov    (%rax),%rax
  4012dc:	e9 53 f5 ff ff       	jmpq   400834 <SECRET+0x81>
  4012e1:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4012e8:	48 83 c0 08          	add    $0x8,%rax
  4012ec:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  4012f3:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4012fa:	48 8b 00             	mov    (%rax),%rax
  4012fd:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  401304:	48 83 ea 08          	sub    $0x8,%rdx
  401308:	0f b7 12             	movzwl (%rdx),%edx
  40130b:	66 89 10             	mov    %dx,(%rax)
  40130e:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401315:	48 83 e8 10          	sub    $0x10,%rax
  401319:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  401320:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401327:	48 8b 00             	mov    (%rax),%rax
  40132a:	e9 05 f5 ff ff       	jmpq   400834 <SECRET+0x81>
  40132f:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401336:	48 83 c0 08          	add    $0x8,%rax
  40133a:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  401341:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401348:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  40134f:	48 8b 12             	mov    (%rdx),%rdx
  401352:	48 89 10             	mov    %rdx,(%rax)
  401355:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40135c:	48 8b 00             	mov    (%rax),%rax
  40135f:	e9 d0 f4 ff ff       	jmpq   400834 <SECRET+0x81>
  401364:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40136b:	48 83 c0 08          	add    $0x8,%rax
  40136f:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  401376:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40137d:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  401381:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401388:	48 83 e8 08          	sub    $0x8,%rax
  40138c:	8b 08                	mov    (%rax),%ecx
  40138e:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401395:	8b 00                	mov    (%rax),%eax
  401397:	01 c8                	add    %ecx,%eax
  401399:	89 02                	mov    %eax,(%rdx)
  40139b:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4013a2:	48 83 e8 08          	sub    $0x8,%rax
  4013a6:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  4013ad:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4013b4:	48 8b 00             	mov    (%rax),%rax
  4013b7:	e9 78 f4 ff ff       	jmpq   400834 <SECRET+0x81>
  4013bc:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4013c3:	48 83 c0 08          	add    $0x8,%rax
  4013c7:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  4013ce:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4013d5:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  4013d9:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4013e0:	8b 00                	mov    (%rax),%eax
  4013e2:	89 c1                	mov    %eax,%ecx
  4013e4:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4013eb:	48 83 e8 08          	sub    $0x8,%rax
  4013ef:	8b 00                	mov    (%rax),%eax
  4013f1:	29 c1                	sub    %eax,%ecx
  4013f3:	89 c8                	mov    %ecx,%eax
  4013f5:	66 89 02             	mov    %ax,(%rdx)
  4013f8:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4013ff:	48 83 e8 08          	sub    $0x8,%rax
  401403:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  40140a:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401411:	48 8b 00             	mov    (%rax),%rax
  401414:	e9 1b f4 ff ff       	jmpq   400834 <SECRET+0x81>
  401419:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401420:	48 83 c0 08          	add    $0x8,%rax
  401424:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  40142b:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401432:	48 8d 50 08          	lea    0x8(%rax),%rdx
  401436:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40143d:	48 8b 00             	mov    (%rax),%rax
  401440:	48 89 02             	mov    %rax,(%rdx)
  401443:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40144a:	48 83 c0 08          	add    $0x8,%rax
  40144e:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  401455:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40145c:	48 83 c0 08          	add    $0x8,%rax
  401460:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  401467:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40146e:	48 8b 00             	mov    (%rax),%rax
  401471:	e9 be f3 ff ff       	jmpq   400834 <SECRET+0x81>
  401476:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40147d:	48 83 c0 08          	add    $0x8,%rax
  401481:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  401488:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40148f:	8b 00                	mov    (%rax),%eax
  401491:	85 c0                	test   %eax,%eax
  401493:	74 22                	je     4014b7 <SECRET+0xd04>
  401495:	48 8b 95 60 fe ff ff 	mov    -0x1a0(%rbp),%rdx
  40149c:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4014a3:	8b 00                	mov    (%rax),%eax
  4014a5:	48 98                	cltq   
  4014a7:	48 c1 e0 03          	shl    $0x3,%rax
  4014ab:	48 01 d0             	add    %rdx,%rax
  4014ae:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  4014b5:	eb 12                	jmp    4014c9 <SECRET+0xd16>
  4014b7:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4014be:	48 83 c0 08          	add    $0x8,%rax
  4014c2:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  4014c9:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4014d0:	48 83 e8 08          	sub    $0x8,%rax
  4014d4:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  4014db:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4014e2:	48 8b 00             	mov    (%rax),%rax
  4014e5:	e9 4a f3 ff ff       	jmpq   400834 <SECRET+0x81>
  4014ea:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4014f1:	48 83 c0 08          	add    $0x8,%rax
  4014f5:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  4014fc:	48 8b 95 60 fe ff ff 	mov    -0x1a0(%rbp),%rdx
  401503:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40150a:	8b 00                	mov    (%rax),%eax
  40150c:	48 98                	cltq   
  40150e:	48 c1 e0 03          	shl    $0x3,%rax
  401512:	48 01 d0             	add    %rdx,%rax
  401515:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  40151c:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401523:	48 8b 00             	mov    (%rax),%rax
  401526:	e9 09 f3 ff ff       	jmpq   400834 <SECRET+0x81>
  40152b:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401532:	48 83 c0 08          	add    $0x8,%rax
  401536:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  40153d:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401544:	48 83 e8 08          	sub    $0x8,%rax
  401548:	48 8b 00             	mov    (%rax),%rax
  40154b:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  401552:	8b 12                	mov    (%rdx),%edx
  401554:	89 10                	mov    %edx,(%rax)
  401556:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40155d:	48 83 e8 10          	sub    $0x10,%rax
  401561:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  401568:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40156f:	48 8b 00             	mov    (%rax),%rax
  401572:	e9 bd f2 ff ff       	jmpq   400834 <SECRET+0x81>
  401577:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40157e:	48 83 c0 08          	add    $0x8,%rax
  401582:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  401589:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401590:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  401594:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40159b:	48 83 e8 08          	sub    $0x8,%rax
  40159f:	48 8b 08             	mov    (%rax),%rcx
  4015a2:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4015a9:	48 8b 00             	mov    (%rax),%rax
  4015ac:	48 f7 d8             	neg    %rax
  4015af:	48 01 c8             	add    %rcx,%rax
  4015b2:	48 89 02             	mov    %rax,(%rdx)
  4015b5:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4015bc:	48 83 e8 08          	sub    $0x8,%rax
  4015c0:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  4015c7:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4015ce:	48 8b 00             	mov    (%rax),%rax
  4015d1:	e9 5e f2 ff ff       	jmpq   400834 <SECRET+0x81>
  4015d6:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4015dd:	48 83 c0 08          	add    $0x8,%rax
  4015e1:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  4015e8:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4015ef:	48 8d 48 f8          	lea    -0x8(%rax),%rcx
  4015f3:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4015fa:	8b 00                	mov    (%rax),%eax
  4015fc:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  401603:	48 83 ea 08          	sub    $0x8,%rdx
  401607:	8b 32                	mov    (%rdx),%esi
  401609:	99                   	cltd   
  40160a:	f7 fe                	idiv   %esi
  40160c:	89 d0                	mov    %edx,%eax
  40160e:	89 01                	mov    %eax,(%rcx)
  401610:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401617:	48 83 e8 08          	sub    $0x8,%rax
  40161b:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  401622:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401629:	48 8b 00             	mov    (%rax),%rax
  40162c:	e9 03 f2 ff ff       	jmpq   400834 <SECRET+0x81>
  401631:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401638:	48 83 c0 08          	add    $0x8,%rax
  40163c:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  401643:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40164a:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  40164e:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401655:	48 8b 08             	mov    (%rax),%rcx
  401658:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40165f:	48 83 e8 08          	sub    $0x8,%rax
  401663:	48 8b 00             	mov    (%rax),%rax
  401666:	48 01 c8             	add    %rcx,%rax
  401669:	48 89 02             	mov    %rax,(%rdx)
  40166c:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401673:	48 83 e8 08          	sub    $0x8,%rax
  401677:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  40167e:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401685:	48 8b 00             	mov    (%rax),%rax
  401688:	e9 a7 f1 ff ff       	jmpq   400834 <SECRET+0x81>
  40168d:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401694:	48 83 c0 08          	add    $0x8,%rax
  401698:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  40169f:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4016a6:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  4016aa:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4016b1:	48 83 e8 08          	sub    $0x8,%rax
  4016b5:	48 8b 08             	mov    (%rax),%rcx
  4016b8:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4016bf:	8b 00                	mov    (%rax),%eax
  4016c1:	48 98                	cltq   
  4016c3:	48 01 c8             	add    %rcx,%rax
  4016c6:	48 89 02             	mov    %rax,(%rdx)
  4016c9:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4016d0:	48 83 e8 08          	sub    $0x8,%rax
  4016d4:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  4016db:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4016e2:	48 8b 00             	mov    (%rax),%rax
  4016e5:	e9 4a f1 ff ff       	jmpq   400834 <SECRET+0x81>
  4016ea:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4016f1:	48 83 c0 08          	add    $0x8,%rax
  4016f5:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  4016fc:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401703:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  40170a:	48 8b 12             	mov    (%rdx),%rdx
  40170d:	48 8b 12             	mov    (%rdx),%rdx
  401710:	48 89 10             	mov    %rdx,(%rax)
  401713:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40171a:	48 8b 00             	mov    (%rax),%rax
  40171d:	e9 12 f1 ff ff       	jmpq   400834 <SECRET+0x81>
  401722:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401729:	48 83 c0 08          	add    $0x8,%rax
  40172d:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  401734:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  40173b:	48 8d 50 08          	lea    0x8(%rax),%rdx
  40173f:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401746:	48 8b 00             	mov    (%rax),%rax
  401749:	48 89 02             	mov    %rax,(%rdx)
  40174c:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401753:	48 83 c0 08          	add    $0x8,%rax
  401757:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  40175e:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401765:	48 83 c0 08          	add    $0x8,%rax
  401769:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  401770:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401777:	48 8b 00             	mov    (%rax),%rax
  40177a:	e9 b5 f0 ff ff       	jmpq   400834 <SECRET+0x81>
  40177f:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  401786:	48 83 c0 08          	add    $0x8,%rax
  40178a:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  401791:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  401798:	48 8b 95 50 fe ff ff 	mov    -0x1b0(%rbp),%rdx
  40179f:	8b 12                	mov    (%rdx),%edx
  4017a1:	48 63 d2             	movslq %edx,%rdx
  4017a4:	48 89 10             	mov    %rdx,(%rax)
  4017a7:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4017ae:	48 8b 00             	mov    (%rax),%rax
  4017b1:	e9 7e f0 ff ff       	jmpq   400834 <SECRET+0x81>
  4017b6:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  4017bd:	48 83 c0 08          	add    $0x8,%rax
  4017c1:	48 89 85 60 fe ff ff 	mov    %rax,-0x1a0(%rbp)
  4017c8:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4017cf:	48 8d 50 f8          	lea    -0x8(%rax),%rdx
  4017d3:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4017da:	48 83 e8 08          	sub    $0x8,%rax
  4017de:	48 8b 08             	mov    (%rax),%rcx
  4017e1:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4017e8:	48 8b 00             	mov    (%rax),%rax
  4017eb:	48 0f af c1          	imul   %rcx,%rax
  4017ef:	48 89 02             	mov    %rax,(%rdx)
  4017f2:	48 8b 85 50 fe ff ff 	mov    -0x1b0(%rbp),%rax
  4017f9:	48 83 e8 08          	sub    $0x8,%rax
  4017fd:	48 89 85 50 fe ff ff 	mov    %rax,-0x1b0(%rbp)
  401804:	48 8b 85 60 fe ff ff 	mov    -0x1a0(%rbp),%rax
  40180b:	48 8b 00             	mov    (%rax),%rax
  40180e:	e9 21 f0 ff ff       	jmpq   400834 <SECRET+0x81>
  401813:	e8 e8 ec ff ff       	callq  400500 <__stack_chk_fail@plt>
  401818:	48 81 c4 b8 01 00 00 	add    $0x1b8,%rsp
  40181f:	5b                   	pop    %rbx
  401820:	5d                   	pop    %rbp
  401821:	c3                   	retq   

0000000000401822 <obfmegaInit>:
  401822:	55                   	push   %rbp
  401823:	48 89 e5             	mov    %rsp,%rbp
  401826:	90                   	nop
  401827:	5d                   	pop    %rbp
  401828:	c3                   	retq   

0000000000401829 <_obf_3_stringEncoder>:
  401829:	55                   	push   %rbp
  40182a:	48 89 e5             	mov    %rsp,%rbp
  40182d:	89 7d ec             	mov    %edi,-0x14(%rbp)
  401830:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
  401834:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)
  40183b:	90                   	nop
  40183c:	5d                   	pop    %rbp
  40183d:	c3                   	retq   
  40183e:	90                   	nop
  40183f:	90                   	nop

0000000000401840 <__libc_csu_init>:
  401840:	48 89 6c 24 d8       	mov    %rbp,-0x28(%rsp)
  401845:	4c 89 64 24 e0       	mov    %r12,-0x20(%rsp)
  40184a:	48 8d 2d af 05 20 00 	lea    0x2005af(%rip),%rbp        # 601e00 <__init_array_end>
  401851:	4c 8d 25 a0 05 20 00 	lea    0x2005a0(%rip),%r12        # 601df8 <__frame_dummy_init_array_entry>
  401858:	4c 89 6c 24 e8       	mov    %r13,-0x18(%rsp)
  40185d:	4c 89 74 24 f0       	mov    %r14,-0x10(%rsp)
  401862:	4c 89 7c 24 f8       	mov    %r15,-0x8(%rsp)
  401867:	48 89 5c 24 d0       	mov    %rbx,-0x30(%rsp)
  40186c:	48 83 ec 38          	sub    $0x38,%rsp
  401870:	4c 29 e5             	sub    %r12,%rbp
  401873:	41 89 fd             	mov    %edi,%r13d
  401876:	49 89 f6             	mov    %rsi,%r14
  401879:	48 c1 fd 03          	sar    $0x3,%rbp
  40187d:	49 89 d7             	mov    %rdx,%r15
  401880:	e8 53 ec ff ff       	callq  4004d8 <_init>
  401885:	48 85 ed             	test   %rbp,%rbp
  401888:	74 1c                	je     4018a6 <__libc_csu_init+0x66>
  40188a:	31 db                	xor    %ebx,%ebx
  40188c:	0f 1f 40 00          	nopl   0x0(%rax)
  401890:	4c 89 fa             	mov    %r15,%rdx
  401893:	4c 89 f6             	mov    %r14,%rsi
  401896:	44 89 ef             	mov    %r13d,%edi
  401899:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  40189d:	48 83 c3 01          	add    $0x1,%rbx
  4018a1:	48 39 eb             	cmp    %rbp,%rbx
  4018a4:	75 ea                	jne    401890 <__libc_csu_init+0x50>
  4018a6:	48 8b 5c 24 08       	mov    0x8(%rsp),%rbx
  4018ab:	48 8b 6c 24 10       	mov    0x10(%rsp),%rbp
  4018b0:	4c 8b 64 24 18       	mov    0x18(%rsp),%r12
  4018b5:	4c 8b 6c 24 20       	mov    0x20(%rsp),%r13
  4018ba:	4c 8b 74 24 28       	mov    0x28(%rsp),%r14
  4018bf:	4c 8b 7c 24 30       	mov    0x30(%rsp),%r15
  4018c4:	48 83 c4 38          	add    $0x38,%rsp
  4018c8:	c3                   	retq   
  4018c9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000004018d0 <__libc_csu_fini>:
  4018d0:	f3 c3                	repz retq 
  4018d2:	90                   	nop
  4018d3:	90                   	nop

Disassembly of section .fini:

00000000004018d4 <_fini>:
  4018d4:	48 83 ec 08          	sub    $0x8,%rsp
  4018d8:	48 83 c4 08          	add    $0x8,%rsp
  4018dc:	c3                   	retq   
