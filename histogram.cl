kernel void histogram(global unsigned int *image_data,
			global unsigned int *histogram_results,
			unsigned int _size)
{
	int i = get_global_id(0);
	int j = get_global_id(1);
	int k = get_global_id(2);

	if(i < _size)
		histogram_results[image_data[i * 3]]++;

	histogram_results += 256;
	if((j + 1) < _size)
		histogram_results[image_data[j * 3 + 1]]++;

	histogram_results += 256;
	if((k + 2) < _size)
		histogram_results[image_data[k * 3 + 2]]++;
}
