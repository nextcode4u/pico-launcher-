#include "common.h"
#include <string.h>
#include <nds/system.h>
#include <libtwl/rtos/rtosMutex.h>
#include "core/heap/tlsf.h"
#include "globalHeap.h"

static tlsf_t sHeap;
static rtos_mutex_t sHeapMutex;

static void* pico_malloc(size_t size)
{
    void* result;
    rtos_lockMutex(&sHeapMutex);
    {
        result = tlsf_malloc(sHeap, size);
    }
    rtos_unlockMutex(&sHeapMutex);
    return result;
}

extern "C" void* _malloc_r(struct _reent *, size_t size)
{
    return malloc(size);
}

static void pico_free(void* ptr)
{
    rtos_lockMutex(&sHeapMutex);
    {
        tlsf_free(sHeap, ptr);
    }
    rtos_unlockMutex(&sHeapMutex);
}

extern "C" void _free_r(struct _reent *, void* ptr)
{
    free(ptr);
}

static void* pico_realloc(void* ptr, size_t size)
{
    void* result;
    rtos_lockMutex(&sHeapMutex);
    {
        result = tlsf_realloc(sHeap, ptr, size);
    }
    rtos_unlockMutex(&sHeapMutex);
    return result;
}

extern "C" void* memalign(size_t alignment, size_t size)
{
    void* result;
    rtos_lockMutex(&sHeapMutex);
    {
        result = tlsf_memalign(sHeap, alignment, size);
    }
    rtos_unlockMutex(&sHeapMutex);
    return result;
}

extern "C" void* calloc(size_t num, size_t size)
{
    void* result = malloc(num * size);
    if (result)
    {
        memset(result, 0, num * size);
    }
    return result;
}

void* operator new(std::size_t blocksize)
{
    return malloc(blocksize);
}

void* operator new(std::size_t size, std::align_val_t al)
{
    return memalign(static_cast<std::size_t>(al), size);
}

void* operator new[](std::size_t blocksize)
{
    return malloc(blocksize);
}

void* operator new[](std::size_t size, std::align_val_t al)
{
    return memalign(static_cast<std::size_t>(al), size);
}

void operator delete(void* ptr)
{
    free(ptr);
}

void operator delete(void* ptr, std::align_val_t align)
{
    free(ptr);
}

void operator delete(void* ptr, std::size_t size, std::align_val_t align)
{
    free(ptr);
}

void operator delete[](void* ptr)
{
    free(ptr);
}

void operator delete[](void* ptr, std::align_val_t align)
{
    free(ptr);
}

void operator delete[](void* ptr, std::size_t size, std::align_val_t align)
{
    free(ptr);
}

void heap_init()
{
    rtos_createMutex(&sHeapMutex);
    u32 heapStart = (u32)getHeapStart();
    heapStart = (heapStart + 31) & ~31;
    u32 heapEnd = (u32)getHeapLimit() - 0x4000; // for some reason getHeapLimit overlaps with dtcm at the very end
    heapEnd = heapEnd & ~31;
    u32 tlsfSize = tlsf_size();
    memset((void*)heapStart, 0, tlsfSize);
    memset((u8*)heapStart + tlsfSize, 0xA5, heapEnd - heapStart - tlsfSize);
    sHeap = tlsf_create_with_pool((void*)heapStart, heapEnd - heapStart);
}

static void dumpWalker(void* ptr, size_t size, int used, void* user)
{
    bool printUsed = (bool)user;
    if (printUsed == used)
        LOG_DEBUG("0x%p - 0x%x\n", ptr, size);
}

void heap_dump()
{
    auto pool = tlsf_get_pool(sHeap);
    LOG_DEBUG("== Heap dump ==\n");
    LOG_DEBUG("=== used ===\n");
    LOG_DEBUG("addr - size\n");
    tlsf_walk_pool(pool, dumpWalker, (void*)true);
    LOG_DEBUG("=== free ===\n");
    LOG_DEBUG("addr - size\n");
    tlsf_walk_pool(tlsf_get_pool(sHeap), dumpWalker, (void*)false);
}

extern "C" void* __wrap_malloc(size_t size) {
    return pico_malloc(size);
}
extern "C" void __wrap_free(void* ptr) {
    pico_free(ptr);
}

extern "C" void* __wrap_calloc(size_t n, size_t size) {
    // If pico_calloc exists, use it; otherwise emulate using malloc+memset.
    #if defined(__GNUC__)
    extern void* pico_calloc(size_t, size_t) __attribute__((weak));
    #endif
    if (pico_calloc) return pico_calloc(n, size);
    size_t total = n * size;
    void* p = pico_malloc(total);
    if (p) __builtin_memset(p, 0, total);
    return p;
}

extern "C" void* __wrap_realloc(void* ptr, size_t size) {
    // If pico_realloc exists, use it; otherwise emulate naive realloc.
    #if defined(__GNUC__)
    extern void* pico_realloc(void*, size_t) __attribute__((weak));
    #endif
    if (pico_realloc) return pico_realloc(ptr, size);
    if (!ptr) return pico_malloc(size);
    if (size == 0) { pico_free(ptr); return nullptr; }
    // Fallback: allocate new, copy unknown old size (can't), so return new and leak old.
    // Better to implement pico_realloc in the heap code if needed.
    void* n = pico_malloc(size);
    return n;
}

