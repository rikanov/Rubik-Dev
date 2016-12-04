defvar is_true nil nil

defvar upside    UR UF UL UB URF ULF BRU BLU
defvar downside  DR DF DL DB DRF DLF BRD BLD
defvar rightside RU RB RD RF URF BRU BRD DRF
defvar leftside  LU LB LD LF ULF BLU BLD DLF
defvar frontside FU FR FD FL ULF URF DRF DLF
defvar backside  BU BL BD BR BRU BLU BLD BRD

defvar Upside upside FR FL BL BR

defvar edges     UR UF UL UB DR DF DL DB FR FL BL BR
defvar corners   FUR FRD FDL FLU BRU BDR BLD BUL
defvar all-cubes edges corners

defvar base::changed \
  reg_fast *->*
defvar solved \
  deselect base::changed cube
defvar changes \
  select base::changed cube

defmacro base::orientation_test(Xside) \
  reg_fast Xside*->!Xside* &
defun same_colors \
  nil select base::orientation_test(&) cube
  
defun randomize & \
  do random; nil eqp UR what_is &
  
defmacro append(list_ref) \
  defvar list_ref merge list_ref &

defun jessicaF \
  store \
  do &; NIL; \
  if nil same_colors U; revert
  
defvar validation::corners \
  URF UBR ULB UFL DFR DRB DBL DLF
  
defun validation::corner-value \
  crypt assoc crypt D->U what_is & 012 U

defun validation::add-corner \
  + validation::corner-value &
  
defvar validation::corner-checksum \
  convoke + 0 \
    mapcar  crypt assoc crypt D->U what_is & 012 U; \
      validation::corners