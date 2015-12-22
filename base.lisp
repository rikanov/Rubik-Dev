defvar base::changed \
  regexp (.*)(->)(.*)
defvar solved \
  deselect base::changed cube
defvar changes \
  select base::changed cube

defmacro base::orientation_test(Xside) \
  regex Xside[A-Z]*->[^Xside][A-Z]* &
defun same_colors \
  nil select base::orientation_test(&) cube