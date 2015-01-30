#include <iostream>
#include <IL/il.h>
#include <IL/ilu.h>
#include "floatfann.h"

using namespace std;


int main(int argc, char* argv[])
{
	ILuint image;
	unsigned char* orig_data;
	fann_type input[28*28];
	fann_type *calc_out;

	ilInit();
	ilGenImages(1, &image);
	ilBindImage(image);
	ilLoadImage(argv[1]);
	iluScale(28, 28, 24);

	orig_data = ilGetData();
	int pixel;
	cout << "+----------------------------+" << endl;
	for (int i = 0; i < 28*28; i++)
	{
		pixel = orig_data[i*3];
		input[i] = pixel;

		if (i%28==0) cout << "|";
		if (pixel > 0)
			if (pixel > 127)
				cout << "X";
			else
				cout << ".";
		else
			cout << " ";
		if (i%28==27) cout << "|" << endl;
	}
	cout << "+----------------------------+" << endl;


	struct fann *ann = fann_create_from_file("net");
	calc_out = fann_run(ann, input);

	float out[2] = {-1, -1000};
	for (int i=0; i<10; i++)
	{
		if (out[1] <= calc_out[i])
		{
			out[0] = i;
			out[1] = calc_out[i];
		}
		//cout << "P(" << i << ") = " << calc_out[i] << "; " << endl;
	}
	//cout << endl << endl;
	cout << "Calcolato: " << out[0] << endl;

	fann_destroy(ann);

	ilDeleteImages(1, &image);
}
