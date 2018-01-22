/*
 
 Aligned heap POD allocator v1
 
 
Copyright (c) 2018 Dimitris Vlachos https://github.com/DimitrisVlachos

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#ifndef __aligned_pod_allocator_hpp__
#define __aligned_pod_allocator_hpp__
#include <stdint.h>
 


namespace sys {
    namespace allocators {
        namespace basic_aligned_pod_alloc {
            #define basic_aligned_alloc_trace(___x___,...) /*printf( ___x___,__VA_ARGS__) */
            namespace _internal {
                static inline void wr_addr(void* dst,const void* src) {
                        uint8_t* w = (uint8_t*)dst;
                        const uint8_t* r = (const uint8_t*)src;
                        const uint8_t* r2 = r + sizeof(void*);

                        do {
                                *(w++) = *(r++);
                        } while (r < r2);
                }
            }
            
            template <class base_t,const ptrdiff_t alignment>
            static inline base_t* heap_new(const ptrdiff_t size) {
                    const ptrdiff_t hdr_sz = alignment  ;
                    const ptrdiff_t msize = (hdr_sz + alignment + size) & (~ (alignment-1));
                    void* ent;
                    void* aligned;
                    ptrdiff_t addr,saddr;

                    ent = (void*) new uint8_t[msize];
                    if (nullptr == ent) {
                        return nullptr;
                    }

                    basic_aligned_alloc_trace("Requested block size : %llu / alignment = %llu\n ",size,alignment);
                    basic_aligned_alloc_trace("Total size : %llu \n ",msize);
                    addr = (ptrdiff_t)ent;
                    saddr = addr;
                    basic_aligned_alloc_trace("Saddr diff  aligned :  %s\n ", ( saddr % alignment == 0) ? "yes" : "no" );
                    addr += alignment - (addr % alignment);
                    aligned = (void*)addr;
                    _internal::wr_addr(aligned,&saddr);

                    basic_aligned_alloc_trace("Ptr diff  : %llu \n ",(ptrdiff_t)addr - (ptrdiff_t)ent);
                    
                    void* ret = (void*)((uint8_t*)((uint8_t*)aligned + hdr_sz));

                    basic_aligned_alloc_trace("Base    p %p\n",ent);
                    basic_aligned_alloc_trace("Patched p %p\n",addr);
                    basic_aligned_alloc_trace("Ptr diff  aligned :  %s\n ", ( (ptrdiff_t)ret % alignment == 0) ? "yes" : "no" );

                    return (base_t*)ret;
            }
            
            template <class base_t,const uint64_t alignment>
            static void heap_delete(void* ent) {
                    const ptrdiff_t hdr_sz = alignment;
                    ptrdiff_t addr;

                    if (ent) {
                            _internal::wr_addr(&addr,(uint8_t*)ent - hdr_sz);
                            basic_aligned_alloc_trace("Delete  p %p\n",addr);
                            delete[] (base_t*)addr;
                    }
            }
            #undef basic_aligned_alloc_trace
        }
    }
}
#endif
