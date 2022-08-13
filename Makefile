# Collin Bowers
# Makefile for Flappy Bird

CMP = gcc
MAIN = flappybird
FUNC = flappyfunc
EXEC = flappybird

$(EXEC): $(FUNC).o $(MAIN).o 
	$(CMP) $(FUNC).o $(MAIN).o gfx2.o -lX11 -lm -o $(EXEC)

$(FUNC).o: $(FUNC).c $(FUNC).h 
	$(CMP) -c $(FUNC).c -o $(FUNC).o 

$(MAIN).o: $(MAIN).c $(FUNC).h
	$(CMP) -c $(MAIN).c -o $(MAIN).o 

clean:
	rm $(FUNC).o 
	rm $(MAIN).o
	rm $(EXEC)