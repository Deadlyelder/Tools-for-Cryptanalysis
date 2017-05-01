load "hashutils.m";

maxR := 4;                              /* round numbers */
L := 4;                                 /* state length (in words) */
n := 16;                                /* message length (in words) */
w := 32;                                /* word size (in bits) */

r := assigned r select StringToInteger(Sprint(r)) else 0;

r := r eq 0 select n*maxR else r;
error if r lt 0, "r must be > 0";

b2w,w2b := bits2wordsB(w);
i2w,w2i := integers2words(w);

perms := [[(1*(i-1) + 0) mod n + 1 : i in [1..n]],
          [(5*(i-1) + 1) mod n + 1 : i in [1..n]],
          [(3*(i-1) + 5) mod n + 1 : i in [1..n]],
          [(7*(i-1) + 0) mod n + 1 : i in [1..n]]];

f_mux1 := func<x,y,z | _or(_and(x,y),_and(_not(x),z))>;
f_mux2 := func<x,y,z | _or(_and(x,z),_and(y,_not(z)))>;
f_exor := func<x,y,z | _xor(x,y,z)>;
f_min3 := func<x,y,z | _xor(y,_or(x,_not(z)))>;

funcs := [f_mux1, f_mux2, f_exor, f_min3];

shifts := [];
Append(~shifts, &cat [[ 7,12,17,22] : i in [1..maxR]]);
Append(~shifts, &cat [[ 5, 9,14,20] : i in [1..maxR]]);
Append(~shifts, &cat [[ 4,11,16,23] : i in [1..maxR]]);
Append(~shifts, &cat [[ 6,10,15,21] : i in [1..maxR]]);

constants := i2w([Floor(2^32*Abs(Sin(i))) : i in [1..n*maxR]]);

IV := i2w([0x67452301,0xefcdab89,0x98badcfe,0x10325476]);

cv2var := func<cv | [cv[i] : i in [1,4,3,2]]>;
var2cv := func<var | [var[i] : i in [1,4,3,2]]>;

function compression(x,key,r)
    cv := key;
    for R := 1 to maxR do
        for k := 1 to n do
            cv[1] := _add(_rotl(_add(cv[1],funcs[R](cv[2],cv[3],cv[4]),
                                     x[perms[R,k]],constants[(R-1)*n+k]),
                                shifts[R,k]),cv[2]);
            Rotate(~cv,1);
            if (R-1)*n+k eq r then
                break R;
            end if;
        end for;
    end for;
    return [_add(key[i],cv[i]) : i in [1..L]];
end function;

function compression_val(x,key,r)
    var := cv2var(key);
    tmp := [];
    for R := 1 to maxR do
        for k := 1 to n do
            rr := (R-1)*n+k-1;
            cv := var2cv(var[rr+1..rr+L]);
            t := [];
            t[1] := funcs[R](cv[2],cv[3],cv[4]);
            t[2] := _add(cv[1],t[1]);
            t[3] := _add(t[2],x[perms[R,k]]);
            t[4] := _add(t[3],constants[rr+1]);
            v := _add(_rotl(t[4],shifts[R,k]),cv[2]);
            tmp cat:= t;
            Append(~var,v);
            if (R-1)*n+k eq r then
                break R;
            end if;
        end for;
    end for;
    cv := var2cv(var[r+1..r+L]);
    h := [_add(key[i],cv[i]) : i in [1..L]];
    return x,key,var[L+1..#var],tmp,h;
end function;

function compression_eqs(x,key,var,tmp,h,r)
    eqs := [];
    var := cv2var(key) cat var;
    for R := 1 to maxR do
        for k := 1 to n do
            rr := (R-1)*n+k-1;
            cv := var2cv(var[rr+1..rr+L]);
            t := tmp[4*rr+1..4*(rr+1)];
            eqs cat:= _xor(t[1],funcs[R](cv[2],cv[3],cv[4]));
            eqs cat:= _add_eqs(cv[1],t[1],t[2]);
            eqs cat:= _add_eqs(t[2],x[perms[R,k]],t[3]);
            eqs cat:= _add_eqs(t[3],constants[rr+1],t[4]);
            eqs cat:= _add_eqs(_rotl(t[4],shifts[R,k]),cv[2],var[rr+L+1]);
            if (R-1)*n+k eq r then
                break R;
            end if;
        end for;
    end for;
    cv := var2cv(var[r+1..r+L]);
    eqs cat:= &cat [_add_eqs(key[i],cv[i],h[i]) : i in [1..L]];
    return eqs;
end function;

function pad(msg)
    size := Intseq(#msg,2,2*w);
    x := msg cat [1] cat
         [GF(2)!0 : i in [(#msg+2*w) mod (n*w)+2..n*w]];
    return x cat &cat b2w(size);
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
    nbvar := (n+r*(1+4)+L)*w;
    if not fixedIV then
        nbvar +:= L*w;
    end if;
    P<[x]> := BooleanPolynomialRing(nbvar);
    cpt := 0;
    xx := b2w(x[cpt+1..cpt+n*w]); cpt +:= n*w;
    if fixedIV then
        kk := IV;
    else
        kk := b2w(x[cpt+1..cpt+L*w]); cpt +:= L*w;
    end if;
    vv := b2w(x[cpt+1..cpt+r*w]); cpt +:= r*w;
    tt := b2w(x[cpt+1..cpt+4*r*w]); cpt +:= 4*r*w;
    hh := b2w(x[cpt+1..cpt+L*w]); cpt +:= L*w;
    
    return compression_eqs(xx,kk,vv,tt,hh,r), w2b(xx), w2b(hh);
end function;
