CFLAGS=-Wall -O3 -Wconversion -fPIC
CC=gcc
CXXC=g++ -Wall -march=native
FLAGS=-O3 -Wall -march=native
BLASLIB=blas/blas.a
IFLAGS=-I$(OPF_DIR)/include -I$(OPF_DIR)/include/util 
FANN=$(PWD)/fann-2.0.0
FANNLIB=$(FANN)/lib/libfann.a

all: libopf svm-train svm-predict svm.o svm_linear-train svm_linear-predict tron.o linear.o blas/blas.a mysvm ann-mlp kohonen knn statistics opf2svm opf2lasvm opf2ann bayes assemblegnufile generatetxtfile4gnu svmtorch opf2svmtorch fann 

libopf:
	$(MAKE) $(OPF_DIR)

svm-train: src/svm-train.c src/svm.o
	$(CXXC) $(FLAGS) src/svm-train.c src/svm.o -o svm-train -lm

svm-predict: src/svm-predict.c src/svm.o
	$(CXXC) $(FLAGS) src/svm-predict.c src/svm.o -o svm-predict -lm

svm.o: src/svm.cpp src/svm.h
	$(CXXC) $(FLAGS) -c -w src/svm.cpp

svm_linear-train: tron.o linear.o src/svm_linear-train.c blas/blas.a
	$(CXX) $(CFLAGS) -o svm_linear-train src/svm_linear-train.c tron.o linear.o $(BLASLIB)

svm_linear-predict: tron.o linear.o src/svm_linear-predict.c blas/blas.a
	$(CXX) $(CFLAGS) -o svm_linear-predict src/svm_linear-predict.c tron.o linear.o $(BLASLIB)

tron.o: src/tron.cpp src/tron.h
	$(CXX) $(CFLAGS) -c -o tron.o src/tron.cpp

linear.o: src/linear.cpp src/linear.h
	$(CXX) $(CFLAGS) -c -o linear.o src/linear.cpp

blas/blas.a:
	cd blas; make OPTFLAGS='$(CFLAGS)' CC='$(CC)';

mysvm: 
	$(CC) -w $(FLAGS) $(IFLAGS) src/mysvm.c -L$(OPF_DIR)/lib -o mysvm -lm -lOPF

ann-mlp: src/ann-mlp.c
	$(CC) $(CFLAGS) $(IFLAGS) src/ann-mlp.c -L$(OPF_DIR)/lib -lOPF $(FANNLIB) -o ann-mlp -lm

statistics:
	$(CC) -w $(FLAGS) src/statistics.c -L$(OPF_DIR)/lib -lOPF -o statistics  -lm

opf2svm:
	$(CC) -w $(FLAGS) $(IFLAGS) src/opf2svm.c -L$(OPF_DIR)/lib -lOPF -o opf2svm  -lm

opf2lasvm:
	$(CC) -w $(FLAGS) $(IFLAGS) src/opf2lasvm.c -L$(OPF_DIR)/lib -lOPF -o opf2lasvm  -lm

kohonen:
	$(CC) -w $(FLAGS) $(IFLAGS) src/kohonen.c -L$(OPF_DIR)/lib -lOPF -o kohonen  -lm

knn:
	$(CC) -w $(FLAGS) $(IFLAGS) src/knn.c -L$(OPF_DIR)/lib -lOPF -o knn  -lm

opf2ann: 
	$(CC) -w $(FLAGS) $(IFLAGS) src/opf2ann.c -L$(OPF_DIR)/lib -lOPF -o opf2ann  -lm

bayes:
	$(CC) -w $(FLAGS) $(IFLAGS) src/bayes.c -L$(OPF_DIR)/lib -lOPF -o bayes  -lm

assemblegnufile:
	$(CC) utils/src/assemblegnufile.c -o utils/assemblegnufile

generatetxtfile4gnu:
	$(CC) utils/src/generatetxtfile4gnu.c -o utils/generatetxtfile4gnu

fann:
	cd fann-2.0.0; /bin/bash configure --prefix=$(PWD) --exec-prefix=$(PWD) --program-prefix=$(PWD); make; make install;

lasvm:
	cd lasvm-source; make;

opf2svmtorch:
	$(CC) -w $(FLAGS) $(IFLAGS) src/opf2svmtorch.c -L$(OPF_DIR)/lib -lOPF -o opf2svmtorch  -lm

svmtorch:
	cd SVMTorch; make;

clean:
	cd blas; make clean;
	cd fann-2.0.0; make distclean;
	cd SVMTorch; make clean;
	rm -f *~ opf2svmtorch svm-train svm-predict svm.o mysvm statistics src/svm.o svm_linear-train svm_linear-predict tron.o linear.o ann-mlp ann-rbf kohonen knn bayes opf2svm opf2lasvm opf2ann
