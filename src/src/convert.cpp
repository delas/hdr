#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main()
{

	cout << endl << "Convertitore database a formato per FANN" << endl <<
	                "========================================" << endl << endl;

	string images = "train-images.idx3-ubyte";
	string labels = "train-labels.idx1-ubyte";
	string destination = "training.data";

	int to_read = 150;
	int from = 0;
	cout << "Numero di esempi da leggere: "; cin >> to_read;
	cout << "A partire da: "; cin >> from;


	ofstream outfile;
	outfile.open(destination.c_str());
	FILE* img_file;
	FILE* lbl_file;

	if(!(img_file = fopen(images.c_str(), "rb")))
	{
		cout << "Impossibile aprire il file delle immagini!" << endl;
		return 0;
	}

	if(!(lbl_file = fopen(labels.c_str(), "rb")))
	{
		cout << "Impossibile aprire il file dei valori!" << endl;
		return 0;
	}

	fseek(img_file, 16, SEEK_SET);
	fseek(lbl_file, 8, SEEK_SET);
	if (from > 0)
	{
		fseek(img_file, 28*28*from, SEEK_CUR);
		fseek(lbl_file, from, SEEK_CUR);
	}

	unsigned char* img = new unsigned char[28*28];
	unsigned char* lbl = new unsigned char[to_read];
	fread(lbl, 1, to_read, lbl_file);

	outfile << to_read << " " << 28*28 << " " << 10 << endl;

	for(int i=0; i<to_read; i++)
	{
		int pixel;
		int value;
		value = lbl[i];

		fread(img, 1, 28*28, img_file);

		for(int j=0; j<28*28; j++)
		{
			pixel = img[j];
			outfile << pixel;
			if (j+1<28*28)
				outfile << " ";
		}
		outfile << endl;
		for(int j=0; j<10; j++)
		{
			if(j==value)
			{
				outfile << "0.9";
			}
			else
			{
				outfile << "0.1";
			}
			if(j<9)
			{
				outfile << " ";
			}
		}
		if (i+1<to_read)
			outfile << endl;
	}

	cout << "File degli esempi ("<< destination <<") generato!" << endl;

	delete[] img;
	delete[] lbl;

	fclose(img_file);
	fclose(lbl_file);
	outfile.close();

	return 0;
}
