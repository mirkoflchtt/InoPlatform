#include <stdio.h>
#include "ino_rbuffer.h"

int main(int argc, char* argv[])
{
#ifdef HAS_INO_BUILTIN_TESTS
  printf("  [ino_builtin_tests_ino_rbuffer] : ");
  if (0==ino_builtin_tests_ino_rbuffer(argc, argv)) {
    printf(INO_COLOR_GREEN "OK!\n" INO_COLOR_OFF);
    return 0;
  }
  printf(INO_COLOR_RED "KO!\n" INO_COLOR_OFF);
  return -1;
#endif
}
