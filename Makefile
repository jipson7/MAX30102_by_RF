#------------------------------------------------------------------------------

SOURCE=main.cpp
MYPROGRAM=algos
MYINCLUDES=./includes

CC=g++

#------------------------------------------------------------------------------



all: $(MYPROGRAM)

$(MYPROGRAM): $(SOURCE)

	$(CC) -I $(MYINCLUDES) $(SOURCE) -o $(MYPROGRAM)

clean:
	rm -f $(MYPROGRAM)

sample: clean all
	./$(MYPROGRAM) < sample_param.txt
