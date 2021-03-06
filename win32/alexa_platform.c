/*******************************************************************************
    Copyright Ringsd. 2017.
    All Rights Reserved.
    
    File: alexa_platform.c

    Description:

    TIME LIST:
    CREATE By Ringsd   2017/01/17 09:46:03

*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>
#include <process.h>

#include "alexa_platform.h"

#define TAG            "win32"

void* alexa_malloc( int size )
{
    return malloc(size);
}

void* alexa_zmalloc(int size)
{
    void* p = alexa_malloc(size);
    if (p) memset(p, 0, size);
    return p;
}

void alexa_free(void* p)
{
    free( p );
}

void alexa_generate_uuid( char* uuid, int len )
{
    GUID guid;
    int w_len = 0;
    CoCreateGuid(&guid);
    w_len = _snprintf(uuid, len, "%08x-%04x-%04x-", guid.Data1, guid.Data2, guid.Data3);
    if(w_len >= len)
    {
        uuid[len - 1] = '\0';
        return;
    }
    for (size_t i = 0; i < 8; i++)
    {
        if (i == 2)
        {
            w_len += _snprintf(&uuid[w_len], len - w_len, "-%02x", guid.Data4[i]);
        }
        else
        {
            w_len += _snprintf(&uuid[w_len], len - w_len, "%02x", guid.Data4[i]);
        }
        if (w_len >= len)
        {
            uuid[len - 1] = '\0';
            return;
        }
    }

}

/*******************************************************************************
    thread
*******************************************************************************/

int alexa_begin_thread(alexa_thread_proc proc, void* p_data, void* stack, int prio)
{
    prio = prio;
    stack = stack;
    return _beginthread(proc, 0, p_data);
}

int alexa_begin_thread2(alexa_thread_proc proc, void* p_data, void* stack, int prio, int size)
{
    prio = prio;
    stack = stack;
    size = size;
    return _beginthread(proc, 0, p_data);
}

int alexa_end_thread(int id)
{
    id = id;
//    _endthreadex(id);
    return 0;
}

/*******************************************************************************
    mutex
*******************************************************************************/

struct alexa_mutex{
    HANDLE  h;
};

struct alexa_cond{
    HANDLE  h;
};

struct alexa_mutex* alexa_mutex_create(void)
{
    struct alexa_mutex* mutex = alexa_new( struct alexa_mutex );
    if( mutex )
    {
        mutex->h = CreateEvent(0, 0, 1, 0);
        if( mutex->h == NULL )
        {
            alexa_delete(mutex);
            mutex = NULL;
        }

        return mutex;
    }
    return NULL;
}

int alexa_mutex_lock(struct alexa_mutex* mutex)
{
    int ret;

    ret = WaitForSingleObject(mutex->h, 0);

    if (WAIT_OBJECT_0 == ret)
        return 0;

    if (WAIT_TIMEOUT == ret)
        return 1;

    return -1;
}

int alexa_mutex_unlock(struct alexa_mutex* mutex)
{
    SetEvent(mutex->h);
    return 0;
}

void alexa_mutex_destroy(struct alexa_mutex* mutex)
{
    CloseHandle(mutex->h);
    alexa_delete(mutex);
}

struct alexa_cond* alexa_cond_create( void )
{
    struct alexa_cond* cond = alexa_new( struct alexa_cond );
    if( cond )
    {
        return cond;
    }
    return NULL;
}

int alexa_cond_signal(struct alexa_cond* cond)
{
    cond = cond;
    return 0;
}

int alexa_cond_broadcast(struct alexa_cond* cond)
{
    cond = cond;
    return 0;
}

int alexa_cond_wait(struct alexa_cond* cond, struct alexa_mutex* mutex)
{
    cond = cond;
    mutex = mutex;
    return 0;
}

void alexa_cond_destroy( struct alexa_cond* cond )
{
    alexa_delete(cond);
}


void alexa_delay(long ms)
{
    Sleep(ms);
}


/*******************************************************************************
    END OF FILE
*******************************************************************************/
