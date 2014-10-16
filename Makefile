CXXFLAGS=-Wall -O3 -g
BINARIES=mta mta_direct

# Where our library resides. It is split between includes and the binary
# library in lib
RGB_INCDIR=matrix/include
RGB_LIBDIR=matrix/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

all : $(BINARIES)

$(RGB_LIBRARY):
	$(MAKE) -C $(RGB_LIBDIR)

%.o : %.cc
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

mta : mta.o $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) mta.o -o $@ $(LDFLAGS)

mta_direct : mta_direct.o $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) mta_direct.o -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(BINARIES)
	$(MAKE) -C lib clean
