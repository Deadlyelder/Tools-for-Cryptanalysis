# This script implements a toolkit for counting active S-boxes using Mixed-Integer 
# Linear Programming for Enocoro from N. Mouha, Q. Wang, D. Gu, and B. Preneel,
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



#-- Enocoro Linear--
# Counting the minimal number of active S-boxes for Enocoro using "Coin" solver in Sage.
#(at least version 5.0 is required, other solvers are possible)
# - Linear cryptanalysis 
# - Any number of rounds (default 10)


#Writes the constraints implied by the Three Forked Branch to the file f
#in: i (input to TFB), o1,o2 (outputs from TFB), d (dummy variable), f (file to write to)
#out: \
def EqTFB(i,o1,o2,d,f):
  #TFB has linear branch number equal to 2
  f.write("p.add_constraint(x["+str(i)+"] + x["+str(o1)+"] + x["+str(o2)+"] - 2*d["+str(d)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(i)+"] >= 0)" +'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(o1)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(o2)+"] >= 0)"+'\n')

#Writes the constraints implied by the linear transformation L to the file f
#in: i1,i2 (inputs to L), o1,o2 (outputs from L), d (dummy variable), f (file to write to)
#out: \
def EqL(i1,i2,o1,o2,d,f):
  #Linear transformation L has differential/linear branch number equal to 3
  f.write("p.add_constraint(x["+str(i1)+"] + x["+str(i2)+"] + x["+str(o1)+"] + x["+str(o2)+"] - 3*d["+str(d)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(i1)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(i2)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(o1)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(o2)+"] >= 0)"+'\n')

#Updates x,next,dummy,O,CTFB and CL after for one round
#in: x (difference vector), next (the next unused index for x), dummy (the next unused index for d), O (list of indices for objective #function), CXOR (list of constraints implied by the TFB's),CL (list of constraints implied by the linear transformation L)
#out:x (difference vector), next (the next unused index for x), dummy (the next unused index for d), O (list of indices for objective #function), CTFB (list of constraints implied by the TFB's),CL (list of constraints implied by the linear transformation L)
def round(x,next,dummy,O,CTFB,CL):
  new_var=[]
  new_dummy=[]
  new_x=[] 
  for i in range(0,10):
    new_var.append(next)
    next=next+1
  for i in range(0,9):
    new_dummy.append(dummy)
    dummy=dummy+1
  for i in range(0,34):
    new_x.append(0)

  DoS=S(x,new_var,O)
  x=DoS[0]
  new_var=DoS[1]
  O=DoS[2]  

  for i in range(0,34):
    if i == 0:
      new_x[i]=x[31]
    else:
      if i==3:
        new_x[i]=new_var[3]
      else:
        if i==7:
          new_x[i]=new_var[4]
        else:
          if i==8:
            new_x[i]=new_var[5]
          else:
            if i==16:
              new_x[i]=new_var[6]
            else:
              if i==17:
                new_x[i]=new_var[7]            
              else:
                if i==29:
                  new_x[i]=new_var[8]
                else:
                  if i==30:
                    new_x[i]=new_var[9]
                  else:
                    if i==32:
                      new_x[i]=new_var[1]
                    else:
                      if i==33:    
                        new_x[i]=new_var[2]
                      else:
                        new_x[i]=x[i-1]
  
  CTFB.append([x[31],x[32],new_var[0],new_dummy[0]])
  CL.append([new_var[0],x[33],new_var[1],new_var[2],new_dummy[1]])
  CTFB.append([x[2],new_var[0],new_var[3],new_dummy[2]])
  CTFB.append([x[6],new_var[3],new_var[4],new_dummy[3]])
  CTFB.append([x[7],x[33],new_var[5],new_dummy[4]])
  CTFB.append([x[15],new_var[5],new_var[6],new_dummy[5]])
  CTFB.append([x[16],new_var[1],new_var[7],new_dummy[6]])
  CTFB.append([x[28],new_var[7],new_var[8],new_dummy[7]])
  CTFB.append([x[29],new_var[2],new_var[9],new_dummy[8]])

  return [new_x,next,dummy,O,CTFB,CL]

#Updates the difference vector x and list O for the S-box transformation
#in: x (difference vector), new_var(vector containing the new indices for the round that calls S), O (list of indices for objective #function)
#out: x (difference vector),new_var(vector containing the new indices for the round that calls S), O (list of indices for objective #function)
def S(x,new_var,O):
  O.append(new_var[0])
  O.append(x[33])
  O.append(new_var[1])
  O.append(new_var[2])
  return [x,new_var,O]

#Generates a MILP (Sage) for calculating the minimum number of linearly active S-boxes for 'rounds' rounds of Enocoro-128v2 and solves #this program using the "Coin" solver
#in: rounds (the number of rounds)
#out: the MILP (for calculating the minimum number of linearly active S-boxes for 'rounds' rounds of Enocoro-128v2 and prints the #solution of this program
def giveEquationsEnocoroLinearandSolve(rounds):
  x=[]     #state variable
  O=[]     #list of indices for objective function  
  CTFB=[]  #list of constraints implied by the TFB's
  CL=[]    #list of constraints implied by the linear transformation L
  CA=[]    #list of indices for constraint to ensure not all inputs are zero  
  next=0   #next unused state variable index 
  dummy=0  #next unused dummy variable index 

  for i in range(0,34):
    x.append(next)
    CA.append(next)
    next=next+1
  
  for i in range(0,rounds):
    NR=round(x,next,dummy,O,CTFB,CL)
    x=NR[0]
    next=NR[1]
    dummy=NR[2]
    O=NR[3]
    CTFB=NR[4]
    CL=NR[5]
  
  f = open('Eq-Enocoro-With-'+str(rounds)+'-Rounds'+'Linear.sage','w')
  f.write("p = MixedIntegerLinearProgram(maximization=False, solver=\"Coin\")"+'\n')
  f.write("x = p.new_variable(binary=True)"+'\n')
  f.write("d = p.new_variable(binary=True)"+'\n')
  f.write('\n')
  f.write("p.set_objective(" ) 
  for i in range (0, len(O)-1):
    f.write("x["+str(O[i])+"] + ")
  f.write("x["+str(O[len(O)-1])+"])"+'\n')
  f.write('\n')
  f.write("p.add_constraint(")
  for i in range (0, len(CA)-1):
    f.write("x["+str(CA[i])+"] + ")
  f.write("x["+str(CA[len(CA)-1])+"] >= 1)"+'\n')
  for i in range (0, len(CTFB)):
    EqTFB(CTFB[i][0],CTFB[i][1],CTFB[i][2],CTFB[i][3],f)
  for i in range (0, len(CL)):
    EqL(CL[i][0],CL[i][1],CL[i][2],CL[i][3],CL[i][4],f)
  f.write('\n')
  f.write("solution=p.solve()"+'\n')
  f.write("print \"Minimal number of S-boxes:\", solution"+'\n')
  f.close()
  execfile('Eq-Enocoro-With-'+str(rounds)+'-Rounds'+'Linear.sage')

giveEquationsEnocoroLinearandSolve(10)