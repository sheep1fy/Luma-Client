#include <stdio.h>
#include <time.h>
#include <unistd.h>

__attribute__((constructor))
void luma_init() {
    FILE *f = fopen("/tmp/luma_proof.txt", "w");
    fprintf(f, "LUMA LOADED %ld PID=%d\n", (long)time(NULL), getpid());
    fclose(f);
    
    // 10x FORCE spam - IMPOSSIBLE to miss!
    for(int i = 0; i < 10; i++) {
        printf("[LUMA #%d] 🎉 LOADED SUCCESSFULLY! K=Menu!\n", i);
    }
    fflush(stdout);  // FORCE output
}

__attribute__((destructor))
void luma_exit() {
    printf("[LUMA] EXIT\n");
    fflush(stdout);
}
