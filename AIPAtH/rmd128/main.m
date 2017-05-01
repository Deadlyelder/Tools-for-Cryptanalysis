load "hashutils.m";

maxR := 4;                              /* number of rounds */
L := 4;                                 /* state length (in words) */
ntmp := 3;                              /* number of temporary words */
r := assigned r select StringToInteger(Sprint(r)) else 64;

function cv2var(cv)
    return [cv[i] : i in [1,4,3,2]];
end function;

function var2cv(var)
    return [var[i] : i in [1,4,3,2]];
end function;

procedure round(~cv,x,f,k,s)
    cv[1] := _rotl(_add(cv[1],f(cv[2],cv[3],cv[4]),x,k),s);
    Rotate(~cv,1);
end procedure;

function round_val(cv,x,f,k,s)
    t := [];
    t[1] := f(cv[2],cv[3],cv[4]);
    t[2] := _add(cv[1],t[1]);
    t[3] := _add(t[2],x);
    v := _rotl(_add(t[3],k),s);
    return t,v;
end function;

function round_eqs(cv,t,x,f,k,s,v)
    eqs := [];
    eqs cat:= _xor(t[1],f(cv[2],cv[3],cv[4]));
    eqs cat:= _add_eqs(cv[1],t[1],t[2]);
    eqs cat:= _add_eqs(t[2],x,t[3]);
    eqs cat:= _add_eqs(t[3],k,_rotr(v,s));
    return eqs;
end function;

load "ripemd/template.m";
