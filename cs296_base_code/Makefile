.SUFFIXES: .cpp .hpp

# Programs
SHELL 	= bash
CC     	= g++
LD	= ld
RM 	= rm
ECHO	= /bin/echo
CAT	= cat
PRINTF	= printf
SED	= sed
DOXYGEN = doxygen
######################################
# Project Name (generate executable with this name)
TARGET = cs296_base

# Project Paths
PROJECT_ROOT=$(HOME)/project296/cs296_base_code
EXTERNAL_ROOT=$(PROJECT_ROOT)/external
SRCDIR = $(PROJECT_ROOT)/src
OBJDIR = $(PROJECT_ROOT)/obj
BINDIR = $(PROJECT_ROOT)/bin
DOCDIR = $(PROJECT_ROOT)/doc
SCRIPTDIR = $(PROJECT_ROOT)/scripts
DATADIR = $(PROJECT_ROOT)/data
PLOTDIR = $(PROJECT_ROOT)/plots
BUILD2DDIR  = $(EXTERNAL_ROOT)/src
PROJECT_ROOT_TAR=$(HOME)/project296


# Library Paths
BOX2D_ROOT=$(EXTERNAL_ROOT)
GLUI_ROOT=/usr
GL_ROOT=/usr

#Libraries
LIBS = -lBox2D -lglui -lglut -lGLU -lGL

# Compiler and Linker flags
cxxflags.debug := -g -Wall
cxxflags.release := -g -O3 -Wall
CPPFLAGS = ${cxxflags.${BUILD_MYflag}}
#CPPFLAGS =-g -O3 -Wall 
CPPFLAGS+=-I $(BOX2D_ROOT)/include -I $(GLUI_ROOT)/include
LDFLAGS+=-L $(BOX2D_ROOT)/lib -L $(GLUI_ROOT)/lib

######################################

NO_COLOR=\e[0m
OK_COLOR=\e[1;32m
ERR_COLOR=\e[1;31m
WARN_COLOR=\e[1;33m
MESG_COLOR=\e[1;34m
FILE_COLOR=\e[1;37m

OK_STRING="[OK]"
ERR_STRING="[ERRORS]"
WARN_STRING="[WARNINGS]"
OK_FMT="${OK_COLOR}%30s\n${NO_COLOR}"
ERR_FMT="${ERR_COLOR}%30s\n${NO_COLOR}"
WARN_FMT="${WARN_COLOR}%30s\n${NO_COLOR}"
######################################

SRCS := $(wildcard $(SRCDIR)/*.cpp)
INCS := $(wildcard $(SRCDIR)/*.hpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)


.PHONY: all setup dist install doc clean distclean

all: setup $(BINDIR)/$(TARGET)

setup:
	@$(ECHO) "Setting up compilation..."
	@mkdir -p obj
	@mkdir -p bin

$(BINDIR)/$(TARGET): myinstall $(OBJS)
	@$(PRINTF) "$(MESG_COLOR)Building executable:$(NO_COLOR) $(FILE_COLOR) %16s$(NO_COLOR)" "$(notdir $@)"
	@$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LIBS) 2> temp.log || touch temp.err
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else $(PRINTF) $(OK_FMT) $(OK_STRING); \
	fi;
	@$(RM) -f temp.log temp.err

-include $(OBJS:.o=.d)

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(PRINTF) "$(MESG_COLOR)Compiling: $(NO_COLOR) $(FILE_COLOR) %25s$(NO_COLOR)" "$(notdir $<)"
	@$(CC) $(CPPFLAGS) -c $< -o $@ -MD 2> temp.log || touch temp.err
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else printf "${OK_COLOR}%30s\n${NO_COLOR}" "[OK]"; \
	fi;
	@$(RM) -f temp.log temp.err

doc:
	@$(ECHO) -n "Generating Doxygen Documentation ...  "
	@$(RM) -rf doc/html
	@$(DOXYGEN) $(DOCDIR)/Doxyfile 2 > /dev/null
	@$(ECHO) "Done"


debug_prof:clean $(BUILD2DDIR)/Box2D.tgz
	BUILD_MYflag=debug
		@cd $(BUILD2DDIR) && rm -rf Box2D
		@cd $(BUILD2DDIR) && tar -xvf Box2D.tgz 
		@cd $(BUILD2DDIR)/Box2D && mkdir -p build296
		@cd $(BUILD2DDIR)/Box2D/build296 && cmake -DCMAKE_BUILD_TYPE=Debug ../ && make  && make install
		@make
		@mkdir -p data
		@perf record -g ./bin/cs296_base 200
		@chmod 777 gprof2dot.py
		@perf script | ./gprof2dot.py -f perf | dot -Tpng -o ./data/deb_output200.png
		@rm -f perf.*
		
		
release_prof:clean $(BUILD2DDIR)/Box2D.tgz
	BUILD_MYflag=release
		@cd $(BUILD2DDIR) && rm -rf Box2D
		@cd $(BUILD2DDIR) && tar -xvf Box2D.tgz 
		@cd $(BUILD2DDIR)/Box2D && mkdir -p build296
		@cd $(BUILD2DDIR)/Box2D/build296 && cmake -DCMAKE_BUILD_TYPE=Release ../ && make  && make install
		@make
		@mkdir -p data
		@perf record -g ./bin/cs296_base 200
		@chmod 777 gprof2dot.py
		@perf script | ./gprof2dot.py -f perf | dot -Tpng -o ./data/rel_output200.png
		@rm -f perf.*

exe: myinstall  setup $(BINDIR)/$(TARGET)


		
dist:$(BUILD2DDIR)/Box2D.tgz
	@cd $(PROJECT_ROOT) && rm -rf obj && rm -rf bin
	@cd $(BUILD2DDIR)  && rm -rf Box2D
	@cd $(EXTERNAL_ROOT) && rm -rf include && rm -rf lib
	@cd $(PROJECT_ROOT_TAR) && tar -zcvf cs296_g31_project.tar.gz  cs296_base_code

myinstall:$(BUILD2DDIR)/Box2D.tgz
		@cd $(BUILD2DDIR) && tar -xvf Box2D.tgz 
		@cd $(BUILD2DDIR)/Box2D && mkdir -p build296
		@cd $(BUILD2DDIR)/Box2D/build296 && cmake  ../ && make  && make install
		
install: exe report 
		@$ rm -rf ../install
		@$ mkdir ../install
		@$ mkdir ../install/doc
		@$ cp -r bin/ ../install/
		@$ cp ./doc/profiling_report.html ./doc/g31_prof_report.pdf ../install/doc/
		
		
datadir : 
	@mkdir -p data


setup1:
	@mkdir -p plots
	@mkdir -p data

gen_csv:all
	@$(ECHO) -n "Generating lab05_g31_data.csv File"
	@chmod 777 $(SCRIPTDIR)/gen_data_csv.sh
	@$(SCRIPTDIR)/gen_data_csv.sh
	
plot:setup1 gen_csv
	@chmod 777 $(SCRIPTDIR)/*
	@$(ECHO) -n "Generating Plot1..."
	@awk -F "\"*,\"*" -f $(SCRIPTDIR)/g31_plot01.awk $(DATADIR)/lab05_g31_data.csv > $(DATADIR)/plot01.dat
	@gnuplot $(SCRIPTDIR)/g31_plot01.gpt
	@$(ECHO) -n "Generating Plot2..."
	@awk -F "\"*,\"*" -f $(SCRIPTDIR)/g31_plot02.awk $(DATADIR)/lab05_g31_data.csv > $(DATADIR)/plot02.dat
	@gnuplot $(SCRIPTDIR)/g31_plot02.gpt
	@$(ECHO) -n "Generating Plot3..."
	@awk -F "\"*,\"*" -f $(SCRIPTDIR)/g31_plot03.awk $(DATADIR)/lab05_g31_data.csv > $(DATADIR)/plot03.dat
	@gnuplot $(SCRIPTDIR)/g31_plot03.gpt
	@$(ECHO) -n "Generating Plot4..."
	@awk -F "\"*,\"*" -f $(SCRIPTDIR)/g31_plot04.awk $(DATADIR)/lab05_g31_data.csv > $(DATADIR)/plot04.dat
	@gnuplot $(SCRIPTDIR)/g31_plot04.gpt
	@$(ECHO) -n "Generating Plot5..."
	@awk -F "\"*,\"*" -f $(SCRIPTDIR)/g31_plot05.awk $(DATADIR)/lab05_g31_data.csv > $(DATADIR)/plot05.dat
	@gnuplot $(SCRIPTDIR)/g31_plot05.gpt
	@$(ECHO) -n "Generating Plot6..."
	@awk -F "\"*,\"*" -f $(SCRIPTDIR)/g31_plot06.awk $(DATADIR)/lab05_g31_data.csv > $(DATADIR)/plot06.dat
	@gnuplot $(SCRIPTDIR)/g31_plot06.gpt
	rm -f $(DATADIR)/*.dat
	@$(ECHO) -n "All Graphs are saved in plots folder"

report:$(DOCDIR)/report.tex $(DOCDIR)/photo1.png $(DOCDIR)/idea.png $(DOCDIR)/deb_output200.png $(DOCDIR)/rel_output200.png $(PLOTDIR)/g31_plot01.png $(PLOTDIR)/g31_plot02.png $(PLOTDIR)/g31_plot03.png $(PLOTDIR)/g31_plot04.png $(PLOTDIR)/g31_plot05.png $(PLOTDIR)/g31_plot06.png
	@pdflatex $(DOCDIR)/report.tex
	@bibtex $(PROJECT_ROOT)/report.aux
	@pdflatex $(DOCDIR)/report.tex
	@pdflatex $(DOCDIR)/report.tex
	@pdflatex $(DOCDIR)/report.tex
	@mv report.pdf $(DOCDIR)/report.pdf
	@rm report.*

clean:
	@$(ECHO) -n "Cleaning up..."
	@$(RM) -rf $(OBJDIR) *~ $(DEPS) $(SRCDIR)/*~ $(DATADIR)
	@$(RM) -rf $(BINDIR)
	@$(RM) -rf doc/html
	@$(ECHO) "Done"

distclean: clean
	@$(RM) -rf $(BINDIR) $(DOCDIR)/html
	@cd $(PROJECT_ROOT) && rm -rf obj && rm -rf bin
	@cd $(BUILD2DDIR)  && rm -rf Box2D
	@cd $(EXTERNAL_ROOT) && rm -rf include && rm -rf lib
	
