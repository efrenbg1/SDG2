# SDG2 - arkanoPi (2021)

Proyecto realizado por:

 - Efrén Boyarizo Gargallo
 - Juan González Vázquez

# Mejoras

 - Desarrollado con VSCode
 - Animación de inactividad
 - Posición inciial de la pelota
 - Aumento de la velocidad del juego
 - Niveles aleatorios
 - Jugabilidad vía navegador web

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
 5. El servidor web del juego escuchará las peticiones en el puerto 8081. Para conectarse hay que introducir la dirección del servidor en el navegador web:
 ![8081](/images/url.png)

# Videos
[![SDG2 - arkanoPi (2021) - HTTP](/images/youtube3.png)](https://youtu.be/s7bFaaa1dQc "SDG2 - arkanoPi (2021) - HTTP")
[![SDG2 - arkanoPi (2021)](/images/youtube1.png)](https://youtu.be/xqU5KDnA7Vo "SDG2 - arkanoPi (2021)")
[![SDG2 - arkanoPi (2021) - Sin parpadeo](/images/youtube2.png)](https://youtu.be/3kmyAcbGGjY "SDG2 - arkanoPi (2021) - Sin parpadeo")
