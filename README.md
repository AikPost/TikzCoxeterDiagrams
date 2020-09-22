# TikzCoxeterDiagrams
A c++ program to create tikz (latex) code for coxeter diagrams.
qwrfg df
Enter Code in the following format:
Diagrams:
Name1[angle1](label,

The code does not (yet) accept space characters.
[Angle1] is optional, default are 0° to the horizontal axis.
The unit of measurement is degree (360 equals one full rotation), it is stored as a double.

Label can be any string within the possibilities of c++without spaces or the characters used for separation
3 returns a line with no label,-1 a dotted line and 4 a double line as is standard for tits diagrams.

After the, continue with this scheme starting with the next node name.
Put ) to end a branch, you can then start another with a new (.

Orbits:
So far, only transpositions are possible to enter.

These are entered the following way:
(Name1&name2)(name3)(name4&name5)
Where namex is a string corresponding to an earlier entered name for a node
The single name3 creates a one node orbit, leaving a node out of here will result in it not being included in any orbit
