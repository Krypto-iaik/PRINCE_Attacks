#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_Round 6

/*NUMBER of TEST = numberTest.
For each of this test, the program generates the desired number of Chosen Plaintexts.
Then, using them, it finds the right key.

Note that the chosen plaintexts are generated randomly (with the condition that belong to the same coset of C).
Thus, the number of key finds by the program depends on the them.
That is, it could be happen that with the same number of CP, the program finds only the right key, or more than 1 possible key.
*/
#define numberTest 5

typedef unsigned char word8;

//S-box
const unsigned char sBox[16] = {
  0xb, 0xf, 0x3, 0x2, 0xa, 0xc, 0x9, 0x1, 0x6, 0x7, 0x8, 0x0, 0xe, 0x5, 0xd, 0x4
};

//Inverse S-box
const unsigned char inv_s[16] = {
  0xb, 0x7, 0x3, 0x2, 0xf, 0xd, 0x8, 0x9, 0xa, 0x6, 0x4, 0x0, 0x5, 0xe, 0xc, 0x1
};

const unsigned char alpha[16] = {
  0xc, 0x0, 0xa, 0xc, 0x2, 0x9, 0xb, 0x7, 0xc, 0x9, 0x7, 0xc, 0x5, 0x0, 0xd, 0xd
};

const unsigned char constants[6][16] = {
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x1, 0x3, 0x1, 0x9, 0x8, 0xa, 0x2, 0xe, 0x0, 0x3, 0x7, 0x0, 0x7, 0x3, 0x4, 0x4,
  0xa, 0x4, 0x0, 0x9, 0x3, 0x8, 0x2, 0x2, 0x2, 0x9, 0x9, 0xf, 0x3, 0x1, 0xd, 0x0,
  0x0, 0x8, 0x2, 0xe, 0xf, 0xa, 0x9, 0x8, 0xe, 0xc, 0x4, 0xe, 0x6, 0xc, 0x8, 0x9,
  0x4, 0x5, 0x2, 0x8, 0x2, 0x1, 0xe, 0x6, 0x3, 0x8, 0xd, 0x0, 0x1, 0x3, 0x7, 0x7,
  0xb, 0xe, 0x5, 0x4, 0x6, 0x6, 0xc, 0xf, 0x3, 0x4, 0xe, 0x9, 0x0, 0xc, 0x6, 0xc
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*GENERATE RANDOM NIBBLE*/

int randomInRange(int min, int max){

  int range = max - min + 1;
  return (min + rand() % range);

}

int randomInRange2(int min, int max){

  int range = max - min + 1;
  int a, b, c;

  a = rand() % range;
  b = rand() % range;
  c = ((a*b) + rand() ) % range;

  return (min + c);

}

word8 randomNibble(){
  return (word8) randomInRange2(0, 15);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*INIZIALIZE A VECTOR*/

void initialization(word8 *p, unsigned char initialMessage[]){

  int i;

  for(i=0; i<16; i++){
    *(p+i) = initialMessage[i];
  }

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*ADD ROUND KEY OPERATION*/

void addRoundKey(word8 *p, word8 key[]){

  int i;

  for(i=0; i<16; i++){
    *(p+i) ^= key[i];
  }

}

/*ADD ROUND CONSTANT*/

void addCostante(word8 *p, int round){

  int i;

  for(i=0; i<16; i++){
    *(p+i) ^= constants[round][i];
  }

}

/*ADD ROUND CONSTANT AND ALPHA*/

void addCostante2(word8 *p, int round){

  int i;

  for(i=0; i<16; i++){
    *(p+i) ^= constants[round][i];
    *(p+i) ^= alpha[i];
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*PRINT*/

void printtt(word8 file[]){

  int i;

  for(i=0; i<16; i++)
  {
    printf("0x%x", file[i]);
    if(i<15)
       printf(", ");
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Byte sub transformation with S-box*/

word8 byteTransformation(word8 byte){

  return sBox[byte];

}

/*Inverse byte sub transformation with Inverse S-box*/

word8 inverseByteTransformation(word8 byte){

  return inv_s[byte];

}

/*S-BOX OPERATION*/

void byteSubTransformation(word8 *puntatore){

  int i;

  for(i=0; i<16; i++){
     *(puntatore+i)=byteTransformation(*(puntatore+i));
  }
}

/*INVERSE S-BOX OPERATION*/

void inverseByteSubTransformation(word8 *puntatore){

  int i;

  for(i=0; i<16; i++){
     *(puntatore+i)=inverseByteTransformation(*(puntatore+i));
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*GENERATION OF THE FINAL KEY OF FX CONSTRUCTION, that is k_0'*/

void keyGeneration(word8 k[], word8 *k2)
{
    int i;
    word8 temp, temp2;

    temp = 0;
    temp2 = (k[1] & 0x08) >> 3;
    k2[0] = ((k[0] &0xe)>> 1) ^ ((k[0] & 0x07) << 1) ^ temp ^ temp2;

    for(i=1; i<15; i++)
    {
        temp = (k[i-1] & 0x01) << 3;
        temp2 = (k[i+1] & 0x08) >> 3;
        k2[i] = ((k[i]&0xe) >> 1) ^ ((k[i] & 0x07) << 1) ^ temp ^ temp2;
    }

    temp = (k[14] & 0x01) << 3;
    temp2 = (k[0] & 0x08) >> 3;
    k2[15] = ((k[15]&0xe) >> 1) ^ ((k[15] & 0x07) << 1) ^ temp ^ temp2;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*SHIFT ROWS OPERATION*/

void shiftRows(word8 *p){

    int i;
    word8 vett[16];

    for(i=0; i<16; i++)
        vett[i] = *(p+i);

    *p = vett[0];
    *(p+1) = vett[5];
    *(p+2) = vett[10];
    *(p+3) = vett[15];
    *(p+4) = vett[4];
    *(p+5) = vett[9];
    *(p+6) = vett[14];
    *(p+7) = vett[3];
    *(p+8) = vett[8];
    *(p+9) = vett[13];
    *(p+10) = vett[2];
    *(p+11) = vett[7];
    *(p+12) = vett[12];
    *(p+13) = vett[1];
    *(p+14) = vett[6];
    *(p+15) = vett[11];

}

/*INVERSE SHIFT ROWS OPERATION*/

void inverseShiftRows(word8 *p){

  int i;
    word8 vett[16];

    for(i=0; i<16; i++)
        vett[i] = *(p+i);

    *p = vett[0];
    *(p+1) = vett[13];
    *(p+2) = vett[10];
    *(p+3) = vett[7];
    *(p+4) = vett[4];
    *(p+5) = vett[1];
    *(p+6) = vett[14];
    *(p+7) = vett[11];
    *(p+8) = vett[8];
    *(p+9) = vett[5];
    *(p+10) = vett[2];
    *(p+11) = vett[15];
    *(p+12) = vett[12];
    *(p+13) = vett[9];
    *(p+14) = vett[6];
    *(p+15) = vett[3];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*MIX COLUMN OPERATION*/

void mixColumn0(word8 *p)
{
    word8 x[4];
    int i;

    for(i=0; i<4;i++)
        x[i]=*(p+i);

    *p = ((x[1]^x[2]^x[3])&0x08) ^ ((x[3]^x[2]^x[0])&0x04) ^ ((x[1]^x[0]^x[3])&0x2) ^ ((x[1]^x[2]^x[0])&0x1);
    *(p+1) = ((x[0]^x[2]^x[1])&0x08) ^ ((x[1]^x[2]^x[3])&0x04) ^ ((x[3]^x[0]^x[2])&0x2) ^ ((x[1]^x[0]^x[3])&0x1);
    *(p+2) = ((x[0]^x[1]^x[3])&0x08) ^ ((x[0]^x[2]^x[1])&0x04) ^ ((x[1]^x[3]^x[2])&0x2) ^ ((x[3]^x[0]^x[2])&0x1);
    *(p+3) = ((x[0]^x[2]^x[3])&0x08) ^ ((x[1]^x[0]^x[3])&0x04) ^ ((x[1]^x[0]^x[2])&0x2) ^ ((x[1]^x[2]^x[3])&0x1);
}

void mixColumn1(word8 *p)
{
    word8 x[4];
    int i;

    for(i=0; i<4;i++)
        x[i]=*(p+i);

    *p = ((x[0]^x[2]^x[1])&0x08) ^ ((x[1]^x[2]^x[3])&0x04) ^ ((x[3]^x[0]^x[2])&0x2) ^ ((x[1]^x[0]^x[3])&0x1);
    *(p+1) = ((x[0]^x[1]^x[3])&0x08) ^ ((x[0]^x[2]^x[1])&0x04) ^ ((x[1]^x[3]^x[2])&0x2) ^ ((x[3]^x[0]^x[2])&0x1);
    *(p+2) = ((x[0]^x[2]^x[3])&0x08) ^ ((x[1]^x[0]^x[3])&0x04) ^ ((x[1]^x[0]^x[2])&0x2) ^ ((x[1]^x[2]^x[3])&0x1);
    *(p+3) = ((x[1]^x[2]^x[3])&0x08) ^ ((x[3]^x[2]^x[0])&0x04) ^ ((x[1]^x[0]^x[3])&0x2) ^ ((x[1]^x[2]^x[0])&0x1);
}

void mixColumn(word8 *stato)
{
    mixColumn0(stato);
    mixColumn1(stato+4);
    mixColumn1(stato+8);
    mixColumn0(stato+12);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*COMPLETE ENCRYPTION*/

void encryption(word8 initialMessage[], word8 initialKey[], word8 initialKey2[], word8 *ciphertext){

  int i, j, differe;

  unsigned char state[16], keyy22[16];

  initialization(&(state[0]), initialMessage);
  //Generation of k_0'
  keyGeneration(initialKey, &(keyy22[0]));

  printf("key - core2\n");
  printtt(keyy22);
  printf("\n");

  //Prince
  addRoundKey(&(state[0]), initialKey);

  //PrinceCore
  addRoundKey(&(state[0]), initialKey2);

  for(i=1;i<N_Round;i++)
  {
      byteSubTransformation(&(state[0]));
      mixColumn(&(state[0]));
      shiftRows(&(state[0]));
      addRoundKey(&(state[0]), initialKey2);
      addCostante(&(state[0]), i);
  }

  //Middle Round
  byteSubTransformation(&(state[0]));
  mixColumn(&(state[0]));
  inverseByteSubTransformation(&(state[0]));

  for(i=N_Round;i<N_Round+5;i++)
  {

      differe = 11 - i;
      addRoundKey(&(state[0]), initialKey2);
      addCostante2(&(state[0]), differe);
      inverseShiftRows(&(state[0]));
      mixColumn(&(state[0]));
      inverseByteSubTransformation(&(state[0]));
  }

  addRoundKey(&(state[0]), initialKey2);
  addCostante2(&(state[0]), 0);
  addRoundKey(&(state[0]), keyy22);

  //Store the encryption Message
  for(i=0; i<16; i++){
     *(ciphertext+i)=state[i];
  }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*ENCRYPTION OF 3 ROUNDS - which are: 1 FORWARD ROUND AND THE MIDDLE ROUND*/

void encryption3(word8 initialMessage[], word8 initialKey[], word8 initialKey2[], word8 *ciphertext)
{

  int i;

  unsigned char state[16], keyy22[16];

  initialization(&(state[0]), initialMessage);
  //Generation of k_0'
  keyGeneration(initialKey, &(keyy22[0]));

  //Prince
  addRoundKey(&(state[0]), initialKey);

  //PrinceCore
  addRoundKey(&(state[0]), initialKey2);

  /*1 Round*/
  byteSubTransformation(&(state[0]));
  mixColumn(&(state[0]));
  shiftRows(&(state[0]));
  addRoundKey(&(state[0]), initialKey2);
  addCostante(&(state[0]), 1);

  //MiddleRound

  byteSubTransformation(&(state[0]));
  mixColumn(&(state[0]));
  inverseByteSubTransformation(&(state[0]));

  //Final AddRound Key
  addRoundKey(&(state[0]), initialKey2);
  addCostante2(&(state[0]), 0);
  addRoundKey(&(state[0]), keyy22);

  //Store the encryption ciphertext
  for(i=0; i<16; i++){
     *(ciphertext+i)=state[i];
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*ATTACK ON 3 ROUNDS USING THE SUBSPACE TRAIL - ATTACK DESCRIBED IN SECT. 4.2*/

unsigned char check(word8 c1, word8 c2, word8 key, word8 alpha, word8 test)
{
    unsigned char value;

    value = byteTransformation(c1 ^ key ^ alpha) ^ byteTransformation(c2 ^ key ^ alpha);

    value = value & test;

    return value;
}

unsigned char check2(word8 moment, word8 c2, word8 key, word8 alpha, word8 test)
{
    unsigned char value;

    value = moment ^ byteTransformation(c2 ^ key ^ alpha);

    value = value & test;

    return value;
}

void attack3TruncatedAttack(unsigned char initialKey[16], unsigned char initialKey2[16], int Ntest)
{
    word8 p[Ntest][16], c[Ntest][16], keyy22[16], test[16], test2[16], state[Ntest];

    word8 indice1, indice, indice3[16], indice0[16], flag0, indice2[16], values, flag = 0, moment;

    int i, j, k, cont , numeroSBOX = 0;

    //Generation of k_0'
    keyGeneration(initialKey, &(keyy22[0]));

    /*Generation of the chosen plaintexts in the same coset of U*/
    for(j=1;j<16;j++)
    {
       p[0][j] = randomNibble();

       for(i=1;i<Ntest;i++)
       {
           p[i][j] = p[0][j];
       }
    }
    for(i=0;i<Ntest;i++)
    {
        p[i][0] = (word8) i;
    }

    /*Encryption of the chosen plaintexts*/
    for(i=0;i<Ntest;i++)
    {
        for(j=0;j<16;j++)
            test[j] = p[i][j];

        encryption3(test, initialKey, initialKey2, &(test2[0]));

        for(j=0;j<16;j++)
            c[i][j] = test2[j];
    }


    /*ATTACK*/
    printf("FIRST Nibble\n");
    //Guess nibble in position 1st
    for(i=0;i<16;i++)
    {
        indice1 = (word8) i;
        values = byteTransformation(c[0][0] ^ indice1 ^ alpha[0] ^ constants[0][0]);
        numeroSBOX++;

        flag = 1;

        for(j = 1; ((j<Ntest)&&(flag == 1)); j++)
        {

            moment = values ^ byteTransformation(c[j][0] ^ indice1 ^ alpha[0]  ^ constants[0][0]);
            numeroSBOX++;

            if((moment & 0x8) != 0x0)
            {
                flag = 0;
            }
        }

        if(flag == 1)
        {
            printf("\tCorrect values: %x - Found Values: %x", (initialKey2[0] ^ keyy22[0]), indice1);
            if((initialKey2[0] ^ keyy22[0])==indice1)
                printf(" - RIGHT NIBBLE\n");
            else
                printf("\n");
        }
    }

    /*ATTACK*/
    printf("SECOND Nibble\n");
    //Guess nibble in position 1st
    for(i=0;i<16;i++)
    {
        indice1 = (word8) i;
        values = byteTransformation(c[0][1] ^ indice1 ^ alpha[1] ^ constants[0][1]);
        numeroSBOX++;

        flag = 1;

        for(j = 1; ((j<Ntest)&&(flag == 1)); j++)
        {

            moment = values ^ byteTransformation(c[j][1] ^ indice1 ^ alpha[1]  ^ constants[0][1]);
            numeroSBOX++;

            if((moment & 0x4) != 0x0)
            {
                flag = 0;
            }
        }

        if(flag == 1)
        {
            printf("\tCorrect values: %x - Found Values: %x", (initialKey2[1] ^ keyy22[1]), indice1);
            if((initialKey2[1] ^ keyy22[1])==indice1)
                printf(" - RIGHT NIBBLE\n");
            else
                printf("\n");
        }
    }

    /*ATTACK*/
    printf("THIRD Nibble\n");
    //Guess nibble in position 1st
    for(i=0;i<16;i++)
    {
        indice1 = (word8) i;
        values = byteTransformation(c[0][2] ^ indice1 ^ alpha[2] ^ constants[0][2]);
        numeroSBOX++;

        flag = 1;

        for(j = 1; ((j<Ntest)&&(flag == 1)); j++)
        {

            moment = values ^ byteTransformation(c[j][2] ^ indice1 ^ alpha[2]  ^ constants[0][2]);
            numeroSBOX++;

            if((moment & 0x2) != 0x0)
            {
                flag = 0;
            }
        }

        if(flag == 1)
        {
            printf("\tCorrect values: %x - Found Values: %x", (initialKey2[2] ^ keyy22[2]), indice1);
            if((initialKey2[2] ^ keyy22[2])==indice1)
                printf(" - RIGHT NIBBLE\n");
            else
                printf("\n");
        }
    }

    /*ATTACK*/
    printf("FOURTH Nibble\n");
    //Guess nibble in position 1st
    for(i=0;i<16;i++)
    {
        indice1 = (word8) i;
        values = byteTransformation(c[0][3] ^ indice1 ^ alpha[3] ^ constants[0][3]);
        numeroSBOX++;

        flag = 1;

        for(j = 1; ((j<Ntest)&&(flag == 1)); j++)
        {

            moment = values ^ byteTransformation(c[j][3] ^ indice1 ^ alpha[3]  ^ constants[0][3]);
            numeroSBOX++;

            if((moment & 0x1) != 0x0)
            {
                flag = 0;
            }
        }

        if(flag == 1)
        {
            printf("\tCorrect values: %x - Found Values: %x", (initialKey2[3] ^ keyy22[3]), indice1);
            if((initialKey2[3] ^ keyy22[3])==indice1)
                printf(" - RIGHT NIBBLE\n");
            else
                printf("\n");
        }
    }


    printf("\nThe total number of S-Box look-ups is %d.\n", numeroSBOX);
    printf("The theoretical value for this step is of 2^(7.9)=240 S-Box look-ups for 9 CP.\n\n");
    printf("(This program is not optimized, using the way described in the paper. The missing ");
    printf("optimization is the following: if x satisfies S-Box(x+a)+S-Box(x)=b, then also x+a satisfies it.)\n");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*This program attack 3 rounds of Prince using the attack described in App. D.1, that is the subspace trail attack.

The user can choose:
- key of the FX construction, that is k_0 in the paper;
- key of Prince Core, that is k_1;
- number of chosen plaintexts used for the attack (we advise to use 5) - maximum values = 16;
- number of test - "numberTest" (see at the beginning).

The output of the program is:
- one column of k_0' \oplus k_1 (which is the XoR of the keys of the final round)- the real values and the found ones.

In this program we work only a single column.
However, we think it is sufficient to verify our theory.
Indeed, completely in the same way, the program can find the other column of k_0' \oplus k_1 and, with other 5 plaintexts,
the key of k_1.
*/

int main()
{
    //Key of FX construction - k_0
    const unsigned char FXkey[16] = {
        0x2, 0x7, 0xe, 0xd, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xb, 0x0, 0x0
    };

    //Key of PRINCE CORE - k_1
    const unsigned char KeyPrinceCore[16] = {
       0x6, 0x7, 0x4, 0xc, 0x8, 0xb, 0xd, 0x7, 0xc, 0x9, 0x2, 0xc, 0x4, 0x4, 0x2, 0xd
    };

    //Number of chosen plaintexts generate by the program and used for the attack
    int numberPlaintext = 9;

    unsigned char finalKey[16], finalKey1[16];
    int i;

    //We generate the key of the final round of the FX construction - k_0'
    keyGeneration(FXkey, &(finalKey[0]));

    printf("TRUNCATED DIFFERENTIAL ATTACK on 3 ROUNDS of PRINCE WORKING INDEPENDENT ON EACH NIBBLE (App. D.1).\n\n");
    printf("The goal of this attack is to find the first column (4 nibbles) of the key of the final round, defined as k_0' \oplus k_1.\n");
    printf("The number of chosen plaintexts used for the attack is %d.\n\n", numberPlaintext);

    for(i = 0; i<16; i++)
        finalKey1[i] = finalKey[i] ^ KeyPrinceCore[i];

    printf("The (right) Key of the Final round (k_0' \oplus k_1) is:\n");
    printtt(finalKey1);
    printf("\n\n");

    //We're looking for 4 nibbles of k_0' \oplus k_1.

    //generation seed - For random generator Test
    srand(time(NULL));

    //Attack
    for(i=0;i<numberTest;i++)
    {
        printf("RESULT of the TEST NUMBER %d.\n", i);
        attack3TruncatedAttack(FXkey, KeyPrinceCore, numberPlaintext);
        printf("\n");
    }

    return 0;
}

