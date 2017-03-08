CC=${CROSS_COMPILE}gcc
CFLAGS= -g -Wall
TARGET=libeasel_ES920.so
VERSION=0.0.1
TARGET_ROOTFS := ${CPS_SDK_INSTALL_FULLDIR}/${CPS_SDK_ROOTFS}

ifeq ($(CC),gcc)
	INCLUDEPATH = -I.
	INSTALL_DIR=/usr/local/lib/
else
	INCLUDEPATH = -I${CPS_SDK_ROOTDIR}/lib/include
	INSTALL_DIR= ${TARGET_ROOTFS}/usr/local/lib/
endif

all:${TARGET}.${VERSION}	

${TARGET}.${VERSION}:libeasel_ES920_func.o libeasel_ES920_wrap.o libeasel_ES920_test_rs232c.o	

libeasel_ES920_func.o:
	${CC} libeasel_ES920_func.c -c -fPIC -o libeasel_ES920_func.o ${INCLUDEPATH}
	
libeasel_ES920_wrap.o: 
	${CC} libeasel_ES920_wrap.c -c -fPIC -o libeasel_ES920_wrap.o ${INCLUDEPATH}

libeasel_ES920_test_rs232c.o: 
	${CC} libeasel_ES920_test_rs232c.c -c -fPIC -o libeasel_ES920_test_rs232c.o ${INCLUDEPATH}

${TARGET}.${VERSION}: 
	${CC} -shared -O2 -Wl,-soname,${TARGET} -o ${TARGET}.${VERSION} libeasel_ES920_func.o libeasel_ES920_wrap.o libeasel_ES920_test_rs232c.o

install:
	cp ${TARGET}.${VERSION} ${INSTALL_DIR}
	@if [ "${CC}" = "gcc" ] ; then \
		ldconfig ; \
	else \
		CURRENTDIR=`pwd` ; \
		cd ${INSTALL_DIR} ; \
		if [ -f ${TARGET} ] ; then \
			rm ${TARGET} ; \
		fi; \
		ln -s ${TARGET}.${VERSION} ${TARGET} ; \
		cd ${CURRENTDIR} ; \
	fi

uninstall:
	rm ${INSTALL_DIR}/${TARGET}.*
	ldconfig

clean:
	rm -f *.o *.so *.so.*
