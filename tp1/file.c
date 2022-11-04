#include "file.h"

/*
  Funcion que elimina, si existe, el \n del final de un archivo añadiendo un \0
  Post: Devuelve un string mas corto en caso de existir al final de este un \n 
*/
void delete_slash_n(char* str) {
  if(str[strlen(str) - 1] == '\n') str[strlen(str) - 1] = '\0';
}

/*
  Funcion que libera la memoria de un string si solo consiste del \0
  Pre: El puntero al string nos pertenece
  Post: Libera el puntero si consiste en el \0 y devuelve True. En caso contrario,
  no hace nada y devuelve False
*/
bool delete_empty_buffer(char* str) {
  if(str[0] == '\0') {
    free(str);
    return true;
  }
  return false;
}

queue_t *read_file(int size_memory, FILE* file) {
  char* buffer = malloc(size_memory * sizeof(char));
  queue_t *queue = create_queue();

  while(fgets(buffer, size_memory, file) != NULL) {
    delete_slash_n(buffer);
    if(!delete_empty_buffer(buffer)) 
      add_element_queue(queue, buffer);
    buffer = malloc(size_memory * sizeof(char));
  }
  free(buffer);
  
  return queue;
}

void write_output(queue_t* queue) {
  char* buffer;
  while(!is_empty_queue(queue)) {
    buffer = (char *) delete_element_queue(queue);
    fprintf(stdout, "%s\n", buffer);
    free(buffer);
  }
}