#include <syscall.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int data = 1;
int bss;
int thread_count = 7;

__thread int bss_thread;
__thread int data_thread = 1;

__thread void *code_p;
__thread void *data_p;
__thread void *bss_p;
int *heap_main_p;
__thread int *heap_thread_p;
__thread void *libc_p;
void *stack_main_p;
__thread void *stack_thread_p;
__thread void *data_thread_p;
__thread void *bss_thread_p;

__thread void *data_2_p;
__thread void *stack_thread_2_p;

void test_stack() {
    int stack_thread_2 = 1;
    stack_thread_2_p = &stack_thread_2;
    unsigned long result;
    syscall(443, 1, (void*)&stack_thread_2_p, (void*)&result);
    printf("stack_thread_2_vaddr: %lx\n", stack_thread_2_p);
    printf("stack_thread_2_paddrr: %lx\n", result);
}

void *thread_f(void *thread_ID) {
    int ID = *((int *) thread_ID);
    sleep(ID);
    
    code_p = &thread_f;
    data_p = &data;
    bss_p = &bss;
    heap_thread_p = malloc(sizeof(int) * 1);
    *heap_thread_p = *heap_thread_p + 1;
    libc_p = &printf;
    int stack_thread;
    stack_thread = 1;
    stack_thread_p = &stack_thread;
    //data_thread = data_thread + 1;
    data_thread_p = &data_thread;
    bss_thread = bss_thread + 1;
    bss_thread_p = &bss_thread;
    
    unsigned long vaddr_thread[10];
    unsigned long paddr_thread[10];
    vaddr_thread[0] = code_p;
    vaddr_thread[1] = data_p;
    vaddr_thread[2] = bss_p;
    vaddr_thread[3] = heap_main_p;
    vaddr_thread[4] = heap_thread_p;
    vaddr_thread[5] = libc_p;
    vaddr_thread[6] = stack_main_p;
    vaddr_thread[7] = stack_thread_p;
    vaddr_thread[8] = data_thread_p;
    vaddr_thread[9] = bss_thread_p;
    syscall(443, 10, (void*)&vaddr_thread, (void*)&paddr_thread);
    printf("thread %d:\n", ID);
    //printf("code_vaddr: %lx, ", vaddr_thread[0]);
    //printf("code_paddr: %lx\n", paddr_thread[0]);
    //printf("data_vaddr: %lx, ", vaddr_thread[1]);
    //printf("data_paddr: %lx\n", paddr_thread[1]);
    //printf("bss_vaddr: %lx, ", vaddr_thread[2]);
    //printf("bss_paddr: %lx\n", paddr_thread[2]);
    //printf("heap_main_vaddr: %lx, ", vaddr_thread[3]);
    //printf("heap_main_paddr: %lx\n", paddr_thread[3]);
    printf("heap_thread_vaddr: %lx, ", vaddr_thread[4]);
    printf("heap_thread_paddr: %lx\n", paddr_thread[4]);
    //printf("libc_vaddr: %lx, ", vaddr_thread[5]);
    //printf("libc_paddr: %lx\n", paddr_thread[5]);
    //printf("stack_main_vaddr: %lx, ", vaddr_thread[6]);
    //printf("stack_main_paddr: %lx\n", paddr_thread[6]);
    printf("stack_thread_vaddr: %lx, ", vaddr_thread[7]);
    printf("stack_thread_paddr: %lx\n", paddr_thread[7]);
    printf("data_thread_vaddr: %lx, ", vaddr_thread[8]);
    printf("data_thread_paddr: %lx\n", paddr_thread[8]);
    printf("bss_thread_vaddr: %lx, ", vaddr_thread[9]);
    printf("bss_thread_paddr: %lx\n", paddr_thread[9]);
    test_stack();
    
    free(heap_thread_p);
    sleep(thread_count+1-ID);
    pthread_exit(NULL);
}

int main() {
    code_p = &thread_f;
    data = data + 1;
    data_p = &data;
    bss = bss + 1;
    bss_p = &bss;
    heap_main_p = malloc(sizeof(int) * 1);
    *heap_main_p = *heap_main_p + 1;
    printf("&printf: %lx\n", &printf);
    libc_p = &printf;
    int thread_IDs[thread_count+1];
    for(int i = 1; i <= thread_count; i++){
        thread_IDs[i] = i;
    }
    stack_main_p = thread_IDs;
    data_thread = data_thread + 1;
    data_thread_p = &data_thread;
    bss_thread = bss_thread + 1;
    bss_thread_p = &bss_thread;
    data_2_p = &thread_count;
    
    unsigned long vaddr_main[9];
    unsigned long paddr_main[9];
    vaddr_main[0] = code_p;
    vaddr_main[1] = data_p;
    vaddr_main[2] = bss_p;
    vaddr_main[3] = heap_main_p;
    vaddr_main[4] = libc_p;
    vaddr_main[5] = stack_main_p;
    vaddr_main[6] = data_thread_p;
    vaddr_main[7] = bss_thread_p;
    vaddr_main[8] = data_2_p;
    syscall(443, 9, (void*)&vaddr_main, (void*)&paddr_main);
    printf("main:\n");
    //printf("code_vaddr: %lx, ", vaddr_main[0]);
    //printf("code_paddr: %lx\n", paddr_main[0]);
    printf("data_vaddr: %lx, ", vaddr_main[1]);
    printf("data_paddr: %lx\n", paddr_main[1]);
    printf("bss_vaddr: %lx, ", vaddr_main[2]);
    printf("bss_paddr: %lx\n", paddr_main[2]);
    //printf("heap_main_vaddr: %lx, ", vaddr_main[3]);
    //printf("heap_main_paddr: %lx\n", paddr_main[3]);
    printf("libc_vaddr: %lx, ", vaddr_main[4]);
    printf("libc_paddr: %lx\n", paddr_main[4]);
    //printf("stack_main_vaddr: %lx, ", vaddr_main[5]);
    //printf("stack_main_paddr: %lx\n", paddr_main[5]);
    printf("data_thread_vaddr: %lx, ", vaddr_main[6]);
    printf("data_thread_paddr: %lx\n", paddr_main[6]);
    printf("bss_thread_vaddr: %lx, ", vaddr_main[7]);
    printf("bss_thread_paddr: %lx\n", paddr_main[7]);
    printf("data_2_vaddr: %lx, ", vaddr_main[8]);
    printf("data_2_paddr: %lx\n", paddr_main[8]);
    
    
    pthread_t threads[thread_count+1];
    for(int i = 1; i <= thread_count; i++){
        pthread_create(&threads[i], NULL, &thread_f, &thread_IDs[i]);
    }
    for(int i = 1; i <= thread_count; i++){
        pthread_join(threads[i], NULL);
    }
    
    printf("pid: %d\n", getpid());
    printf("Input 0 to end. Search:\n");
    unsigned long vs, ps;
    while(1){
        scanf("%lx%*c", &vs);
        if(vs == 0){
            break;
        }
        int r = syscall(443, 1, (void*)&vs, (void*)&ps);
        printf("vaddr: %lx\n", vs);
        printf("paddr: %lx\n", ps);
        printf("system call return: %d\n", r);
    }
    printf("End.\n");
    
    free(heap_main_p);
    return 0;
}
