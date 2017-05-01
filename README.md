# Crypto-Tools

## KeccakTools

*A set of C++ classes that can help analyze the Keccak sponge function family*

* Developers: Guido Bertoni, Joan Daemen, Michael Peeters, Gilles Van Assche
* [Direct download](http://keccak.noekeon.org/KeccakTools-3.3.zip)
* [Official website](http://keccak.noekeon.org/KeccakTools-doc/)

KeccakTools is a set of C++ classes aimed as an assitant in analyzing the sponge function family `Keccak`. These classes and methods were used to obtain the results reported in the paper `Differential propagation analysis of Keccak` presented at FSE 2012 (available here [IACR ePrint 2012/163](http://eprint.iacr.org/2012/163)).

## S-Box Mixed-Integer Linear Programming tool

*Toolkit for Counting Active S-boxes using Mixed-Integer Linear Programming (MILP)*

* Developer: Nicky Mouha
* [Direct download](http://www.ecrypt.eu.org/tools/uploads/sbox-milp.zip)

This toolkit can be used to prove the security of cryptographic ciphers against linear and differential cryptanalysis. This toolkit generates Mixed-Integer Linear Progreamming problem which counts the minimum number of (linearly or differentially) active S-boxes for a given cipher. The toolkit currently supports AES and xAES (both in the single-key and related-key setting), as well as Enocoro-128v2 (in the related-key setting). The paper that introduced this tookkit is [available online](https://www.esat.kuleuven.be/cosic/publications/article-2080.pdf).

## HashClash

*Framework for MD5 & SHA-1 Differential Path Construction and Chosen-Prefix Collisions for MD5*

* Developer: Marc Stevens
* Repo

This framework contains tools for the constructions of differential paths for MD5 and SHA-1, including chosen-prefix collisions for MD5.

## ARX Toolkit

*The ARX toolkit is a set of tools to study ARX ciphers and hash functions*

* Developer: Gaetan Leurent
* [Official website](http://www.di.ens.fr/~leurent/arxtools.html)

The ARX toolkit is a set of tools to study ARX ciphers and hash functions. This tookkit was presented at the SHA-3 conference in March 2012.

## Information Set Decoding

*A tool for information set decoding*

* Developers: Unknown?
* [Official source](https://github.com/isd-dev/isd/downloads)

This library, written in C++ that is efficient at finding low weight codewords of a linear code using information set decoding.

## Linear Hull Cryptanalysis of PRESENT

*A tool to compute linear hulls for PRESENT cipher*

* Developer: Bingsheng Zhang
* [Direct download](http://www.ecrypt.eu.org/tools/uploads/present-linear-hull.zip)
* [Paper](http://dx.doi.org/10.1007/978-3-642-10433-6_5)

This tool computes linear hulls for the original PRESENT cipher. It confirms and even improves on the predicted bias (and the corresponding attack complexities) of conventional linear relations based on a single linear trail.

## CodingTool Library

*Tool for cryptanalysis based on coding theory*

* Developer: Tomislav Nad
* [Direct download](https://www.iaik.tugraz.at/content/research/krypto/codingtool/downloads/CodingTool-0.9.zip)
* [Official website](http://www.iaik.tugraz.at/content/research/krypto/codingtool/)

The CodingTool library is a collection of tools to use techniques from coding theory in cryptanalysis. The core part is an implementation of a probabilistic algorithm to search for code words with low Hamming weight. Additional functionalities like shortening and puncturing of a linear code or adding a weight to each bit of a code word are implemented. Furthermore, the library provides data structures to assist the user in creating a linear code for a specific problem. An easy to use interface to the provided algorithms, powerful data structures and a command line parser reduces the implementation work of a cryptanalyst to a minimum.

## Sage

*Free open-source mathematics especially number-theory software system, also contains many modules for cryptography*

* [Download](http://www.sagemath.org/)
* [Documentation](http://www.sagemath.org/doc/reference/cryptography.html)

Sage is a free open-source mathematics software system licensed under the GPL. It combines many existing open-source packages into a common Python-based interface. It also contains modules to perform both basic and more advanced computations in cryptography and cryptanalysis.

## Grain of Salt

*An automated way to test stream ciphers through SAT solvers*

* Developer: Mate Soos
* [Official website](http://planete.inrialpes.fr/~soos/GrainOfSalt/)

Grain of Salt is a tool developed to automatically test stream ciphers against standard SAT solver-based attacks. The tool takes as input a set of configuration options and the definition of each filter and feedback function of the stream cipher. It outputs a problem in the language of SAT solvers describing the cipher. The tool can automatically generate SAT problem instances for Crypto-1, HiTag2, Grain, Bivium-B and Trivium. 

## SYMAES

*A Fully Symbolic Polynomial System Generator for AES-128*

* Developers: Vesselin Velichkov, Vincent Rijmen, Bart Preneel
* [Paper](http://www.cosic.esat.kuleuven.be/publications/article-1476.pdf)

SYMAES is a software tool that generates a system of polynomials in GF(2), corresponding to the round transformation and key schedule of the block cipher AES-128.

## Automated Algebraic Cryptanalysis

*A simple tool for the automatic algebraic cryptanalysis of a large array of stream- and block ciphers*

* Developer: Paul Stankovski
* [Official website](http://www.eit.lth.se/index.php?id=260&uhpuid=dhs.pas&hpuid=584&L=1)
* Direct download: [Windows Executeable](http://www.eit.lth.se/fileadmin/eit/home/dhs.pas/win_exec.zip), [Linux Executable](http://www.eit.lth.se/fileadmin/eit/home/dhs.pas/linux_exec.zip), [C code](http://www.eit.lth.se/fileadmin/eit/home/dhs.pas/source.zip)
 
A simple tool for the automatic algebraic cryptanalysis of a large array of stream and block ciphers. Three tests have been implemented and the best results have led to continued work on a computational cluster.

## Algebraic Preimage Attack on Hash functions (AlPAtH)

*A software framework AlPAtH (Algebraic Preimage Attack on Hash functions) to run algebraic attacks on hash function*

* Developer: Luk Bettale
* [Official website](https://www.lukbettale.ze.cx/alpath/)

AlPAtH is a software framework to run algebraic attacks on hash functions. This framework is intended to run algebraic attacks on hash functions, but could be extended to any kind of ciphers (block, stream). It provides a framework to generate equations, solve these equations and interpret the results.

## Lex Toolkit

*A Tool for Algebraic Analysis of Stream Cipher LEX*

* Developer: V. Velichkov, V. Rijmen, and B. Preneel

The Lex Toolkit is a collection of Python programs for the computer algebra system Sage. The programs generate Boolean algebraic equations for a small-scale version of stream cipher LEX.

## Yafu (Yet Another Factorization Utility)

* [Official website](https://sites.google.com/site/bbuhrow/)
* [Official repository](https://sourceforge.net/projects/yafu/)

YAFU (with assistance from other free software) uses the most powerful modern algorithms (and implementations of them) to factor input integers in a completely automated way. Usefull for RSA attacks.

## Msieve

* [Official repository](https://sourceforge.net/projects/msieve/)

Msieve is a C library implementing a suite of algorithms to factor large integers. It contains an implementation of the SIQS and GNFS algorithms. Usefull for RSA attacks.

## PARI/GP

* [Official website](https://pari.math.u-bordeaux.fr/)

PARI/GP is a widely used computer algebra system designed for fast computations in number theory (factorizations, algebraic number theory, elliptic curves...), but also contains a large number of other useful functions to compute with mathematical entities such as matrices, polynomials, power series, algebraic numbers etc., and a lot of transcendental functions. PARI is also available as a C library to allow for faster computations.

## CADO-NFS

* [Official website](http://cado-nfs.gforge.inria.fr)
* [Official repository](https://gforge.inria.fr/scm/?group_id=2065)

CADO-NFS (Crible Algebrique: Distribution, Optimisation - Number Field Sieve) is a complete implementation in C/C++ of the Number Field Sieve (NFS) algorithm for factoring integers. It consists in various programs corresponding to all the phases of the algorithm, and a general script that runs them, possibly in parallel over a network of computers.

## sha1collisiondetection

* Developer: Marc Stevens
* [Official repository](https://github.com/cr-marcstevens/sha1collisiondetection)

sha1collisiondetection library and command line tool is designed as near drop-in replacements for common SHA-1 libraries and sha1sum. It will compute the SHA-1 hash of any given file and additionally will detect cryptanalytic collision attacks against SHA-1 present in each file. It is very fast and takes less than twice the amount of time as regular SHA-1.