store

defvar loop-stack

# beginner (L2L) method test
#
# we solve the upper side
do heuristic *6 upside
#
# Now, we shall arrange the middle-layer edges as well
#
# Front-Right:
do heuristic *6 upside FR
#
# Front-Left:
do heuristic *6 upside FR FL
#
# Back-Right:
do heuristic *6 upside FR FL BR
#
# Back-Left:
do heuristic *6 upside FR FL BR BL
#
# we solve the buttom side now:
#
# arrange edge pieces and one corner first:
do heuristic *6 Upside DL DF DR DB DFL
#
# arrange all the remained corners then:
do heuristic *6 Upside DL DF DR DB DFL DFR DBL DBR
#

revert

