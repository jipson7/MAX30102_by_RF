#------------------------------------------------------------------------------

SOURCE=main.cpp algorithm.cpp algorithm_by_RF.cpp
MYPROGRAM=algos

CC=g++

#------------------------------------------------------------------------------



all: $(MYPROGRAM)

$(MYPROGRAM): $(SOURCE)

	$(CC) -c $(SOURCE)
	$(CC) ./*.o -o $(MYPROGRAM)

clean:
	rm -f ./*.o
	rm -f $(MYPROGRAM)

sample: clean all
	./$(MYPROGRAM) < sample_param.txt
