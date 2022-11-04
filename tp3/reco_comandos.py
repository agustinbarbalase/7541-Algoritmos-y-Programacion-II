from biblioteca import *

# O(U + C + L)
def camino_mas_corto(grafo, canciones, origen, destino):
  if origen not in canciones or destino not in canciones:
    return print('Tanto el origen como el destino deben ser canciones')

  _, padre = bfs_camino(grafo, origen, destino=destino)
  camino = armar_camino(padre, origen, destino)
  if not camino: return print('No se encontro recorrido')

  for i in range(len(camino) - 1):
    playlist = grafo.peso_arista(camino[i], camino[i + 1])
    if i % 2 == 0:
      print(f"{camino[i]} --> aparece en playlist --> ", end='')
      print(f"{playlist} --> de --> ", end='')
    else:
      print(f"{camino[i]} --> tiene una playlist --> ", end='')
      print(f"{playlist} --> donde aparece --> ", end='')

  print(f"{camino[len(camino) - 1]}")

def canciones_mas_importantes(mas_importantes, n):
  for i in range(n - 1):
    print(f"{mas_importantes[i]['nombre']}; ", end="")
  print(f"{mas_importantes[n - 1]['nombre']}")

def recomendaciones(grafo, lista, tipo, n, canciones, usuarios):
  lista_recomendaciones = {}
  reco = []
  
  for cancion in lista:
    camino = random_walk(grafo, cancion)
    for v in camino:
      if v not in lista:
        lista_recomendaciones[v] = 1
    pagerank_personalizado(grafo, lista_recomendaciones, camino)
  
  for r in lista_recomendaciones:
    if r in canciones and tipo == "canciones" and r not in lista:
      reco.append({
        'nombre': r,
        'importancia': lista_recomendaciones[r]
      })
    elif r in usuarios and tipo == "usuarios" and r not in lista:
      reco.append({
        'nombre': r,
        'importancia': lista_recomendaciones[r]
      })

  reco.sort(key=lambda x: x['importancia'], reverse=True)
  for r in reco:
    print(f"{r['nombre']} {r['importancia']}")

  for i in range(n):
    print(f"{reco[i]['nombre']}; ", end='')
  print(f"{reco[len(reco) - 1]['nombre']}")


# O(C^n) 
def ciclo_de_n_canciones(grafo, n, origen):
  ciclo = ciclo_n(grafo, n, origen)
  if not ciclo: return print('No se encontro recorrido')

  for i, c in enumerate(ciclo):
    print(c, end='')
    if i == len(ciclo) - 1: print('')
    else: print(' --> ')

# O(C + L)
def todas_en_rango(grafo, rango, origen):
  if origen not in grafo: return print(0)
  total_en_rango = bfs_en_rango(grafo, origen, rango)
  print(total_en_rango)