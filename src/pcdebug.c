#include "pcdebug.h"

int PCDebug_IPCRead(uint32_t handle, void* dest, int len){
    
    int amt;
    int read_count;
    
    int total_read = 0;

    if (len != 0) {
        while (TRUE) {
            amt = len;
            if (len > 0x8000) amt = 0x8000;
            read_count = PCDebug_PCRead(handle, amt, dest);
            total_read += read_count;
            if (read_count == -1) break;
            (u8*)dest += read_count;
            len -= read_count;
            if (read_count < amt) return total_read;
            if (len == 0) return total_read;
        }
        total_read = -1;
    }
    return total_read;
}