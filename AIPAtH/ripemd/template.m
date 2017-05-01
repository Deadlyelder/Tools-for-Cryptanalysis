load "hashutils.m";

error if not (assigned maxR and assigned r and
              assigned round and assigned round_val and assigned round_eqs),
      "parameters should be assigned";

n := 16;                                /* message length (in words) */
w := 32;                                /* word size (in bits)  */

r := r eq 0 select n*maxR else r;
error if r lt 0, "r must be > 0";

b2B := func<bits|[Reverse(bits[(i-1)*8+1..i*8]) : i in [1..#bits div 8]]>;
B2w := func<bytes|[&cat bytes[(i-1)*(w div 8)+1..i*(w div 8)]
                   : i in [1..#bytes div (w div 8)]]>;
b2w := func<bits|B2w(b2B(bits))>;
B2b := func<bytes|&cat [Reverse(B) : B in bytes]>;
w2B := func<words|&cat [[x[(i-1)*8+1..i*8] : i in [1..(w div 8)]]
                        : x in words]>;
w2b := func<words|B2b(w2B(words))>;
i2w := func<ints|[ChangeUniverse(Intseq(i,2,w),GF(2)) : i in ints]>;
w2i := func<words|[Seqint(ChangeUniverse(w,IntegerRing()),2) : w in words]>;

rhoL := func<t | [t[i] : i in [8,5,14,2,11,7,16,4,13,1,10,6,3,15,12,9]]>;
rhoR := func<t | [t[i] : i in [10,7,15,3,4,9,1,14,2,11,12,16,8,5,13,6]]>;
permsL := [(r eq 1) select [i : i in [1..n]]
           else rhoL(Self(r-1)) : r in [1..maxR]];
permsR := [(r eq 1) select [((9*(i-1)+5) mod n)+1 : i in [1..n]]
           else rhoR(Self(r-1)) : r in [1..maxR]];

f_exor := func<x,y,z | _xor(x,y,z)>;
f_mux1 := func<x,y,z | _or(_and(x,y),_and(_not(x),z))>;
f_min1 := func<x,y,z | _xor(_or(x,_not(y)),z)>;
f_mux2 := func<x,y,z | _or(_and(x,z),_and(y,_not(z)))>;
f_min2 := func<x,y,z | _xor(x,_or(y,_not(z)))>;

funcsL := [f_exor, f_mux1, f_min1, f_mux2, f_min2][1..maxR];
funcsR := Reverse(funcsL);

shiftsL := [];
Append(~shiftsL, [11,14,15,12, 5, 8, 7, 9,11,13,14,15, 6, 7, 9, 8]);
Append(~shiftsL, [ 7, 6, 8,13,11, 9, 7,15, 7,12,15, 9,11, 7,13,12]);
Append(~shiftsL, [11,13, 6, 7,14, 9,13,15,14, 8,13, 6, 5,12, 7, 5]);
Append(~shiftsL, [11,12,14,15,14,15, 9, 8, 9,14, 5, 6, 8, 6, 5,12]);
Append(~shiftsL, [ 9,15, 5,11, 6, 8,13,12, 5,12,13,14,11, 8, 5, 6]);

shiftsR := [];
Append(~shiftsR, [ 8, 9, 9,11,13,15,15, 5, 7, 7, 8,11,14,14,12, 6]);
Append(~shiftsR, [ 9,13,15, 7,12, 8, 9,11, 7, 7,12, 7, 6,15,13,11]);
Append(~shiftsR, [ 9, 7,15,11, 8, 6, 6,14,12,13, 5,14,13,13, 7, 5]);
Append(~shiftsR, [15, 5, 8,11,14,14, 6,14, 6, 9,12, 9,12, 5,15, 8]);
Append(~shiftsR, [ 8, 5,12, 9,12, 5,14, 6, 8,13, 6, 5,15,13,11,11]);

primes := [2,3,5,7][1..maxR-1];
constantsL := i2w([Floor(2^30 * Root(p,2)) : p in [0] cat primes]);
constantsR := i2w([Floor(2^30 * Root(p,3)) : p in primes cat [0]]);

IV := i2w([0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0][1..maxR]);

function compression(x,key,r)
    cvL := key;
    cvR := key;
    for R := 1 to maxR do
        for k := 1 to n do
            round(~cvL,x[permsL[R,k]],funcsL[R],constantsL[R],shiftsL[R,k]);
            round(~cvR,x[permsR[R,k]],funcsR[R],constantsR[R],shiftsR[R,k]);
            if (R-1)*n+k eq r then
                break R;
            end if;
        end for;
    end for;
    return [_add(key[(i mod maxR) + 1],
                 cvL[((i+1) mod maxR) + 1],
                 cvR[((i+2) mod maxR) + 1]) : i in [1..maxR]];
end function;

function compression_val(x,key,r)
    var := cv2var(key);
    varL := var;
    varR := var;
    tmpL := [];
    tmpR := [];
    for R := 1 to maxR do
        for k := 1 to n do
            rr := (R-1)*n+k-1;
            cvL := var2cv(varL[rr+1..rr+maxR]);
            cvR := var2cv(varR[rr+1..rr+maxR]);
            tL,vL := round_val(cvL,x[permsL[R,k]],funcsL[R],
                               constantsL[R],shiftsL[R,k]);
            tR,vR := round_val(cvR,x[permsR[R,k]],funcsR[R],
                               constantsR[R],shiftsR[R,k]);
            tmpL cat:= tL;
            tmpR cat:= tR;
            Append(~varL,vL);
            Append(~varR,vR);
            if (R-1)*n+k eq r then
                break R;
            end if;
        end for;
    end for;
    cvL := var2cv(varL[r+1..r+maxR]);
    cvR := var2cv(varR[r+1..r+maxR]);
    tmph := [_add(key[(i mod maxR) + 1],
                  cvL[((i+1) mod maxR) + 1])
             : i in [1..maxR]];
    h := [_add(tmph[i],
               cvR[((i+2) mod maxR) + 1])
          : i in [1..maxR]];
    return x,key,varL[maxR+1..#varL],varR[maxR+1..#varR],tmpL,tmpR,tmph,h;
end function;

function compression_eqs(x,key,varL,varR,tmpL,tmpR,tmph,h,r)
    eqs := [];
    var := cv2var(key);
    varL := var cat varL;
    varR := var cat varR;
    for R := 1 to maxR do
        for k := 1 to n do
            rr := (R-1)*n+k-1;
            cvL := var2cv(varL[rr+1..rr+maxR]);
            cvR := var2cv(varR[rr+1..rr+maxR]);
            tL := tmpL[ntmp*rr+1..ntmp*(rr+1)];
            tR := tmpR[ntmp*rr+1..ntmp*(rr+1)];
            eqs cat:= round_eqs(cvL,tL,x[permsL[R,k]],funcsL[R],
                                constantsL[R],shiftsL[R,k],varL[rr+maxR+1]);
            eqs cat:= round_eqs(cvR,tR,x[permsR[R,k]],funcsR[R],
                                constantsR[R],shiftsR[R,k],varR[rr+maxR+1]);
            if (R-1)*n+k eq r then
                break R;
            end if;
        end for;
    end for;
    cvL := var2cv(varL[r+1..r+maxR]);
    cvR := var2cv(varR[r+1..r+maxR]);
    eqs cat:= &cat [_add_eqs(key[(i mod maxR)+1],cvL[((i+1) mod maxR)+1],
                             tmph[i])
                    : i in [1..maxR]];
    eqs cat:= &cat [_add_eqs(tmph[i],cvR[((i+2) mod maxR)+1],h[i])
                    : i in [1..maxR]];
    return eqs;
end function;

function pad(msg)
    size := Intseq(#msg,2,2*w);
    x := msg cat [1] cat
         [GF(2)!0 : i in [(#msg+2*w) mod (n*w)+2..n*w]];
    return x cat &cat b2B(size);
end function;

function hash(msg)
    x := pad(msg);
    cv := IV;
    N := n*w;
    for i in [1..#x div N] do
        cv := compression(b2w(x[(i-1)*N+1..i*N]),cv,r);
    end for;
    return w2b(cv);
end function;

function equations(:fixedIV:=true)
    i2s := func<i|IntegerToString(i)>;
    nbvar := (n+2*r*(1+ntmp)+2*maxR)*w;
    if not fixedIV then
        nbvar +:= maxR*w;
    end if;
    P<[x]> := BooleanPolynomialRing(nbvar);
    cpt := 0;
    xx := b2w(x[cpt+1..cpt+n*w]); cpt +:= n*w;
    if fixedIV then
        kk := IV;
    else
        kk := b2w(x[cpt+1..cpt+maxR*w]); cpt +:= maxR*w;
    end if;
    vvL := b2w(x[cpt+1..cpt+r*w]); cpt +:= r*w;
    vvR := b2w(x[cpt+1..cpt+r*w]); cpt +:= r*w;
    ttL := b2w(x[cpt+1..cpt+ntmp*r*w]); cpt +:= ntmp*r*w;
    ttR := b2w(x[cpt+1..cpt+ntmp*r*w]); cpt +:= ntmp*r*w;
    th := b2w(x[cpt+1..cpt+maxR*w]); cpt +:= maxR*w;
    hh := b2w(x[cpt+1..cpt+maxR*w]); cpt +:= maxR*w;
    
    return compression_eqs(xx,kk,vvL,vvR,ttL,ttR,th,hh,r), w2b(xx), w2b(hh);
end function;
