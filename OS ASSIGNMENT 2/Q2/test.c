#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

#define kernel_2d_memcpy 448

int main(int argc, char **argv){

  float originalMatrix[4][4] = {
      {6.34, 2.56, 3.67, 4.89},
      {5.45, 6.47, 7.25, 8.92},
      {9.59, 1.61, 0.89, 1.05},
      {7.69, 6.21, 5.68, 6.77}
  };
  float copyMatrix[4][4];

  int sys = syscall(kernel_2d_memcpy,copyMatrix,originalMatrix,4,4);

  if(sys == 0){
    printf("System call was Successful.\n");
  }else{
    perror("kernel_2d_memcpy");
    return -1;
  }
  printf("\nThe Copied Matrix is :\n");
  for(int i=0;i<4;++i){
    for(int j=0;j<4;++j){
      printf("%f ",copyMatrix[i][j]);
    }
    printf("\n");
  }
  return 0;
}