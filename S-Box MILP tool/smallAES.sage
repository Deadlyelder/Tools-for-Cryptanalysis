# This script implements a toolkit for counting active S-boxes using Mixed-Integer 
# Linear Programming for small AES from the paper by N. Mouha, Q. Wang, D. Gu, and B. Preneel,
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



#-- small AES --
# Counting the minimal number of active S-boxes for small AES S(n,r,c,e) using "Coin" solver in Sage.
# (at least version 5.0 is required, other solvers are possible)
# - Differential cryptanalysis related and non-related key setting (default non-related setting)
# - Linear cryptanalysis only in non-related key setting
# - Any number of rounds n, rows r, columns c and wordsize e(default (10,2,4,4))
# - Any number of rounds (default 10)

#Writes the constraints implied by the XOR to the file f
#in: i1,i2 (inputs to XOR), o (output from XOR), d (dummy variable), f (file to write to)
#out: \
def EqXOR(i1,i2,o,d,f):
  #XOR has differential branch number equal to 2
  f.write("p.add_constraint(x["+str(i1)+"] + x["+str(i2)+"] + x["+str(o)+"] - 2*d["+str(d)+"] >= 0)"+'\n') 
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(i1)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(i2)+"] >= 0)"+'\n')
  f.write("p.add_constraint(d["+str(d)+"] - x["+str(o)+"] >= 0)"+'\n')

#Writes the constraints implied by the MixColumn operation to the file f
#in: i (inputs to MC), o (outputs from MC), d (dummy variable), f (file to write to), r (number of rows of the #small AES)
#out: \
def EqMC(i,o,d,f,r):
  #MC operation has differential/linear branch number equal to r+1
  f.write("p.add_constraint(")
  for j in range(0,len(i)):
    f.write("x["+str(i[j])+"] + ")
  for j in range(0,len(o)-1):
    f.write("x["+str(o[j])+"] + ")
  f.write("x["+str(o[len(o)-1])+"] - "+str(r+1)+"*d["+str(d)+"] >= 0)"+'\n')  
  for j in range(0,len(i)):
    f.write("p.add_constraint(d["+str(d)+"] - x["+str(i[j])+"] >= 0)"+'\n')
  for j in range(0,len(o)):
    f.write("p.add_constraint(d["+str(d)+"] - x["+str(o[j])+"] >= 0)"+'\n')

#Updates a,next,dummy,CXOR for the KeyAddition operation
#in: a (difference vector), r (number of rows of the small AES),c (number of columns of the small AES), rk (round key), next (the next #unused index for a), dummy (the next unused index for d), CXOR (list of constraints implied by the XOR's)
#out: a (difference vector), next (the next unused index for a), dummy (the next unused index for d), CXOR (list of constraints #implied by the XOR's) 
def KeyAddition(a,r,c,rk,next,dummy,CXOR):
  for i in range(0,r):
    for j in range(0,c):
      CXOR.append([a[i][j],rk[i][j],next,dummy])
      a[i][j]=next
      next=next+1
      dummy=dummy+1
  return[a,next,dummy,CXOR]

#Updates a and O for the S-box transformation
#in: a (difference vector), r (number of rows of the small AES),c (number of columns of the small AES), O (list of indices for #objective function)
#out: a (difference vector), O (list of indices for objective function)
def Substitution(a,r,c,O):
  for i in range(0,r):
    for j in range(0,c): 
      O.append(a[i][j])
  return [a,O]

#Updates a for the S-box transformation
#in: a (difference vector), r (number of rows of the small AES),c (number of columns of the small AES) 
#out: a (difference vector)
def ShiftRows(a,r,c):
  temp=[0,0,0,0]    
  for i in range(1,r):
    for j in range(0,c):
      temp[j]=a[i][(j+i)%c]
    for j in range(0,c):        
      a[i][j]=temp[j]
  return a

#Updates a,next,dummy,CMC for the MixColumn operation
#in: a (difference vector),r (number of rows of the small AES),c (number of columns of the small AES), next (the next unused index for #a), dummy (the next unused index for d), CMC (list of constraints implied by the MixColumn operation)
#out: a (difference vector), next (the next unused index for a), dummy (the next unused index for d), CMC (list of constraints #implied by the MixColumn operation) 
def MixColumn(a,r,c,next,dummy,CMC):
  for j in range(0,c):
    toAdd=[[],[],dummy]
    for i in range(0,r):
      toAdd[0].append(a[i][j])
      toAdd[1].append(next)
      a[i][j]=next
      next=next+1
    CMC.append(toAdd)
    dummy=dummy+1
  return [a,next,dummy,CMC]

#Generates the round keys for 'rounds' of rounds and updates next,dummy,O,CXOR
#in: r (number of rows of the small AES),c (number of columns of the small AES), k (key), ks(number of columns of k), rounds (number #of rounds), O(list of indices for objective function), CXOR (list of constraints implied by the XOR's), next (the next unused index #for a), dummy (the next unused index for d)
#out: W (round keys), next (the next unused index for a), dummy (the next unused index for d), O(list of indices for objective #function), CXOR (list of constraints implied by the XOR's)
def KeySchedule(r,c,k,ks,rounds,O,CXOR,next,dummy):
  temp=[]
  W=[]
  for i in range(0,14+1):
    W.append([])
    for j in range(0,r):
      W[i].append([])
      for l in range(0,c):
        W[i][j].append(0)
  for i in range(0,r):
    temp.append([])
    for j in range(0,ks):
      temp[i].append(k[i][j])
  t=0
  
  for j in range(0,ks):
    if t<(rounds+1)*c:
      for i in range(0,r):
        W[int(t/4)][i][t%c]=temp[i][j]
      t=t+1

  while t<(rounds+1)*c:
    if t<(rounds+1)*c:
      for i in range(0,r):
        CXOR.append([temp[i][0],temp[(i+1)%r][ks-1],next,dummy])
        O.append(temp[(i+1)%r][ks-1])
        temp[i][0]= next
        next=next+1
        dummy=dummy+1
        W[int(t/4)][i][t%c]=temp[i][0]
    t=t+1
    for j in range(1,ks):
      if t<(rounds+1)*c:
        for i in range(0,r):
          CXOR.append([temp[i][j],temp[i][j-1],next,dummy])
          temp[i][j]= next
          next=next+1
          dummy=dummy+1
          W[int(t/4)][i][t%c]=temp[i][j]
        t=t+1
    
  return [W,next,dummy,O,CXOR]
  
#Updates a,next,dummy,O,CXOR and CMC after one round
#in: a (difference vector),r (number of rows of the small AES),c (number of columns of the small AES), rk (round key), next (the next 
#unused index for x), dummy (the next unused index for d), O (list of #indices for objective #function), CXOR (list of constraints #implied by the XOR's), CMC (list of constraints implied by the MixColumn #operation), related (true in the related-key setting, false #in the non-related key setting)  
#out: a (difference vector), next (the next unused index for x), dummy (the next unused index for d), O (list of indices for objective #function), CXOR (list of constraints implied by the XOR's), CMC (list of constraints implied by the MixColumn operation)  
def round(a,r,c,rk,next,dummy,i,O,CXOR,CMC,related):
  S=Substitution(a,r,c,O)   
  a=S[0]
  O=S[1]
  a=ShiftRows(a,r,c) 
  MC=MixColumn(a,r,c,next,dummy,CMC)
  a=MC[0]
  next=MC[1]
  dummy=MC[2]
  CMC=MC[3]
  if related:
    KA=KeyAddition(a,r,c,rk,next,dummy,CXOR) 
    a=KA[0]
    next=KA[1]
    dummy=KA[2]
    CXOR=KA[3]
  return[a,next,dummy,O,CXOR,CMC]

#Generates a MILP (Sage) for calculating the minimum number of active S-boxes for 'rounds' rounds of AES with key size 'keysize' in #the non-related (related=false, differential or linear) or related (related=true, only for differential) key setting and solves this #program using the "Coin" solver
#in: rounds (the number of rounds), n (number of rounds of the small AES), r (number of rows of the small AES),c (number of columns of #the small AES), keysize (the key size), related (false for the non-related key setting, true for the related key #setting)
#out: the MILP (for calculating the minimum number of active S-boxes for 'rounds' rounds of AES with key #size 'keysize' in the non-related (related=false) or related (related=true) key setting and prints the solution of this program
def giveEquationsSmallAESandSolve(rounds,n,r,c,e,related):
  next=0   #next unused state/key variable index 
  dummy=0  #next unused dummy variable index 
  a=[]     #state variable
  k=[]     #key variable
  O=[]     #list of indices for objective function  
  CXOR=[]  #list of constraints implied by the XOR's
  CMC=[]   #list of constraints implied by the MixColumn operation
  CA=[]    #list of indices for constraint to ensure not all inputs are zero 
  RK=[]
  if related:
    for i in range(0,r):    
      k.append([])
      for j in range(0,c):
        k[i].append(next)
        CA.append(next)
        next=next+1

    KS=KeySchedule(r,c,k,ks,rounds,O,CXOR,next,dummy)
    RK=KS[0]
    next=KS[1]
    dummy=KS[2]
    O=KS[3]
    CXOR=KS[4]

  for i in range(0,r):
    a.append([])
    for j in range(0,c):
      a[i].append(next)
      CA.append(next)
      next=next+1
  
  if related:
    KA=KeyAddition(a,r,c,RK[0],next,dummy,CXOR) 
    a=KA[0]
    next=KA[1]
    dummy=KA[2]
    CXOR=KA[3]
  else:
    for i in range(0,rounds+1):
      RK.append(0)
  
  for i in range(1,rounds+1):
    NR=round(a,r,c,RK[i],next,dummy,i,O,CXOR,CMC,related)
    a=NR[0]
    next=NR[1]
    dummy=NR[2]
    O=NR[3]
    CXOR=NR[4]
    CMC=NR[5]
  
  f = open('Eq-SR('+str(n)+','+str(r)+','+str(c)+','+str(e)+')-With-'+str(rounds)+'-Rounds-RelatedKey-'+str(related)+'-Differential.sage','w')
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
  for i in range (0, len(CXOR)):
    EqXOR(CXOR[i][0],CXOR[i][1],CXOR[i][2],CXOR[i][3],f)
  for i in range (0, len(CMC)):
    EqMC(CMC[i][0],CMC[i][1],CMC[i][2],f,r)
  f.write('\n')
  f.write("solution=p.solve()"+'\n')
  f.write("print \"Minimal number of S-boxes:\", solution"+'\n')
  f.close()
  execfile('Eq-SR('+str(n)+','+str(r)+','+str(c)+','+str(e)+')-With-'+str(rounds)+'-Rounds-RelatedKey-'+str(related)+'-Differential.sage')

giveEquationsSmallAESandSolve(10,10,2,4,4,false)