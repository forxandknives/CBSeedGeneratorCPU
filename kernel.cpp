//#include "Random.cuh"
//#include "CreateSeed.cuh"
//#include "Extents.cuh"
//
//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"
//#include "float.h"
//
//#include <stdio.h>
//
//#include <iostream>
//#include <cstdlib>
//#include <chrono>
//#include <vector>
//
//__device__ void dummy();
//
//__global__ void testFunction(uint32_t offset, uint8_t* outputArray, float* extents);
//
//int main()
//{
//    cudaError_t c;   
//
//    static const char* filename = "overlaps.txt";
//
//    FILE* file = fopen(filename, "w");
//
//    if (file == NULL) printf("Failed to open file.\n");
//
//    ////////////////////////////////////EXTENTS///////////////////////////////
//    const int32_t extentSize = 48960;
//    float* hostExtents = (float*)malloc(extentSize * sizeof(float));
//
//    PopulateRoomExtents(hostExtents);
//
//    float* deviceExtents;
//    c = cudaMalloc((void**)&deviceExtents, extentSize * sizeof(float));
//    if (c != cudaSuccess) {
//        printf("Failed to allocate memory for extents!\n");
//        exit(1);
//    }
//
//    c = cudaMemcpy(deviceExtents, hostExtents, extentSize * sizeof(float), cudaMemcpyHostToDevice);
//    if (c != cudaSuccess) {
//        printf("Failed to copy extents to gpu!\n");
//        exit(1);
//    }
//    ////////////////////////////////////EXTENTS///////////////////////////////   
//
//    int32_t offset = 0;
//
//    int32_t gridSize;
//    int32_t minGridSize;
//    int32_t blockSize;
//
//    //int32_t totalThreads = 524288;
//    constexpr int32_t totalThreads = 1048576*2;
//
//    cudaOccupancyMaxPotentialBlockSize(&minGridSize, &blockSize, testFunction, 0, totalThreads);
//
//    gridSize = (totalThreads + blockSize - 1) / blockSize;
//
//    uint32_t actualNumberOfThreads = gridSize * blockSize;   
//
//    uint8_t* output;
//    cudaMallocManaged(&output, actualNumberOfThreads * sizeof(output));
//
//    printf("MINGRIDSIZE: %d\n", minGridSize);
//    printf("GRIDSIZE: %d\n", gridSize);
//    printf("BLOCKSIZE: %d\n", blockSize);
//
//    int32_t kernels = 1;
//    int32_t kernels2 = ceilf(2147483648 / actualNumberOfThreads);   
//
//    std::vector<double> times(kernels);
//
//    printf("Launching %d Kernels with %d Threads Per Kernel!\n", kernels, actualNumberOfThreads);       
//
//    uint32_t overlapCounter = 0;
//
//    std::chrono::steady_clock::time_point start, end, start2, end2;
//
//    start2 = std::chrono::steady_clock::now();
//
//    #pragma unroll
//    for (int32_t i = 0; i < kernels; i++) {
//
//        start = std::chrono::steady_clock::now();
//
//        offset = i * actualNumberOfThreads;      
//
//        //DEBUG
//        testFunction << <1, 256>> > (offset, output, deviceExtents);        
//
//        //RELEASE
//        //testFunction << <gridSize, blockSize >> > (offset, output, deviceExtents);
//
//        cudaDeviceSynchronize();
//
//        //for (uint32_t j = 0; j < actualNumberOfThreads; j++) {
//        //    if (output[j] == 1) {
//        //        uint32_t thread = j + offset;
//        //        //printf("FOUND OVERLAP IN SEED %d\n", thread);
//        //        //(void)fprintf(file, "%d\n", thread);
//        //        overlapCounter++;                
//        //    }
//        //}
//
//        end = std::chrono::steady_clock::now();
//
//        double ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//
//        //printf("Kernel Stopped!\n");
//
//        times.push_back(ms);
//
//        printf("Run %d took %f milliseconds.\n", i, ms);
//    }
//
//    end2 = std::chrono::steady_clock::now();
//
//    double ms = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count();
//
//    printf("Total Runtme: %f milliseconds.\n", ms);
//
//    double total = 0.0;
//    for (double t : times) {
//        total += t;
//    }
//    total /= kernels;  
//    printf("Average RunTime: %f\n", total);
//
//    printf("Generated %d seeds.\n", (gridSize * blockSize) * kernels);
//
//    printf("Found %d start-372 overlaps.\n", overlapCounter);
//    (void)fclose(file);
//
//
//    c = cudaGetLastError();
//    if (c != cudaSuccess) {
//        printf("kernel error!\n");
//        printf("%s\n", cudaGetErrorString(c));
//        exit(1);
//    }
//
//    cudaFree(output);  
//
//    free(hostExtents);
//    cudaFree(deviceExtents);
//
//}
//
//__global__ void testFunction(uint32_t offset, uint8_t* outputArray, float* extents) {
//    //int block = blockIdx.x + blockIdx.y * gridDim.x;
//    //int threadNumber = block * (blockDim.x * blockDim.y) + (threadIdx.y * blockDim.x) + threadIdx.x;         
//    
//    uint32_t thread = offset + blockIdx.x * blockDim.x + threadIdx.x;    
//
//    __shared__ RoomTemplates rts[roomTemplateAmount];
//
//    //We will make 65 threads input 52 uint8's into array.
//    __shared__ uint8_t forest[3380];
//
//    //Each thread creates one RoomTemplate.
//    if (threadIdx.x < 96) {
//        CreateRoomTemplates(rts, threadIdx.x+1);    
//    }       
//
//    //Have 65 threads input forest data into shared memory.
//    if (threadIdx.x > 95 && threadIdx.x < 162) {       
//        PopulateForestData(forest, threadIdx.x - 96);       
//    }
//    __syncthreads();
//
//    //Thread 0 is the same as thread 1.
//    //Thread 2147483647 is broken and will infinite loop.
//    //Anything above thread 2147483647 is useless.
//    if (thread == 0 || thread > 2147483646) return;
//
//    //InitNewGame(thread, rts, extents, forest);   
//    outputArray[thread-offset] = CreateMap(thread, rts, extents, forest);        
//
//    //outputArray[threadIdx.x] = InitNewGame(thread, rts, extents, forest);   
//}
//
//__device__ void dummy() {};