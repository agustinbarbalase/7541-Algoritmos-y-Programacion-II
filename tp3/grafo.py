from random import choice

class Grafo:
  def __init__(self, dirigido = False, pesado = False):
    self.lista_adyacencia = {}
    self.dirigido = dirigido
    self.pesado = pesado

  def __iter__(self):
    return iter(self.lista_adyacencia)

  def __len__(self):
    return len(self.lista_adyacencia)

  """
    Funcion que toma un vertice y lo agregar al grafo
    Pre: El vertice no existe
    Post: Agregar el vertice al grafo
  """
  def agregar_vertice(self, v):
    if v not in self.lista_adyacencia: 
      self.lista_adyacencia[v] = {}

  """
    Funcion que borra un vertice del grafo
    Pre: El vertice existe
    Post: Borra el vertice y todas su arista adyacentes.
    Tanto de salida como de entrada
  """
  def borrar_vertice(self, v):
    if v in self.lista_adyacencia:
      del self.lista_adyacencia[v]
      for x in self.lista_adyacencia:
        if v in self.lista_adyacencia[x]:
          del self.lista_adyacencia[x][v]

  """
    Funcion que agrega una arista al grafo
    Pre: Ambos vertices existen
    Post: Agrega la arista:
    - Si es dirigido la agrega de v -> w
    - Si es no dirigio la agrega de v <-> w
    - Si es pesado por defecto sera 1. Caso contrario, el
      que se haya pasado por parametro
    - Si es no pesado sera None
  """
  def agregar_arista(self, v, w, peso = 1):
    if v in self.lista_adyacencia and w in self.lista_adyacencia:
      nueva_arista = { 'peso' : peso }
      if not self.pesado:
        nueva_arista['peso'] = None
      self.lista_adyacencia[v][w] = nueva_arista
      if not self.dirigido:
        self.lista_adyacencia[w][v] = nueva_arista

  """
    Funcion que borra una arista del grafo
    Pre: Ambos vertices existen
    Post: Borra la arista:
    - Si es dirigido se borra v -> w
    - Si es no dirigido se borra v <-> w
  """
  def borrar_arista(self, v, w):
    if v in self.lista_adyacencia and w in self.lista_adyacencia:
      del self.lista_adyacencia[v][w]
      if not self.dirigido:
        del self.lista_adyacencia[w][v]
  
  """
    Funcion que borra una arista del grafo
    Pre: Ambos vertices existen
    Post: Borra la arista:
    - Si es dirigido se borra v -> w
    - Si es no dirigido se borra v <-> w
  """
  def estan_unidos(self, v, w):
    if v not in self.lista_adyacencia:
      return False
    if w not in self.lista_adyacencia[v]:
      return False
    return True

  """
    Funcion que devuelve el peso de una arista del grafo
    Post: Devuele el peso de la arista:
    - Si es pesado devuelve el peso de v -> w, 
      si es no dirigido el peso es igual a w -> v
    - Si es no pesado devuele None
    - Si la arista o alguno de los vertices no existen,
      devulve False
  """
  def peso_arista(self, v, w):
    if self.estan_unidos(v, w):
      return self.lista_adyacencia[v][w]['peso']
    return False

  """
    Funcion que devuelve los vertices del grafo
    Post: Devuele una lista con los vertices
  """
  def obtener_vertices(self):
    return list(self.lista_adyacencia)

  """
    Funcion que devuelve un vertice al azar del grafo
    Post: Devuele cualquier vertice
  """
  def vertice_aletorio(self):
    return choice(list(self.lista_adyacencia))

  """
    Funcion que devuelve las aristas adyacentes de vertice 
    del grafo
    Pre: El vertice existe 
    Post: Devuele las aristas adyacentes, donde las aristas
    corresponde a v -> w, donde v es el vertice que ingresamos
    y w son los vertices adyacentes. Si no existe el vertice 
    devuele una lista vacia
  """
  def adyacentes(self, v):
    if v not in self.lista_adyacencia:
      return []
    return list(self.lista_adyacencia[v])
