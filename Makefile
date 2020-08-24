CC=clang
CXX=clang++
CFLAGS=-g -O0
CXXFLAGS=-g -O0
objs=snake2.o \
	 zhc.o
objs2=FourierSeries.o \
	  ComplexValue.o \
	  ComplexIntegral.o \
	  Epicycle.o \
	  zhc.o \
	  test.o
objs3=zhc.o \
	  sqliteTest.o
objs4=zhc.o \
	  DiaryCharactersSatistics.o \
	  utf8.o \
	  CountCharacters.o
allObjs=$(objs) $(objs2) $(objs3) $(objs4)
diaryCharactersSatistics.o : $(allObjs)
	$(CXX) $(objs4) -lsqlite3 -O0
snake.o : ./zhc.h
zhc.o : ./zhc.h
FourierSeries.o : ./FourierSeries.h
ComplexValue.o : ./ComplexValue.h
ComplexIntegral: ./ComplexIntegral.h
Epicycle.o : ./Epicycle.h
zhc.o : ./zhc.h
sqliteTest.o : ./zhc.h
utf8.o : utf8.h
DiaryCharactersSatistics.o : CountCharacters.h zhc.h utf8.h
./CountCharacters.o : zhc.h utf8.h 
.PHONY : clean
clean:
	rm -rf $(objs) $(objs2)
