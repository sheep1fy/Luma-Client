#include <stdio.h>
#include <time.h>
#include <unistd.h>

__attribute__((constructor))
void luma_init() {
    FILE *f = fopen("/tmp/luma_proof.txt", "w");
    fprintf(f, "LUMA v1.0.1 LOADED %ld\n", (long)time(NULL));
    fclose(f);
    
    // 15x UNMISSABLE PROOF
    for(int i = 0; i < 15; i++) {
        printf("LUMA[%d/15]: 🎉 LOADED! K=ClickGUI!\n", i);
    }
    fflush(stdout);
}
