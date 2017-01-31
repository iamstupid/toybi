# toybi
Just a toy project of big int, even without division(now).

Contains a non-trivial allocator(though very memory-consuming),a karatsuba implementation,and a balanced toom-3 implementation. Correctness hasn't been fully tested; but a squaring is performed in the `test.cpp` with correct output anyway, though the testcase is very weak (due to convenience).

(Yesterday for that testcase running karatsuba is roughly nlogn..)

## What to add
FFT multiplication. Higher-order toom-cook&inbalanced splitting. Division by inverse.

## Tricks
The substraction routine is very tricky, only being correct under some case(most usable case).

Division by 3 is implemented with inverse element on char[2] field.
