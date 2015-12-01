
/*
 * main.c
 *
 *  Created on: Sep 16, 2015
 *      Author: mdv
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#define MAX_LEN 6
#define INDEX_USED -1
#define DUPPRINT(fp, fmt...) \
    do {                     \
        printf(fmt);         \
        fprintf(fp, fmt);    \
    } while(0)

static int g_k, g_n, *g_arr, g_cnt;
static FILE* fd;

void get_t(char* buff)
{
    // see http://www.cplusplus.com/reference/ctime/strftime/
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buff, 80, "%YY_%b%d%a_%Hh%Mm%Ss_", timeinfo);
}

int parse_arg(int m_argc, char** m_argv)
{
    if(m_argc < 3) {
        fprintf(stderr, "Usage: <program name> k n\n");
        return EINVAL;
    }
    g_k = atoi(m_argv[1]);
    g_n = atoi(m_argv[2]);
    if(g_k == 0 || g_n == 0 || (g_n - g_k < 30)) { // bad input !
        fprintf(stderr, "Bad params.");
        return EINVAL;
    }
    return 0;
}

int init(int** m_arr, int n)
{
    *m_arr = (int*)realloc(NULL,n * sizeof(*m_arr));
    if(!(*m_arr)) {
        fprintf(stderr,"Out of memory.");
        return ENOMEM;
    }
    return EXIT_SUCCESS;
}

void load(int* m_arr, int n)
{
    int j;
    for(j = 0; j < n; j++) {
        m_arr[j] = j + 1;
    }
}

void print_nums(int* m_arr, int k)
{
    int j, q, c, r;
    q = g_n;
    c = 0;
    for(j = 0; j < k; j++) {
        do {
            r = (int)(rand() % q);
        } while(m_arr[r] == INDEX_USED);
        if(j == 0) {
            DUPPRINT(fd, "%s", "{ ");
        }
        DUPPRINT(fd, "%d ", m_arr[r]);
        m_arr[r] = INDEX_USED;
        c++;
        q--;
        if (c % MAX_LEN == 0 && (j < k-1)) {
            DUPPRINT(fd,"%s","\n");
        }
        if(j == k - 1) {
            DUPPRINT(fd, "%s", "}\n");
        }
    }
}

int kill_arr(int** m_arr)
{
    if(!(*m_arr)) {
        return EFAULT;
    }
    free(*m_arr);
    *m_arr = NULL;
    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    srand((unsigned)time(NULL));
    char* b = (char*) malloc(255 * sizeof(*b));
    int ret;
    g_cnt = 1;
    g_arr = NULL;
    get_t(b);

    if((ret = parse_arg(argc, argv)) == EINVAL) {
        return ret;
    }
    strcat(b, argv[1]);
    strcat(b, "-");
    strcat(b, argv[2]);
    strcat(b, "_lotto");

    fd = fopen(b, "a+");
    if((ret = init(&g_arr, g_n)) == ENOMEM) {
                return ret;
    }
    while(g_cnt <= g_k) {
        load(g_arr, g_n);
        print_nums(g_arr, g_k);
        g_cnt++;
    }
    ret = kill_arr(&g_arr);
    fclose(fd);
    free(b);
    return ret;
}


