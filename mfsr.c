/* Author: Dominic van der Zypen
 * Goal: Majority Feedback Shift Register: instead of XORing
 * fixed taps as in LFSR (linear feedback shift registers), 
 * do a "majority voting" for an odd number of taps. If 
 * majority is 0, output 1, and vice versa.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>   //  --> for using uint16_t

char popcount(uint16_t x);
char majority_vote(uint64_t x, char* taps, char num_of_taps);
void mfsr(uint16_t* x, char* taps, char num_of_taps);
//--------------------
char popcount(uint16_t x)
  // counts number of 1's in binary representation of x
{
  char count = 0;
  while (x)
  {
    if (x & 1) {count++;}
    x = x >> 1;
  }
  return count;
}
//--------------------
char majority_vote(uint64_t x, char* taps, char num_of_taps)
  // taps need to be distinct bit positions \in \{0, ... , 15\}
  // num_of_taps needs to be odd, so we get no ties.
  // return 1 if the 0's have majority, 0 otherwise
{
  uint16_t mask = 0;

  // build mask: 1 at the taps positions, 0 otherwise

  char i = 0;
  while (i < num_of_taps)
  {
    mask = mask | (1 << *(taps+i));
    i++;
  }
  char majority1 = ((popcount(mask & x) << 1) > num_of_taps);
    // majority1 == 1 if and only if there are more 1 than 0's in masked x
  return 1 - majority1;
}
//--------------------
void mfsr(uint16_t* up, char* taps, char num_of_taps)
  // new bit is determined with majority rule and then inserted after shift
{
  char my_new_bit = majority_vote(*up, taps, num_of_taps);
  *up = (my_new_bit << 15) | ((*up) >> 1); 
          // ... << 15 -> put new bit at left end!
}
//--------------------
void print_uint16_bin(uint16_t x)
{
  char i = 0;
  char my_bit = 0;
  while (i < 16)
  {
    my_bit = (x >> 15) & 1;
    printf("%d", my_bit);
    x = x << 1;
    i++;
    if (!(i & 3)) {printf(" ");} // formatting; "& 3" is "modulo 4 != 0
  }
  printf("\n");
}
//--------------------
int main()
{
  char* taps = (char*)malloc(sizeof(char) * 3);
  *taps = 0; *(taps + 1) = 4; *(taps + 2) = 7; // example taps (3 in total)
  char i = 0;
  uint16_t x = 0x95c1; // 4 nibbles -> 16 bit
  while (i < 20)
  {
    print_uint16_bin(x);
    mfsr(&x, taps, 3);
    i++;
  }
  return 0;
}
