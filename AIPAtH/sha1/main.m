load "hashutils.m";

maxR := 4;                              /* round numbers */
L := 5;                                 /* state length (in words) */
n := 16;                                /* message length (in words) */
l := 20;                                /* round length */
w := 32;                                /* word size (in bits) */

r := assigned r select StringToInteger(Sprint(r)) else 0;

r := r eq 0 select l*maxR else r;
error if r lt 0, "r must be > 0";

b2w,w2b := bits2words(w);
i2w,w2i := integers2words(w);

f_ch := func<x,y,z | _xor(_and(x,y),_and(_not(x),z))>;
f_parity := func<x,y,z | _xor(x,y,z)>;
f_maj := func<x,y,z | _xor(_and(x,y),_and(x,z),_and(y,z))>;

funcs := [f_ch, f_parity, f_maj, f_parity];

constants := i2w([0x5a827999,0x6ed9eba1,0x8f1bbcdc,0xca62c1d6]);

IV := i2w([0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0]);

function cv2var(cv)
    return [_rotr(cv[5],30),_rotr(cv[4],30),_rotr(cv[3],30),cv[2],cv[1]];
end function;

function var2cv(var)
    return [var[5],var[4],_rotl(var[3],30),_rotl(var[2],30),_rotl(var[1],30)];
end function;

function expansion(x,r)
    w := x;
    for i in [#x+1..r] do
        w[i] := _rotl(_xor(w[i-3],w[i-8],w[i-14],w[i-16]),1);
    end for;
    return w;
end function;

function compression(x,key,r)
    w := expansion(x,r);
    cv := key;
    for R := 1 to maxR do
        for k := 1 to l do
            cv[5] := _add(_rotl(cv[1],5),funcs[R](cv[2],cv[3],cv[4]),
                      cv[5],constants[R],w[(R-1)*l+k]);
            cv[2] := _rotl(cv[2],30);
            Rotate(~cv,1);
            if (R-1)*l+k eq r then
                break R;
            end if;
        end for;
    end for;
    return [_add(key[i],cv[i]) : i in [1..L]];
end function;

function compression_eqs(x,key,var,tmp,h,r)
    eqs := [];
    w := expansion(x,r);
    var := cv2var(key) cat var;
    for R := 1 to maxR do
        for k := 1 to l do
            rr := (R-1)*l+k-1;
            cv := var2cv(var[rr+1..rr+L]);
            t := tmp[4*rr+1..4*(rr+1)];
            eqs cat:= _xor(t[1],funcs[R](cv[2],cv[3],cv[4]));
            eqs cat:= _add_eqs(_rotl(cv[1],5),t[1],t[2]);
            eqs cat:= _add_eqs(t[2],cv[5],t[3]);
            eqs cat:= _add_eqs(t[3],constants[R],t[4]);
            eqs cat:= _add_eqs(t[4],w[rr+1],var[rr+L+1]);
            if rr+1 eq r then
                break R;
            end if;
        end for;
    end for;
    cv := var2cv(var[r+1..r+L]);
    eqs cat:= &cat [_add_eqs(key[i],cv[i],h[i]) : i in [1..L]];
    return eqs;
end function;

function compression_val(x,key,r)
    w := expansion(x,r);
    var := cv2var(key);
    tmp := [];
    for R := 1 to maxR do
        for k := 1 to l do
            rr := (R-1)*l+k-1;
            cv := var2cv(var[rr+1..rr+L]);
            t := [];
            t[1] := funcs[R](cv[2],cv[3],cv[4]);
            t[2] := _add(_rotl(cv[1],5),t[1]);
            t[3] := _add(t[2],cv[5]);
            t[4] := _add(t[3],constants[R]);
            v := _add(t[4],w[rr+1]);
            tmp cat:= t;
            Append(~var,v);
            if rr+1 eq r then
                break R;
            end if;
        end for;
    end for;
    cv := var2cv(var[r+1..r+L]);
    h := [_add(key[i],cv[i]) : i in [1..L]];
    return x,key,var[L+1..#var],tmp,h;
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
