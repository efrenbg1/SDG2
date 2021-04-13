#include "http.h"

pthread_mutex_t lock_pantalla;
char pantalla_http[250] = {'\0'};

static ssize_t
file_reader(void *cls, uint64_t pos, char *buf, size_t max)
{
    FILE *file = cls;

    (void)fseek(file, pos, SEEK_SET);
    return fread(buf, 1, max, file);
}

static void
free_callback(void *cls)
{
    FILE *file = cls;
    fclose(file);
}

static int thread_http(void *cls,
                       struct MHD_Connection *connection,
                       const char *url,
                       const char *method,
                       const char *version,
                       const char *upload_data,
                       size_t *upload_data_size, void **ptr)
{
    // Comprobar si la petición es tipo GET o no se han recibido datos aun
    if (0 != strcmp(method, MHD_HTTP_METHOD_GET))
        return MHD_NO;
    static int aptr;
    if (&aptr != *ptr)
    {
        *ptr = &aptr;
        return MHD_YES;
    }
    *ptr = NULL;

    // Inicializar variables necesarias para elaborar la resupuesta
    struct MHD_Response *respuesta;
    int resultado;

    // Enviar datos de la pantalla del juego si la URL es "/update"
    if (strcmp(url, "/update") == 0)
    {
        pthread_mutex_lock(&lock_pantalla);
        respuesta = MHD_create_response_from_buffer(strlen(pantalla_http), (void *)pantalla_http, MHD_RESPMEM_PERSISTENT);
        resultado = MHD_queue_response(connection, MHD_HTTP_OK, respuesta);
        pthread_mutex_unlock(&lock_pantalla);
        MHD_destroy_response(respuesta);
        return resultado;
    }

    // Iniciar variables necesarias para responder con un archivo
    FILE *archivo = NULL;
    struct stat buf;
    char directorio[30];

    // Si el directorio es "/" responder con "index.html", sino añadir el directorio static
    if (strcmp(url, "/") == 0)
    {
        strcpy(directorio, "static/index.html");
    }
    else
    {
        strcpy(directorio, "static");
        strcat(directorio, url);
    }
    printf("%s\n", directorio);

    // Abrir archivo si existe
    if (0 == stat(directorio, &buf))
    {
        archivo = fopen(directorio, "rb");
    }

    // Enviar archivo si existe, sino enviar la págana 404 (not found)
    if (archivo == NULL)
    {
        respuesta = MHD_create_response_from_buffer(strlen(no_encontrado), (void *)no_encontrado, MHD_RESPMEM_PERSISTENT);
        resultado = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, respuesta);
    }
    else
    {
        respuesta = MHD_create_response_from_callback(buf.st_size, 32 * 1024, &file_reader, archivo, &free_callback);
        if (respuesta == NULL)
        {
            fclose(archivo);
            MHD_destroy_response(respuesta);
            return MHD_NO;
        }
        resultado = MHD_queue_response(connection, MHD_HTTP_OK, respuesta);
    }

    // Liberar memoria alocada
    MHD_destroy_response(respuesta);
    return resultado;
}

void actualizaPantallaHttp(char *_pantalla)
{
    pthread_mutex_lock(&lock_pantalla);
    //memcpy(pantalla_http, _pantalla, strlen(_pantalla);
    strncpy(pantalla_http, _pantalla, strlen(_pantalla));
    pthread_mutex_unlock(&lock_pantalla);
}

int inicializaServidorHttp()
{
    struct MHD_Daemon *d = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION | MHD_USE_INTERNAL_POLLING_THREAD, puerto, NULL, NULL, &thread_http, no_encontrado, MHD_OPTION_END);

    return d == NULL;
}