#include <microhttpd.h>
#include <stdio.h>
#include <string.h>

#define PORT 8888

static ssize_t file_reader(void *cls, uint64_t pos, char *buf, size_t max)
{
    FILE *file = cls;
    (void)fseek(file, pos, SEEK_SET);
    return fread(buf, 1, max, file);
}

static void file_free_callback(void *cls)
{
    FILE *file = cls;
    fclose(file);
}

int answer_to_connection(void *cls, struct MHD_Connection *connection,
                         const char *url,
                         const char *method, const char *version,
                         const char *upload_data,
                         size_t *upload_data_size, void **con_cls)
{
    const char *index = "Hello, Juanete!";
    const char *notfound = "Bad URL menn";

    struct MHD_Response *response;
    int ret;

    if (strcmp(url, "/") == 0)
    {
        FILE *file = fopen("public/index.html", "rb");
        struct stat buf;
        response = MHD_create_response_from_callback(buf.st_size, 32 * 1024, &file_reader, file, &file_free_callback);

        response = MHD_create_response_from_fd(sz, (int)fd);
    }
    else
    {
        response = MHD_create_response_from_buffer(strlen(notfound),
                                                   (void *)notfound, MHD_RESPMEM_PERSISTENT);
    }

    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

int main()
{
    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                              &answer_to_connection, NULL, MHD_OPTION_END);
    if (NULL == daemon)
        return 1;
    getchar();

    MHD_stop_daemon(daemon);
    return 0;
}