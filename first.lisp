brute_force all UR UL UF UB 3 \
		DR DL DF DB 3 \
		RU RB RD RF 3 \
		LU LB LD LF 3 \
		BU BR BD BL 3 \
		FU FR FD FL 3

if is_true bf-result do bf-result
		
if eqp bf-success 2 align DF UF
if eqp bf-success 3 align RF UF
if eqp bf-success 4 align LF UF
if eqp bf-success 5 align BR UR
if eqp bf-success 6 align FL UL

