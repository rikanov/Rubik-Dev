defvar loop-stack

do brute_force +1 UR UL UF UB 4 \
                   DR DL DF DB 4 \
                   RU RB RD RF 4 \
                   LU LB LD LF 4 \
                   BU BR BD BL 4 \
                   FU FR FD FL 4;\
                   nil brute-force-success
if eqp brute-force-success 2 align DF UF
if eqp brute-force-success 3 align RF UF
if eqp brute-force-success 4 align LF UF
if eqp brute-force-success 5 align BR UR
if eqp brute-force-success 6 align FL UL

do brute_force +2 upside; nil brute-force-success
