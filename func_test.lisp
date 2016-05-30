defvar test::test-result-ok OK
defvar test::test-result-wrong WRONG
defvar test::test-begin \
	# ; \
	# new test begin ... ; \
	reset
	
defvar test::test-execute \
	do heuristic *5 Upside; NIL; \
	# checking the result of heuristic solution: ; \
	if check-cubes Upside;       \
	  print test::test-result-ok;      \
	  print test::test-result-wrong;
	  
defun FRU-swap \
	swap FRU & ; \
	# corner moving: ; \
	if eqp where_is FRU corner-sidemarks; \
	  print test::test-result-ok; \
	  print test::test-result-wrong; 

defun FR-swap  \
	swap FR & ; \
        # edge moving: ; \
	if eqp where_is FR edge-sidemarks; \
	  print test::test-result-ok; \
	  print test::test-result-wrong; 
	  
for_in I FRU FDR FDL BDR BDL;         \
  for_in corner-sidemarks permute I;  \
    for_in J FR DF DL DB DR;          \
      for_in edge-sidemarks permute J;\
        if eqp eigenvalue FRU eigenvalue corner-sidemarks; \
	  test::test-begin                 \                           
	  print assoc FRU corner-sidemarks \
	  print assoc FR  edge-sidemarks   \
	  FRU-swap corner-sidemarks        \
	  FR-swap edge-sidemarks           \
	  test::test-execute

delete test::test-result-ok 
delete test::test-result-wrong 
delete test::test-begin 
delete test::test-execute 
delete FR-swap
delete FRU-swap