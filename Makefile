CC=g++
LIB=-lpthread
LIBDIR=-L/opt/local/lib 
INCDIR=-I. -I/opt/local/include
CFLAGS=-Wall -pedantic -O3
TARGETS=sdlab
SRCS=sdlab.cc sdlab_http.cc sdlab_signal.cc fftsg.cc sdlab_http_signal.cc sdlab_http_cmd.cc sdlab_http_status.cc sdlab_http_var.cc
OBJS=$(SRCS:.cc=.o)
DEPS=$(SRCS:.cc=.d)


all: $(TARGETS) $(OBJS)

$(TARGETS): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(INCDIR) $(LIBDIR) $(LIB)

.cc.o:
	$(CC) $(CFLAGS) -c -MMD -MP $< $(INCDIR)


.PHONY: clean
clean:
	$(RM) *~ $(TARGETS) $(OBJS) $(DEPS) *.stackdump

-include $(DEPS)
