// TODO some functions to read from msr and write into msr

/** 
 * L1 d cache misses 
 * L1 d cache reference
 * L1 cache miss rate = L1 d cache misses / L1 d cache references
 * Total cache misses
 * Total cache references 
 * cache miss rate = Total cache misses / Total cache references
 * Some heuristic to decide whether to migrate the current process or not
*/


/***/

#define PERFEVTSELx_MSR_BASE   0x00000186
#define PMCx_MSR_BASE          0x000000c1      /* NB: write when evt disabled*/

#define PERFEVTSELx_USR        (1U << 16)      /* count in rings 1, 2, or 3 */
#define PERFEVTSELx_OS         (1U << 17)      /* count in ring 0 */
#define PERFEVTSELx_EN         (1U << 22)      /* enable counter */


/*Kernel has support for uint32 (u32 ?) and uint64 (u64 ?) */
// static void
// write_msr(uint32_t msr, uint64_t val)
// {
//     uint32_t lo = val & 0xffffffff;
//     uint32_t hi = val >> 32;
//     __asm __volatile("wrmsr" : : "c" (msr), "a" (lo), "d" (hi));
// }


// static uint64_t
// read_msr(uint32_t msr)
// {
//     uint32_t hi, lo;
//     __asm __volatile("rdmsr" : "=d" (hi), "=a" (lo) : "c" (msr));
//     return ((uint64_t) lo) | (((uint64_t) hi) << 32);
// }

#define L1_HIT 0x004101d1
#define L2_HIT 0x004102d1
#define L3_HIT 0x004104d1
#define L1_MISS 0x004108d1
#define L2_MISS 0x004110d1
#define L3_MISS 0x004120d1
#define L1D_HIT 0x00410143

#define ull unsigned long long

#define MSR_BASE 0x00000186

struct proc_msr {
    ull l1_hit;
    ull l2_hit;
    ull l3_hit;
    ull l1_miss;
    ull l2_miss;
    ull l3_miss;
};

static void write_msr(int msr, long long val) {
    ull lo = val & 0xffffffff;
    ull hi = val >> 32;
    wrmsr(msr, lo, hi);
}

static ull read_msr(int msr) {
    ull hi, lo;
    rdmsr(msr, lo, hi);
    return ((long long) lo) | (((long long) hi) << 32);
}

ull find_L1_HIT() {
    write_msr(MSR_BASE, L1_HIT);
    return read_msr(MSR_BASE);
}

ull find_L2_HIT() {
    write_msr(MSR_BASE, L2_HIT);
    return read_msr(MSR_BASE);
}   

ull find_L3_HIT() {
    write_msr(MSR_BASE, L3_HIT);
    return read_msr(MSR_BASE);
}   

ull find_L1_MISS() {
    write_msr(MSR_BASE + 1, L1_MISS);
    return read_msr(MSR_BASE + 1);
}

ull find_L2_MISS() {
    write_msr(MSR_BASE + 1, L2_MISS);
    return read_msr(MSR_BASE + 1);
}   

ull find_L3_MISS() {
    write_msr(MSR_BASE + 1, L3_MISS);
    return read_msr(MSR_BASE + 1);
}   

void get_proc_msr(struct proc_msr* p) {
    p->l1_hit = find_L1_HIT();
    p->l2_hit = find_L2_HIT();
    p->l3_hit = find_L3_HIT();
    p->l1_miss = find_L1_MISS();
    p->l2_miss = find_L2_MISS();
    p->l3_miss = find_L3_MISS();
}

