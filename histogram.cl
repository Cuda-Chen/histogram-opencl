kernel void histogram(global unsigned int *image_data,
			global unsigned int *histogram_results,
			unsigned int _size)
{
	int index = get_global_id(0);
	//int index = get_local_id(0);
	global unsigned int *ptr;

	ptr = histogram_results;

	ptr += 256 * index;
}
