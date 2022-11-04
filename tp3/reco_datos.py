from grafo import Grafo

def leer_datos(nombre):
  playlists = {}
  usuarios = set()
  canciones = set()
  grafo_usuario_cancion = Grafo(pesado=True)

  with open(nombre, 'r') as archivo:
    for i, linea in enumerate(archivo):
      if i == 0: continue

      linea = linea.rstrip("\n").split("\t")
      _, usuario, nombre_cancion, artista, playlist_id, playlist_nombre, generos = linea
      cancion = '{} - {}'.format(nombre_cancion, artista)

      if playlist_nombre not in playlists:
        playlists[playlist_nombre] = {
          'nombre': playlist_nombre,
          'usuario': usuario,
          'canciones': set()
        }
      playlists[playlist_nombre]['canciones'].add(cancion)

      if usuario not in grafo_usuario_cancion:
        usuarios.add(usuario)
        grafo_usuario_cancion.agregar_vertice(usuario)
      if cancion not in grafo_usuario_cancion:
        canciones.add(cancion)
        grafo_usuario_cancion.agregar_vertice(cancion)
      if not grafo_usuario_cancion.estan_unidos(usuario, cancion):
        grafo_usuario_cancion.agregar_arista(usuario, cancion, peso=playlist_nombre)

  return playlists, usuarios, canciones, grafo_usuario_cancion