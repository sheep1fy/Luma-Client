#include <stdio.h>
#include <time.h>
#include <unistd.h>

static volatile int luma_counter = 0;

__attribute__((constructor))
void luma_init() {
    FILE *f = fopen("/tmp/luma_proof.txt", "w");
    fprintf(f, "LUMA LOADED %ld PID=%d\n", (long)time(NULL), getpid());
    fclose(f);
    
    // FORCE console spam
    for(int i = 0; i < 10; i++) {
        printf("[LUMA] 🎉 LOADED #%d - Press K for menu!\n", i);
    }
    
    luma_counter = 1;
}

// Simple K key check (game loop)
void luma_check_keys() {
    if (luma_counter > 0) {
        printf("[LUMA] TICK %d\n", luma_counter++);
        // TODO: K key detection here
    }
}

__attribute__((destructor))
void luma_exit() {
    printf("[LUMA] EXIT\n");
}
