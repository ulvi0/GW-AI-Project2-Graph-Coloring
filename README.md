# GW-AI-Project2-Graph-Coloring
# Project 2: CSP Graph Coloring (CS 6511 – AI)

### The George Washington University
### Spring 2022
### Professor: Dr. Amrinder Arora
### Student: Ulvi Aslanli
---
## Task description
You are given a graph in the form of a text file, that you are supposed to color.  The proper vertex coloring 
is such that each vertex is assigned a color and no two adjacent vertices are assigned the same color.
Write  a  CSP  algorithm  to  solve  this  coloring  problem.    The  CSP  algorithm  should  have  the  following 
components:  
- Search algorithm to solve the CSP  
- Heuristics (min remaining values, least constraining value)  
- Constraint propagation using AC3.  
---
## Implementation
 - First I parse the input file to get the number of availible colors and the edges of the graph. The edges are stored in the form of
adjancency list. 
 - In the backtrack_start function, domains of the variables are initialized. Domains of each varaible are stored as a vector of boolean. 
   True means, we can use this color for this node.
 - Additionaly, a set is created to keep track of non-colored nodes. It is always sorted by the domain size in the ascending order, so the node with the least domain size will be the next.
 - In the backtrack function, first the next node is chosen from the set, then every color in its domain is tried one by one. Any color will not be considered, if a neighboring node has the same color.
 - I used AC3 algorithm at each step of the backtracking for the constraint propogation. Because it would not affect the domains if i used it at the beginning.
 - If the algrothim does not find any availble coloring scheme, then the program returns -1
