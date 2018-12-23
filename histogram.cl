kernel void histogram(global unsigned int *image_data,
			global unsigned int *histogram_results,
			unsigned int _size)
{
	int i = get_global_id(0);

	if(i < _size)
	{
		if(i % 3 == 0)
			ptr[image_data[i]]++; // histogram of R
		else if(i % 3 == 1)
			ptr[256 + image_data[i]]++; // histogram of G
		else if(i % 3 == 2)
			ptr[256 + 256 + image_data[i]]++; // histogram of B
	}
}
