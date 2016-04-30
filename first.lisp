randomize FL
randomize DB

defvar loop-stack
defvar pool upside

# beginner (L2L) method test
#
# we solve the upper side
do brute_force *6 upside
#
# Now, we shall arrange the middle-layer edges as well
#
# Front-Right:
do brute_force *6 upside FR
#
# Front-Left:
do brute_force *6 upside FR FL
#
# Back-Right:
do brute_force *6 upside FR FL BR
#
# Back-Left:
do brute_force *6 upside FR FL BR BL
#
# we solve the buttom side now:
#
# arrange edge pieces and one corner first:
do brute_force *6 Upside DL DF DR DB DFL
#
# arrange all remained corners then:
do brute_force *6 Upside DL DF DR DB DFL DFR DBL DBR
#
