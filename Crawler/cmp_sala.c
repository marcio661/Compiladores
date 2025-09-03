#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <curl/curl.h>

typedef struct {
    char *data;
    size_t size;
} Buffer;

static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total = size * nmemb;
    Buffer *buf = (Buffer*)userdata;
    char *newp = (char*)realloc(buf->data, buf->size + total + 1);
    if (!newp) return 0;
    buf->data = newp;
    memcpy(buf->data + buf->size, ptr, total);
    buf->size += total;
    buf->data[buf->size] = '\0';
    return total;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <URL>\n", argv[0]);
        return 1;
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *curl = curl_easy_init();
    if (!curl) { fprintf(stderr, "Erro: cURL init\n"); return 1; }

    Buffer buf = {0};
    curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "cmp_sala/1.0");
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");        // descomprime gzip/deflate/br
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Erro cURL: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        free(buf.data);
        return 1;
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    if (!buf.data || buf.size == 0) { free(buf.data); return 0; }

    
    const char *pattern = "[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}";
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Erro ao compilar regex\n");
        free(buf.data);
        return 1;
    }

    // Varre o buffer inteiro
    const char *p = buf.data;
    regmatch_t m;
    int found = 0;
    while (regexec(&re, p, 1, &m, 0) == 0) {
        int len = (int)(m.rm_eo - m.rm_so);
        if (len > 0) {
            fwrite(p + m.rm_so, 1, len, stdout);
            fputc('\n', stdout);
            found = 1;
        }
        p += m.rm_eo; // continua após o match
    }
    regfree(&re);
    free(buf.data);

    return found ? 0 : 0;
}
