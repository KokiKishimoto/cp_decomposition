#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib> 
#include <set>
#include <stdio.h>
#include <string.h>

int vocab_hash_size = 200;
int GetWordHash(char *word) {
  unsigned long long a, hash = 0;
  for (a = 0; a < strlen(word); a++){
      hash = hash * 257 + word[a];
      printf("%d\n",word[a]);
  }
  //std::cout<< word[a] <<std::endl;
  std::cout<< 5 <<std::endl;
  hash = hash % vocab_hash_size;
  return hash;
}

int main(void){
    char *word = "aiueo";
    int a = GetWordHash(word);
    //std::cout<< a <<std::endl;
    char *b = "aiueo";
    char *c = "aiueo";
    printf("aiueo:%d",!strcmp(b,c));
    return -1;

}

