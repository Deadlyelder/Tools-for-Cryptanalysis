# This script implements a toolkit for counting active S-boxes using Mixed-Integer 
# Linear Programming for mCrypton from the paper by N. Mouha, Q. Wang, D. Gu, and B. Preneel,
# "Differential and Linear Cryptanalysis using Mixed-Integer Linear Programming,"
# in Information Security and Cryptology - Inscrypt 2011,
# Lecture Notes in Computer Science, Springer-Verlag, 20 pages, 2011."(available at 
# http://www.cosic.esat.kuleuven.be/publications/article-2080.pdf)
# and was downloaded from (http://www.ecrypt.eu.org/tools/)
#
# This work was done in the context of an internship done at NXP
# under supervision of Ventzislav Nikov <vetnzislav.nikov@nxp.com>
# and is published under MIT license:
# =============================================================================
# Copyright (C) 2012 Laura Winnen <laura.winnen@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# =============================================================================



#-- mCrypton--
# Counting the minimal number of active S-boxes for mCrypton using "Coin" solver in Sage.
# (at least version 5.0 is required, other solvers are possible)
# - Differential cryptanalysis non-related key setting 
# - Linear cryptanalysis non-related key setting
# - Any number of rounds (default 10)
 

#Writes the constraints implied by the permutation Pi operation to the file f
#in: i1,i2,i3,i4 (inputs to Pi), o1,o2,o3,o4 (outputs from Pi),i (input difference), o (output difference), d (dummy variable), f #file to write to)
#out: \
def EqPi(i1,i2,i3,i4,o1,o2,o3,o4,i,o,d,f):
  #Permutation operation has differential/linear branch number equal to  4
  f.write("p.add_constraint(x["+str(i1)+"] + x["+str(i2)+"] + x["+str(i3)+"] + x["+str(i4)+"] + x["+str(o1)+"] + x["+str(o2)+"] + x["+str(o3)+"] + x["+str(o4)+"] - 4*d["+str(d)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(i1)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(i2)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(i3)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(i4)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(o1)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(o2)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(o3)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(o4)+"] >= 0)"+'\n')
  
  #Additional constraints to ensure that an input difference always leads to an output difference 
  f.write("p.add_constraint(x["+str(i1)+"] + x["+str(i2)+"] + x["+str(i3)+"] + x["+str(i4)+"] - i["+str(i)+"] >= 0)"+'\n')
  f.write("p.add_constraint(x["+str(o1)+"] + x["+str(o2)+"] + x["+str(o3)+"] + x["+str(o4)+"] - o["+str(o)+"] >= 0)"+'\n')
  f.write("p.add_constraint(i["+str(i)+"] - x["+str(i1)+"] >= 0)"+'\n')
  f.write("p.add_constraint(i["+str(i)+"] - x["+str(i2)+"] >= 0)"+'\n')
  f.write("p.add_constraint(i["+str(i)+"] - x["+str(i3)+"] >= 0)"+'\n')
  f.write("p.add_constraint(i["+str(i)+"] - x["+str(i4)+"] >= 0)"+'\n')
  f.write("p.add_constraint(o["+str(o)+"] - x["+str(o1)+"] >= 0)"+'\n')
  f.write("p.add_constraint(o["+str(o)+"] - x["+str(o2)+"] >= 0)"+'\n')
  f.write("p.add_constraint(o["+str(o)+"] - x["+str(o3)+"] >= 0)"+'\n')
  f.write("p.add_constraint(o["+str(o)+"] - x["+str(o4)+"] >= 0)"+'\n')
  f.write("p.add_constraint(o["+str(o)+"] - i["+str(i)+"] >= 0)"+'\n')

#Updates a and O for the S-box transformation
#in: a (difference vector), O (list of indices for objective function)
#out: a (difference vector), O (list of indices for objective function)
def Substitution(a,O):
  for i in range(0,4):
    for j in range(0,4): 
      O.append(a[i][j])
  return [a,O]

#Updates a for the transposition transformation
#in: a (difference vector)
#out: a (difference vector)
def Transposition(a):
  new_a=[]
  for i in range(0,len(a[0])):
    new_a.append([])
    for j in range(0,len(a)):
      new_a[i].append(a[j][i]) 
  return new_a

#Updates a,next,dummy,CPi for the permutation operation
#in: a (difference vector), next (the next unused index for a), dummy (the next unused index for d), CPi (list of constraints implied #by the permutation)
#out: a (difference vector), next (the next unused index for a), dummy (the next unused index for d), CPi (list of constraints implied #by the permutation) 
def Permutation(a,next,dummy,CPi,next_i,next_o):
  for j in range(0,4):
    CPi.append([a[0][j],a[1][j],a[2][j],a[3][j],next,next+1,next+2,next+3,next_i,next_o,dummy])
    for i in range(0,4):
      a[i][j]=next
      next=next+1
    dummy=dummy+1
    next_i=next_i+1
    next_o=next_o+1
  return [a,next,dummy,CPi,next_i,next_o]

#Updates a,next,dummy,O,CPi, next_i and next_o after one round
#in: a (difference vector), next (the next unused index for x), dummy (the next unused index for d), O (list of indices for objective #function), CPi (list of constraints implied by the permutation operation), next_i (the next unused index for i), next_o (the next #unused index for o)  
#out: a (difference vector), next (the next unused index for x), dummy (the next unused index for d), O (list of indices for objective #function), CPi (list of constraints implied by the permutation operation),next_i (the next unused index for i), next_o (the next #unused index for o)   
def round(a,next,dummy,O,CPi,next_i,next_o):
  S=Substitution(a,O)   
  a=S[0]
  O=S[1]
  MC=Permutation(a,next,dummy,CPi,next_i,next_o)
  a=MC[0]
  next=MC[1]
  dummy=MC[2]
  CPi=MC[3]
  next_i=MC[4]
  next_o=MC[5]
  a=Transposition(a)
  return[a,next,dummy,O,CPi,next_i,next_o]

#Generates a MILP (Sage) for calculating the minimum number of differentially/lineary active S-boxes for 'rounds' rounds of mCrypton 
#in the non-related key setting and solves this program using the "Coin" solver
#in: rounds (the number of rounds)
#out: the MILP (for calculating the minimum number of differentially/lineary active S-boxes for 'rounds' rounds of mCrypton in the non-#related (related=false) key setting and prints the solution of this program
def giveEquationsmCryptonandSolve(rounds):
  next=0   #next unused state/key variable index 
  dummy=0  #next unused dummy variable index
  next_i=0 #next unused input variable index
  next_o=0 #next unused output variable index
  a=[]     #state variable
  O=[]     #list of indices for objective function  
  CPi=[]   #list of constraints implied by the permutation Pi operation
  CA=[]    #list of indices for constraint to ensure not all inputs are zero 
  
  RK=[]
  for i in range(0,4):
    a.append([])
    for j in range(0,4):
      a[i].append(next)
      CA.append(next)    
      next=next+1

  for i in range(0,rounds):
    NR=round(a,next,dummy,O,CPi,next_i,next_o)
    a=NR[0]
    next=NR[1]
    dummy=NR[2]
    O=NR[3]
    CPI=NR[4]
    next_i=NR[5]
    next_o=NR[6]
  
  f = open('Eq-mCrypton-With-'+str(rounds)+'-Rounds.sage','w')
  f.write("p = MixedIntegerLinearProgram(maximization=False, solver=\"Coin\")"+'\n')
  f.write("x = p.new_variable(binary=True)"+'\n')
  f.write("d = p.new_variable(binary=True)"+'\n')
  f.write("i = p.new_variable(binary=True)"+'\n')
  f.write("o = p.new_variable(binary=True)"+'\n')
  f.write('\n')
  f.write("p.set_objective(" )    
  for i in range (0, len(O)-1):
    f.write("x["+str(O[i])+"] + ")
  f.write("x["+str(O[len(O)-1])+"] )"+'\n')
  f.write('\n')
  f.write("p.add_constraint(")
  for i in range (0, len(CA)-1):
    f.write("x["+str(CA[i])+"] + ")
  f.write("x["+str(CA[len(CA)-1])+"] >= 1 )"+'\n') 
  for i in range (0, len(CPi)):
    EqPi(CPi[i][0],CPi[i][1],CPi[i][2],CPi[i][3],CPi[i][4],CPi[i][5],CPi[i][6],CPi[i][7],CPi[i][8],CPi[i][9],CPi[i][10],f)
  f.write('\n')
  f.write("solution=p.solve()"+'\n')
  f.write("print \"Minimal number of S-boxes:\", solution"+'\n')
  f.close()
  execfile('Eq-mCrypton-With-'+str(rounds)+'-Rounds.sage')

giveEquationsmCryptonandSolve(10)