PROG = restful_api
CFLAGS = -W -Wall -I ../.. -pthread -g -O0
SOURCES = main.c ../mongoose.c ../restApi.c

$(PROG):$(SOURCES)
	$(CC) -o $(PROG) $(SOURCES) $(CFLAGS)

clean:
	rm -rf $(PROG) *.exe *.dSYM *.obj *.exp .*o *.lib
