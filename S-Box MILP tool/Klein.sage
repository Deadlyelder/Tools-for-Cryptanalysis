# This script implements a toolkit for counting active S-boxes using Mixed-Integer 
# Linear Programming for Klein from the paper by N. Mouha, Q. Wang, D. Gu, and B. Preneel,
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
#=============================================================================



#-- Klein --
# Counting the minimal number of active S-boxes for Klein using "Coin" solver in Sage.
# (at least version 5.0 is required, other solvers are possible)
# - Differential cryptanalysis non-related key setting 
# - Any number of rounds (default 5)


#Writes the constraints implied by the mixNibbles operation to the file f
#in: i1,i2,i3,i4,i5,i6,i7,i8 (nibble inputs to MN), o1,o2,o3,o4,o5,o6,o7,o8 (nibble outputs from MN), m1,m2,m3,m4 (byte inputs to MN), #mo1,mo2,mo3,mo4 (byte inputs to MN), d (dummy variable), f (file to write to)
#out: \
def EqMC(i1,i2,i3,i4,i5,i6,i7,i8,o1,o2,o3,o4,o5,o6,o7,o8,m1,m2,m3,m4,mo1,mo2,mo3,mo4,d,f):
  f.write("p.add_constraint(x["+str(i1)+"] + x["+str(i2)+"] - m["+str(m1)+"] >= 0)"+'\n')
  f.write("p.add_constraint(x["+str(i3)+"] + x["+str(i4)+"] - m["+str(m2)+"] >= 0)"+'\n')
  f.write("p.add_constraint(x["+str(i5)+"] + x["+str(i6)+"] - m["+str(m3)+"] >= 0)"+'\n')
  f.write("p.add_constraint(x["+str(i7)+"] + x["+str(i8)+"] - m["+str(m4)+"] >= 0)"+'\n')

  f.write("p.add_constraint(m["+str(m1)+"] - x["+str(i1)+"] >= 0)"+'\n')
  f.write("p.add_constraint(m["+str(m1)+"] - x["+str(i2)+"] >= 0)"+'\n')
  f.write("p.add_constraint(m["+str(m2)+"] - x["+str(i3)+"] >= 0)"+'\n')
  f.write("p.add_constraint(m["+str(m2)+"] - x["+str(i4)+"] >= 0)"+'\n')
  f.write("p.add_constraint(m["+str(m3)+"] - x["+str(i5)+"] >= 0)"+'\n')
  f.write("p.add_constraint(m["+str(m3)+"] - x["+str(i6)+"] >= 0)"+'\n')
  f.write("p.add_constraint(m["+str(m4)+"] - x["+str(i7)+"] >= 0)"+'\n')
  f.write("p.add_constraint(m["+str(m4)+"] - x["+str(i8)+"] >= 0)"+'\n')
  
  #MC operation has differential/linear branch number equal to  5
  f.write("p.add_constraint(m["+str(m1)+"] + m["+str(m2)+"] + m["+str(m3)+"] + m["+str(m4)+"] + m["+str(mo1)+"] + m["+str(mo2)+"] + m["+str(mo3)+"] + m["+str(mo4)+"] - 5*d["+str(d)+"] >= 0)"+'\n') 
  f.write("p.add_constraint(d["+str(d)+"] - m["+str(m1)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - m["+str(m2)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - m["+str(m3)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - m["+str(m4)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - m["+str(mo1)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - m["+str(mo2)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - m["+str(mo3)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - m["+str(mo4)+"] >= 0)"+'\n')
  
  f.write("p.add_constraint(x["+str(o1)+"] + x["+str(o2)+"] - m["+str(mo1)+"] >= 0)"+'\n')
  f.write("p.add_constraint(x["+str(o3)+"] + x["+str(o4)+"] - m["+str(mo2)+"] >= 0)"+'\n')
  f.write("p.add_constraint(x["+str(o5)+"] + x["+str(o6)+"] - m["+str(mo3)+"] >= 0)"+'\n')
  f.write("p.add_constraint(x["+str(o7)+"] + x["+str(o8)+"] - m["+str(mo4)+"] >= 0)"+'\n')

  f.write("p.add_constraint(m["+str(mo1)+"] - x["+str(o1)+"] >= 0)"+'\n')
  f.write("p.add_constraint(m["+str(mo1)+"] - x["+str(o2)+"] >= 0)"+'\n')
  f.write("p.add_constraint(m["+str(mo2)+"] - x["+str(o3)+"] >= 0)"+'\n')
  f.write("p.add_constraint(m["+str(mo2)+"] - x["+str(o4)+"] >= 0)"+'\n')
  f.write("p.add_constraint(m["+str(mo3)+"] - x["+str(o5)+"] >= 0)"+'\n')
  f.write("p.add_constraint(m["+str(mo3)+"] - x["+str(o6)+"] >= 0)"+'\n')
  f.write("p.add_constraint(m["+str(mo4)+"] - x["+str(o7)+"] >= 0)"+'\n')
  f.write("p.add_constraint(m["+str(mo4)+"] - x["+str(o8)+"] >= 0)"+'\n')

#Updates a and O for the S-box transformation
#in: a (difference vector), O (list of indices for objective function)
#out: a (difference vector), O (list of indices for objective function)
def subNibbles(a,O):
  for i in range(0,16):
    O.append(a[i])
  return [a,O]

#Updates a for the Rotate Nibbles transformation
#in: a (difference vector)
#out: a (difference vector)
def rotateNibbles(a):
  new_a=[]
  for i in range(0,16):
    new_a.append(a[(i+4)%16])
  a=new_a
  return a


#Updates a,next,dummy,CMC for the mixNibbles operation
#in: a (difference vector), m (byte difference vector for mixNibbles), next (the next unused index for a), dummy (the next unused #index for d), next_m (the next unused index for m), CMC (list of constraints implied by the mixNibbles operation)
#out: a (difference vector), m (byte difference vector for mixNibbles), next (the next unused index for a), dummy (the next unused #index for d), next_m (the next unused index for m), CMC (list of constraints implied by the mixNibbles operation)
def mixNibbles(a,m,next,dummy,next_m,CMC):
  CMC.append([a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],next,next+1,next+2,next+3,next+4,next+5,next+6,next+7,m[0],m[1],m[2],m[3],next_m,next_m+1,next_m+2,next_m+3,dummy])
  dummy=dummy+1
  next_m=next_m+4
  for i in range(0,8):
    a[i]=next
    next=next+1
  for i in range(0,4):
    m[i]=next_m
    next_m=next_m+1
  CMC.append([a[8],a[9],a[10],a[11],a[12],a[13],a[14],a[15],next,next+1,next+2,next+3,next+4,next+5,next+6,next+7,m[0],m[1],m[2],m[3],next_m,next_m+1,next_m+2,next_m+3,dummy])
  dummy=dummy+1
  next_m=next_m+4
  for i in range(8,16):
    a[i]=next
    next=next+1
  for i in range(0,4):
    m[i]=next_m
    next_m=next_m+1
  return [a,m,next,dummy,next_m,CMC]

#Updates a,m,next,dummy,next_m,O and CMC after one round
#in: a (difference vector), m (byte difference vector for mixNibbles), next (the next unused index for a), dummy (the next unused #index for d), next_m (the next unused index for m), O (list of indices for objective function), CMC (list of constraints implied by #the mixNibbles operation)
#out: a (difference vector), m (byte difference vector for mixNibbles), next (the next unused index for a), dummy (the next unused #index for d), next_m (the next unused index for m), O (list of indices for objective function), CMC (list of constraints implied by #the mixNibbles operation)
def round(a,m,next,dummy,next_m,O,CMC):
  S=subNibbles(a,O)
  a=S[0]
  O=S[1]
  a=rotateNibbles(a)
  M=mixNibbles(a,m,next,dummy,next_m,CMC)
  a=M[0]
  m=M[1]
  next=M[2]
  dummy=M[3]
  next_m=M[4]
  CMC=M[5]
  return [a,m,next,dummy,next_m,O,CMC]

#Generates a MILP (Sage) for calculating the minimum number of active S-boxes for 'rounds' rounds of Klein with in the non-related key #setting and solves this program using the "Coin" solver
#in: rounds (the number of rounds)
#out: the MILP (for calculating the minimum number of active S-boxes for 'rounds' rounds of Klein in the non-related key setting and #prints the solution of this program
def giveEquationsKleinandSolve(rounds):
  next=0       #next unused state/key variable index 
  dummy=0      #next unused dummy variable index
  next_m=4     #next unused state byte variable index 
  a=[]         #state variable
  k=[]         #key variable
  m=[0,1,2,3]  #state byte variable
  O=[]         #list of indices for objective function  
  CMC=[]       #list of constraints implied by the MixColumn operation
  CA=[]        #list of indices for constraint to ensure not all inputs are zero 

  for i in range(0,16):
    a.append(next)
    CA.append(next)
    next=next+1
  
  for i in range(0,rounds):
    NR=round(a,m,next,dummy,next_m,O,CMC)
    a=NR[0]
    m=NR[1]
    next=NR[2]
    dummy=NR[3]
    next_m=NR[4]
    O=NR[5]
    CMC=NR[6]
  f = open('Eq-Klein-With-'+str(rounds)+'-Rounds-Differential.sage','w')
  f.write("p = MixedIntegerLinearProgram(maximization=False, solver=\"Coin\")"+'\n')
  f.write("x = p.new_variable(binary=True)"+'\n')
  f.write("d = p.new_variable(binary=True)"+'\n')
  f.write("m = p.new_variable(binary=True)"+'\n')
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
  for i in range (0, len(CMC)):
    EqMC(CMC[i][0],CMC[i][1],CMC[i][2],CMC[i][3],CMC[i][4],CMC[i][5],CMC[i][6],CMC[i][7],CMC[i][8],CMC[i][9],CMC[i][10],CMC[i][11],CMC[i][12],CMC[i][13],CMC[i][14],CMC[i][15],CMC[i][16],CMC[i][17],CMC[i][18],CMC[i][19],CMC[i][20],CMC[i][21],CMC[i][22],CMC[i][23],CMC[i][24],f)
  f.write('\n')
  f.write("solution=p.solve()"+'\n')
  f.write("print \"Minimal number of S-boxes:\", solution"+'\n')
  f.close()
  execfile('Eq-Klein-With-'+str(rounds)+'-Rounds-Differential.sage')

giveEquationsKleinandSolve(4)