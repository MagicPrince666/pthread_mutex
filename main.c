#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "ringbuf.h"

pthread_mutex_t data_mutex;
cycle_buffer *buff = NULL;

typedef struct {
    const char*  str;   //发送内容
    char product;       //类型 0 大写字母 1 小写字母 2 数字
} param;

param str[3] = { //声明并初始化消息体
        {
            .str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
            .product = 0
        },
        {
            .str = "abcdefghijklmnopqrstuvwxyz",
            .product = 1
        },
        {
            .str = "1234567890",
            .product = 2
        }
    };
/**
 * @brief 生成者线程
 * @param thread_data 
 * @return void* 
 */
void* producter( void* thread_data ) {

    param *data = (param *)thread_data;
    char *str = (char *)data->str;
    int size = strlen(str);
    int len = 0;
    time_t times = 0;

    for(;;) {
        FILE *fp = fopen( "file.txt" , "at+");
        times = time(NULL);
        struct tm* utcTime = gmtime(&times);
        pthread_mutex_lock( &data_mutex );

        if (cycle_overage(buff) > size) {
            len = cycle_write(buff, str, size);
            pthread_mutex_unlock( &data_mutex );
            fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d maker %d %s\n",
                utcTime->tm_year + 1900,
                utcTime->tm_mon + 1,
                utcTime->tm_mday,
                utcTime->tm_hour,
                utcTime->tm_min,
                utcTime->tm_sec,
                data->product,
                str);
        } else {
            pthread_mutex_unlock( &data_mutex );
            fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d maker %d\n",
                utcTime->tm_year + 1900,
                utcTime->tm_mon + 1,
                utcTime->tm_mday,
                utcTime->tm_hour,
                utcTime->tm_min,
                utcTime->tm_sec,
                data->product);
        }

        fclose(fp);
        usleep(150000);
    }

    return NULL;
}

/**
 * @brief 消费者线程
 * @param thread_data 
 * @return void* 
 */
void* consumer( void* thread_data ) {
    char buf[512] = {0};
    for(;;) {
        if(!cycle_empty(buff)) {
            pthread_mutex_lock( &data_mutex );

            int len = cycle_read(buff, buf, 512);

            pthread_mutex_unlock( &data_mutex );
            buf[len] = 0;
            printf("consumer read buf = %s\n", buf);
        }
        usleep(100000);
    }
    return NULL;
}

int main( int argc, char* argv[] ) {

    pthread_t pthread_id[4];
    buff = ring_init(512);
    remove("file.txt");

    if ( pthread_mutex_init( &data_mutex, NULL ) ) {
        perror( "ERROR initializing the print_mutex\n" );
    }

    //循环创建3个生成者，消息体通过参数传入
    for(int i = 0; i < 3; i++) { 
        if (pthread_create(&pthread_id[0], NULL, producter, (void*)(str + i)))
            perror( "ERROR creating producter\n");
    }
    // 单独创建1个消费者
    if (pthread_create(&pthread_id[3], NULL, consumer, NULL))
        perror( "ERROR creating consumer\n");

    // 等待线程退出
    if(pthread_id[0] != 0) {
        pthread_join(pthread_id[0], NULL);
    }
    if(pthread_id[1] != 0) {
        pthread_join(pthread_id[1], NULL);
    }
    if(pthread_id[2] != 0) {
        pthread_join(pthread_id[2], NULL);
    }
    if(pthread_id[3] != 0) {
        pthread_join(pthread_id[3], NULL);
    }

    for(;;) {
        sleep(1);
    }

    pthread_mutex_destroy( &data_mutex );
    return 0;
}
