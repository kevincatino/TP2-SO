# Manual de Usuario Trabajo Especial Arquitectura de Computadoras

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

Una vez ejecutado, se va a visualizar la shell. Para poder ver los comandos disponibles, se debe escribir: *help* y se desplegaran todos los comandos con sus descripciones.

##### Dentro de la seccion de juegos

Si usted accedio a la seccion de juegos, lo primero que va a visulaizar son las reglas. Para poder continuar, uds debe apretar cualquer letra, menos en enter. Luego, podra visulizar lo siguiente:
- Una mini shell
- El tiempo
- Un cronometro
- Un sudoku
- Un ahorcado

Para poder ingresar a cualquier juego, usted debe escribir el siguiente comando: *cs num* siendo num el numero de pantalla a la cual desea acceder. Si usted no recuerda algun comando, ingresando *help* le dira los comandos disponibles de esta seccion.
**Nota: Dentro de cada juego, se va explicando paso a paso como se debe jugar**


## Trabajo realizado por:

* Banfi, Malena     (Legajo: 61008)
* Fleischer, Lucas  (Legajo: 61153)
* Occhipinti, Abril (Legajo: 61159)
