
# Repositorio para el curso de Diseño de lenguajes de programación y compiladores.

Como se dijo en el curso, con cada entrega de lab se busca ir creando poco a poco 
un compilador básico y funcional.

## Dependencias:

+ Graphviz (graficar automatas)
+ Catch2 (Pruebas)

## Herramientas de compilación:
+ gcc (g++)
+ make (build system)
+ cmake (abstraccion para el build system)
+ vcpkg (manejador de paquetes de C/C++ de Microsoft)

El programa se desarrollo en un ambiente Linux, no he probado compilar en Windows o Mac.

## Compilar con CMAKE:
+ Crear una carpeta para la compilacion:
> mkdir build
+ Ve a esa carpeta y ejecuta los comandos de cmake. Esto creara todas las configuraciones necesaria para
compilar el proyecto:
> cd build
> cmake ..

Si ya existe un entorno para c++, cmake lo detectara y hara las configuraciones necesarias para poder
compilar en el sistema objetivo. (En caso de linux, lo configurara para 'make' y para windows, creara un .sln
para usar con Visual Stidio si está instalado)

+ Compila el proyecto:
> make (linux)
o
> cmake --build .

