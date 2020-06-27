/*
 * grav_run: Ankit Verma, 2020
 *
 * This file runs the merge sorting on GPU
 *
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

#include <cuda_runtime.h>
#include "cuda_sorting.cuh"

#define GPU_THREAD_NUM		1024

void export_gpu_outputs(bool verbose);

cudaEvent_t start;
cudaEvent_t stop;
#define START_TIMER() {                         \
      CUDA_CALL(cudaEventCreate(&start));       \
      CUDA_CALL(cudaEventCreate(&stop));        \
      CUDA_CALL(cudaEventRecord(start));        \
    }

#define STOP_RECORD_TIMER(name) {                           \
      CUDA_CALL(cudaEventRecord(stop));                     \
      CUDA_CALL(cudaEventSynchronize(stop));                \
      CUDA_CALL(cudaEventElapsedTime(&name, start, stop));  \
      CUDA_CALL(cudaEventDestroy(start));                   \
      CUDA_CALL(cudaEventDestroy(stop));                    \
    }


/*******************************************************************************
 * Function:        chech_args
 *
 * Description:     Checks for the user inputs arguments to run the file
 *
 * Arguments:       int argc, char argv
 *
 * Return Values:   0
*******************************************************************************/
int check_args(int argc, char **argv){
	if (argc != 3){
        // printf("Usage: ./grav [depth] [thread_per_block] \n");
        printf("Usage: ./grav [depth] [verbose: 0/1]\n");
        return 1;
    }
    return 0;
}

/*******************************************************************************
 * Function:        time_profile_gpu
 *
 * Description:     RUNS the GPU code
 *
 * Arguments:       bool verbose: If true then it will prints messages on the c
 *                  console
 *
 * Return Values:   GPU computational time
*******************************************************************************/
void time_profile_gpu(bool verbose){

	float gpu_time_sorting = 0;
	float gpu_time_indata_cpy = 0;
	float gpu_time_outdata_cpy = 0;

	cudaError err;

	START_TIMER();
		cuda_cpy_input_data();
	STOP_RECORD_TIMER(gpu_time_indata_cpy);


	START_TIMER();
		cudacall_merge_sort(GPU_THREAD_NUM);
	STOP_RECORD_TIMER(gpu_time_sorting);
    err = cudaGetLastError();
    if (cudaSuccess != err){
        cerr << "Error " << cudaGetErrorString(err) << endl;
    }else{
    	if(verbose)
        	cerr << "No kernel error detected" << endl;
    }

    START_TIMER();
		cuda_cpy_output_data();
	STOP_RECORD_TIMER(gpu_time_outdata_cpy);
	
	for(unsigned int i = 0; i<arr_len; i++){
        cout << gpu_out_arr[i] << endl;
    }

	if(verbose){
		printf("GPU Input data copy time: %f ms\n", gpu_time_indata_cpy);
	    printf("GPU Fill vertices: %f ms\n", gpu_time_sorting);
		printf("GPU Output data copy time: %f ms\n", gpu_time_outdata_cpy);
	}
}


/*******************************************************************************
 * Function:        init_vars
 *
 * Description:     This function initializes global variables. This should be
 *					the first function to be called from this file.
 *
 * Arguments:       unsigned int depth: The maximum depth of the icosphere
 *					float r: The radius of sphere
 *
 * Return Values:   None.
 *
*******************************************************************************/
void init_vars(unsigned int len){
	arr_len = len;
    cpu_arr = (int *)malloc(arr_len*sizeof(float));
    srand(0);
    for(unsigned int i = 0; i<arr_len; i++){
        cpu_arr[i] = rand()%100;
        // cout << cpu_arr[i] << endl;
    }
}

/*******************************************************************************
 * Function:        main
 *
 * Description:     Run the main function
 *
 * Arguments:       int argc, char argv
 *
 * Return Values:   int 1 if code executes successfully else 0.
*******************************************************************************/
int main(int argc, char **argv) {

	if(check_args(argc, argv))
		return 0;

	int len = atoi(argv[1]);

	bool verbose = (bool)atoi(argv[2]);
	
	if(verbose)
		cout << "Verbose ON" << endl;
	else
		cout << "Verbose OFF" << endl;

	init_vars(len);

	if(verbose)
		cout << "\n----------Running GPU Code----------\n" << endl;
	
	time_profile_gpu(verbose);

	export_gpu_outputs(verbose);
	
	free(cpu_arr);
	free_gpu_memory();

    return 1;
}

/*******************************************************************************
 * Function:        export_gpu_outputs
 *
 * Description:     Exports the gpu_vertices, gpu_sorted_vertices, and gpu_potentials
 *
 * Arguments:       bool verbose: If true then it will prints messages on the c
 *                  console
 *
 * Return Values:   none
*******************************************************************************/
void export_gpu_outputs(bool verbose){

    cout << "Exporting: gpu_arr.csv"<<endl;

    string filename2 = "results/gpu_arr.csv";
    ofstream obj_stream2;
    obj_stream2.open(filename2);
    obj_stream2 << "array" << endl;
    cout <<"-----------------------" << endl;
    for(unsigned int i=0; i< arr_len; i++){
        obj_stream2 << gpu_out_arr[i] << endl;
    }
    obj_stream2.close();
}