# SDG2 - arkanoPi (2021)

Proyecto realizado por:

 - Efrén Boyarizo Gargallo
 - Juan González Vázquez

## Cómo compilar

 1. Clonar repositorio (directamente en la Raspberry Pi):
```console
$ git clone https://github.com/efrenbg1/SDG2.git && cd SDG2
```
 2. Instalar dependencias:
```console
$ sudo apt update && sudo apt install wiringpi libmicrohttpd-dev
```
 3. Compilar el código:
```console
$ gcc -lpthread -lwiringPi -lrt -lmicrohttpd *.c -o arkanoPi
```
 4. Ejecutar el juego:
```console
$ ./arkanoPi
```

# Mejoras

 - Desarrollado con VSCode
 - Animación de inactividad
 - Posición inciial de la pelota
 - Aumento de la velocidad del juego
 - Niveles aleatorios
 - Jugabilidad vía navegador web

# Videos
[![SDG2 - arkanoPi (2021) - Sin parpadeo](/images/youtube2.png)](https://youtu.be/3kmyAcbGGjY "SDG2 - arkanoPi (2021) - Sin parpadeo")
