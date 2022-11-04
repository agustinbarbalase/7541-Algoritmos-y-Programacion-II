---
math: true
---

# AlgoGram

## Análisis y diseño de la solución

Teniendo en cuenta lo que requería hacer el programa, decidimos empezar dividiendo
los pasos en los que la aplicación recibía y procesaba la información.
Primero debíamos crear una aplicación principal, que soportaría y guardaría todo
lo que pasase en la misma.
Creamos entonces el TDA _app_ que contiene todo el comportamiento necesario para
permitir el funcionamiento de la aplicación. Su estructura es:

```c
typedef struct app {
  char* usuario_loggeado;
  hash_t* usuarios;
  hash_t* posts;
} app_t;
```

Donde cada elemento representa:
- __usuario_loggeado__: Cadena con el nombre de usuario que se encuentra loggeado actualmente
en la aplicación. _NULL_ en caso de no haber usuario loggeado.
- __usuarios__: Diccionario con todos los _usuarios_ que entraron desde el archivo.
- __post__: Diccionario que guarda todos los posts que los _usuarios_ pudiesen publicar.

Luego habría que leer del archivo de texto todos los _usuarios_ y guardar de alguna manera
información sobre ellos para luego poder interactuar con los mismos.
Así que creamos un TDA _usuario_ estructurado de la siguiente manera:

```c
struct usuario {
  size_t id;
  heap_t* feed;
};
```

Donde:

- __id__: Manera de representar numéricamente al _usuario_.
- __feed__: Un _heap_ donde se guardarían los _posts_ hechos por otros _usuarios_,
donde el _post_ a desencolar es aquel del _usuario_ que más afinidad tenga con el primero.
Se hace uso de un _heap_ porque se requiere tener siempre el _post_ con mayor _afinidad_
posible al _usuario_, y esto se permite fácilmente con el uso de un _heap_. (La afinidad
será definida más adelante).

Entonces leemos línea por línea agregando nuevos usuarios con _feeds_ vacíos y con un
determinado id para cada uno de ellos.

En el _feed_ no agregaríamos un _post_ _per se_ sino que agregaríamos una estructura que enlaza el _post_
al usuario, de esta manera tenemos un solo diccionario con todos los _posts_ publicados.
Dicha estructura es un _feed_ y se conforma:

```c
typedef struct feed {
  size_t usuario_id;
  size_t creador_id;
  size_t post_id;
} feed_t;
```

Donde:

- __usuario_id__: Representa el número de identificación del _usuario_ que
recibió el _post_, el motivo de que se tenga esta información es que
desde el _heap_, no es posible acceder al mismo _usuario_.
- __creador_id__: Reprensentan atributos del _usuario_ que
creó el post. El motivo de que tengamos repetida esta información, es que
no es posible desde el _heap_ acceder al identificador del usuario. El
identificador nos será de utilidad para definir el orden del _heap_, que
llamaremos "Afinidad".
- __post_id__: Reprensenta el número de identificación del _post_.

Y por último, luego de cargados los usuarios, debemos recibir por _entrada estándar_
todos los comandos ingresados y responder a cada comando recibido con la acción
adecuada.

### Comandos y funcionamiento

Todas las implementaciones de los comandos estarán en un lenguaje de tipo pseudocódigo,
para mostrar de una forma rápida y sencilla cómo se implementan y determinar sus complejidades.
Ninguna de las siguientes implementaciones corresponden a la versión final de la
implementación.

### Login

Conociendo que _login_ debe funcionar en $\mathcal{O}(1)$ se entiende el por qué de la elección
de un _hash_ para el guardado de los usuarios, pues de esa forma, para revisar si dicho usuario
existe, basta con buscar su clave (_nombre de usuario_) en el hash que es $\mathcal{O}(1)$,
luego el código es:

```py
def login(app, nombre_usuario):
  if(hay_loggeado(app)): #O(1)
    return error("Ya habia un usuario loggeado") #O(1)
  else if(hash_obtener(app.usuarios, nombre_usuario)): #O(1)
    return error("Usuario no existente") #O(1)
  app.usuario_loggeado = nombre_usuario #O(1)
```

El resultado es $\mathcal{O}(1)$, porque todas las complejidades que conforman la
función son $\mathcal{O}(1)$.
Ver si un usuario está loggeado es $\mathcal{O}(1)$, puesto que es revisar si una cadena es
_NULL_ o no lo es.

### Logout

Basta con asignarle _NULL_ al usuario loggeado. Su código es:

```py
def logout(app):
  if(!hay_loggeado(app)): #O(1)
    return error("No habia usuario loggeado") #O(1)
  app.usuario_loggeado = NULL #O(1)
```

El resultado es $\mathcal{O}(1)$, porque todas las complejidades que conforman la
función son $\mathcal{O}(1)$.

### Publicar post

Para publicar un _post_, primero hay que definir qué consideramos como "_post_". Entonces pensamos
en una estructura que guarde lo que queramos publicar y la consideramos así:

```c
struct post {
  size_t id;
  char* informacion;
  char* nombre_usuario;
  abb_t* likes;
};
```

Donde:

- __id__: Representación numérica del _post_. Una forma sencilla de identificarlo.
- __informacion__: Cadena que guarda el texto que se quiere publicar.
- __nombre_usuario__: Cadena con el nombre del usuario creador.
- __likes__: Abb que guarda los usuarios que dieron like al _post_.

Una vez definido esto, publicar dicho _post_ solo supone crear un nuevo _post_ con la
información que se quiera publicar, y agregar la relación entre este _post_ y el resto
de usuarios de la aplicación, siendo una representación del código:

```py
def publicar_post(app, nuevo_post):
  if(!hay_loggeado(app)): #O(1)
    return error("No habia usuario loggeado") #O(1)
  hash_guardar(app.posts, nuevo_post) #O(1)

  usuario_creador = hash_obtener(app.usuarios, nuevo_post.nombre_usuario) # O(1)

  hash_iter = hash_iter_crear(app.usuarios) # O(1)
  
  while(!hash_iter_al_final(hash_iter)): # O(u)
    clave_actual = hash_iter_ver_actual(hash_iter) # O(1)

    if(clave_actual != nuevo_post.nombre_usuario):
      usuario_actual = hash_obtener(app.usuarios, clave_actual) # O(1)
      nuevo_feed = feed_crear(
        usuario_actual.id, 
        nuevo_post.id, 
        usuario_creador
      ) # O(1)
      heap_insertar(usuario_actual, nuevo_feed) # O(log p)
    
    hash_iter_avanzar(hash_iter) # O(1)
  
  hash_iter_destruir(hash_iter) # O(1)
```

Vemos entonces que la función se compone de instrucciones $\mathcal{O}(1)$ (cabe destacar
que la creación del _post_ también es $\mathcal{O}(1)$) seguido de un ciclo que recorre
a todos los usuarios ( $\mathcal{O}(u)$ ). Y dentro del mismo a cada usuario se le encola
en el heap _feed_ el enlace a dicho _post_ (El TDA _feed_) cuya complejidad es $\mathcal{O}(log p)$.

La complejidad final de la función resulta ser $\mathcal{O}(u \log p)$ que cumple con los
requisitos del trabajo práctico.

### Ver proximo post en el feed

Ver un _post_ en el _feed_ de un usuario solo supone desencolar el heap "_feed_" del usuario, obteniendo
así el id del _post_ del usuario con mayor afinidad al usuario loggeado.

Antes de seguir con el código le daremos sentido a la afinidad.
Anteriomente, hablamos de la afinidad como la manera de definir la prioridad
entre cada uno de los _feeds_. De manera que decidamos en qué orden deberían
ir apareciendo los _posts_.
Básicamente, el orden en el que fueron ingresados los
_usuarios_ nos definirá si un _post_ es más relevante o no para el _usuario_
que lo recibe.

En la siguiente figura, ilustramos cómo sería la base de datos que recibimos
como archivo a través de parámetros al iniciar el programa.

+:--:+:---:+:-:+:---:+:--:+
| U1 | ... | V | ... | U2 |
+----+-----+---+-----+----+

Según el enunciado del problema, un _post_ será más relevante para un _usuario_,
si su creador se encuentra más cerca en la base de datos inicial. Es decir, la
distancia entre el _usuario_ creador y el _usuario_ receptor es mínima.

Sea $d(x, y)$ la distancia entre el _usuario x_ y el _usuario y_, donde
la distancia se calcula a partir de los _ids_ de los _usuarios_, es decir

$$d(x, y) = abs(usuario(x).id - usuario(y).id)$$

Entonces, si queremos saber si un post es más relevante para el _usuario v_
debemos analizar la distancia con los _usuarios_ $U_1$ y $U_2$. Es decir,
debemos analizar $d(U_1, v) - d(U_2, v)$

Sabemos que la implementación del _heap_ es de máximos, es decir, que
veremos el mayor de los elementos en un orden de $\mathcal{O}(1)$ y devolverá
el máximo en $\mathcal{O}(\log p)$, donde _p_ representa los _post_. Por lo
tanto, el máximo de los elementos deberá ser el que está más cerca. Para la
matemática de nuestra definición eso implica que

$$d(U_1, v) - d(U_2, v) < 0 \to U_1 > U_2 $$

Ahora, si ambas distancias son iguales, significa que ambos _usuarios_ creadores
son igual de relevantes para el _usuario_ receptor. Según el enunciado, si
ambos son importantes, debemos ir por el post que se publicó primero, es decir,
el que tiene el _id_ más chico. Por lo tanto, la función de afinidad queda de la
siguiente manera

$$
  afinidad(v, U_1, U_2, P_1, P_2) = \left\{
  \begin{array}{lcc}
    1           & si & d(U_1, v) - d(U_2, v) < 0 \\
    -1          & si & d(U_1, v) - d(U_2, v) > 0 \\
    P_2 - P_1   & si & d(U_1, v) - d(U_2, v) = 0 \\
  \end{array}
  \right.
$$

Donde $P_1$ y $P_2$ son los _ids_ de los _post_ que se publicaron asociados
a los _usuarios_ creadores $U_1$ y $U_2$ respectivamente. Si los _ids_ de los
_post_ son iguales, eso significa que estamos insertando dos veces el mismo _post_,
algo que debemos evitar.

Definida la afinidad, podemos pasar al código:

```py
def proximo_post(app):
  usuario_creador = hash_obtener(app.usuarios, app.usuario_loggeado) # O(1)
  feed = heap_desencolar(usuario.feed) # O(log p)

  if(!hay_loggeado(app) || !feed):
    return error("Usuario no loggeado o no hay mas posts para ver") # O(1)
  
  post = hash_obtener(app.posts, str(feed.post_id)) # O(1)

  imprimir_post(post) # O(1)

  feed_destruir(feed) # O(1)
```

La complejidad final es $\mathcal{O}(\log p)$, donde _p_ es la cantidad de _posts_.
Obtener de un _hash_ es $\mathcal{O}(1)$. Ahora, obtener en un _heap_
donde se guarda información relacionada con los _posts_ es $\mathcal{O}(\log p)$.
Esto es porque hay que desencolar el máximo y buscar un lugar para otro elemento,
como se trata de un _heap_, esa búsqueda es logarítmica.

### Likear un post

Una de las funcionalidades que requerimos es poder dar like a un _post_ que hayamos visto.
Cuando definimos al _post_, usamos un abb para guardar quién dio like a nuestro _post_.
El uso de un abb para hacer esto es porque este nos permite dar un orden alfabético
a los elementos guardados en el mismo, además de poder recorrerlo en dicho orden,
que es algo solicitado para la resolución del enunciado.

Otra ventaja que nos aporta, es que si un _usuario_ da dos veces like al mismo _post_,
este no se computará dos veces.

Además, el tiempo de inserción es de $\mathcal{O}(\log u)$, donde _u_ es el número de _usuarios_
que dieron like. Este tiempo es requerido por el enunciado del problema.

Entonces, para dar like solo basta con insertar el usuario al abb de likes. El código es:

```py
def likear_post(app, post_id):
  post = hash_obtener(app.post, str(post_id)) # O(1)

  if(!hay_loggeado(app) || !post):
    return error("Usuario no loggeado o Post inexistente") # O(1)
  
  abb_guardar(post.likes , app.usuario_loggeado) # O(log u)
```

La complejidad final es $\mathcal{O}(\log u)$, donde _u_ es la cantidad de _usuarios_
que dieron like.
Obtener de un _hash_ es $\mathcal{O}(1)$. Ahora, guardar en un _abb_ es $\mathcal{O}(\log u)$.
Esto es porque hay que guardar el elemento y buscar
su lugar, como se trata de un _abb_, esa búsqueda es logarítmica.

### Mostrar likes

Ahora, para mostrar los likes de un _post_ solo hay que iterar el abb de likes del _post_ e
ir imprimiendo cada elemento iterado. El código puede ser representado como:

```py
def visitar_likes(clave, dato, extra):
  imprimir_usuario(dato) # O(1)
  return True

def mostrar_likes(app, post_id):
  post = hash_obtener(app.post, str(post_id)) # O(1)

  if(!post || abb_cantidad(post.likes) == 0): # O(1)
    return error("Post inexistente o sin likes") # O(1)
  
  imprimir_cant_likes(abb_cantidad(post.likes)) # O(1)
  abb_in_order(post.likes, visitar_likes, NULL) # O(u)
```

La complejidad final es $\mathcal{O}(u)$, donde _u_ es la cantidad de _usuarios_ que dieron
like al post. Obtener de un _hash_ es $\mathcal{O}(1)$. Ahora, iterar un _abb_ _in order_
es $\mathcal{O}(u)$. Ya que se recorren todos los elementos. Y obtener la cantidad
de elementos de un _abb_ es $\mathcal{O}(1)$.

### Conclusión

El código final fue modularizado y separado en diferentes archivos donde se
guarda el código relacionado con el funcionamiento de una parte del programa
siendo estos:

- __tp2.c__: El archivo "_main_" principal, encargado de llamar las tres partes importantes
del programa (crear la aplicación, cargar a los _usuarios_ y recibir los comandos).
- __app.c__: Archivo que contiene las primitivas del TDA _app t_ básicas como crear el
TDA, eliminarlo, devolver sus datos y modificarlos.
- __comandos.c__: Archivo encargado de la interacción con la _entrada estándar_.
Se encarga de cargar los _usuarios_ en la aplicación y recibir los comandos.
- __usuario.c__: Archivo que contiene las primitivas del TDA _usuario t_ y del
TDA _feed t_ básicas como crear los TDA, eliminarls, devolver sus datos y modificarlos.
- __post.c__: Archivo que contiene las primitivas del TDA _post t_ básicas como crear el
TDA, eliminarlo, devolver sus datos y modificarlos.

### Complejidades finales

- __login__: $\mathcal{O}(1)$.
- __logout__: $\mathcal{O}(1)$.
- __publicar__: $\mathcal{O}(u \log p)$. Donde _u_ son la cantidad de _usuarios_ y _p_ la de _posts_.
- __ver_siguiente_feed__: $\mathcal{O}(\log p)$. Donde _p_ es la cantidad de _posts_.
- __likear_post__: $\mathcal{O}(\log u)$ Donde _u_ es la cantidad de _usuarios_.
- __mostrar_likes__: $\mathcal{O}(u)$. Donde _u_ es la cantidad de _usuarios_.
