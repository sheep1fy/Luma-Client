#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <android/log.h>

// This makes it easier to send messages to the launcher's log
#define LOG_TAG "LUMA"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// 1. The "Front Door" the launcher is looking for
// This clears the "does not have a mod_preinit function" warning
void mod_preinit() {
    LOGI("======= Luma Client Starting =======");
    
    // Create the proof file in the current directory (usually game data)
    FILE *f = fopen("luma_proof.txt", "w");
    if (f) {
        fprintf(f, "LUMA v1.0.1 LOADED %ld\n", (long)time(NULL));
        fclose(f);
    }
    
    // 15x Proof with professional logging
    for(int i = 0; i < 15; i++) {
        LOGI("[%d/15]: 🎉 LOADED! K=ClickGUI!", i);
    }
}

// 2. The backup constructor (runs when the library is loaded)
__attribute__((constructor))
void luma_init() {
    // We call preinit here as well to ensure it runs regardless of launcher version
    static int initialized = 0;
    if (!initialized) {
        mod_preinit();
        initialized = 1;
    }
}
