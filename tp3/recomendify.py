#!/usr/bin/python3
import sys
import fileinput
import biblioteca
from grafo import Grafo
from reco_comandos import *
from reco_datos import leer_datos

CAMINO="camino"
MAS_IMPORTANTES="mas_importantes"
RECOMENDACION="recomendacion"
CICLO="ciclo"
RANGO="rango"
COMANDOS=[CAMINO, MAS_IMPORTANTES, RECOMENDACION, CICLO, RANGO]

class Recomendify:
  def __init__(self, playlists, usuarios, canciones, usuario_cancion):
    self.playlists = playlists
    self.usuarios = usuarios
    self.canciones = canciones
    self.usuario_cancion = usuario_cancion
    self.entre_canciones = None
    self.mas_importantes = None
  
  def armar_grafo_entre_canciones(self):
    self.entre_canciones = Grafo()
    
    for cancion in self.canciones:
      self.entre_canciones.agregar_vertice(cancion)

    for playlist in self.playlists:
      for cancion_uno in self.playlists[playlist]['canciones']:
        for cancion_dos in self.playlists[playlist]['canciones']:
          if cancion_uno == cancion_dos: continue
          if not self.entre_canciones.estan_unidos(cancion_uno, cancion_dos):
            self.entre_canciones.agregar_arista(cancion_uno, cancion_dos)

  def armar_mas_importantes(self):
    mas_importantes = []
    PR = pagerank(self.usuario_cancion)
    for pr in PR:
      if pr in self.canciones:
        mas_importantes.append({
          'nombre': pr,
          'importancia': PR[pr]
        })
    mas_importantes.sort(key=lambda x: x['importancia'], reverse=True)
    self.mas_importantes = mas_importantes

def ejecutar_comando(recomendify):
  linea = sys.stdin.readline().rstrip('\n')

  while linea:
    linea = linea.split(maxsplit=1)
    comando = linea[0]
    if comando not in COMANDOS: raise ValueError('El comando no existe')
    elif comando == CAMINO:
      parametros = linea[1].split(" >>>> ")
      origen, destino = parametros
      camino_mas_corto(
        recomendify.usuario_cancion,
        recomendify.canciones,
        origen, 
        destino
      )
    elif comando == MAS_IMPORTANTES:
      parametros = linea[1].split()
      cantidad = parametros[0]
      if not recomendify.mas_importantes: recomendify.armar_mas_importantes()
      canciones_mas_importantes(recomendify.mas_importantes, int(cantidad))
    elif comando == RECOMENDACION:
      parametros = linea[1].split(maxsplit=2)
      tipo, n, canciones = parametros
      canciones = canciones.split(" >>>> ")
      recomendaciones(
        recomendify.usuario_cancion, 
        canciones, 
        tipo, 
        int(n), 
        recomendify.canciones, 
        recomendify.usuarios
      )
    elif comando == CICLO:
      parametros = linea[1].split(maxsplit=1)
      n, origen = parametros
      if recomendify.entre_canciones == None: recomendify.armar_grafo_entre_canciones()
      ciclo_de_n_canciones(recomendify.entre_canciones, int(n), origen)
    elif comando == RANGO:
      parametros = linea[1].split(maxsplit=1)
      rango, origen = parametros
      if recomendify.entre_canciones == None: recomendify.armar_grafo_entre_canciones()
      todas_en_rango(recomendify.entre_canciones, int(rango), origen)
    linea = sys.stdin.readline().rstrip('\n')

if __name__ == "__main__":
  if len(sys.argv) != 2: raise ValueError('Cantidad de argumentos invalida')
  
  playlists, usuarios, canciones, grafo_usuario_cancion = leer_datos(sys.argv[1])
  recomendify = Recomendify(playlists, usuarios, canciones, grafo_usuario_cancion)
  ejecutar_comando(recomendify)
