CFLAGS=-Wall -O3 -Wconversion -fPIC
CC=gcc
CXXC=g++ -Wall
FLAGS=-O3 -Wall
BLASLIB=blas/blas.a
#FANNLIB=/usr/local/lib/libfann.a
IFLAGS=-I$(OPF_DIR)/include -I$(OPF_DIR)/include/util 
FANN=$(PWD)/fann-2.0.0
FANNLIB=$(FANN)/lib/libfann.a

all: fann libopf svm-train svm-predict svm.o svm_linear-train svm_linear-predict tron.o linear.o blas/blas.a mysvm ann-mlp kohonen knn statistics opf2svm opf2lasvm opf2ann bayes assemblegnufile generatetxtfile4gnu

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

mysvm: libopf
	$(CC) -w $(FLAGS) $(IFLAGS) src/mysvm.c -L$(OPF_DIR)/lib -o mysvm -lm -lOPF

ann-mlp: src/ann-mlp.c
	$(CC) $(CFLAGS) $(IFLAGS) src/ann-mlp.c -L$(OPF_DIR)/lib -lOPF $(FANNLIB) -o ann-mlp -lm

statistics:
	$(CC) -w $(FLAGS) src/statistics.c -L$(OPF_DIR)/lib -lOPF -o statistics  -lm

opf2svm: libopf
	$(CC) -w $(FLAGS) $(IFLAGS) src/opf2svm.c -L$(OPF_DIR)/lib -lOPF -o opf2svm  -lm

opf2lasvm: libopf
	$(CC) -w $(FLAGS) $(IFLAGS) src/opf2lasvm.c -L$(OPF_DIR)/lib -lOPF -o opf2lasvm  -lm

kohonen: libopf
	$(CC) -w $(FLAGS) $(IFLAGS) src/kohonen.c -L$(OPF_DIR)/lib -lOPF -o kohonen  -lm

knn: libopf
	$(CC) -w $(FLAGS) $(IFLAGS) src/knn.c -L$(OPF_DIR)/lib -lOPF -o knn  -lm

opf2ann: libopf
	$(CC) -w $(FLAGS) $(IFLAGS) src/opf2ann.c -L$(OPF_DIR)/lib -lOPF -o opf2ann  -lm

bayes: libopf
	$(CC) -w $(FLAGS) $(IFLAGS) src/bayes.c -L$(OPF_DIR)/lib -lOPF -o bayes  -lm

assemblegnufile:
	$(CC) utils/src/assemblegnufile.c -o utils/assemblegnufile

generatetxtfile4gnu:
	$(CC) utils/src/generatetxtfile4gnu.c -o utils/generatetxtfile4gnu

fann:
	cd fann-2.0.0; make distclean; /bin/bash configure --prefix=$(FANN) --exec-prefix=$(FANN) --program-prefix=$(FANN); make; make install; exit;

clean:
	cd blas; make clean
	rm -f *~ svm-train svm-predict svm.o mysvm statistics src/svm.o svm_linear-train svm_linear-predict tron.o linear.o ann-mlp ann-rbf kohonen knn bayes opf2svm opf2lasvm opf2ann
