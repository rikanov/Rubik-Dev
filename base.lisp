defvar is_true nil nil

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