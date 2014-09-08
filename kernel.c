__kernel void foo(
    __global int* data,
    __global int* result){
    int i = get_global_id(0);
    result[i] = data[i] % 2 == 0 ? data[i] : 0;
};

// Computes the accumulated sum of an array, example:
// scanSum [1 1 3 2 4 6 1 2] = [1 2 5 7 11 17 18 20]
__kernel void scanSumLocal(
    __global int* data,
    __local int* loc) {
    int lid = get_local_id(0);
    int gid = get_global_id(0);
    int group_size = get_local_size(0);

    loc[lid] = data[gid];
    barrier(CLK_LOCAL_MEM_FENCE);

    int a = 0;
    for (a=0; group_size>>=1; ++a){
        int k = (lid >> a << (a+1)) + (1<<a);
        int x = k - 1;
        int y = k + (lid&((1<<a)-1));
        loc[y] = loc[x] + loc[y];
        barrier(CLK_LOCAL_MEM_FENCE);
    };

    data[gid] = loc[lid];
}

__kernel void scanSumGlobal(__global int* data, int a){
    int gid = get_global_id(0);
    int k = (gid >> a << (a+1)) + (1<<a);
    int x = k - 1;
    int y = k + (gid&((1<<a)-1));
    data[y] = data[x] + data[y];
}

/*
// On host:
void scanSum(size_t local_work_size, cl_mem data_buff, size_t data_size, int* data){
    int i;
    size_t half_local_work_size = local_work_size/2;
    clSetKernelArg(scanSumLocalKernel, 0, sizeof(cl_mem), &data_buff); 
    clSetKernelArg(scanSumLocalKernel, 1, sizeof(cl_mem), NULL); 
    clEnqueueNDRangeKernel(cl.queue,scanSumLocalKernel,1,NULL,&data_size,&half_local_work_size,0,NULL,NULL);
    clEnqueueBarrier(cl.queue);
    for (i=log2(half_local_work_size); i<=log2(data_size); ++i){
        clSetKernelArg(scanSumGlobalKernel, 0, sizeof(cl_mem), &data_buff); 
        clSetKernelArg(scanSumGlobalKernel, 1, sizeof(int), &i); 
        clEnqueueNDRangeKernel(cl.queue,scanSumGlobalKernel,1,NULL,&data_size,&half_local_work_size,0,NULL,NULL);
        clEnqueueBarrier(cl.queue);
    };
};
*/



/*
f(00,00) = (00,01) | Correct: 1 1
f(00,01) = (02,03) | Correct: 1 1
f(00,02) = (04,05) | Correct: 1 1
f(00,03) = (06,07) | Correct: 1 1
f(00,04) = (08,09) | Correct: 1 1
f(00,05) = (10,11) | Correct: 1 1
f(00,06) = (12,13) | Correct: 1 1
f(00,07) = (14,15) | Correct: 1 1
f(01,00) = (01,02) | Correct: 1 1
f(01,01) = (01,03) | Correct: 1 1
f(01,02) = (05,06) | Correct: 1 1
f(01,03) = (05,07) | Correct: 1 1
f(01,04) = (09,10) | Correct: 1 1
f(01,05) = (09,11) | Correct: 1 1
f(01,06) = (13,14) | Correct: 1 1
f(01,07) = (13,15) | Correct: 1 1
f(02,00) = (03,04) | Correct: 1 1
f(02,01) = (03,05) | Correct: 1 1
f(02,02) = (03,06) | Correct: 1 1
f(02,03) = (03,07) | Correct: 1 1
f(02,04) = (11,12) | Correct: 1 1
f(02,05) = (11,13) | Correct: 1 1
f(02,06) = (11,14) | Correct: 1 1
f(02,07) = (11,15) | Correct: 1 1
*/
