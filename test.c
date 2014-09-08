// :map r :w!<cr>:!clear;clang -framework OpenCL % -o %:r;./%:r<cr>
#define PROGRAM_FILE 

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <OpenCL/cl.h>
#include <math.h>
#include <time.h>

float now(){ return ((float)clock())/CLOCKS_PER_SEC; };
void mark(char* str){ 
    static float last = 0;
    if (last != 0) printf("%s: %fs\n",str,now() - last);
    last = now();
};

cl_program createProgram(cl_context context, const char* source_file){
    FILE* program_handle = fopen(source_file, "r");
    fseek(program_handle, 0, SEEK_END);
    size_t program_size = ftell(program_handle);
    rewind(program_handle);
    char* program_buffer = (char*)malloc(program_size + 1);
    fread(program_buffer, sizeof(char), program_size, program_handle);
    program_buffer[program_size] = '\0';
    cl_program program = clCreateProgramWithSource(
        context,1,(const char**)&program_buffer,&program_size,NULL);
    fclose(program_handle);
    free(program_buffer);
    return program;
};

typedef struct _CL {
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
} CL;

CL initCL(const char* source_file){
    CL cl;
    cl_int err;
    const int LOG_BUFFER_SIZE = 2048;
    char buffer[LOG_BUFFER_SIZE];
    clGetPlatformIDs(1, &cl.platform, NULL);
    clGetDeviceIDs(cl.platform, CL_DEVICE_TYPE_GPU, 1, &cl.device, NULL);
    cl.context = clCreateContext(NULL, 1, &cl.device, NULL, NULL, &err);
    cl.program = createProgram(cl.context, source_file);
    clBuildProgram(cl.program, 0, NULL, NULL, NULL, NULL);
    clGetProgramBuildInfo(cl.program,cl.device,CL_PROGRAM_BUILD_LOG,LOG_BUFFER_SIZE,buffer,NULL);
    printf("%s",buffer);

    cl.queue = clCreateCommandQueue(cl.context, cl.device, 0, &err);

    return cl;
};

CL cl;
cl_kernel scanSumLocalKernel, scanSumGlobalKernel;
void sum(size_t local_work_size, cl_mem data_buff, size_t data_size, int* data){
    int i;
    clSetKernelArg(scanSumLocalKernel, 0, sizeof(cl_mem), &data_buff); 
    clSetKernelArg(scanSumLocalKernel, 1, sizeof(cl_mem), NULL); 
    clEnqueueNDRangeKernel(cl.queue,scanSumLocalKernel,1,NULL,&data_size,&local_work_size,0,NULL,NULL);
    clEnqueueBarrier(cl.queue);
    for (i=log2(local_work_size); i<=log2(data_size); ++i){
        clSetKernelArg(scanSumGlobalKernel, 0, sizeof(cl_mem), &data_buff); 
        clSetKernelArg(scanSumGlobalKernel, 1, sizeof(int), &i); 
        clEnqueueNDRangeKernel(cl.queue,scanSumGlobalKernel,1,NULL,&data_size,&local_work_size,0,NULL,NULL);
        clEnqueueBarrier(cl.queue);
    };
    clFinish(cl.queue);
};


int main(){
    cl_int i, err;
    cl_ulong clulong;
    cl_mem data_buff, part_buff, res_buff;

    mark("");

    cl = initCL("kernel.c");

    mark("init cl");

    size_t data_size = pow(2,18);
    int data[data_size];
    int result[data_size];
    for (i=0; i<data_size; ++i)
        data[i] = 1;

    mark("init arrays");

    scanSumLocalKernel = clCreateKernel(cl.program, "scanSumLocal", &err);
    scanSumGlobalKernel = clCreateKernel(cl.program, "scanSumGlobal", &err);

    mark("create kernels");

    //clGetKernelWorkGroupInfo(kernel,device,CL_KERNEL_WORK_GROUP_SIZE,sizeof(clulong),&clulong,NULL);
    //printf("-- kernel work group size: %llu\n",clulong);

    //clGetKernelWorkGroupInfo(kernel,device,CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,sizeof(clulong),&clulong,NULL);
    //printf("-- kernel preferred work group size multiple: %llu\n",clulong);

    //clGetKernelWorkGroupInfo(kernel,device,CL_KERNEL_LOCAL_MEM_SIZE,sizeof(clulong),&clulong,NULL);
    //printf("-- kernel local mem size: %llu\n",clulong);

    data_buff = clCreateBuffer(cl.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(data), data, &err);
    part_buff = clCreateBuffer(cl.context, CL_MEM_WRITE_ONLY, sizeof(data), data, &err);
    res_buff = clCreateBuffer(cl.context, CL_MEM_WRITE_ONLY, sizeof(result), NULL, &err); 

    mark("create buffers");

    sum(8, data_buff, data_size, data);
    mark("execute scanSum kernel");

    clEnqueueReadBuffer(cl.queue,data_buff,CL_TRUE,0,sizeof(data),data,0,NULL,NULL);
    clFinish(cl.queue);

    mark("read data");

    for (i=0; i<16; ++i)
        printf("%.2d %.2d\n",i,data[i]);
    for (i=data_size-16; i<data_size; ++i)
        printf("%.2d %.2d\n",i,data[i]);

    mark("print size");

    clReleaseMemObject(data_buff);
    clReleaseMemObject(res_buff);
    clReleaseKernel(scanSumLocalKernel);
    clReleaseKernel(scanSumGlobalKernel);
    clReleaseCommandQueue(cl.queue);
    clReleaseProgram(cl.program);
    clReleaseContext(cl.context);

    mark("release stuff");

    return 0;
};




