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
PROJECT_ROOT=.
EXTERNAL_ROOT=$(PROJECT_ROOT)/external
SRCDIR = $(PROJECT_ROOT)/src
OBJDIR = $(PROJECT_ROOT)/obj
BINDIR = $(PROJECT_ROOT)/bin
DOCDIR = $(PROJECT_ROOT)/doc
SCRIPTDIR = $(PROJECT_ROOT)/scripts
DATADIR = $(PROJECT_ROOT)/data
PLOTDIR = $(PROJECT_ROOT)/plots
BUILD2DDIR  = $(EXTERNAL_ROOT)/src
INSTALL_PATH = $(PROJECT_ROOT)/install


# Library Paths
BOX2D_ROOT=$(EXTERNAL_ROOT)
GLUI_ROOT=/usr
GL_ROOT=/usr

#Libraries
LIBS = -lBox2D -lglui -lglut -lGLU -lGL

# Compiler and Linker flags
CPPFLAGS =-g -O3 -Wall 
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


.PHONY: all setup doc clean distclean

all: $(PROJECT_ROOT)/external/lib/libBox2D.a setup $(BINDIR)/$(TARGET) report doc

$(PROJECT_ROOT)/external/lib/libBox2D.a :
	@cd external/src/; \
	 tar -xvf Box2D.tgz; \
	 cd Box2D; \
	 mkdir build296; \
	 cd build296; \
	 cmake ../ && make && make install


setup:
	@$(ECHO) "Setting up compilation..."
	@mkdir -p obj
	@mkdir -p bin


$(BINDIR)/$(TARGET): $(OBJS)
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


setupplot:
	@rm $(SRCDIR)/main.cpp
	@cp $(SRCDIR)/temp/main_plot.cpp $(SRCDIR)/main.cpp
	@mkdir -p data
	@mkdir -p plots

plot:setupplot	 all 
	@python3 ./scripts/g31_gen_csv.py
	@mv lab09_g31_data.csv ./data/
	@python3 ./scripts/g31_gen_plots.py
	@cp $(SRCDIR)/temp/main_sim.cpp $(SRCDIR)/main.cpp

		
install:all
	@mkdir -p $(INSTALL_PATH)
	@cp -r bin doc plots scripts src $(INSTALL_PATH)/
		
doc:
	@$(ECHO) -n "Generating Doxygen Documentation ...  "
	@$(RM) -rf doc/html
	@$(DOXYGEN) $(DOCDIR)/Doxyfile 2 > /dev/null
	@$(ECHO) "Done"


report:
	@mkdir -p data
	@pdflatex $(DOCDIR)/report.tex
	@bibtex $(PROJECT_ROOT)/report.aux
	@pdflatex $(DOCDIR)/report.tex
	@pdflatex $(DOCDIR)/report.tex
	@pdflatex $(DOCDIR)/report.tex
	@mv report.pdf $(DOCDIR)/report.pdf
	@rm report.*
	@python3 $(SCRIPTDIR)/g31_gen_html.py > ./doc/g31_lab09_report.html
	


dist:distclean $(BUILD2DDIR)/Box2D.tgz
	@cd $(PROJECT_ROOT)/.. && tar -zcvf cs296_g31_project.tar.gz  cs296_base_code

clean:
	@$(ECHO) -n "Cleaning up..."
	@$(RM) -rf $(OBJDIR) *~ $(DEPS) $(SRCDIR)/*~ $(DATADIR)
	@$(RM) -rf $(BINDIR)
	@$(RM) -rf doc/html
	@$(RM) -f doc/report.pdf
	@$(ECHO) "Done"

distclean: clean
	@$(RM) -rf $(BINDIR) $(DOCDIR)/html
	@cd $(PROJECT_ROOT) && rm -rf obj && rm -rf bin
	@cd $(BUILD2DDIR)  && rm -rf Box2D
	@cd $(EXTERNAL_ROOT) && rm -rf include && rm -rf lib
	
