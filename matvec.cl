__kernel void foo(
    __global int* data,
    __global int* result){
    int i = get_global_id(0);
    result[i] = data[i] % 2 == 0 ? data[i] : 0;
        
};


