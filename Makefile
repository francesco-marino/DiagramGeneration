EXE=Main

OLEVEL= -g -O0 -debug all -traceback 	
ARCH= -mavx2 -march=core-avx2 -mtune=core-avx2

CXX=mpiicpc
CXXFLAGS= -std=c++20 -no-multibyte-chars -qopenmp -I${ARMADILLO_INC}

LINK=${OLEVEL} -lstdc++fs -qmkl=parallel -qopenmp -pthread -lmpi -lifcore -lifport -limf ${ARMADILLO_LINK}

ARMADILLO_HOME=/home/dc-mari3/armadillo
ARMADILLO_INC=${ARMADILLO_HOME}/include
ARMADILLO_LINK= -L${ARMADILLO_HOME}/lib64 -Wl,-rpath,${ARMADILLO_HOME}/lib64 -larmadillo
ARMA_FLAGS= -DARMA_ALLOW_FAKE_GCC -DARMA_DONT_USE_WRAPPER -DARMA_USE_BLAS -DARMA_USE_LAPACK -DARMA_64BIT_WORD 
CXXFLAGS+= -I${ARMADILLO_INC}

FFLAGS_ALL=   ${FFLAGS}   ${OLEVEL} $(DEBUG_FF)  ${ARCH}
CXXFLAGS_ALL= ${CXXFLAGS} ${OLEVEL} $(DEBUG_CXX) ${ARCH} ${ARMA_FLAGS} -DUSE_COMPLEX

mbpt_objects = MbptDiagram.o MbptDiagramManager.o
cc_objects   = CoupledClusterDiagram.o CoupledClusterDiagramManager.o


fortran_objects = 
cpp_objects = Main.o ${mbpt_objects} ${cc_objects} \
	DataStructures.o AdjacencyMatrix.o Vertex.o DiagramManager.o Diagram.o Parallel.o

%.o: %.f
	$(FC) $(FFLAGS_ALL) -c $<

%.o: %.f90
	$(FC) $(FFLAGS_ALL) -c $<

%.o : %.cpp
	$(CXX) $(CXXFLAGS_ALL) -c $<


all: Exe

Exe: $(fortran_objects) $(cpp_objects)
	$(CXX) $(CXXFLAGS_ALL) -o $(EXE).exe $(fortran_objects) $(cpp_objects) $(LINK)

Main.o: Main.cpp AdjacencyMatrix.o DataStructures.o DiagramManager.o MbptDiagramManager.o

CoupledClusterDiagramManager.o: CoupledClusterDiagramManager.cpp CoupledClusterDiagram.o  Diagram.o Vertex.o
CoupledClusterDiagram.o: CoupledClusterDiagram.cpp Diagram.o Vertex.o

MbptDiagramManager.o: MbptDiagramManager.cpp DiagramManager.o MbptDiagram.o
MbptDiagram.o: MbptDiagram.cpp Diagram.o

DiagramManager.o: DiagramManager.cpp Diagram.o Vertex.o

Diagram.o: Diagram.cpp Vertex.o

Vertex.o: Vertex.cpp

DataStructures.o: DataStructures.cpp

AdjacencyMatrix.o: AdjacencyMatrix.cpp

Parallel.o: Parallel.cpp

clean:
	rm -f *\.mod *\.o *~