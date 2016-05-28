# F2L strategy

defvar Asymmetric FU LU BU RU BLU BL BRU BR FRU FR
defvar Symmetric Asymmetric FLU FL

defun move-corner \
  swap FRU & ;
  
defvar move-edge \
  swap FR FD;
  
defun move \
  move-corner & ; \
  move-edge 
  
defun setcube \
  print assoc & FRU \
  move & 
  
defvar solvecube \
  heuristic *6 Asymmetric;   \
  heuristic *6 Symmetric;    

defun query-solution \
  setcube & ; \
  solvecube

defun useable \
  eqp eigenvalue FRU eigenvalue &
  
reset 
# teszt
for_in I cube; \
  if useable I; \
      query-solution I ; ;
      