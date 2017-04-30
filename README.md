# Crypto-Tools

## KeccakTools

*A set of C++ classes that can help analyze the Keccak sponge function family*

* Developers: Guido Bertoni, Joan Daemen, Michael Peeters, Gilles Van Assche
* [Direct download](http://keccak.noekeon.org/KeccakTools-3.3.zip)
* [Official website](http://keccak.noekeon.org/KeccakTools-doc/)

KeccakTools is a set of C++ classes aimed as an assitant in analyzing the sponge function family `Keccak`. These classes and methods were used to obtain the results reported in the paper `Differential propagation anaylsis of Keccak` presented at FSE 2012 (available here [IACR ePrint 2012/163](http://eprint.iacr.org/2012/163)).

## S-Box Mixed-Integer Linear Programming tool

Toolkit for Counting Active S-boxes using Mixed-Integer Linear Programming (MILP)

* Developer: Nicky Mouha
* [Direct download](http://www.ecrypt.eu.org/tools/uploads/sbox-milp.zip)

This toolkit can be used to prove the security of cryptographic ciphers against linear and differential cryptanalysis. This toolkit generates Mixed-Integer Linear Progreamming problem which counts the minimum number of (linearly or differentially) active S-boxes for a given cipher. The toolkit currently supports AES and xAES (both in the single-key and related-key setting), as well as Enocoro-128v2 (in the related-key setting). The paper that introduced this tookkit is [available online](https://www.esat.kuleuven.be/cosic/publications/article-2080.pdf).