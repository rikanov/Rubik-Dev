defvar loop-stack

# brute_force all all-cubes
# if bf-success do bf-result

brute_force all UR UL UF UB 4 \
		DR DL DF DB 4 \
		RU RB RD RF 4 \
		LU LB LD LF 4 \
		BU BR BD BL 4 \
		FU FR FD FL 4

		do bf-result
		
if eqp bf-success 2 align DF UF
if eqp bf-success 3 align RF UF
if eqp bf-success 4 align LF UF
if eqp bf-success 5 align BR UR
if eqp bf-success 6 align FL UL

do brute_force all upside; nil bf-success