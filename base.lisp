defvar base::changed \
  regex_fast *->*
defvar solved \
  deselect base::changed cube
defvar changes \
  select base::changed cube

defmacro base::orientation_test(Xside) \
  regex_fast Xside*->!Xside* &
defun same_colors \
  nil select base::orientation_test(&) cube