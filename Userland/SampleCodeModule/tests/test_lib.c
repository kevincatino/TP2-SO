#include <test_util.h>
#include <libc.h>
#include <test_lib.h>

#define MAX_MEMORY 134217728

uint64_t test_mm(uint64_t argc, char *argv[]) {

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;


  if (argc != 1) return -1;

  max_memory = (uint64_t) (satoi(argv[0]))*(MAX_MEMORY)/100;

  if ((max_memory) <= 0){
    print("Memory value is invalid\n");
     return -1;
  }

  print("Testing memory manager with max size = ");
  print(argv[0]);
  print("\%\n");
  // return;
  // while (1){
    rq = 0;
    total = 0;

    // Request as many blocks as we can

    while(rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      void * aux = sys_alloc(mm_rqs[rq].size);
       mm_rqs[rq].address = aux;

      if(mm_rqs[rq].address){
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)){
          print("test_mm ERROR\n");
          return -1;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        sys_free(mm_rqs[i].address);

  // } 
      print("Tests passed!\n");
}
