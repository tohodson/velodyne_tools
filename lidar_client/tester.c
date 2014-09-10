

#include <stdio.h>
#include <stdint.h>

int total = 0;
int count = 0;
uint16_t val;

void foo(FILE*);

int main() {
  
  FILE *f = fopen("dump","r");
  
  for (int i = 0; i<1000; i++) {
    foo(f);
  }

  
  
  printf("%d / %d", total, count);
  return 1;
}


void foo(FILE* f) {


  for (int i = 0; i < 12; i++) {
    if ( fseek(f, 100, SEEK_CUR) < 0) break;
    
    if (fread(&val, sizeof(uint16_t), 1, f) < 1) break;
    
    if (val != 61183) count++;
    
    total++;
  }
  fseek(f, 6, SEEK_CUR);

}
