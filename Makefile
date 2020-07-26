CC=clang
CXX=clang++
CFLAGS=-g -pthread
CXXFLAGS=-g -pthread
objs=snake2.o \
	 zhc.o
objs2=FourierSeries.o \
	  ComplexValue.o \
	  ComplexIntegral.o \
	  Epicycle.o \
	  zhc.o \
	  test.o
snake.out : $(objs) $(objs2)
	$(CXX) -g -pthread $(objs) -o a.out
	# $(CXX) -g $(objs2) -o test
	./a.out
snake.o : ./zhc.h
zhc.o : ./zhc.h
FourierSeries.o : ./FourierSeries.h
ComplexValue.o : ./ComplexValue.h
ComplexIntegral: ./ComplexIntegral.h
Epicycle.o : ./Epicycle.h
zhc.o : ./zhc.h
.PHONY : clean
clean:
	rm -rf $(objs) $(objs2)
