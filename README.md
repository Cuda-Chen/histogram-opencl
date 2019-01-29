# histogram-opencl
stupid histogram program with OpenCL

# How to Compile the Ordinary Version
```
$ g++ -o histogram image-histogram.cpp
$ ./histogram
```

# How to Compile the OpenCL Version
For Linux run the following command
```
$ g++ -std=c++11 -lOpenCL -o histogram_cl histogram_cl.cpp
$ ./histogram_cl
```

For macOS run the following command
```
$ g++ -std=c++11 -framework OpenCL -o histogram_cl histogram_cl.cpp
$ ./histogram_cl
```

# Note 
It seems that my desktop cannot compile this program. However,
I try the following command and it works:
```
$ g++ -std=c++11 -o histogram_cl histogram_cl.cpp -L/usr/local/cuda-9.0/lib64 -lOpenCL
$ ./histogram_cl
```
