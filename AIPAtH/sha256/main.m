load "hashutils.m";

w := 32;                                /* word size (in bits) */

r := assigned r select StringToInteger(Sprint(r)) else 64;
r := r eq 0 select 64 else r;
error if r lt 8, "r must be > 7";

Sigma0 := func<x|_xor(_rotr(x,2),_rotr(x,13),_rotr(x,22))>;
Sigma1 := func<x|_xor(_rotr(x,6),_rotr(x,11),_rotr(x,25))>;
sigma0 := func<x|_xor(_rotr(x,7),_rotr(x,18),_shr(x,3))>;
sigma1 := func<x|_xor(_rotr(x,17),_rotr(x,19),_shr(x,10))>;

load "sha2/template.m";
