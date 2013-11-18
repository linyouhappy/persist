#include "core/core.h"

int  core_cacheline = 0;

void core_init(void) {
    core_cpuinfo();
}

#if (( __i386__ || __amd64__ ) && ( __GNUC__ || __INTEL_COMPILER ))

static inline void cpuid(uint32_t i, uint32_t *buf);


#if ( __i386__ )
static inline void cpuid(uint32_t i, uint32_t *buf) {
    __asm__ (

    "    mov    %%ebx, %%esi;  "

    "    cpuid;                "
    "    mov    %%eax, (%1);   "
    "    mov    %%ebx, 4(%1);  "
    "    mov    %%edx, 8(%1);  "
    "    mov    %%ecx, 12(%1); "

    "    mov    %%esi, %%ebx;  "

    : : "a" (i), "D" (buf) : "ecx", "edx", "esi", "memory" );
}
#else /* __amd64__ */
static inline void cpuid(uint32_t i, uint32_t *buf) {
    uint32_t  eax, ebx, ecx, edx;

    __asm__ (
        "cpuid"

    : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) : "a" (i) );

    buf[0] = eax;
    buf[1] = ebx;
    buf[2] = edx;
    buf[3] = ecx;
}
#endif


/* auto detect the L2 cache line size of modern and widespread CPUs */
void core_cpuinfo(void) {
    u_char    *vendor;
    uint32_t   vbuf[5], cpu[4], model;

    vbuf[0] = 0;
    vbuf[1] = 0;
    vbuf[2] = 0;
    vbuf[3] = 0;
    vbuf[4] = 0;

    cpuid(0, vbuf);

    vendor = (u_char *) &vbuf[1];

    if (vbuf[0] == 0) {
        return;
    }

    cpuid(1, cpu);

    if (strcmp((const char *)vendor, (const char *)"GenuineIntel") == 0) {

        switch ((cpu[0] & 0xf00) >> 8) {

        /* Pentium */
        case 5:
            core_cacheline = 32;
            break;

        /* Pentium Pro, II, III */
        case 6:
            core_cacheline = 32;

            model = ((cpu[0] & 0xf0000) >> 8) | (cpu[0] & 0xf0);

            if (model >= 0xd0) {
                /* Intel Core, Core 2, Atom */
                core_cacheline = 64;
            }

            break;

        /*
         * Pentium 4, although its cache line size is 64 bytes,
         * it prefetches up to two cache lines during memory read
         */
        case 15:
            core_cacheline = 128;
            break;
        }

    } else if (strcmp((const char *)vendor, (const char *)"AuthenticAMD") == 0) {
        core_cacheline = 64;
    }
}

#else
void core_cpuinfo(void) { }
#endif
