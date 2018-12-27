kernel void histogram(global unsigned int *image_data,
			global unsigned int *histogram_results,
			unsigned int _size)
{
	int index = get_global_id(0);
	global unsigned int *ptr;

	ptr = histogram_results;

	if(index % 3 == 0)
	{
		ptr[image_data[index]]++;
	}
	else if(index % 3 == 1)
	{
		ptr = ptr + 256;
		ptr[image_data[index]]++;
	}
	else if(index % 3 == 2)
	{
		ptr = ptr + 256 + 256;
		ptr[image_data[index]]++;
	}
}
