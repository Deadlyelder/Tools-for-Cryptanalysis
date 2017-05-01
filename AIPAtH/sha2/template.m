load "hashutils.m";

error if not (assigned w and assigned r and
              assigned Sigma0 and assigned Sigma1 and
              assigned sigma0 and assigned sigma1),
      "parameters should be assigned";

n := 16;                                /* message length (in words) */

b2w,w2b := bits2words(w);
i2w,w2i := integers2words(w);

primes := [i eq 1 select 2 else NextPrime(Self(i-1)) : i in [1..r]];
constants := i2w([Floor(2^w*(Root(i,3)-Floor(Root(i,3)))) : i in primes]);
IV := i2w([Floor(2^w*(Sqrt(i)-Floor(Sqrt(i)))) : i in primes[1..8]]);

f_ch  := func<x,y,z | _xor(_and(x,y),_and(_not(x),z))>;
f_maj := func<x,y,z | _xor(_and(x,y),_and(x,z),_and(y,z))>;

function expansion(x,r)
    w := x;
    for i in [#x+1..r] do
        w[i] := _add(sigma1(w[i-2]),w[i-7],sigma0(w[i-15]),w[i-16]);
    end for;
    return w;
end function;

function expansion_eqs(w,tmp,r)
    eqs := [];
    for i in [n+1..r] do
        t := (i-n-1)*2;
        eqs cat:= _add_eqs(sigma1(w[i-2]),w[i-7],tmp[t+1]);
        eqs cat:= _add_eqs(tmp[t+1],sigma0(w[i-15]),tmp[t+2]);
        eqs cat:= _add_eqs(tmp[t+2],w[i-16],w[i]);
    end for;
    return eqs;
end function;

function expansion_val(x,r)
    w := x;
    tmp := [];
    for i in [n+1..r] do
        t := (i-n-1)*2;
        tmp[t+1] := _add(sigma1(w[i-2]),w[i-7]);
        tmp[t+2] := _add(tmp[t+1],sigma0(w[i-15]));
        w[i] := _add(tmp[t+2],w[i-16]);
    end for;
    return w,tmp;
end function;

function compression(x,key,r)
    w := expansion(x,r);
    var := key;
    for i in [1..r] do
        t1 := _add(var[8],Sigma1(var[5]),f_ch(var[5],var[6],var[7]),
                   constants[i],w[i]);
        t2 := _add(Sigma0(var[1]),f_maj(var[1],var[2],var[3]));
        var[4] := _add(var[4],t1);
        var[8] := _add(t1,t2);
        var := Rotate(var,1);
    end for;
    h := [_add(var[i],key[i]) : i in [1..#var]];
    return h;
end function;

function compression_eqs(w,tmpw,key,var,tmp,h,r)
    eqs := [];
    eqs cat:= expansion_eqs(w,tmpw,r);
    var := [key[i] : i in [8,4,7,3,6,2,5,1]] cat var;
    for i := 1 to r do
        t := (i-1)*7;
        k := (i-1)*2;
        eqs cat:= _add_eqs(var[k+1],Sigma1(var[k+7]),tmp[t+1]);
        eqs cat:= _xor(tmp[t+2],f_ch(var[k+7],var[k+5],var[k+3]));
        eqs cat:= _add_eqs(tmp[t+1],tmp[t+2],tmp[t+3]);
        eqs cat:= _add_eqs(tmp[t+3],constants[i],tmp[t+4]);
        eqs cat:= _add_eqs(tmp[t+4],w[i],tmp[t+5]); /* t1 */
        
        eqs cat:= _xor(tmp[t+6],f_maj(var[k+8],var[k+6],var[k+4]));
        eqs cat:= _add_eqs(Sigma0(var[k+8]),tmp[t+6],tmp[t+7]); /* t2 */
        
        eqs cat:= _add_eqs(var[k+2],tmp[t+5],var[k+9]);
        eqs cat:= _add_eqs(tmp[t+5],tmp[t+7],var[k+10]);
    end for;
    last := [var[r*2+i] : i in [8,6,4,2,7,5,3,1]];
    eqs cat:= &cat [_add_eqs(last[i],key[i],h[i]) : i in [1..#last]];
    return eqs;
end function;

function compression_val(x,key,r)
    w,tmpw := expansion_val(x,r);
    var := [key[i] : i in [8,4,7,3,6,2,5,1]];
    tmp := [];
    for i := 1 to r do
        t := (i-1)*7;
        k := (i-1)*2;
        tmp[t+1] := _add(var[k+1],Sigma1(var[k+7]));
        tmp[t+2] := f_ch(var[k+7],var[k+5],var[k+3]);
        tmp[t+3] := _add(tmp[t+1],tmp[t+2]);
        tmp[t+4] := _add(tmp[t+3],constants[i]);
        tmp[t+5] := _add(tmp[t+4],w[i]); /* t1 */
        
        tmp[t+6] := f_maj(var[k+8],var[k+6],var[k+4]);
        tmp[t+7] := _add(Sigma0(var[k+8]),tmp[t+6]); /* t2 */
        
        var[k+9] := _add(var[k+2],tmp[t+5]);
        var[k+10] := _add(tmp[t+5],tmp[t+7]);
    end for;
    last := [var[r*2+i] : i in [8,6,4,2,7,5,3,1]];
    h := [_add(last[i],key[i]) : i in [1..#last]];
    return w[1..r],tmpw,key,var[9..#var],tmp,h;
end function;

function pad(msg)
    size := Intseq(#msg,2,2*w);
    x := msg cat [1] cat
         [GF(2)!0 : i in [(#msg+2*w) mod (n*w)+2..n*w]];
    return x cat Reverse(size);
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
    er := (r-n) le 0 select 0 else r-n;
    nbvar := (r+2*er+2*r+7*r+8)*w;
    if not fixedIV then
        nbvar +:= 8*w;
    end if;
    P<[x]> := BooleanPolynomialRing(nbvar);
    cpt := Rank(P);
    xx := [Reverse(y) : y in b2w(Reverse(x[cpt-n*w+1..cpt]))]; cpt-:=n*w;
    xxn := [["msg" cat
             "[" cat i2s(i-1) cat "," cat i2s(j-1) cat "]"
             : j in [1..w]] : i in [1..n]];
    tx := [Reverse(y) : y in b2w(Reverse(x[cpt-3*er*w+1..cpt]))]; cpt-:=3*er*w;
    xw := [tx[i+2] : i in [1..3*er by 3]];
    xwn := [["exp" cat
             "[" cat i2s(i-1) cat "," cat i2s(j-1) cat "]"
             : j in [1..w]] : i in [n+1..r]];
    tw := &cat [tx[i..i+1] : i in [1..3*er by 3]];
    twn := [["tmpexp" cat i2s(k-1) cat
             "[" cat i2s(i-1) cat "," cat i2s(j-1) cat "]"
             : j in [1..w]] : k in [1..2], i in [n+1..r]];
    if fixedIV then
        kk := IV;
        kkn := [];
    else
        kk := [Reverse(y) : y in b2w(Reverse(x[cpt-8*w+1..cpt]))]; cpt-:=8*w;
        kkn := [["iv" cat
                 "[" cat i2s(i-1) cat "," cat i2s(j-1) cat "]"
                 : j in [1..w]] : i in [1..8]];
    end if;
    tv := [Reverse(y) : y in b2w(Reverse(x[cpt-9*r*w+1..cpt]))]; cpt-:=9*r*w;
    vv := &cat [tv[i+7..i+8] : i in [1..9*r by 9]];
    vvn := [["int" cat i2s(k-1) cat
             "[" cat i2s(i-1) cat "," cat i2s(j-1) cat "]"
             : j in [1..w]] : k in [1..2], i in [1..r]];
    tt := &cat [tv[i..i+6] : i in [1..9*r by 9]];
    ttn := [["tmpint" cat i2s(k-1) cat
             "[" cat i2s(i-1) cat "," cat i2s(j-1) cat "]"
             : j in [1..w]] : k in [1..7], i in [1..r]];
    hh := [Reverse(y) : y in b2w(Reverse(x[cpt-8*w+1..cpt]))]; cpt-:=8*w;
    hhn := [["hash" cat
             "[" cat i2s(i-1) cat "," cat i2s(j-1) cat "]"
             : j in [1..w]] : i in [1..8]];

    if fixedIV then
        catvar := [Support(m)[1]
                   : m in &cat [&cat e : e in [xx,xw,tw,vv,tt,hh]]];
        catmsg := &cat [&cat n : n in [xxn,xwn,twn,vvn,ttn,hhn]];
    else
        catvar := [Support(m)[1]
                   : m in &cat [&cat e : e in [xx,xw,tw,kk,vv,tt,hh]]];
        catmsg := &cat [&cat n : n in [xxn,xwn,twn,kkn,vvn,ttn,hhn]];
    end if;
    names := [catmsg[Index(catvar,i)] : i in [1..Rank(P)]];
    AssignNames(~P,names);
    return compression_eqs(xx cat xw,tw,kk,vv,tt,hh,r), w2b(xx), w2b(hh);
end function;
