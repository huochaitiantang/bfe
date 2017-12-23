crypt: crypt.cpp
	g++ $^ -o $@

clean:
	rm *.o
