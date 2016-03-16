Caltech CS2 Assignment 9: Othello

See [assignment9_part1.html](http://htmlpreview.github.io/?https://github.com/caltechcs2/othello/blob/master/assignment9_part1.html) and [assignment9_part2.html](http://htmlpreview.github.io/?https://github.com/caltechcs2/othello/blob/master/assignment9_part2.html)

I improved my heuristic, and with conjunction with the 2-ply depth search for minimax,
I was able to beat ConstantTimePlayer pretty well. I did try to implement a recursive
minimax function so that I could get higher depths, but that method kept seg-faulting,
so I decided to make my heuristics as good as possible. As can be seen, I weighted the
corners very highly, because I observed that prioritizing corners usually led to more
victories. Overall, I don't think my AI would perform that well, as it cannot beat
BetterPlayer, so it won't be able to beat Oogee Player or DeepKwok either.