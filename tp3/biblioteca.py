from random import choice
from grafo import Grafo
from collections import deque

def grados_salida(grafo):
  grados = {}
  for v in grafo:
    grados[v] = len(grafo.adyacentes(v))
  return grados

def vertices_entrada(grafo):
  vertices = {}
  for v in grafo:
    vertices[v] = []
  for v in grafo:
    for w in grafo.adyacentes(v):
      vertices[w].append(v)
  return vertices

def bfs_camino(grafo, origen, destino=None):
  visitados = set()
  distancia = {}
  padre = {}

  distancia[origen] = 0
  padre[origen] = None
  visitados.add(origen)
  
  q = deque()
  q.append(origen)

  while q:
    v = q.popleft()
    if v == destino:
      return distancia, padre
    for w in grafo.adyacentes(v):
      if w not in visitados:
        distancia[w] = distancia[v] + 1
        padre[w] = v
        q.append(w)
        visitados.add(w)

  return distancia, padre

def bfs_en_rango(grafo, origen, limite):
  en_rango = 0
  visitados = set()
  distancia = {}

  distancia[origen] = 0
  visitados.add(origen)
  
  q = deque()
  q.append(origen)

  while q:
    v = q.popleft()
    for w in grafo.adyacentes(v):
      if w not in visitados:
        distancia[w] = distancia[v] + 1
        visitados.add(w)
        if distancia[w] == limite:
          en_rango += 1
        if distancia[w] > limite:
          break
        q.append(w)

  return en_rango

def armar_camino(padre, origen, destino):
  camino = []
  if destino not in padre: return None
  while destino != origen:
    camino.append(destino)
    destino = padre[destino]
  camino.append(destino)
  return camino[::-1]

def existe_camino(grafo, camino):
  for i in range(len(camino) - 1):
    print(i)

def suma_pagerank(grafo, v, PR, L, E, d=0.85):
  resultado = 0
  for w in E[v]:
    resultado += PR[w] / L[w]
  return d * resultado

def pagerank(grafo, d=0.85, iterador=100):
  PR = {}
  N = len(grafo)
  L = grados_salida(grafo)
  E = vertices_entrada(grafo)

  factor_aleatorio = (1 - d) / N

  # Inicializamos los valores del grafo
  for v in grafo:
    PR[v] = factor_aleatorio

  # Calculamos los pagerank de cada vertice
  for _ in range(iterador):
    for v in grafo:
      PR[v] = factor_aleatorio + suma_pagerank(grafo, v, PR, L, E)

  return PR

def random_walk(grafo, origen, largo=100):
  camino = []
  camino.append(origen)
  v = origen
  for _ in range(largo):
    v = choice(grafo.adyacentes(v))
    camino.append(v)
  return camino

def pagerank_personalizado(grafo, lista_recomendaciones, camino, largo=20, iteracion=100):
  for _ in range(iteracion):
    i = 1
    while i <= largo:
      v = camino[i - 1]
      w = camino[i]
      lista_recomendaciones[v] = lista_recomendaciones[w] / len(grafo.adyacentes(v))
      i += 1

def _ciclo_n(grafo, n, actual, origen, visitados, ciclo):
  visitados.add(actual)

  if len(ciclo) == n:
    if origen in grafo.adyacentes(actual):
      return ciclo
    else:
      return False
  for w in grafo.adyacentes(actual):
    if w not in visitados:
      ciclo.append(w)
      if _ciclo_n(grafo, n, w, origen, visitados, ciclo):
        return ciclo

  visitados.remove(actual)  

  return False

def ciclo_n(grafo, n, origen):
  if n < 3: return None
  visitados = set()
  ciclo = []
  ciclo.append(origen)
  if _ciclo_n(grafo, n, origen, origen, visitados, ciclo): 
    return ciclo
  return None