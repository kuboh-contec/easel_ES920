#CC = gcc #selfbuild
CC		=	arm-linux-gnueabi-gcc #crossbuild
LD		=	arm-linux-gnueabi-ld
#LIBDIR		= 	-L${CPS_SDK_INSTALL_FULLDIR}/${CPS_SDK_ROOTFS}/usr/local/lib
LIBS		=	-leasel_ES920 -lSerialFunc

all:easel_ES920_through_test  

easel_ES920_through_test: easel_ES920_through_test.c 
	${CC} ${CFLAGS} -o ES920ThroughTest easel_ES920_through_test.c ${LIBDIR} ${LIBS}

clean:
	rm -f *.o CMM920*Test_*
