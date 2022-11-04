#include "args.h"
#include "file.h"
#include "handler.h"
#include "queue.h"

int main(int argc, char *argv[]) {
  args_t* args = create_args(argc, argv);

  if(!are_valid_args(args)) {
    destroy_args(args);
    error_handler(ERROR_MSG_ARGS);
  }

  /* Leemos el archivo y lo almacenamos en una cola */  
  FILE* file = switch_file(args);
  if(!file) { 
    destroy_args(args);
    error_handler(ERROR_MSG_FILE);
  }
  queue_t *queue = read_file(calc_size_memory(args), file);
  fclose(file);

  write_output(queue);
  destroy_queue(queue, NULL);
  destroy_args(args);

  return 0;
}