kernel void histogram(global unsigned int *image_data,
			global unsigned int *histogram_results,
			unsigned int _size)
{
	unsigned int index = get_global_id(0);
	//int index = get_local_id(0);
	//global unsigned int *ptr;

	histogram_results += 256 * index;
	for(unsigned int i = index; i < _size; i += 3)
	{
		histogram_results[image_data[i]]++;
	}
}
