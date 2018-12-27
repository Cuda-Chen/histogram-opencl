#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>

#ifdef __APPLE__
	#include <OpenCL/opencl.h>
#else
	#include <CL/cl.h>
#endif

#define MAX_SOURCE_SIZE (0x100000)

//#define PROGRAM_FILE "histogram.cl"
//#define KERNEL_FUNC "histogram"

using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;
using std::endl;
using std::vector;
using std::cerr;
using std::cout;

int main(int argc, char const *argv[])
{

	unsigned int *histogram_results;
	unsigned int i = 0, a, input_size;
	std::fstream inFile("input", std::ios_base::in);
	std::ofstream outFile("yyyyyy.out", std::ios_base::out);

	inFile >> input_size;
	unsigned int *image = new unsigned int[input_size];
	while(inFile >> a) {
		image[i++] = a;
	}

	cl_int err;
	cl_platform_id cpPlatform; // OpenCL platform
	cl_device_id device_id; // device ID
	cl_context context; // context
	cl_command_queue queue; // command queue
	cl_program program; // program
	cl_kernel kernel; // kernel

	// get all platforms
	err = clGetPlatformIDs(1, &cpPlatform, NULL);
	if(err != CL_SUCCESS)
	{
		cerr << "OpenCL platform not found" << endl;
		exit(1);
	}

	// get default device of the default platform (here I use GPU)
	err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	if(err != CL_SUCCESS)
	{
		cerr << "No OpenCL device found!" << endl;
		exit(1);
	}

	// create a context
	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	if(err != CL_SUCCESS)
	{
		cerr << "context failed to create" << endl;
		exit(1);
	}

	// create a command queue
	queue = clCreateCommandQueue(context, device_id, 0, &err);
	if(err != CL_SUCCESS)
	{
		cerr << "queue failed to create" << endl;
		exit(1);
	}

	// create the compute program from source buffer
	FILE *fp;
	char *source_str;
	size_t source_size;
	fp = fopen("histogram.cl", "r");
	if(!fp)
	{
		cerr << "error reading histogram.cl" << endl;
		exit(1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
			(const size_t *)&source_size, &err);
	if(err != CL_SUCCESS)
	{
		cerr << "error creating program" << endl;
		exit(1);
	}

	// build program
	err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if(err != CL_SUCCESS)
	{
		cerr << "error building program" << endl;
		cerr << "Error type: " << err << endl;
		char build_log[10000];
		cl_int error2;
		error2 =  clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 10000, build_log, NULL);
		cout << "log:" << endl << build_log << endl;
		exit(1);
	}

	// create kernel
	kernel = clCreateKernel(program, "histogram", &err);
	if(!kernel || err != CL_SUCCESS)
	{
		cerr << "error creating kernel" << endl;
		exit(1);
	}

	// stuffs loaded to device
	histogram_results = new unsigned int [256 * 3 * sizeof(unsigned int)];
	memset(histogram_results, 0x0, 256 * 3 * sizeof(unsigned int));
	cl_mem d_image = clCreateBuffer(context, CL_MEM_READ_ONLY, input_size * sizeof(unsigned int), NULL, NULL);
	cl_mem d_histogram_results = clCreateBuffer(context, CL_MEM_READ_WRITE, 256 * 3 * sizeof(unsigned int), NULL, NULL);

	// write input data to device
	err = clEnqueueWriteBuffer(queue, d_image, CL_TRUE, 0, input_size, image, 0, NULL, NULL);
	if(err != CL_SUCCESS)
	{
		cerr << "error writing d_image" << endl;
		exit(1);
	}

	// set the arguments to our computer kernel
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_image);	
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_histogram_results);	
	err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &input_size);
	if(err != CL_SUCCESS)
	{
		cerr << "error setting arguments" << endl;
		exit(1);
	}

	// execute the OpenCL kernel
	size_t global_item_size = input_size; // process the whole list (image)
	size_t local_item_size = 3;
	err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_item_size, NULL,
					0, NULL, NULL);
	if(err != CL_SUCCESS)
	{
		cerr << "error excuting kernel" << endl;
		exit(1);
	}


	// wait until the kernel is finish	
	clFinish(queue);

	// read the result from device
	err = clEnqueueReadBuffer(queue, d_histogram_results, CL_TRUE, 0, 256 * 3 * sizeof(unsigned int), 
					histogram_results, 0, NULL, NULL);

	//histogram_results = histogram(image, input_size);
	for(unsigned int i = 0; i < 256 * 3; ++i) {
		if (i % 256 == 0 && i != 0)
			outFile << std::endl;
		outFile << histogram_results[i]<< ' ';
	}

	inFile.close();
	outFile.close();

	// release OpenCL resources
	clReleaseMemObject(d_image);
	clReleaseMemObject(d_histogram_results);
	clReleaseProgram(program);
    	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	// release host memory
	delete [] image;
	delete [] histogram_results;
	free(source_str);

	return 0;
}
