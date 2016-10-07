#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_Round 6

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

const unsigned char constante[6][16] = {
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

word8 randomNibble(){
  return (word8) randomInRange(0, 15);
}

int logaritm2base(int a)
{
    int i, n = 0, b=1;

    if(a==0)
        return -1;
    if(a==1)
        return 0;

    do{
        if(b<a)
        {
            n++;
            b = b*2;
        }
    }while(b<a);

    return n;

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

/*ADD ROUND KEY OPERATIONT*/

void addRoundKey(word8 *p, word8 key[]){

  int i;

  for(i=0; i<16; i++){
    *(p+i) ^= key[i];
  }

}

void addRoundKeyMod(word8 *p, word8 key[]){

  int i;

  for(i=0; i<4; i++){
    *(p+i) ^= key[i];
  }

}

/*ADD ROUND CONSTANT*/

void addCostante(word8 *p, int round){

  int i;

  for(i=0; i<16; i++){
    *(p+i) ^= constante[round][i];
  }

}

/*ADD ROUND CONSTANT and ALPHA*/

void addCostante2(word8 *p, int round){

  int i;

  for(i=0; i<16; i++){
    *(p+i) ^= constante[round][i];
    *(p+i) ^= alpha[i];
  }

}

void addCostante2Mod(word8 *p, int round){

  int i;

  for(i=0; i<4; i++){
    *(p+i) ^= constante[round][i];
    *(p+i) ^= alpha[i];
  }

}

void addCostante3Mod(word8 *p, int round){

  int i;

  for(i=0; i<4; i++){
    *(p+i) ^= constante[round][i+4];
    *(p+i) ^= alpha[i+4];
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*PRINT THE VALUES*/

void printtt(word8 file[]){

    int i;

    for(i=0; i<16; i++)
    {
    printf("0x%x", file[i]);
    if(i<15)
        printf(", ");
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Byte sub transformation with S-box*/

word8 byteTransformation(word8 byte){

  return sBox[byte];

}

/*Inverse byte sub transformation with Inverse S-box*/

word8 inverseByteTransformation(word8 byte){

  return inv_s[byte];

}

/*S-BOX TRANSFORMATION*/

void byteSubTransformation(word8 *puntatore){

  int i;

  for(i=0; i<16; i++){
     *(puntatore+i)=byteTransformation(*(puntatore+i));
  }
}

void byteSubTransformationMod(word8 *puntatore){

  int i;

  for(i=0; i<4; i++){
     *(puntatore+i)=byteTransformation(*(puntatore+i));
  }
}

/*INVERSE S-BOX TRANSFORMATION*/

void inverseByteSubTransformation(word8 *puntatore){

  int i;

  for(i=0; i<16; i++){
     *(puntatore+i)=inverseByteTransformation(*(puntatore+i));
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*GENERATION OF THE FINAL KEY OF THE FX CONSTRUCTION*/

void generazioneKey(word8 k[], word8 *k2)
{
    int i;
    word8 temp, temp2;

    for(i=0; i<14; i++)
    {
        k2[i] = (k[i] >> 1) ^  ((k[i+1] & 0x01) << 3);
    }

    k2[0] ^= ((k[15] & 0x08) >> 3);

    k2[15] = (k[15] >> 1) ^  ((k[0] & 0x01) << 3);

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

/*COMPLETE ENCRYPTION */

void encryption(word8 initialMessage[], word8 initialKey[], word8 initialKey2[], word8 *ciphertext){

  int i, j, differe;

  unsigned char state[16], keyy22[16];

  initialization(&(state[0]), initialMessage);
  keyGeneration(initialKey, &(keyy22[0]));

  printf("chiave - core2\n");
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

  //salvo messaggio cifrato
  for(i=0; i<16; i++){
     *(ciphertext+i)=state[i];
  }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*4 ROUND ENCRYPTION PRINCE*/

void encryption4(word8 initialMessage[], word8 initialKey[], word8 initialKey2[], word8 *ciphertext)
{

  int i;

  unsigned char state[16], keyy22[16];

  initialization(&(state[0]), initialMessage);
  //Generation of key of last round of FX construction
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

  //Middle Rounds
  byteSubTransformation(&(state[0]));
  mixColumn(&(state[0]));
  inverseByteSubTransformation(&(state[0]));

  /*1 round*/
  addRoundKey(&(state[0]), initialKey2);
  addCostante2(&(state[0]), 1);
  inverseShiftRows(&(state[0]));
  mixColumn(&(state[0]));
  inverseByteSubTransformation(&(state[0]));

  //Final AddRound Key
  addRoundKey(&(state[0]), initialKey2);
  addCostante2(&(state[0]), 0);
  addRoundKey(&(state[0]), keyy22);

  //Store the Encryption Message
  for(i=0; i<16; i++){
     *(ciphertext+i)=state[i];
  }

}


void encryption3Not(word8 initialMessage[], word8 initialKey[], word8 initialKey2[], word8 *ciphertext)
{

  int i;

  unsigned char state[16], keyy22[16];

  initialization(&(state[0]), initialMessage);
  //Generation of key of last round of FX construction
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

  //Middle Rounds
  byteSubTransformation(&(state[0]));
  mixColumn(&(state[0]));
  /*inverseByteSubTransformation(&(state[0]));

  /*1 round*/
  /*addRoundKey(&(state[0]), initialKey2);
  addCostante2(&(state[0]), 1);
  /*inverseShiftRows(&(state[0]));
  mixColumn(&(state[0]));
  inverseByteSubTransformation(&(state[0]));

  //Final AddRound Key
  addRoundKey(&(state[0]), initialKey2);
  addCostante2(&(state[0]), 0);
  addRoundKey(&(state[0]), keyy22);*/

  //Store the Encryption Message
  for(i=0; i<16; i++){
     *(ciphertext+i)=state[i];
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*ATTACK ON 4 ROUNDS described in SECT. 5 - Extension at the END */

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**Second part of the attack. The attacker has already discovered one column (the first one) of the final key, and four
nibbles of the key of PRINCEcore - one for each column. Now the attacker uses this knowledge and the subspace attack on
3 rounds to discover the second column of the final key, and other four nibbles of the key of PRINCEcore. */

int attack4roundsSubAttackPart2(unsigned char initialKey[16], unsigned char initialKey2[16], int nTest, unsigned char *kfinale, unsigned char k1, unsigned char k2, unsigned char k3, unsigned char k4, int ssboox)
{
    word8 p[nTest][16], c[nTest][16], keyy22[16], test[16], test2[16], stato[nTest][4], testkey[4], decifrato[nTest][4];

    word8 indice, indice1[16], flag, indice2[16], values, indice3[16], indice4[16];

    int i, j, k, l, prova, cont, numeroSBOX = ssboox;

    /*Generation of the last key for the FX construction*/
    keyGeneration(initialKey, &(keyy22[0]));

    //generation of the seed
    srand (time(NULL));

    /*Generation of the requested chosen plaintexts - Remember: they belong to the same coset of W*/
    /*Note: the attacker has to use the same texts used for the first part of the attack!*/
    for(j=4;j<16;j++)
    {
       p[0][j] = randomNibble();

       for(i=1;i<nTest;i++)
       {
           p[i][j] = p[0][j];
       }
    }
    for(j=0;j<4;j++)
    {
        for(i=0;i<nTest;i++)
        {
            p[i][j] = randomNibble();
        }
    }

    //4 Rounds Encryption
    for(i=0;i<nTest;i++)
    {
        for(j=0;j<16;j++)
            test[j] = p[i][j];

        encryption4(test, initialKey, initialKey2, &(test2[0]));

        for(j=0;j<16;j++)
        {
            c[i][j] = test2[j];
        }

    }

    //The attacker uses the knowns nibble of the keys to obtain the four values of the texts after 2.5 rounds.
    for(l=0;l<nTest;l++)
    {
        for(k=0;k<4;k++)
        {
            test[k] = c[l][k];
            testkey[k] = *(kfinale + k);
        }

        addRoundKeyMod(&(test[0]), testkey);
        addCostante2Mod(&(test[0]), 0);
        byteSubTransformationMod(&(test[0]));
        mixColumn0(&(test[0]));

        test[0] = byteTransformation(test[0] ^ k1 ^ constante[1][0] ^ alpha[0]);
        test[1] = byteTransformation(test[1] ^ k2 ^ constante[1][13] ^ alpha[13]);
        test[2] = byteTransformation(test[2] ^ k3 ^ constante[1][10] ^ alpha[10]);
        test[3] = byteTransformation(test[3] ^ k4 ^ constante[1][7] ^ alpha[7]);


        for(k=0;k<4;k++)
        {
            decifrato[l][k] = test[k];
        }
    }

    //Compute the difference
    for(l=1;l<nTest;l++)
    {
        for(k=0;k<4;k++)
        {
            decifrato[l][k] ^= decifrato[0][k];
        }
    }

    //ATTACK - SUBSPACE ATTACK
    /*Guess all the key: (2^{16})^4*/
    for(i=0; i<65536; i++)
    {
        testkey[0] = (word8) ((int)((i - (i % 4096))/4096));
        prova = i - 4096 * ((int) testkey[0]);
        testkey[1] = (word8) ((int)((prova  - (prova  % 256))/256));
        prova = prova - 256 * ((int)testkey[1]);
        testkey[2] = (word8) ((int)((prova  - (prova % 16))/16));
        testkey[3] = (word8) (i%16);

        //Decryption of 1 round
        for(l=0;l<nTest;l++)
        {
            for(k=4;k<8;k++)
            {
                test[k-4] = c[l][k];
            }

            addRoundKeyMod(&(test[0]), testkey);
            addCostante3Mod(&(test[0]), 0);
            byteSubTransformationMod(&(test[0]));
            numeroSBOX = numeroSBOX + 4;
            mixColumn1(&(test[0]));

            for(k=0;k<4;k++)
                stato[l][k] = test[k];
        }

        //Inizialization
        for(l = 0; l<16;l++)
        {
            indice1[l]=0xff;
            indice2[l]=0xff;
            indice3[l]=0xff;
            indice4[l]=0xff;
        }

        //Attack nibble in position 1st of key of PrinceCore
        cont = 0;
        for(j=0;j<16;j++)
        {
            indice = (word8) j;

            k = 1;
            values = 0x7;

            do
            {
                flag = check(stato[0][1], stato[k][1], indice, (constante[1][1]^alpha[1]), values);
                numeroSBOX = numeroSBOX + 2;
                flag ^= ((decifrato[k][0])&0xb);
                k++;

            }while((flag==0)&&(k<nTest));

            if(flag == 0)
            {
                indice1[cont] = indice;
                cont++;
            }
        }

        //Attack nibble in position 14th of key of PrinceCore
        cont = 0;
        for(j=0;((j<16)&&(((int) indice1[0])<16));j++)
        {
            indice = (word8) j;

            k = 1;
            values = 0xd;

            do
            {
                flag = check(stato[0][2], stato[k][2], indice, (constante[1][14]^alpha[14]), values);
                numeroSBOX = numeroSBOX + 2;
                flag ^= ((decifrato[k][1])&0xe);
                k++;

            }while((flag==0)&&(k<nTest));
            if(flag == 0)
            {
                indice2[cont] = indice;
                cont++;
            }
        }

        //Attack nibble in position 11th of key of PrinceCore
        cont = 0;
        for(j=0;((j<16)&&(((int) indice1[0])<16)&&(((int)indice2[0])<16));j++)
        {
            indice = (word8) j;
            k = 1;
            values = 0xb;

            do
            {
                flag = check(stato[0][3], stato[k][3], indice, (constante[1][11]^alpha[11]), values);
                numeroSBOX = numeroSBOX + 2;
                flag ^= (decifrato[k][2]&0xd);
                k++;

            }while((flag==0)&&(k<nTest));
            if(flag == 0)
            {
                indice3[cont] = indice;
                cont++;
            }
        }

        //Attack nibble in position 7th of key of PrinceCore
        cont  = 0;
        for(j=0;((j<16)&&(((int) indice1[0])<16)&&(((int)indice2[0])<16)&&(((int)indice3[0])<16));j++)
        {
            indice = (word8) j;
            k = 1;
            values = 0xe;

            do
            {
                flag = check(stato[0][0], stato[k][0], indice, (constante[1][4]^alpha[4]), values);
                numeroSBOX = numeroSBOX + 2;
                flag ^= (decifrato[k][3]&0x7);
                k++;

            }while((flag==0)&&(k<nTest));
            if(flag == 0)
            {
                indice4[cont] = indice;
                cont++;
            }
        }

        //If a (possible) right key is found, then we print it!
        if((((int) indice1[0])<16)&&(((int)indice2[0])<16)&&(((int)indice3[0])<16)&&(((int)indice4[0])<16))
        {

            printf("LAST KEY of PRINCE\n");
            printf("Nibbles: 0th - 1st - 2nd - 3th - First column\n");
            printf("Real Value:\n");
            printf("\t0x%x - 0x%x - 0x%x - 0x%x\n", (keyy22[0]^initialKey2[0]), (keyy22[1]^initialKey2[1]), (keyy22[2]^initialKey2[2]), (keyy22[3]^initialKey2[3]));
            printf("Found Values:\n");
            printf("\t0x%x - 0x%x - 0x%x - 0x%x\n\n", *(kfinale), *(kfinale + 1), *(kfinale + 2), *(kfinale + 3));

            printf("Nibbles: 4th - 5th - 6th - 7th - Second column\n");
            printf("Real Value:\n");
            printf("\t0x%x - 0x%x - 0x%x - 0x%x\n", (keyy22[4]^initialKey2[4]), (keyy22[5]^initialKey2[5]), (keyy22[6]^initialKey2[6]), (keyy22[7]^initialKey2[7]));
            printf("Found Values:\n");
            printf("\t0x%x - 0x%x - 0x%x - 0x%x\n\n", testkey[0], testkey[1], testkey[2], testkey[3]);

            //Key of PRINCE core
            printf("KEY of PRINCE CORE\n");
            printf("0th Nibble\n");
            cont = 0;
            printf("\tReal value: 0x%x - Found Values: 0x%x\n", initialKey2[0], k1);
            printf("1rs Nibble\n");
            while((cont<16)&&(((int) indice1[cont])<16))
            {
                printf("\tReal value: 0x%x - Found Values: 0x%x\n", initialKey2[1], indice1[cont]);
                cont++;
            }


            printf("4th Nibble\n");
            cont = 0;
            while((cont<16)&&(((int) indice4[cont])<16))
            {
                printf("\tReal value: 0x%x - Found Values: 0x%x\n", initialKey2[4], indice4[cont]);
                cont++;
            }
            printf("7th Nibble\n");
            printf("\tReal value: 0x%x - Found Values: 0x%x\n", initialKey2[7], k4);

            printf("10th Nibble\n");
            cont = 0;
            printf("\tReal value: 0x%x - Found Values: 0x%x\n", initialKey2[10], k3);
            printf("11th Nibble\n");
            while((cont<16)&&(((int) indice3[cont])<16))
            {
                printf("\tReal value: 0x%x - Found Values: 0x%x\n", initialKey2[11], indice3[cont]);
                cont++;
            }

            printf("13th Nibble\n");
            cont = 0;
            printf("\tReal value: 0x%x - Found Values: 0x%x\n", initialKey2[13], k2);
            printf("14th Nibble\n");
            while((cont<16)&&(((int) indice2[cont])<16))
            {
                printf("\tReal value: 0x%x - Found Values: 0x%x\n\n", initialKey2[14], indice2[cont]);
                cont++;
            }
        }
    }

    return numeroSBOX;

    /*printf("\nThe total number of S-Box look-ups is %d.\n", numeroSBOX);
    printf("The theoretical value for this step is of 2^(16)*[2^(6.09) + 2^5*2 + 2^(7.46)*2]=29670258 S-Box look-ups for 17 CP.\n\n");
    printf("(The term *2 is due to the fact that this program is not optimized, using the way described in the paper. The missing ");
    printf("optimization is the following: if x satisfies S-Box(x+a)+S-Box(x)=b, then also x+a satisfies it).\n");*/
}

//******************************************************************************************************************************

/**First part of the attack on 4 rounds: the attacker finds the first column of the final key, and 4 nibbles of the key of
PRINCEcore using the truncated differential attack. When one possible key is found, then the attacker uses the subspace
attack to find other nibbles of the key of PRINCE.*/

void attack4roundsTruncatedDifferential(unsigned char initialKey[16], unsigned char initialKey2[16], int nTest, int nTest2)
{
    word8 p[nTest][16], valueAttack[nTest][16], c[nTest][16], keyy22[16], test[16], test2[16], stato[nTest][4], testkey[4];

    word8 indice, indice1[16], flag, indice2[16], values, indice3[16], indice4[16], flag2, flag3;

    int i, j, k, l, prova, cont, numeroSBOX = 0, cont2, cont3, cont4, exppp;

    /*Generation of the last key for the FX construction*/
    keyGeneration(initialKey, &(keyy22[0]));

    //generation of the seed
    srand (time(NULL));

    /*Generation of the requested chosen plaintexts - Remember: they belong to the same coset of W*/
    for(j=4;j<16;j++)
    {
       p[0][j] = randomNibble();

       for(i=1;i<nTest;i++)
       {
           p[i][j] = p[0][j];
       }
    }
    for(j=0;j<4;j++)
    {
        for(i=0;i<nTest;i++)
        {
            p[i][j] = randomNibble();
        }
    }

    //4 Rounds Encryption
    for(i=0;i<nTest;i++)
    {
        for(j=0;j<16;j++)
            test[j] = p[i][j];

        encryption4(test, initialKey, initialKey2, &(test2[0]));

        for(j=0;j<16;j++)
        {
            c[i][j] = test2[j];
        }
    }


    //ATTACK
    /*Guess all the key: (2^{16})^4*/
    for(i=0; i<65536; i++)
    {
        testkey[0] = (word8) ((int)((i - (i % 4096))/4096));
        prova = i - 4096 * ((int) testkey[0]);
        testkey[1] = (word8) ((int)((prova  - (prova  % 256))/256));
        prova = prova - 256 * ((int)testkey[1]);
        testkey[2] = (word8) ((int)((prova  - (prova % 16))/16));
        testkey[3] = (word8) (i%16);

        //Decryption of 1 round
        for(l=0;l<nTest;l++)
        {
            for(k=0;k<4;k++)
            {
                test[k] = c[l][k];
            }

            addRoundKeyMod(&(test[0]), testkey);
            addCostante2Mod(&(test[0]), 0);
            byteSubTransformationMod(&(test[0]));
            numeroSBOX = numeroSBOX + 4;
            mixColumn0(&(test[0]));

            for(k=0;k<4;k++)
                stato[l][k] = test[k];
        }

        //Inizialization
        for(l = 0; l<16;l++)
        {
            indice1[l]=0xff;
            indice2[l]=0xff;
            indice3[l]=0xff;
            indice4[l]=0xff;
        }

        //Attack nibble in position 0th of key of PrinceCore
        cont = 0;
        for(j=0;j<16;j++)
        {
            indice = (word8) j;

            k = 1;
            values = 0x8;

            do
            {
                flag = check(stato[0][0], stato[k][0], indice, (constante[1][0]^alpha[0]), values);
                numeroSBOX = numeroSBOX + 2;
                k++;

            }while((flag==0)&&(k<nTest));

            if(flag == 0)
            {
                indice1[cont] = indice;
                cont++;
            }
        }

        //Attack nibble in position 13rd of key of PrinceCore
        cont = 0;
        for(j=0;((j<16)&&(((int) indice1[0])<16));j++)
        {
            indice = (word8) j;

            k = 1;
            values = 0x2;

            do
            {
                flag = check(stato[0][1], stato[k][1], indice, (constante[1][13]^alpha[13]), values);
                numeroSBOX = numeroSBOX + 2;
                k++;

            }while((flag==0)&&(k<nTest));
            if(flag == 0)
            {
                indice2[cont] = indice;
                cont++;
            }
        }

        //Attack nibble in position 10th of key of PrinceCore
        cont = 0;
        for(j=0;((j<16)&&(((int) indice2[0])<16));j++)
        {
            indice = (word8) j;
            k = 1;
            values = 0x4;

            do
            {
                flag = check(stato[0][2], stato[k][2], indice, (constante[1][10]^alpha[10]), values);
                numeroSBOX = numeroSBOX + 2;
                k++;

            }while((flag==0)&&(k<nTest));
            if(flag == 0)
            {
                indice3[cont] = indice;
                cont++;
            }
        }

        //Attack nibble in position 7th of key of PrinceCore
        cont  = 0;
        for(j=0;((j<16)&&(((int) indice3[0])<16));j++)
        {
            indice = (word8) j;
            k = 1;
            values = 0x1;

            do
            {
                flag = check(stato[0][3], stato[k][3], indice, (constante[1][7]^alpha[7]), values);
                numeroSBOX = numeroSBOX + 2;
                k++;

            }while((flag==0)&&(k<nTest));
            if(flag == 0)
            {
                indice4[cont] = indice;
                cont++;
            }
        }

        cont = 0;
        cont2 = 0;
        cont3 = 0;
        cont4 = 0;

        /**If a (possible) right key of the final round is found (that is the first column and four nibbles),
        then we use it and a subspace attack to find the second column and other four nibbles*/
        if((((int) indice1[0])<16)&&(((int)indice2[0])<16)&&(((int)indice3[0])<16)&&(((int)indice4[0])<16))
        {
            do
            {
                numeroSBOX = attack4roundsSubAttackPart2(initialKey, initialKey2, nTest2, &(testkey[0]), indice1[cont], indice2[cont2], indice3[cont3], indice4[cont4], numeroSBOX);
                cont++;
                if(indice1[cont] == 0xff)
                {
                    cont = 0;
                    cont2++;
                    if(indice2[cont2] == 0xff)
                    {
                        cont2 = 0;
                        cont3++;
                        if(indice3[cont3] == 0xff)
                        {
                            cont3 = 0;
                            cont4++;
                        }
                    }
                }
            }while((int) indice4[cont4] < 16);
        }
    }

    exppp = logaritm2base(numeroSBOX);

    printf("\nThe total number of S-Box look-ups is %d, where 2^%d<%d<2^%d.\n", numeroSBOX, exppp-1, numeroSBOX, exppp);
    printf("The theoretical value for this step is of approximately 94906265=2^26.5 S-Box look-ups for 8 CP.\n\n");
    printf("(The program is not optimized in the way described in the paper. The missing ");
    printf("optimization is the following: if x satisfies S-Box(x+a)+S-Box(x)=b, then also x+a satisfies it).\n");
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*This program attack 4 rounds of Prince using the attack described in Sect. 5, that is the truncated differential attack
with the extension at the end.

The user can choose:
- key of the FX construction, that is k_0 in the paper;
- key of Prince Core, that is k_1;
- number of chosen plaintexts used for the attack (we advise to use 12 for the first part and 6 for the second, or 8 for both).

The output of the program is:
- one column of k_0' \oplus k_1 (which is the XoR of the keys of the final round)- the real values and the found ones;
- four nibble of k_1.

In this program we work only a single column.
However, we think it is sufficient to verify our theory.
*/

int main()
{
    //Key of FX construction
    const unsigned char FXkey[16] = {
        0x2, 0x7, 0xe, 0xd, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xb, 0x0, 0x0
    };

    //Key of PRINCE CORE
    const unsigned char KeyPrinceCore[16] = {
       0x6, 0x7, 0x4, 0xc, 0x8, 0xb, 0xd, 0xa, 0xc, 0x9, 0x2, 0xc, 0x4, 0x4, 0x2, 0xd
    };

    //Number of chosen plaintexts used for the attack
    int numberPlaintext = 8; //12 or 8
    int numberPlaintext2 = 8; //6 or 8

    unsigned char finalKey[16], finalKey1[16];
    int i;

    printf("TRUNCATED DIFFERENTIAL ATTACK on 4 ROUNDS of PRINCE - EXTENSION at the END (Sect. 5).\n\n");
    printf("The goal of this attack is to find the first and the second columns (8 nibbles) of the key of the final round, ");
    printf("defined as k_0' \oplus k_1, and eight nibbles (two for each column) of the key of PRINCEcore k_1.\n");
    printf("The attack is composed of two parts:\n");
    printf("- the first part exploits the TRUNCATED differential attack - working independent on the nibble;\n");
    printf("- the second part exploit the TRUNCATED differential attack - exploiting the relationships among the nibble.\n");
    printf("The number of chosen plaintexts used for the attack is %d (%d for the second part).\n\n", numberPlaintext, numberPlaintext2);
    //We generate the key of the final round of the FX construction, that is k_0'
    keyGeneration(FXkey, &(finalKey[0]));

    for(i = 0; i<16; i++)
        finalKey1[i] = finalKey[i] ^ KeyPrinceCore[i];

    printf("The (right) Key of the Final round (k_0' \oplus k_1) is:\n");
    printtt(finalKey1);
    printf("\n\n");

    /*We're looking for 4 nibbles of the key of Prince core and of the final key (attack used: truncated differential).
    When they are found, the other 4 nibbles are found using the subspace attack.*/

    //First and Second columns Guess
    attack4roundsTruncatedDifferential(FXkey, KeyPrinceCore, numberPlaintext, numberPlaintext2);

   return 0;
}
