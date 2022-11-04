#include "args.h"

struct args {
  int argc;
  char** argv;
};

args_t* create_args(int argc, char* argv[]) {
  args_t* new_args = malloc(sizeof(args_t));
  if(!new_args) return NULL;  

  new_args->argc = argc;
  new_args->argv = malloc(argc * sizeof(char *));
  if(!new_args->argv) {
    free(new_args);
    return NULL;
  }

  for(int i = 0; i < argc; i++) {
    new_args->argv[i] = argv[i];
  }

  return new_args;
}

int return_numbers_args(args_t* args) {
  return args->argc;
}

char* return_arg(args_t* args, size_t pos) {
  if(pos >= args->argc) return NULL;
  return args->argv[pos];
}

void destroy_args(args_t* args) {
  free(args->argv);
  free(args);
}