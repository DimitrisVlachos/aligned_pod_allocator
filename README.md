# aligned_pod_allocator
A simple aligned allocator for POD data types

Basic example :
    std::size_t num = 1*1024*1024;
    double* s = sys::allocators::basic_aligned_pod_alloc::heap_new<double,sizeof(double)>(num);
    
    for (std::size_t i = 0;i < num;++i) {
        double c = (double)(i + 1);
        s[i] = c;
    }
    
    
    sys::allocators::basic_aligned_pod_alloc::heap_delete<double,sizeof(double)>(s);
    
    
    
