# toybi
Just a toy project of big int, even without division(now).

Contains a non-trivial allocator(though very memory-consuming),a karatsuba implementation,and a balanced toom-3 implementation. Correctness hasn't been fully tested; but a squaring is performed in the `test.cpp` with correct output anyway, though the testcase is very weak (due to convenience).

(Yesterday for that testcase running karatsuba is roughly nlogn..)

## What to add
FFT multiplication. Higher-order toom-cook&inbalanced splitting. Division by inverse.

## Tricks
The substraction routine is very tricky, only being correct under some case(most usable case).

Division by 3 is implemented with inverse element on char[2] field.

## updates

Since then I made many commits but no futher work is done for the bigint part..(It this repo just a grocery storage for me?I don't know either.It is better for management and searching than gist and ubuntu pastebin anyway.)

I added a template function library(for computing within compile time), a number theory template library coded in Python(Python is very convenient in multiprecision integer handling,okay I am just petrified of multiprecision division that is a excruciating task for me..), and zemmud, some code for RNGs..(zemmud is not a existing word but it just looks well..). zemmud should be compiled with `-std=c++11` because I used variadic template..(And zemmud itself contains a simplified version of CPPMPL so it can be built independently)
