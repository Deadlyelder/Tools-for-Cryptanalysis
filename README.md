# Crypto-Tools

A curated list of cryptography and cryptoanalysis related tools and libraries.

## Motivation

The motivation of curating a list of cryptography and crytoanalysis related tools was born from desire to have a centralized point where all such tools can be found. Attempts will be made to keep it updated as frequently as possible. If you find any tools/library that are missing feel free to contribute.

## Contents
1. [KeccakTools](#KeccakTools)
2. [S-Box Mixed-Integer Linear Programming tool](#MILP)
3. [HashClash](#HashClash)
4. [ARX Toolkit](#arxtoolkit)
5. [Information Set Decoding](#isd)
6. [Linear Hull Cryptanalysis PRESENT](#linearpresent)
7. [CodingTool Library](#codingtool)
8. [Grain of Salt](#grainsalt)
9. [SYMAES](#symaes)
10. [Automated Algebric Cryptanalysis](#autoalger)
11. [Algebraic Preimage Attack on Hash functions](#alpath)
12. [Lex Toolkit](#lex)
13. [Yafu](#yafu)
14. [Msieve](#msieve)
15. [CADO-NFS](#cado)
16. [sha1collisiondetection](#sha1coll)
17. [S-function Toolkit](#sfun)
1. [License](#license)

<a name="KeccakTools"></a>
## KeccakTools

*A set of C++ classes that can help analyze the Keccak sponge function family*

* Developers: Guido Bertoni, Joan Daemen, Michael Peeters, Gilles Van Assche
* [Direct download](http://keccak.noekeon.org/KeccakTools-3.3.zip)
* [Official website](http://keccak.noekeon.org/KeccakTools-doc/)

KeccakTools is a set of C++ classes aimed as an assitant in analyzing the sponge function family `Keccak`. These classes and methods were used to obtain the results reported in the paper `Differential propagation analysis of Keccak` presented at FSE 2012 (available here [IACR ePrint 2012/163](http://eprint.iacr.org/2012/163)).

<a name="milp"></a>
## S-Box Mixed-Integer Linear Programming tool

*Toolkit for Counting Active S-boxes using Mixed-Integer Linear Programming (MILP)*

* Developer: Nicky Mouha
* [Direct download](http://www.ecrypt.eu.org/tools/uploads/sbox-milp.zip)

This toolkit can be used to prove the security of cryptographic ciphers against linear and differential cryptanalysis. This toolkit generates Mixed-Integer Linear Progreamming problem which counts the minimum number of (linearly or differentially) active S-boxes for a given cipher. The toolkit currently supports AES and xAES (both in the single-key and related-key setting), as well as Enocoro-128v2 (in the related-key setting). The paper that introduced this toolkit is [available online](https://www.esat.kuleuven.be/cosic/publications/article-2080.pdf).

<a name="HashClash"></a>
## HashClash 

*Framework for MD5 & SHA-1 Differential Path Construction and Chosen-Prefix Collisions for MD5*

* Developer: Marc Stevens
* [Official website](https://marc-stevens.nl/p/hashclash/index.php)

This framework contains tools for the constructions of differential paths for MD5 and SHA-1, including chosen-prefix collisions for MD5.

<a name="arxtoolkit"></a>
## ARX Toolkit

*The ARX toolkit is a set of tools to study ARX ciphers and hash functions*

* Developer: Gaetan Leurent
* [Official website](http://www.di.ens.fr/~leurent/arxtools.html)

The ARX toolkit is a set of tools to study ARX ciphers and hash functions. This tookkit was presented at the SHA-3 conference in March 2012.

<a name="isd"></a>
## Information Set Decoding

*A tool for information set decoding*

* Developers: Unknown?
* [Official source](https://github.com/isd-dev/isd/downloads)

This library, written in C++ that is efficient at finding low weight codewords of a linear code using information set decoding.

<a name="linearpresent"></a>
## Linear Hull Cryptanalysis of PRESENT

*A tool to compute linear hulls for PRESENT cipher*

* Developer: Bingsheng Zhang
* [Direct download](http://www.ecrypt.eu.org/tools/uploads/present-linear-hull.zip)
* [Paper](http://dx.doi.org/10.1007/978-3-642-10433-6_5)

This tool computes linear hulls for the original PRESENT cipher. It confirms and even improves on the predicted bias (and the corresponding attack complexities) of conventional linear relations based on a single linear trail.

<a name="codingtool"></a>
## CodingTool Library

*Tool for cryptanalysis based on coding theory*

* Developer: Tomislav Nad
* [Direct download](https://www.iaik.tugraz.at/content/research/krypto/codingtool/downloads/CodingTool-0.9.zip)
* [Official website](http://www.iaik.tugraz.at/content/research/krypto/codingtool/)

The CodingTool library is a collection of tools to use techniques from coding theory in cryptanalysis. The core part is an implementation of a probabilistic algorithm to search for code words with low Hamming weight. Additional functionalities like shortening and puncturing of a linear code or adding a weight to each bit of a code word are implemented. Furthermore, the library provides data structures to assist the user in creating a linear code for a specific problem. An easy to use interface to the provided algorithms, powerful data structures and a command line parser reduces the implementation work of a cryptanalyst to a minimum.

<a name="grainsalt"></a>
## Grain of Salt

*An automated way to test stream ciphers through SAT solvers*

* Developer: Mate Soos
* [Official website](http://planete.inrialpes.fr/~soos/GrainOfSalt/)

Grain of Salt is a tool developed to automatically test stream ciphers against standard SAT solver-based attacks. The tool takes as input a set of configuration options and the definition of each filter and feedback function of the stream cipher. It outputs a problem in the language of SAT solvers describing the cipher. The tool can automatically generate SAT problem instances for Crypto-1, HiTag2, Grain, Bivium-B and Trivium. 

<a name="symaes"></a>
## SYMAES

*A Fully Symbolic Polynomial System Generator for AES-128*

* Developers: Vesselin Velichkov, Vincent Rijmen, Bart Preneel
* [Paper](http://www.cosic.esat.kuleuven.be/publications/article-1476.pdf)

SYMAES is a software tool that generates a system of polynomials in GF(2), corresponding to the round transformation and key schedule of the block cipher AES-128.

<a name="autoalge"></a>
## Automated Algebraic Cryptanalysis

*A simple tool for the automatic algebraic cryptanalysis of a large array of stream- and block ciphers*

* Developer: Paul Stankovski
* [Official website](http://www.eit.lth.se/index.php?id=260&uhpuid=dhs.pas&hpuid=584&L=1)
* Direct download: [Windows Executeable](http://www.eit.lth.se/fileadmin/eit/home/dhs.pas/win_exec.zip), [Linux Executable](http://www.eit.lth.se/fileadmin/eit/home/dhs.pas/linux_exec.zip), [C code](http://www.eit.lth.se/fileadmin/eit/home/dhs.pas/source.zip)
 
A simple tool for the automatic algebraic cryptanalysis of a large array of stream and block ciphers. Three tests have been implemented and the best results have led to continued work on a computational cluster.

<a name="alpath"></a>
## Algebraic Preimage Attack on Hash functions (AlPAtH)

*A software framework AlPAtH (Algebraic Preimage Attack on Hash functions) to run algebraic attacks on hash function*

* Developer: Luk Bettale
* [Official website](https://www.lukbettale.ze.cx/alpath/)

AlPAtH is a software framework to run algebraic attacks on hash functions. This framework is intended to run algebraic attacks on hash functions, but could be extended to any kind of ciphers (block, stream). It provides a framework to generate equations, solve these equations and interpret the results.

<a name="lex"></a>
## Lex Toolkit

*A Tool for Algebraic Analysis of Stream Cipher LEX*

* Developer: V. Velichkov, V. Rijmen, and B. Preneel

The Lex Toolkit is a collection of Python programs for the computer algebra system Sage. The programs generate Boolean algebraic equations for a small-scale version of stream cipher LEX.

<a name="yafu"></a>
## Yafu (Yet Another Factorization Utility)

* [Official website](https://sites.google.com/site/bbuhrow/)
* [Official repository](https://sourceforge.net/projects/yafu/)

YAFU (with assistance from other free software) uses the most powerful modern algorithms (and implementations of them) to factor input integers in a completely automated way. Usefull for RSA attacks.

<a name="msieve"></a>
## Msieve

* [Official repository](https://sourceforge.net/projects/msieve/)

Msieve is a C library implementing a suite of algorithms to factor large integers. It contains an implementation of the SIQS and GNFS algorithms. Usefull for RSA attacks.

<a name="cado"></a>
## CADO-NFS

* [Official website](http://cado-nfs.gforge.inria.fr)
* [Official repository](https://gforge.inria.fr/scm/?group_id=2065)

CADO-NFS (Crible Algebrique: Distribution, Optimisation - Number Field Sieve) is a complete implementation in C/C++ of the Number Field Sieve (NFS) algorithm for factoring integers. It consists in various programs corresponding to all the phases of the algorithm, and a general script that runs them, possibly in parallel over a network of computers.

<a name="sha1coll"></a>
## sha1collisiondetection

* Developer: Marc Stevens
* [Official repository](https://github.com/cr-marcstevens/sha1collisiondetection)

sha1collisiondetection library and command line tool is designed as near drop-in replacements for common SHA-1 libraries and sha1sum. It will compute the SHA-1 hash of any given file and additionally will detect cryptanalytic collision attacks against SHA-1 present in each file. It is very fast and takes less than twice the amount of time as regular SHA-1.

<a name="sfun"></a>
## S-function Toolkit

Toolkit for differential cryptanalysis of S-functions

* Developers: Nicky Mouha, Vesselin Velichkov, Christophe De Cannière, Bart Preneel
* [Direct download](http://www.ecrypt.eu.org/tools/uploads/s-function_toolkit_v2.zip)
* [Paper](http://www.cosic.esat.kuleuven.be/publications/article-1473.pdf)

An increasing number of cryptographic primitives use operations such as addition modulo 2n, multiplication by a constant and bitwise Boolean functions as a source of non-linearity. In NIST’s SHA-3 competition, this applies to 6 out of the 14 second-round candidates. An S-function is a function that calculates the i-th output bit using only the inputs of the i-th bit position and a finite state S[i]. Although S-functions have been analyzed before, this toolkit is the first to present a fully general and efficient framework to determine their differential properties. A precursor of this framework was used in the cryptanalysis of SHA-1. 

<a name="license"></a>
### License

[![CC0](http://mirrors.creativecommons.org/presskit/buttons/88x31/svg/cc-zero.svg)](https://creativecommons.org/publicdomain/zero/1.0/)

This list is released into the public domain.
