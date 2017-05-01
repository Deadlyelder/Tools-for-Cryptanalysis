load "hashutils.m";

maxR := 5;                              /* number of rounds */
L := 5;                                 /* state length (in words) */
ntmp := 4;                              /* number of temporary words */
r := assigned r select StringToInteger(Sprint(r)) else 80;

function cv2var(cv)
    return [_rotr(cv[i],10) : i in [1,5,4]] cat [cv[i] : i in [3,2]];
end function;

function var2cv(var)
    return [_rotl(var[1],10)] cat
           [var[i] : i in [5,4]] cat
           [_rotl(var[i],10) : i in [3,2]];
end function;

procedure round(~cv,x,f,k,s)
    cv[1] := _add(_rotl(_add(cv[1],f(cv[2],cv[3],cv[4]),x,k),s),cv[5]);
    cv[3] := _rotl(cv[3],10);
    Rotate(~cv,1);
end procedure;

function round_val(cv,x,f,k,s)
    tmp := [];
    tmp[1] := f(cv[2],cv[3],cv[4]);
    tmp[2] := _add(cv[1],tmp[1]);
    tmp[3] := _add(tmp[2],x);
    tmp[4] := _add(tmp[3],k);
    v := _add(_rotl(tmp[4],s),cv[5]);
    return tmp,v;
end function;

function round_eqs(cv,tmp,x,f,k,s,v)
    eqs := [];
    eqs cat:= _xor(tmp[1],f(cv[2],cv[3],cv[4]));
    eqs cat:= _add_eqs(cv[1],tmp[1],tmp[2]);
    eqs cat:= _add_eqs(tmp[2],x,tmp[3]);
    eqs cat:= _add_eqs(tmp[3],k,tmp[4]);
    eqs cat:= _add_eqs(_rotl(tmp[4],s),cv[5],v);
    return eqs;
end function;

load "ripemd/template.m";
