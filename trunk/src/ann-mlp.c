#include "OPF.h"
#include "fann.h"

const unsigned int num_layers = 4;
const float desired_error = (const float) 0.0005;
const unsigned int max_epochs = 70000;
const unsigned int epochs_between_reports = 5000;

//function fann_test_on_data: executes the test phase and generates the resulting  classification file **************************
//Parameters
//P1: structure that contains the ann information obtained in the training phase
//P2: data to be evaluated
//P3: vector will store the resulting classification
//Returns the test phase accuracy
void fann_test_on_data(struct fann *ann, struct fann_train_data *test_data, unsigned int *classified){
	unsigned int fann_length_test_data = fann_length_train_data(test_data), num_output = fann_num_output_train_data(test_data), i, j;
	fann_type *output = NULL;
	float maior_prob;

	//testing phase
	for(i = 0; i < fann_length_test_data; i++){
		output = fann_run(ann,test_data->input[i]);
		
		//gets the label associated to the sample by the ANN in the test phase
		classified[i] = 0;
		maior_prob = output[0];
		for(j = 1; j < num_output; j++){
			if(output[j] > maior_prob){
				maior_prob = output[j];
				classified[i] = j; 
			}
		}
		classified[i]++;
	}

}

int main(int argc, char **argv){
	if(argc != 3){
		fprintf(stderr, "\nusage ann-mlp <training set> <testing set>\n");
		exit(-1);
	}
	timer tic, toc;
	struct fann *ann = NULL;
	struct fann_train_data *training_data = NULL, *testing_data = NULL;
	unsigned int fann_length_train_data_, num_input, num_output, num_neurons_hidden, fann_length_test_data;
	double trainingtime, testingtime;
	unsigned int *classified = NULL, i;
	char trainingtimefilename[256], testtimefilename[256], predictfilename[256];
	FILE *f = NULL;

	training_data = fann_read_train_from_file(argv[1]);
	testing_data = fann_read_train_from_file(argv[2]);

	if(!training_data || !testing_data )
		return -1;

	fann_length_train_data_ = fann_length_train_data(training_data);
	fann_length_test_data = fann_length_train_data(testing_data);
	num_input = fann_num_input_train_data(training_data);
	num_output = fann_num_output_train_data(training_data);
	//num_neurons_hidden = fann_length_train_data(training_data)/4;
	num_neurons_hidden = 8;
	ann = fann_create_standard(num_layers,num_input,num_neurons_hidden,num_neurons_hidden,num_output);
	fann_shuffle_train_data(training_data);
	classified = (unsigned int *)malloc(fann_length_test_data*sizeof(unsigned int));

	/*Training ***/
	gettimeofday(&tic,NULL); fann_train_on_data(ann,training_data,max_epochs,epochs_between_reports,desired_error); gettimeofday(&toc,NULL);
	trainingtime = ((toc.tv_sec-tic.tv_sec)*1000.0 + (toc.tv_usec-tic.tv_usec)*0.001)/1000.0;

	/*Testing ***/
	gettimeofday(&tic,NULL); fann_test_on_data(ann, testing_data, classified); gettimeofday(&toc,NULL);
	testingtime = ((toc.tv_sec-tic.tv_sec)*1000.0 + (toc.tv_usec-tic.tv_usec)*0.001)/1000.0;

	sprintf(trainingtimefilename,"%s.time",argv[1]);
	f = fopen(trainingtimefilename,"a");
	fprintf(f,"%f\n",(float)trainingtime);
	fclose(f);

	sprintf(testtimefilename,"%s.time",argv[2]);
	f = fopen(testtimefilename,"a");
	fprintf(f,"%f\n",(float)testingtime);
	fclose(f);

	sprintf(predictfilename,"%s.predict",argv[2]);
	f = fopen(predictfilename,"w");

	for (i = 0; i < fann_length_test_data; i++)
		fprintf(f,"%d\n",classified[i]);
	fclose(f);

	fann_destroy_train(training_data);
	fann_destroy_train(testing_data);
	fann_destroy(ann);
	free(classified);

	return 0;
}
