#include <stdio.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include "floatfann.h"

using namespace std;

#define print(x) cout << x << endl;

/* global variables definitions */
string images_file = "train-images.idx3-ubyte";
string labels_file = "train-labels.idx1-ubyte";
string network_filename = "network.nn";


void help(const char* cmd)
{
	print("Usage: " << cmd << " [options]")
	print("Options:")
	print("  --network <network-file>  Network filename")
	print("  --lbl <lbl-file>          Labels filename")
	print("  --img <img-file>          Images filename")
	print("  --help                    Display this information")
	print("")
}


int main(int argc, char* argv[])
{
	srandom((unsigned)time(0));

	/* header */
	cout << endl << "Network tester" << endl <<
	                "==============" << endl << endl;

	int item = (random()%55000)+1;
	fann_type *calc_out;
	fann_type input[28*28];
	struct fann *ann;
	FILE* img_file;
	FILE* lbl_file;

	/* parameter parsing */
	if (argc < 2)
	{
		print("Usage: " << argv[0] << " [options]")
		print("For more informations, use: " << argv[0] << " --help")
		return(1);
	}
	for (int i = 1; i < argc; i++)
	{
		if ((strcmp(argv[i], "--network")==0) && (i < argc-1))
		{
			network_filename = argv[i+1];
			i++;
		}
		else if ((strcmp(argv[i], "--lbl")==0) && (i < argc-1))
		{
			labels_file = argv[i+1];
			i++;
		}
		else if ((strcmp(argv[i], "--img")==0) && (i < argc-1))
		{
			images_file = argv[i+1];
			i++;
		}
		else if (strcmp(argv[i], "--help")==0)
		{
			help(argv[0]);
			return(0);
		}
	}

	ann = fann_create_from_file(network_filename.c_str());

	if(!(img_file = fopen(images_file.c_str(), "rb")))
	{
		print("Can't open images file!")
		return 0;
	}

	if(!(lbl_file = fopen(labels_file.c_str(), "rb")))
	{
		print("Can't open labels file!")
		return 0;
	}

	fseek(img_file, 16, SEEK_SET);
	fseek(img_file, 28*28*item, SEEK_CUR);
	
	fseek(lbl_file, 8, SEEK_SET);
	fseek(lbl_file, item, SEEK_CUR);

	unsigned char* lbl = new unsigned char;
	unsigned char* img = new unsigned char[28*28];
	fread(img, 1, 28*28, img_file);
	fread(lbl, 1, 1, lbl_file);

	int pixel;
	int label = *lbl;

	cout << "+----------------------------+" << endl;
	for (int i=0; i<28*28; i++)
	{
		pixel = img[i];
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

	calc_out = fann_run(ann, input);

	float out[2] = {-1, -1000};
	for (int i=0; i<10; i++)
	{
		if (out[1] <= calc_out[i])
		{
			out[0] = i;
			out[1] = calc_out[i];
		}
		cout << "P(" << i << ") = " << calc_out[i] << "; " << endl;
	}
	cout << endl << endl;

	if (out[0]==label)
		cout << "  *** CORRECT ***";
	else
		cout << "  --- WRONG ---";
	cout << endl << endl;

	cout << "Example number " << item << endl <<
	        "Expected  : " << label << endl <<
	        "Calculated: " << out[0] << endl;

	fann_destroy(ann);

	delete[] img;
	delete[] lbl;

	fclose(img_file);
	fclose(lbl_file);

	return 0;
}
