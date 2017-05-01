load "hashutils.m";

w := 64;                                /* word size (in bits) */

r := assigned r select StringToInteger(Sprint(r)) else 80;
r := r eq 0 select 80 else r;
error if r lt 8, "r must be > 7";

Sigma0 := func<x|_xor(_rotr(x,28),_rotr(x,34),_rotr(x,39))>;
Sigma1 := func<x|_xor(_rotr(x,14),_rotr(x,18),_rotr(x,41))>;
sigma0 := func<x|_xor(_rotr(x,1),_rotr(x,8),_shr(x,7))>;
sigma1 := func<x|_xor(_rotr(x,19),_rotr(x,61),_shr(x,6))>;

load "sha2/template.m";
