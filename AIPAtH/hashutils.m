/* bitwise logic */

function _not(a)
    return [1-ai : ai in a];
end function;

function _and(a,b,...)
    c := a;
    for bj in b do
        c := [c[i]*bj[i] : i in [1..#a]];
    end for;
    return c;
end function;

function _or(a,b,...)
    c := a;
    for bj in b do
        c := [c[i] + bj[i] +  c[i]*bj[i] : i in [1..#a]];
    end for;
    return c;
end function;

function _xor(a,b,...)
    c := a;
    for bj in b do
        c := [c[i]+bj[i] : i in [1..#a]];
    end for;
    return c;
end function;

/* shift / rotate */

function _shl(a,k)
    return Parent(a)!([0 : i in [1..k]] cat a[1..#a-k]);
end function;

function _shr(a,k)
    return Parent(a)!(a[k+1..#a] cat [0 : i in [1..k]]);
end function;

function _rotl(a,k)
    return Rotate(a,k);
end function;

function _rotr(a,k)
    return Rotate(a,-k);
end function;

/* modular addition */

function _add(a,b,...)
    c := a;
    b := Reverse(b);
    while not IsEmpty(b) do
        z := [];
        z[1] := c[1] + b[#b][1];
        for i in [2..#c] do
            z[i] := c[i] + b[#b][i] + c[i-1] * b[#b][i-1] + c[i-1]
                    + b[#b][i-1] + z[i-1] * (c[i-1] + b[#b][i-1]);
        end for;
        c := z;
        Prune(~b);
    end while;
    return c;
end function;

function _add_eqs(a,b,c)
    eqs := [];
    eqs[1] := c[1] + a[1] + b[1];
    for i in [2..#a] do
        eqs[i] := c[i] + a[i] + b[i] + a[i-1] * b[i-1]
                  + a[i-1] + b[i-1] + c[i-1] * (a[i-1] + b[i-1]);
    end for;
    return eqs;
end function;

/* words/bytes/bits conversion */

function integers2words(w)
    i2w := func<ints|[ChangeUniverse(Intseq(x,2,w),GF(2)) : x in ints]>;
    w2i := func<wrds|[Seqint(ChangeUniverse(x,IntegerRing()),2) : x in wrds]>;
    return i2w,w2i;
end function;

function bits2words(w)
    b2w := func<bits|[Reverse(bits[(i-1)*w+1..i*w]) : i in [1..#bits div w]]>;
    w2b := func<wrds|&cat [Reverse(x) : x in wrds]>;
    return b2w,w2b;
end function;

function bits2wordsB(w)
    l := w div 8;
    b2B,B2b := bits2words(8);
    b2w := func<bits|[&cat bytes[(i-1)*l+1..i*l] : i in [1..#bytes div l]]
                where bytes is b2B(bits)>;
    w2b := func<wrds|B2b(bytes) where bytes is
                     &cat [[x[(i-1)*8+1..i*8] : i in [1..l]] : x in wrds]>;
    return b2w,w2b;
end function;
