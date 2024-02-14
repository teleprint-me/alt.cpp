#include <cblas.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>

void print_sys_info() {
  struct utsname buffer;
  if (uname(&buffer) != 0) {
    perror("uname");
    exit(EXIT_FAILURE);
  }

  struct sysinfo info;
  if (sysinfo(&info) != 0) {
    perror("sysinfo");
    exit(EXIT_FAILURE);
  }

  printf("System Information:\n");
  printf("  OS: %s\n", buffer.sysname);
  printf("  Version: %s\n", buffer.release);
  printf("  Total RAM: %ld MB\n", info.totalram / 1024 / 1024);
  printf("\n");
}

void simple_openblas_example() {
  // Create matrices A, B, C
  double A[6] = {1.0, 2.0, 1.0, -3.0, 4.0, -1.0};
  double B[6] = {1.0, 2.0, 1.0, -3.0, 4.0, -1.0};
  double C[9] = {.5, .5, .5, .5, .5, .5, .5, .5, .5};

  // Perform matrix multiplication C = A * B
  // using OpenBLAS function
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, 3, 3, 2, 1, A, 2, B, 2,
              1, C, 3);

  // Print result
  printf("Result matrix:\n");
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      printf("%lf ", C[i * 3 + j]);
    }
    printf("\n");
  }
}

int main() {
  print_sys_info();
  simple_openblas_example();

  return 0;
}
