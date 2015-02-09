#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include "fann.h"

using namespace std;

#define PROG_VERSION 0.1
#define print(x) cout << x << endl;

/* global variables definitions */
string error_data_log = "error_data.dat";
string network_file = "network.nn";
string training_file = "";

/* network topology */
const unsigned int num_input = 28*28;
const unsigned int num_output = 10;
const unsigned int num_layers = 3;
const unsigned int num_neurons_hidden = 300;

int FANN_API test_callback(struct fann *ann, struct fann_train_data *train,
                           unsigned int max_epochs, unsigned int epochs_between_reports,
                           float desired_error, unsigned int epochs)
{
	ofstream outfile;
	outfile.open(error_data_log.c_str(), ios::app);
	outfile << fann_get_MSE(ann) << endl;
	outfile.close();

	if (epochs % 50 == 0)
	{
		print(" > Saving network... ");
		fann_save(ann, network_file.c_str());
	}
	print("Epoch " << epochs << ", current mse: "<< fann_get_MSE(ann));
	return 0;
}



void help(const char* cmd)
{
	print("Usage: " << cmd << " [options]")
	print("Options:")
	print("  --mse <error>             Mean square error to reach")
	print("  --epoch <max>             Maximum epochs number")
	print("  --network <network-file>  Network file")
	print("  --train <train-file>      The training file")
	print("  --backprop-batch          Learning algorithm")
	print("  --backprop-incr           Learning algorithm")
	print("  --quickprop               Learning algorithm")
	print("  --help                    Display this information")
	print("")
}



int main(int argc, char* argv[])
{
	/* header */
	cout << endl << "Neural network trainer v. " << PROG_VERSION << endl <<
	                "=============================" << endl << endl;

	float desired_error;
	int max_epochs;
	int epochs_between_reports = 1;
	struct fann *ann;
	fann_train_enum learning_algorithm;

	/* parameter parsing */
	if (argc < 2)
	{
		print("Usage: " << argv[0] << " [options]")
		print("For more informations, use: " << argv[0] << " --help")
		return(1);
	}
	for (int i = 1; i < argc; i++)
	{
		if ((strcmp(argv[i], "--mse")==0) && (i < argc-1))
		{
			stringstream ss;
			ss << argv[i+1];
			ss >> desired_error;
			i++;
		}
		else if ((strcmp(argv[i], "--epoch")==0) && (i < argc-1))
		{
			stringstream ss;
			ss << argv[i+1];
			ss >> max_epochs;
			i++;
		}
		else if ((strcmp(argv[i], "--network")==0) && (i < argc-1))
		{
			network_file = argv[i+1];
			i++;
		}
		else if ((strcmp(argv[i], "--train")==0) && (i < argc-1))
		{
			training_file = argv[i+1];
			i++;
		}
		else if (strcmp(argv[i], "--backprop-batch")==0)
		{
			learning_algorithm = FANN_TRAIN_BATCH;
		}
		else if (strcmp(argv[i], "--backprop-incr")==0)
		{
			learning_algorithm = FANN_TRAIN_INCREMENTAL;
		}
		
		else if (strcmp(argv[i], "--quickprop")==0)
		{
			learning_algorithm = FANN_TRAIN_QUICKPROP;
		}
		else if (strcmp(argv[i], "--help")==0)
		{
			help(argv[0]);
			return(0);
		}
	}

	print("Training parameters:")
	print("   Desired error: " << desired_error)
	print("   Max epochs: " << max_epochs)
	if (network_file == "network.nn")
	{
		print("   Creating new network: network.nn")
		ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
	}
	else
	{
		print("   Using network file: " << network_file)
		ann = fann_create_from_file(network_file.c_str());
	}
	if (learning_algorithm == FANN_TRAIN_BATCH)
		print("   Learning algorithm: Backpropagation - batch")
	if (learning_algorithm == FANN_TRAIN_INCREMENTAL)
		print("   Learning algorithm: Backpropagation - incremental")
	if (learning_algorithm == FANN_TRAIN_QUICKPROP)
		print("   Learning algorithm: Quickprop")
	print("")

	fann_set_activation_function_hidden(ann, FANN_SIGMOID);
	fann_set_activation_function_output(ann, FANN_SIGMOID);

	fann_set_training_algorithm(ann, learning_algorithm);
	fann_set_learning_momentum(ann, 0.9);
	fann_set_learning_rate(ann, 1.3);

	fann_set_callback(ann, *(test_callback));

	fann_train_on_file(ann, training_file.c_str(), max_epochs, epochs_between_reports, desired_error);
	fann_save(ann, network_file.c_str());

	fann_destroy(ann);

	return 0;
}
