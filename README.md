# Manual de Usuario Trabajo Practico Sistemas Operativos

## Pasos a seguir para la ejecucion: 

### 1. Compilacion y linkedicion

Para compilar y linkeditar el proyecto correctamente, se provee un script de compilacion llamado *compile.sh*, el cual compila y linkedita el proyecto con docker utilizando los siguientes comandos:
- sudo docker run -v ${PWD}:/root -ti -w /root agodio/itba-so:1.0 ${1} make clean
- sudo docker run -v ${PWD}:/root -ti -w /root agodio/itba-so:1.0 ${1} make all

Este script se encuntra tanto en Toolchain, como en la raiz del proyecto, ya que para obtener una compilacion y linkedicion exitosa, primero se debe compilar desde Toolchain y luego desde la raiz. Es decir, usted primero debe dirigirse a Toolchain y ejecutar de la siguiente manera: *./compile.sh*. Luego debe regresar a la raiz del proyecto y ejecutar el mismo comando.


### 2. Ejecucion

Una vez compilado el proyecto, vamos a la ejecucion. Para ejecutar, primero se debe abrir la terminal, colocarse en la raiz del proyecto y luego ejecutar run.sh de la siguiente manera: *./run.sh*. 
**Nota: En caso de tener el sigueinte error: "qemu-system-x86_64: -hda Image/x64BareBonesImage.qcow2: Could not open 'Image/x64BareBonesImage.qcow2': Permission denied" al ejecutar, agregar un sudo delante, es decir, *sudo ./run.sh***

## Dentro del programa

Una vez ejecutado, se va a visualizar la shell. Los comandos disponibles son los siguientes:
| Comando | Accion |
| :---:   | :-: |
| help | Despliega en pantalla las funciones disponibles para el usuario y que accion realizan. |
| clean   | Limpia la shell. |
| mmTest   | Test de memoria. |
| syncTest   | Test de semaforo. |
| loop   | Imprime el ID de un proceso con un mensaje, determinado por una cantidad de segundos. |
| kill   | Mata un proceso dado su ID. |
| cat   | Imprime en pantalla los caracteres recibidos por STDIN. |
| wc   | Cuenta la cantidad de lineas ingresadas por STDIN. |
| filter   | Filtra las vocales de lo que recibe por STDIN. |
| phylo   | Implementa el problema de sincronización de los filósofos. |
| block   | Alterna el estado de un proceso dado su ID. |
| ps   | Despliega una lista de procesos con sus propiedades. |
| memStatus   | Imprime el estado de la memoria. |
| nice   | Cambia la prioridad de un proceso dado su ID y la nueva prioridad. |
| pipe   |  Despliega una lista de los pipes activos con sus propiedades. |
| sem   |  Despliega una lista de los semaforos activos con sus propiedades. |


## Trabajo realizado por:

* Banfi, Malena     (Legajo: 61008)
* Catino, Kevin     (Legajo: 61643)
* Fleischer, Lucas  (Legajo: 61153)
