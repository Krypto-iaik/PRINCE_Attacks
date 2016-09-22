# Practical Low-Data Cryptanalysis of Round-Reduced PRINCE

For a complete discussion about the cryptanalysis of PRINCE, we refer to the following paper:

Title: "Practical low data-complexity subspace-trail cryptanalysis of round-reduced PRINCE" Authors: Lorenzo Grassi and Christian Rechberger at: http://eprint.iacr.org/

We enclose the following individual programs:

0) PrinceImplementation.c
1) Attack3Rounds_TruncatedAttack_1.c
2) Attack3Rounds_TruncatedAttack_2.c
3) Attack4Rounds_ExtensionEnd.c
4) SquareAttack_ModifiedPRINCE.c

We have used gcc version 4.8.1, but all three should run with most C-compilers.
None of these programs is speed-optimized, they are for verification purposes only.

"PrinceImplementation.c" is our implementation of PRINCE which has been verified against test-vectors.
0) The first program verifies the Truncated Differential attack on 3 rounds described in Appendix D.1, where the attacker works independetently on each nibble.
1 - 2) The second and the third one verify the attack described in Appendix D.2, that is the Truncated Differential Attack on 3 rounds that exploits the relationships among the nibbles.
3) The third one verifies the attack described in Sect. 5 - that is, the Truncated Differential Attack with the Extension at the End on 4 rounds, which won the PRINCE challenge (see https://www.emsec.rub.de/research/research_startseite/prince-challenge/).
4) The final one verifies the balance property on 5.5 rounds of PRINCE (the 6 middle rounds), when the ShiftRows and the MixLayer operations are exchanged in positions - Sect. 4.

Each program contains our implementation of complete PRINCE, and 3- and 4-round-reduced PRINCE.
About the 3 rounds, the implementation is composed of 1 forward round and of the middle rounds.

In each program, the user can choose the key of PRINCEcore (k_1), the key of the FX construction (k_0) and the number of chosen plaintexts (that belong to the same coset of C used for the attack) - about the number of CP, we advise to use the ones given in the paper. 
In the second program, the number of CP used for the truncated differential part of the attack ("numberPlaintext") can be different by the number of CP used for the invariant subspace attack of the attack ("numberPlaintext2"). The user can choose both these numbers.
Each of these parameters can be found in the main part (which is the last one in each of the programs).

The program generates the requested number of chosen plaintexts (in a random way), computes the ciphertexts and implements the attack.
Finally, the program counts the number of S-Box look-ups and compares it with the theoretically values.
In all the programs, we don't have used the following optimization:
"x satisfies the equality S-Box(x \oplus a) \oplus S-Box(x) = b iff x \oplus a satisfies it".
For this reason, the theoretical values given in the paper must be multiplied by a factor 2 (the first program) and of a factor 4
(the second program). This explains why the subspace attack requires a higher number of S-Box look-ups than the truncated differential attack (as opposite of the theory).

For the attacks on 3 rounds, the attacker can also choose how many times repeat the attack (using the parameter "numberTest" in
line 15).
For each time, the program generates random plaintexts. It is interesting to note that with the same number of chosen plaintexts,
the number of found keys is not always the same. That is, with high probability the program finds only the right one, but it is possible that it finds also wrong keys besides the right one. As we said in the paper, this fact depends on the ciphertexts.

Only for simplicity, the programs don't try to find the complete secret key, but only part of it.
In particular, the first, the second and the third programs (that is, the programs on 3 rounds) find 4 nibbles (one column) of the final key, which is k_0' \oplus k_1.

About the third program (that is, the one on 4 rounds) finds two columns of the final key (k_0' \oplus k_1) and 8 nibbles of k_1.
In particular, the attack finds one column of the final key and 4 nibbles of k_1 using the truncated differential attack.
When these values are found, the attack finds the second column of the final key and other 4 nibbles of k_1 using the subspace attack.
In this case, the user can choose the number of texts used for the first part of the attack - the truncated differential one (namely "numberPlaintext"  line 1051) and the number of texts for the second part - the subspace attack (namely "numberPlaintext2" - line 1052). In particular, these values are "numberPlaintext=12 or 8" and "numberPlaintext2 = 6 or 8".
However, WE NOTICED THAT (WITH HIGH PROBABILITY) 8 PLAINTEXTS ARE SUFFICIENT FOR THIS ATTACK.

We believe that these tests are sufficient to verify our theory. Indeed, the other columns can be found exactly in the same way.
Moreover, as we said in the paper, for the first 3 programs, when the attacker finds them, she can decrypt one round and recovers the key of PRINCEcore. 

The final program verifies the balance property on 5.5 rounds of PRINCE (the 6 middle rounds), when the ShiftRows and the MixLayer operations are exchanged in positions.
The input active nibble is the first one (in row and column 0). The user can choose the secret key.



