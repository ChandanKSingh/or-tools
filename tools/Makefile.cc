# OR_ROOT is the minimal prefix to define the root of or-tools, if we
# are compiling in the or-tools root, it is empty. Otherwise, it is
# $(OR_TOOLS_TOP)/ or $(OR_TOOLS_TOP)\\ depending on the platform. It
# contains the trailing separator if not empty.

# Let's discover something about where we run
# Let's discover something about where we run
ifeq ($(OS), Windows_NT)
SYSTEM = win
else
SYSTEM = unix
endif

# Define the OR_ROOT directory.
ifeq ($(OR_TOOLS_TOP),)
OR_ROOT =
else
ifeq "$(SYSTEM)" "win"
OR_ROOT = $(OR_TOOLS_TOP)\\
else
OR_ROOT = $(OR_TOOLS_TOP)/
endif
endif

# Useful directories.
BIN_DIR = $(OR_ROOT)bin
OBJ_DIR = $(OR_ROOT)objs
EX_DIR = $(OR_ROOT)examples
CPP_EX_DIR = $(OR_ROOT)examples$Scpp
CS_EX_DIR = $(OR_ROOT)examples$Scsharp
INC_EX_DIR = $(OR_ROOT).
INC_DIR = $(OR_ROOT)include
LIB_DIR = $(OR_ROOT)lib

CLR_DLL_NAME?=Google.OrTools

JAVAFLAGS = -Djava.library.path=$(LIB_DIR)

# Unix specific part.
ifeq ($(SYSTEM),unix)
  # Defines OR_TOOLS_TOP if it is not already defined.
  OR_TOOLS_TOP ?= $(shell pwd)
  OS = $(shell uname -s)
  ifeq ($(OS),Linux)
    CXX = g++
    LDFLAGS = \
-Wl,-rpath,"\$$ORIGIN" \
-Wl,-rpath,"\$$ORIGIN/../lib64" \
-Wl,-rpath,"\$$ORIGIN/../lib" \
-lz -lrt -lpthread
    LBITS = $(shell getconf LONG_BIT)
    ifeq ($(LBITS),64)
      PORT = Linux64
      ARCH = -DARCH_K8
      NETPLATFORM = anycpu
    else
      PORT = Linux32
      ARCH =
      NETPLATFORM = x86
    endif
    MONO = LD_LIBRARY_PATH=$(LIB_DIR):$(LD_LIBRARY_PATH) mono
    L = so
  endif # ifeq($(OS),Linux)
  ifeq ($(OS),Darwin) # Assume Mac Os X
    CXX = clang++
    LDFLAGS = \
-Wl,-rpath,@loader_path \
-Wl,-rpath,@loader_path/../lib \
-lz
    PORT = MacOsX64
    ARCH = -DARCH_K8
    NETPLATFORM = x64
    MONO = DYLD_FALLBACK_LIBRARY_PATH=$(LIB_DIR):$(DYLD_LIBRARY_PATH) mono
    L = dylib
  endif # ifeq($(OS),Darwin)
  LIB_PREFIX = lib
  PRE_LIB = -L$(OR_ROOT)lib -L$(OR_ROOT)lib64
  OR_TOOLS_LNK = $(PRE_LIB) -lprotobuf -lglog -lgflags -lCbcSolver -lCbc -lClp -lOsiClp -lOsi -lCoinUtils -lortools
  CVRPTW_LNK = $(PRE_LIB) -lcvrptw_lib -lprotobuf -lglog -lgflags -lCbcSolver -lCbc -lClp -lOsiClp -lCoinUtils -lortools
  DIMACS_LNK = $(PRE_LIB) -ldimacs -lgflags -lortools
  O = o
  E =
  OBJ_OUT = -o #
  EXE_OUT = -o #
  S = /
  CPSEP = :
  DEBUG = -O4 -DNDEBUG
  CXXFLAGS = -fPIC -std=c++11 $(DEBUG) -I$(INC_DIR) -I$(INC_EX_DIR) $(ARCH) -Wno-deprecated \
-DUSE_CBC -DUSE_CLP -DUSE_BOP -DUSE_GLOP
  DEL = rm -f
  CXX_BIN := $(shell command -v $(CXX) 2> /dev/null)
ifneq ($(JAVA_HOME),)
  JAVAC_BIN := $(shell command -v $(JAVA_HOME)/bin/javac 2> /dev/null)
  JAVA_BIN := $(shell command -v $(JAVA_HOME)/bin/java 2> /dev/null)
else
  JAVAC_BIN := $(shell command -v javac 2> /dev/null)
  JAVA_BIN := $(shell command -v java 2> /dev/null)
endif
  CSC_BIN := $(shell command -v mcs 2> /dev/null)
endif

# Windows specific part.
ifeq ($(SYSTEM),win)
  ifeq ("$(Platform)", "X64")
    PLATFORM = Win64
  endif
  ifeq ("$(Platform)", "x64")
    PLATFORM = Win64
  endif
  ifeq ("$(PLATFORM)", "Win64")
    PORT = VisualStudio$(VISUAL_STUDIO)-64b
    NETPLATFORM = x64
  else
    PORT = VisualStudio$(VISUAL_STUDIO)-32b
    NETPLATFORM = x86
  endif
  CXX = cl
  CXXFLAGS = /EHsc /MD /nologo /D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS -nologo $(DEBUG) \
    /D__WIN32__ /DGFLAGS_DLL_DECL= /DGFLAGS_DLL_DECLARE_FLAG= /DGFLAGS_DLL_DEFINE_FLAG= \
    /I$(INC_DIR)\\src\\windows /I$(INC_DIR) /I$(INC_EX_DIR) \
    -DUSE_CBC -DUSE_CLP -DUSE_BOP -DUSE_GLOP
  LDFLAGS = psapi.lib ws2_32.lib
  LIB_PREFIX =
  PRE_LIB =
  L = lib
  OR_TOOLS_LNK = lib\\ortools.lib
  CVRPTW_LNK = lib\\cvrptw_lib.lib lib\\ortools.lib
  DIMACS_LNK = lib\\dimacs.lib lib\\ortools.lib
  O=obj
  E=.exe
  OBJ_OUT = /Fo
  EXE_OUT = /Fe
  DEL = del
  S = \\
  CPSEP = ;
  DEBUG=/O2 -DNDEBUG
  # We can't use `where` since it's return all matching pathnames
  # so we ship which.exe and use it
  CXX_BIN := $(shell which $(CXX) 2> NUL)
  JAVAC_BIN := $(shell which javac 2> NUL)
  JAVA_BIN := $(shell which java 2> NUL)
  CSC_BIN := $(shell which csc 2> NUL)
  MONO=
endif

OR_TOOLS_LIBS = $(LIB_DIR)/$(LIB_PREFIX)ortools.$L
CVRPTW_LIBS = $(LIB_DIR)/$(LIB_PREFIX)cvrptw_lib.$L
DIMACS_LIBS = $(LIB_DIR)/$(LIB_PREFIX)dimacs.$L
FAP_LIBS = $(LIB_DIR)/$(LIB_PREFIX)fap.$L

.PHONY: all clean test

all: cc test

EXE = \
$(BIN_DIR)/costas_array$E \
$(BIN_DIR)/cryptarithm$E \
$(BIN_DIR)/cvrp_disjoint_tw$E \
$(BIN_DIR)/cvrptw$E \
$(BIN_DIR)/cvrptw_with_breaks$E \
$(BIN_DIR)/cvrptw_with_refueling$E \
$(BIN_DIR)/cvrptw_with_resources$E \
$(BIN_DIR)/cvrptw_with_stop_times_and_resources$E \
$(BIN_DIR)/dimacs_assignment$E \
$(BIN_DIR)/dobble_ls$E \
$(BIN_DIR)/flexible_jobshop$E \
$(BIN_DIR)/golomb$E \
$(BIN_DIR)/jobshop$E \
$(BIN_DIR)/jobshop_ls$E \
$(BIN_DIR)/jobshop_earlytardy$E \
$(BIN_DIR)/magic_square$E \
$(BIN_DIR)/model_util$E \
$(BIN_DIR)/multidim_knapsack$E \
$(BIN_DIR)/network_routing$E \
$(BIN_DIR)/nqueens$E \
$(BIN_DIR)/pdptw$E \
$(BIN_DIR)/sports_scheduling$E \
$(BIN_DIR)/tsp$E \
$(BIN_DIR)/linear_assignment_api$E \
$(BIN_DIR)/strawberry_fields_with_column_generation$E \
$(BIN_DIR)/linear_programming$E \
$(BIN_DIR)/linear_solver_protocol_buffers$E \
$(BIN_DIR)/integer_programming$E \
$(BIN_DIR)/flow_api$E

cc: $(EXE)

clean:
	-$(DEL) $(EXE)
	-$(DEL) $(BIN_DIR)$S*.exe
	-$(DEL) $(OBJ_DIR)$S*.$O

ifndef CXX_BIN
test_cc ccc rcc:
	@echo the $(CXX) command was not found in your PATH
else
test_cc: $(BIN_DIR)/golomb$E $(BIN_DIR)/cvrptw$E
	$(BIN_DIR)$Sgolomb$E
	$(BIN_DIR)/cvrptw$E
# C++ generic running command
ifeq ($(EX),)
ccc rcc:
	@echo No C++ file was provided, the $@ target must be used like so: \
	make $@ EX=path/to/the/example/example.cc
else # ifeq ($(EX),)
ccc: $(BIN_DIR)$S$(basename $(notdir $(EX)))$E

rcc: $(BIN_DIR)$S$(basename $(notdir $(EX)))$E
	@echo running $(BIN_DIR)$S$(basename $(notdir $(EX)))$E
	$(BIN_DIR)$S$(basename $(notdir $(EX)))$E $(ARGS)
endif # ifeq ($(EX),)
endif # ifeq ($(CXX_BIN),)

HAS_JAVA = true
ifndef JAVA_BIN
HAS_JAVA =
endif
ifndef JAVAC_BIN
HAS_JAVA =
endif

ifndef HAS_JAVA
test_java rjava cjava:
	@echo the java or javac command was not found in your PATH
else
test_java:
	$(JAVAC_BIN) -d $(OBJ_DIR) -cp $(LIB_DIR)$Scom.google.ortools.jar$(CPSEP)$(LIB_DIR)$Sprotobuf.jar \
 $(EX_DIR)$Scom$Sgoogle$Sortools$Ssamples$STsp.java
	$(JAVA_BIN) $(JAVAFLAGS) -cp $(OBJ_DIR)$(CPSEP)$(LIB_DIR)$Scom.google.ortools.jar$(CPSEP)$(LIB_DIR)$Sprotobuf.jar \
 com.google.ortools.samples.Tsp $(ARGS)
	@echo javac = $(JAVAC_BIN)
	@echo java = $(JAVA_BIN)

# Java generic compilation command.
  ifeq ($(EX),)
rjava cjava:
	@echo No java file was provided, the rjava target must be used like so: \
	make rjava EX=path/to/the/example/example.java
  else
    ifeq ($(SYSTEM),win)
      EX_read_package = $(shell findstr /r "^package.*\;" $(EX))
    else
      EX_read_package = $(shell grep '^package.*\;' $(EX))
    endif
    EX_name = $(basename $(notdir $(EX)))
    EX_package = $(subst ;,,$(subst package ,,$(EX_read_package)))
    ifeq ($(EX_read_package),)
      EX_class_file = $(OBJ_DIR)$S$(EX_name).class
      EX_class = $(EX_name)
    else
    EX_class_file = $(OBJ_DIR)$S$(subst .,$S,$(EX_package))$S$(EX_name).class
    EX_class = $(EX_package).$(EX_name)
    endif
$(EX_class_file): $(LIB_DIR)$Scom.google.ortools.jar $(LIB_DIR)$Sprotobuf.jar  $(EX)
	$(JAVAC_BIN) -d $(OBJ_DIR) -cp $(LIB_DIR)$Scom.google.ortools.jar$(CPSEP)$(LIB_DIR)$Sprotobuf.jar $(EX)

cjava: $(EX_class_file)

rjava: $(EX_class_file)
	$(JAVA_BIN) -Djava.library.path=$(LIB_DIR) -cp $(OBJ_DIR)$(CPSEP)$(LIB_DIR)$Scom.google.ortools.jar$(CPSEP)$(LIB_DIR)$Sprotobuf.jar $(EX_class) $(ARGS)
  endif
endif

ifndef CSC_BIN
test_cs:
	@echo the csc(win) or mcs(unix) command was not found in your PATH
else
test_cs:
	$(CSC_BIN) /target:exe /out:$(BIN_DIR)$Scsflow.exe /platform:$(NETPLATFORM) /lib:$(BIN_DIR) /r:Google.OrTools.dll /r:Google.Protobuf.dll $(CS_EX_DIR)$Scsflow.cs
	$(MONO) $(BIN_DIR)$Scsflow.exe $(ARGS)

test: test_cc test_java test_cs

  ifeq ($(EX),)
rcs:
	@echo No csharp file was provided, the rcs target must be used like so: \
	make rcs EX=path/to/the/example/example.cs
  else
# .NET generic compilation command.
$(BIN_DIR)$S$(basename $(notdir $(EX))).exe: $(BIN_DIR)/$(CLR_DLL_NAME).dll $(EX)
	$(CSC_BIN) $(SIGNING_FLAGS) /target:exe /out:$(BIN_DIR)$S$(basename $(notdir $(EX))).exe /platform:$(NETPLATFORM) /lib:$(BIN_DIR) /r:$(CLR_DLL_NAME).dll /r:Google.Protobuf.dll $(EX)

csc: $(BIN_DIR)$S$(basename $(notdir $(EX))).exe

rcs: $(BIN_DIR)$S$(basename $(notdir $(EX))).exe
	@echo running $(BIN_DIR)$S$(basename $(notdir $(EX))).exe
	$(MONO) $(BIN_DIR)$S$(basename $(notdir $(EX))).exe $(ARGS)
  endif
endif


# Constraint Programming and Routing examples.

$(OBJ_DIR)$Scostas_array.$O: $(CPP_EX_DIR)$Scostas_array.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Scostas_array.cc $(OBJ_OUT)$(OBJ_DIR)$Scostas_array.$O

$(BIN_DIR)/costas_array$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Scostas_array.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Scostas_array.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Scostas_array$E

$(OBJ_DIR)$Scryptarithm.$O:$(CPP_EX_DIR)$Scryptarithm.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Scryptarithm.cc $(OBJ_OUT)$(OBJ_DIR)$Scryptarithm.$O

$(BIN_DIR)/cryptarithm$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Scryptarithm.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Scryptarithm.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Scryptarithm$E

$(OBJ_DIR)$Scvrp_disjoint_tw.$O: $(CPP_EX_DIR)$Scvrp_disjoint_tw.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Scvrp_disjoint_tw.cc $(OBJ_OUT)$(OBJ_DIR)$Scvrp_disjoint_tw.$O

$(BIN_DIR)/cvrp_disjoint_tw$E: $(OR_TOOLS_LIBS) $(CVRPTW_LIBS) $(OBJ_DIR)$Scvrp_disjoint_tw.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Scvrp_disjoint_tw.$O $(CVRPTW_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Scvrp_disjoint_tw$E

$(OBJ_DIR)$Scvrptw.$O: $(CPP_EX_DIR)$Scvrptw.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Scvrptw.cc $(OBJ_OUT)$(OBJ_DIR)$Scvrptw.$O

$(BIN_DIR)/cvrptw$E: $(OR_TOOLS_LIBS) $(CVRPTW_LIBS) $(OBJ_DIR)$Scvrptw.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Scvrptw.$O $(CVRPTW_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Scvrptw$E

$(OBJ_DIR)$Scvrptw_with_breaks.$O: $(CPP_EX_DIR)$Scvrptw_with_breaks.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Scvrptw_with_breaks.cc $(OBJ_OUT)$(OBJ_DIR)$Scvrptw_with_breaks.$O

$(BIN_DIR)/cvrptw_with_breaks$E: $(OR_TOOLS_LIBS) $(CVRPTW_LIBS) $(CVRPTW_LIBS) $(OBJ_DIR)$Scvrptw_with_breaks.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Scvrptw_with_breaks.$O $(CVRPTW_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Scvrptw_with_breaks$E

$(OBJ_DIR)$Scvrptw_with_refueling.$O: $(CPP_EX_DIR)$Scvrptw_with_refueling.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Scvrptw_with_refueling.cc $(OBJ_OUT)$(OBJ_DIR)$Scvrptw_with_refueling.$O

$(BIN_DIR)/cvrptw_with_refueling$E: $(OR_TOOLS_LIBS) $(CVRPTW_LIBS) $(CVRPTW_LIBS) $(OBJ_DIR)$Scvrptw_with_refueling.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Scvrptw_with_refueling.$O $(CVRPTW_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Scvrptw_with_refueling$E

$(OBJ_DIR)$Scvrptw_with_resources.$O: $(CPP_EX_DIR)$Scvrptw_with_resources.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Scvrptw_with_resources.cc $(OBJ_OUT)$(OBJ_DIR)$Scvrptw_with_resources.$O

$(BIN_DIR)/cvrptw_with_resources$E: $(OR_TOOLS_LIBS) $(CVRPTW_LIBS) $(OBJ_DIR)$Scvrptw_with_resources.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Scvrptw_with_resources.$O $(CVRPTW_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Scvrptw_with_resources$E

$(OBJ_DIR)$Scvrptw_with_stop_times_and_resources.$O: $(CPP_EX_DIR)$Scvrptw_with_stop_times_and_resources.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Scvrptw_with_stop_times_and_resources.cc $(OBJ_OUT)$(OBJ_DIR)$Scvrptw_with_stop_times_and_resources.$O

$(BIN_DIR)/cvrptw_with_stop_times_and_resources$E: $(OR_TOOLS_LIBS) $(CVRPTW_LIBS) $(OBJ_DIR)$Scvrptw_with_stop_times_and_resources.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Scvrptw_with_stop_times_and_resources.$O $(CVRPTW_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Scvrptw_with_stop_times_and_resources$E

$(OBJ_DIR)$Sdimacs_assignment.$O:$(CPP_EX_DIR)$Sdimacs_assignment.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Sdimacs_assignment.cc $(OBJ_OUT)$(OBJ_DIR)$Sdimacs_assignment.$O

$(BIN_DIR)/dimacs_assignment$E: $(OR_TOOLS_LIBS) $(DIMACS_LIBS) $(OBJ_DIR)$Sdimacs_assignment.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Sdimacs_assignment.$O $(DIMACS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Sdimacs_assignment$E

$(OBJ_DIR)$Sdobble_ls.$O:$(CPP_EX_DIR)$Sdobble_ls.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Sdobble_ls.cc $(OBJ_OUT)$(OBJ_DIR)$Sdobble_ls.$O

$(BIN_DIR)/dobble_ls$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Sdobble_ls.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Sdobble_ls.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Sdobble_ls$E

$(OBJ_DIR)$Sflexible_jobshop.$O:$(CPP_EX_DIR)$Sflexible_jobshop.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h $(CPP_EX_DIR)$Sflexible_jobshop.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Sflexible_jobshop.cc $(OBJ_OUT)$(OBJ_DIR)$Sflexible_jobshop.$O

$(BIN_DIR)/flexible_jobshop$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Sflexible_jobshop.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Sflexible_jobshop.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Sflexible_jobshop$E

$(OBJ_DIR)$Sgolomb.$O:$(CPP_EX_DIR)$Sgolomb.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Sgolomb.cc $(OBJ_OUT)$(OBJ_DIR)$Sgolomb.$O

$(BIN_DIR)/golomb$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Sgolomb.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Sgolomb.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Sgolomb$E

$(OBJ_DIR)$Sjobshop.$O:$(CPP_EX_DIR)$Sjobshop.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Sjobshop.cc $(OBJ_OUT)$(OBJ_DIR)$Sjobshop.$O

$(BIN_DIR)/jobshop$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Sjobshop.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Sjobshop.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Sjobshop$E

$(OBJ_DIR)$Sjobshop_ls.$O:$(CPP_EX_DIR)$Sjobshop_ls.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Sjobshop_ls.cc $(OBJ_OUT)$(OBJ_DIR)$Sjobshop_ls.$O

$(BIN_DIR)/jobshop_ls$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Sjobshop_ls.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Sjobshop_ls.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Sjobshop_ls$E

$(OBJ_DIR)$Sjobshop_earlytardy.$O:$(CPP_EX_DIR)$Sjobshop_earlytardy.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h $(CPP_EX_DIR)$Sjobshop_earlytardy.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Sjobshop_earlytardy.cc $(OBJ_OUT)$(OBJ_DIR)$Sjobshop_earlytardy.$O

$(BIN_DIR)/jobshop_earlytardy$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Sjobshop_earlytardy.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Sjobshop_earlytardy.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Sjobshop_earlytardy$E

$(OBJ_DIR)$Smagic_square.$O:$(CPP_EX_DIR)$Smagic_square.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Smagic_square.cc $(OBJ_OUT)$(OBJ_DIR)$Smagic_square.$O

$(BIN_DIR)/magic_square$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Smagic_square.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Smagic_square.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Smagic_square$E

$(OBJ_DIR)$Smodel_util.$O:$(CPP_EX_DIR)$Smodel_util.cc $(INC_DIR)$Sortools$Sconstraint_solver$Smodel.pb.h $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Smodel_util.cc $(OBJ_OUT)$(OBJ_DIR)$Smodel_util.$O

$(BIN_DIR)/model_util$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Smodel_util.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Smodel_util.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Smodel_util$E

$(OBJ_DIR)$Smultidim_knapsack.$O:$(CPP_EX_DIR)$Smultidim_knapsack.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Smultidim_knapsack.cc $(OBJ_OUT)$(OBJ_DIR)$Smultidim_knapsack.$O

$(BIN_DIR)/multidim_knapsack$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Smultidim_knapsack.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Smultidim_knapsack.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Smultidim_knapsack$E

$(OBJ_DIR)$Snetwork_routing.$O:$(CPP_EX_DIR)$Snetwork_routing.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Snetwork_routing.cc $(OBJ_OUT)$(OBJ_DIR)$Snetwork_routing.$O

$(BIN_DIR)/network_routing$E: $(OR_TOOLS_LIBS) $(GRAPH_DEPS) $(OBJ_DIR)$Snetwork_routing.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Snetwork_routing.$O $(OR_TOOLS_LNK) $(GRAPH_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Snetwork_routing$E

$(OBJ_DIR)$Snqueens.$O: $(CPP_EX_DIR)$Snqueens.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Snqueens.cc $(OBJ_OUT)$(OBJ_DIR)$Snqueens.$O

$(BIN_DIR)/nqueens$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Snqueens.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Snqueens.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Snqueens$E

$(OBJ_DIR)$Spdptw.$O: $(CPP_EX_DIR)$Spdptw.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Spdptw.cc $(OBJ_OUT)$(OBJ_DIR)$Spdptw.$O

$(BIN_DIR)/pdptw$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Spdptw.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Spdptw.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Spdptw$E

$(OBJ_DIR)$Ssports_scheduling.$O:$(CPP_EX_DIR)$Ssports_scheduling.cc $(INC_DIR)$Sortools$Sconstraint_solver$Sconstraint_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Ssports_scheduling.cc $(OBJ_OUT)$(OBJ_DIR)$Ssports_scheduling.$O

$(BIN_DIR)/sports_scheduling$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Ssports_scheduling.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Ssports_scheduling.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Ssports_scheduling$E

$(OBJ_DIR)$Stsp.$O: $(CPP_EX_DIR)$Stsp.cc $(INC_DIR)$Sortools$Sconstraint_solver$Srouting.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Stsp.cc $(OBJ_OUT)$(OBJ_DIR)$Stsp.$O

$(BIN_DIR)/tsp$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Stsp.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Stsp.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Stsp$E

# Flow and linear assignment cpp

$(OBJ_DIR)$Slinear_assignment_api.$O:$(CPP_EX_DIR)$Slinear_assignment_api.cc
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Slinear_assignment_api.cc $(OBJ_OUT)$(OBJ_DIR)$Slinear_assignment_api.$O

$(BIN_DIR)/linear_assignment_api$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Slinear_assignment_api.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Slinear_assignment_api.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Slinear_assignment_api$E

$(OBJ_DIR)$Sflow_api.$O:$(CPP_EX_DIR)$Sflow_api.cc
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Sflow_api.cc $(OBJ_OUT)$(OBJ_DIR)$Sflow_api.$O

$(BIN_DIR)/flow_api$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Sflow_api.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Sflow_api.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Sflow_api$E

# Linear Programming Examples

$(OBJ_DIR)$Sstrawberry_fields_with_column_generation.$O: $(CPP_EX_DIR)$Sstrawberry_fields_with_column_generation.cc $(INC_DIR)$Sortools$Slinear_solver$Slinear_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Sstrawberry_fields_with_column_generation.cc $(OBJ_OUT)$(OBJ_DIR)$Sstrawberry_fields_with_column_generation.$O

$(BIN_DIR)/strawberry_fields_with_column_generation$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Sstrawberry_fields_with_column_generation.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Sstrawberry_fields_with_column_generation.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Sstrawberry_fields_with_column_generation$E

$(OBJ_DIR)$Slinear_programming.$O: $(CPP_EX_DIR)$Slinear_programming.cc $(INC_DIR)$Sortools$Slinear_solver$Slinear_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Slinear_programming.cc $(OBJ_OUT)$(OBJ_DIR)$Slinear_programming.$O

$(BIN_DIR)/linear_programming$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Slinear_programming.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Slinear_programming.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Slinear_programming$E

$(OBJ_DIR)$Slinear_solver_protocol_buffers.$O: $(CPP_EX_DIR)$Slinear_solver_protocol_buffers.cc $(INC_DIR)$Sortools$Slinear_solver$Slinear_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Slinear_solver_protocol_buffers.cc $(OBJ_OUT)$(OBJ_DIR)$Slinear_solver_protocol_buffers.$O

$(BIN_DIR)/linear_solver_protocol_buffers$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Slinear_solver_protocol_buffers.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Slinear_solver_protocol_buffers.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Slinear_solver_protocol_buffers$E

$(OBJ_DIR)$Sinteger_programming.$O: $(CPP_EX_DIR)$Sinteger_programming.cc $(INC_DIR)$Sortools$Slinear_solver$Slinear_solver.h
	$(CXX) $(CXXFLAGS) -c $(CPP_EX_DIR)$Sinteger_programming.cc $(OBJ_OUT)$(OBJ_DIR)$Sinteger_programming.$O

$(BIN_DIR)/integer_programming$E: $(OR_TOOLS_LIBS) $(OBJ_DIR)$Sinteger_programming.$O
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)$Sinteger_programming.$O $(OR_TOOLS_LNK) $(LDFLAGS) $(EXE_OUT)$(BIN_DIR)$Sinteger_programming$E

# Debug
.PHONY: detect
detect: detect_port detect_cc detect_java detect_csharp

.PHONY: detect_port
detect_port:
	@echo SHELL = $(SHELL)
	@echo SYSTEM = $(SYSTEM)
	@echo PORT = $(PORT)
	@echo OS = $(OS)

.PHONY: detect_cc
detect_cc:
	@echo CXX = $(CXX)
	@echo CXX_BIN = $(CXX_BIN)
	@echo CXXFLAGS = $(CXXFLAGS)
	@echo LDFLAGS = $(LDFLAGS)
	@echo OR_TOOLS_LNK = $(OR_TOOLS_LNK)
	@echo CVRPTW_LNK = $(CVRPTW_LNK)
	@echo DIMACS_LNK = $(DIMACS_LNK)

.PHONY: detect_java
detect_java:
	@echo JAVAC_BIN = $(JAVAC_BIN)
	@echo JAVA_BIN = $(JAVA_BIN)
	@echo JAVAFLAGS = -Djava.library.path=$(LIB_DIR)

.PHONY: detect_csharp
detect_csharp:
	@echo CSC_BIN = $(CSC_BIN)
	@echo SIGNING_FLAGS = $(SIGNING_FLAGS)
	@echo CLR_DLL_NAME = $(CLR_DLL_NAME)

# Include user makefile
-include $(OR_ROOT)Makefile.user

print-%	: ; @echo $* = $($*)
