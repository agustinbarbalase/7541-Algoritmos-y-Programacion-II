#include "app.h"
#include "comandos.h"

int main(int argc, char* argv[]) {
  if (argc != 2) return -1;
  	
  app_t* app = app_crear();
  cargar_usuarios(app, argv[1]);
  recibir_comando(app);
  
  app_destruir(app);
  return 0;
}
