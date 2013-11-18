#CC            = gcc
CC            = ccache g++
CXX		= ccache g++ 	
#CXX            = clang++
CFLAGS        = 
CPPFLAGS      = -O0 -Wall -I/home/ayu/local/include -std=c++11 -I/home/ayu/local/include/soci -I/usr/include/mysql -g -MD -I ../include
#CPPFLAGS      = -O2 -Wall -I/home/ayu/local/include -std=c++11 -I /lib/gcc/i686-pc-cygwin/4.7.3/include/c++/ -I /lib/gcc/i686-pc-cygwin/4.7.3/include/c++/i686-pc-cygwin/
DEST          = /usr/local/bin
LDFLAGS       = -L/home/ayu/local/lib/ -Wall 
LIBS          = -lsoci_core -lsoci_mysql
#OBJS          = query_test.o table_test_1.o table_test_2.o new_table.o
OBJS          = query_test.o Table1.o Table2.o
PROGRAM       = query_test

all:            $(PROGRAM)

$(PROGRAM):     $(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM) $(CFLAGS)

clean:;	rm -f *.o *~ *.d $(PROGRAM)

-include *.d

