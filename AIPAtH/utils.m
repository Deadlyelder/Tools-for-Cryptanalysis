/* bitfield from a string */
function stringtobits(s)
    return &cat [Reverse(Intseq(BString(s)[i],2,8)) : i in [1..#s]];
end function;

/* printing */
procedure printbits(bits)
    printf "%h\n",Seqint(Reverse(ChangeUniverse(bits,Integers())),2);
end procedure;

/* dump a polynomial system */
procedure dump(F,file)
    for f in F do
        if Type(f) eq RngMPolBoolElt then
            for m in Monomials(f) do
                for v in Support(m) do
                    fprintf file, "%o,", v;
                end for;
                fprintf file, "|";
            end for;
        else
            for m in Monomials(f) do
                for vd in Factorization(m) do
                    v := Support(vd[1])[1];
                    d := vd[2];
                    for i := 1 to d do
                        fprintf file, "%o,", v;
                    end for;
                end for;
                fprintf file, "|";
            end for;
        end if;
        fprintf file, "\n";
    end for;
end procedure;
