#include "includes.h"

void DelayUs(U16 n) // 1US
{
    volatile U16 t = n * (SystemCoreClock / 1000000 / 6);
    while (t--)
    {
    }

    /**
     *  6 inst per loop. For CPU freq f MHz: 6 * N * (1/f) = 1
     *   => N = f / 6
     */
    /**
080025a4 <DelayUs>:
 80025a4:	b082      	sub	sp, #8
 80025a6:	466b      	mov	r3, sp
 80025a8:	00c0      	lsls	r0, r0, #3
 80025aa:	b280      	uxth	r0, r0
 80025ac:	3306      	adds	r3, #6
 80025ae:	8018      	strh	r0, [r3, #0]
 80025b0:	8819      	ldrh	r1, [r3, #0]
 80025b2:	1e4a      	subs	r2, r1, #1
 80025b4:	b292      	uxth	r2, r2
 80025b6:	801a      	strh	r2, [r3, #0]
 80025b8:	2900      	cmp	r1, #0
 80025ba:	d1f9      	bne.n	80025b0 <DelayUs+0xc>
 80025bc:	b002      	add	sp, #8
 80025be:	4770      	bx	lr
     */
}
void DelayMs(U16 n)
{
    U16 i = 0;
    for (i = 0; i < n; i++)
    {
        DelayUs(1000);
    }
}
