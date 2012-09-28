Goals
=====

To develop a voting module for integration with Drupal that features flexible 
voting mechanisms, delegatable votes, taxonomy/category influenced controls 
and weighted voting to begin with.

We will make one or more Drupal modules for this and aim to take advantage of existing modules as much as possible.

Design principles
=================

* The tool should allow people to reproduce and surpass the kinds of
  decision-making instances that exist in face-to-face meetings
* The tool should also allow evaluation of the
  representativity/distribution of decisions
* Decision-making/voting systems should be modular and flexible to
  reflect the nature of the groups rather than impose a structure on
  the groups
* Decisions should be auditable

Desired characteristics
=======================

* modularity allows users to pick between different voting mechanisms:
  * binary voting 
  * ranking mechanism: the automatic spinoff voting would need this.
  * instant runoff voting
  * allocation voting (aka "scoring")
  * etc.
* optional quorum validation
* variable quorum conditions (concensus, 50% + 1, 2/3 +1, etc.)
* variable closing conditions (time limit vs. first past the post
  vs. quorum)
* delegatable votes
* weightable (allow weighting of votes to reflect, for example, role,
  karma, or impact-of-decision-on-user)
* spoilable votes
* notifications
* ... and a whole lot more that we'll come up with

Installation
============

See INSTALL.txt.

Dependencies
============

This module uses the new forms API of the Drupal 4.7.x branch and
requires the voting_api.module.

License
=======

Under the GPL. See LICENSE.txt.

Contributors
============

Antoine Beaupré aka anarcat
Omar Bickell
Stéphane Couture
Patrice D'Amours
Chris Kennedy
Mathieu Petit-Clair
Yann Rocq
[who's missing here?]

Updates
=======

For more information and the latest news, visit
http://decisions.gnuvernment.org/
