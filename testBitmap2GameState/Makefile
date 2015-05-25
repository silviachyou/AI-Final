CC = g++
INCLUDE = -I ../include/ -I ../include/opencv/ -I ../include/opencv2
LIB = -L ../lib/
OPT = -lopencv_core -lopencv_highgui -lopencv_imgproc

main:
	${CC} testB2GS.cpp ${INCLUDE} ${LIB} -o test ${OPT}
clean:
	rm -f test
