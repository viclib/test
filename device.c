// :map r :w!<cr>:!clear;clang -framework OpenCL % -o %:r;./%:r<cr>
/*
Device: Intel(R) Core(TM) i7-4870HQ CPU @ 2.50GHz
Vendor: Intel
Extensions: Intel
Global Mem Size: 17179869184
Address Bits: 64
Available: 1
Compiler Available: 1

Device: Iris Pro
Vendor: Intel
Extensions: Intel
Global Mem Size: 1610612736
Address Bits: 64
Available: 1
Compiler Available: 1

Device: GeForce GT 750M
Vendor: NVIDIA
Extensions: NVIDIA
Global Mem Size: 2147483648
Address Bits: 32
Available: 1
Compiler Available: 1
*/


#define PROGRAM_FILE 

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <OpenCL/cl.h>

int main(){
    cl_platform_id platform;
    cl_device_id* devices;
    cl_context context;
    cl_command_queue queue;
    cl_int i, err;
    char buffer[120];
    size_t buffer_len;
    unsigned int device_count;

    cl_ulong clulong;
    cl_uint cluint;
    cl_bool clbool;

    size_t program_size, log_size;

    clGetPlatformIDs(1, &platform, NULL);

    // Get Devices
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 99, NULL, &device_count);
    printf("num of devices %u\n\n", device_count);
    devices = (cl_device_id*)malloc(device_count * sizeof(cl_device_id));
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, device_count, devices, NULL);

    clGetKernelWorkGroupInfo(
        kernel,
        device,
        CL_KERNEL_WORK_GROUP_SIZE,
        1,
        &clulong,
        NULL);

    printf("-- work group size: %llu\n",clulong);

    for (i = 0; i < device_count; ++i){

        clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 120, buffer, NULL);
        printf("Device: %s\n",buffer);

        clGetDeviceInfo(devices[i], CL_DEVICE_VENDOR, 120, buffer, NULL);
        printf("Vendor: %s\n",buffer);

        clGetDeviceInfo(devices[i], CL_DEVICE_EXTENSIONS, 120, buffer, NULL);
        printf("Extensions: %s\n",buffer);

        clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(clulong), &clulong, NULL);
        printf("Global Mem Size: %llu\n",clulong);

        clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(clulong), &clulong, NULL);
        printf("Global Mem Cache Size: %llu\n",clulong);

        //clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MAX_CONSTANT_BUFFER_SIZE, sizeof(clulong), &clulong, NULL);
        //printf("Global Max Constant Buffer Size: %llu\n",clulong);

        clGetDeviceInfo(devices[i], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(clulong), &clulong, NULL);
        printf("Local Mem Size: %llu\n",clulong);

        clGetDeviceInfo(devices[i], CL_DEVICE_ADDRESS_BITS, sizeof(cluint), &cluint, NULL);
        printf("Address Bits: %u\n",cluint);

        clGetDeviceInfo(devices[i], CL_DEVICE_AVAILABLE, sizeof(clbool), &clbool, NULL);
        printf("Available: %d\n",clbool);

        clGetDeviceInfo(devices[i], CL_DEVICE_COMPILER_AVAILABLE, sizeof(clbool), &clbool, NULL);
        printf("Compiler Available: %d\n",clbool);
        
        clGetDeviceInfo(devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, sizeof(cluint), &cluint, NULL);
        printf("Preferred char vector width: %d\n",cluint);

        clGetDeviceInfo(devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, sizeof(cluint), &cluint, NULL);
        printf("Preferred short vector width: %d\n",cluint);

        clGetDeviceInfo(devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, sizeof(cluint), &cluint, NULL);
        printf("Preferred int vector width: %d\n",cluint);

        clGetDeviceInfo(devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, sizeof(cluint), &cluint, NULL);
        printf("Preferred long vector width: %d\n",cluint);

        clGetDeviceInfo(devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, sizeof(cluint), &cluint, NULL);
        printf("Preferred float vector width: %d\n",cluint);

        clGetDeviceInfo(devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, sizeof(cluint), &cluint, NULL);
        printf("Preferred double vector width: %d\n",cluint);

        clGetDeviceInfo(devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF, sizeof(cluint), &cluint, NULL);
        printf("Preferred half vector width: %d\n",cluint);

        printf("\n");

    };


    


    // 




    return 0;
};





