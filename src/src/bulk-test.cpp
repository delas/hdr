#include <stdio.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include <string.h>
#include "floatfann.h"

using namespace std;

#define print(x) cout << x << endl;

/* global variables definitions */
string images_file = "train-images.idx3-ubyte";
string labels_file = "train-labels.idx1-ubyte";
string network_filename = "network.nn";
int tests_number = 0;
int examples_skip = 0;


void help(const char* cmd)
{
	print("Usage: " << cmd << " [options]")
	print("Options:")
	print("  --network <network-file>  Network filename")
	print("  --lbl <lbl-file>          Labels filename")
	print("  --img <img-file>          Images filename")
	print("  --tot <tests-no>          Number of test to do")
	print("  --skip <starting-from>    Number of image to skip from begin")
	print("  --help                    Display this information")
	print("")
}


int main(int argc, char* argv[])
{
	srandom((unsigned)time(0));

	/* header */
	cout << endl << "Network Bulk Tester" << endl <<
	                "===================" << endl << endl;


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
		else if ((strcmp(argv[i], "--tot")==0) && (i < argc-1))
		{
			stringstream ss;
			ss << argv[i+1];
			ss >> tests_number;
			i++;
		}
		else if ((strcmp(argv[i], "--skip")==0) && (i < argc-1))
		{
			stringstream ss;
			ss << argv[i+1];
			ss >> examples_skip;
			i++;
		}
		else if (strcmp(argv[i], "--help")==0)
		{
			help(argv[0]);
			return(0);
		}
	}



	fann_type *calc_out;
	fann_type input[28*28];
	unsigned char* lbl = new unsigned char;
	unsigned char* img = new unsigned char[28*28];
	struct fann *ann = fann_create_from_file(network_filename.c_str());

	FILE* img_file;
	FILE* lbl_file;

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

	int done = 0;
	int passed = 0;
	int console_columns = 80;
	int step = tests_number / (console_columns-2);

	if (tests_number > (console_columns - 2))
		cout << "Testing...\n[";
	while(tests_number>0)
	{
		tests_number--;

		int item = (random()%55000)+examples_skip;

		fseek(img_file, 16, SEEK_SET);
		fseek(img_file, 28*28*item, SEEK_CUR);
		
		fseek(lbl_file, 8, SEEK_SET);
		fseek(lbl_file, item, SEEK_CUR);

		fread(img, 1, 28*28, img_file);
		fread(lbl, 1, 1, lbl_file);

		for (int i=0; i<28*28; i++)
		{
			input[i] = img[i];
		}

		calc_out = fann_run(ann, input);

		float out[2] = {-1, -1000};
		for (int i=0; i<10; i++)
		{
			if (out[1] <= calc_out[i])
			{
				out[0] = i;
				out[1] = calc_out[i];
			}
		}
		if (out[0]==*lbl)
		{
			passed++;
		}
		done++;

		if ((tests_number > (console_columns - 2)) && (done % step == 0))
		{
			cout << "=";
			cout.flush();
		}
	}
	if (done > (console_columns - 2))
		cout << "]" << endl << endl;

	cout << "Test done: " << done << endl;
	cout << "Test passed: " << passed << endl;
	if (done > 0)
		cout << (int)(passed * 100 / done) << "% of passed test" << endl;

	fann_destroy(ann);

	delete[] img;
	delete[] lbl;

	fclose(img_file);
	fclose(lbl_file);

	return 0;
}
