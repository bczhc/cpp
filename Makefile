CXX=clang++
objs=a.o \
	zhc.o
objs2=FourierSeries.o \
	ComplexValue.o \
	ComplexIntegral.o \
	Epicycle.o \
	zhc.o
a.out : $(objs) $(objs2)
	$(CXX) $(objs) -o a.out
	./a.out
a.o : ./zhc.h ./a.h
zhc.o : ./zhc.h
FourierSeries.o : ./FourierSeries.h
ComplexValue.o : ./ComplexValue.h
ComplexIntegral: ./ComplexIntegral.h
Epicycle.o : ./Epicycle.h
zhc.o : ./zhc.h
.PHONY : clean
clean:
	rm -rf $(objs) $(objs2)
