#include "handler.h"

bool are_valid_args(args_t* args) {
  return (
    (return_numbers_args(args) <= 3) &&
    (return_numbers_args(args) >= 2) &&
    atoi(return_arg(args, 1))
  );
}

void error_handler(const char* message) {
  fprintf(stderr, "%s\n", message);
  exit(1);
} 

FILE* switch_file(args_t* args) {
  FILE* file = NULL;

  if(return_numbers_args(args) == FILE_STDIN) file = stdin;
  else if(return_numbers_args(args) == FILE_PATH) 
    file = fopen(return_arg(args, 2), "r");

  return file;
}

int calc_size_memory(args_t* args) {
  return atoi(return_arg(args, 1)) + 1;
}