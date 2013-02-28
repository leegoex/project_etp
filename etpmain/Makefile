TARGET=etpmain
OBJECTS=main.o ETPImage.o ETPDetectImage.o trace.o ETPConfig.o DBConnection.o ETPUpload.o
INCDIR=/usr/local/include/ImageMagick
LIBDIR=/usr/local/lib

$(TARGET):$(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) -L$(LIBDIR) -lMagick++ -lsqlite3 -lcurl
	cp $(TARGET) /usr/bin

main.o:main.cpp
	g++ -c main.cpp -I$(INCDIR)

ETPImage.o:ETPImage.h ETPImage.cpp
	g++ -c ETPImage.cpp -I$(INCDIR)

ETPDetectImage.o:ETPDetectImage.h ETPDetectImage.cpp
	g++ -c ETPDetectImage.cpp

trace.o:trace.h trace.cpp
	g++ -c trace.cpp

ETPConfig.o:ETPConfig.h ETPConfig.cpp ETPDef.h
	g++ -c ETPConfig.cpp

ETPUpload.o:ETPUpload.h ETPUpload.cpp
	g++ -c ETPUpload.cpp

DBConnection.o:DBConnection.h DBConnection.cpp
	g++ -c DBConnection.cpp

clean:
	rm -f $(TARGET) $(OBJECTS)
