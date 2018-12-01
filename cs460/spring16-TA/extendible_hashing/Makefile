CXX=g++
CXX_FLAGS=--std=c++11
# CXX_FLAGS=--std=c++0x # for older version of g++

all: CreateDBFile BuildHashIndex query

CreateDBFile: CreateDBFile.cpp SPMRecord.cpp
	$(CXX) -o $@ $^ $(CXX_FLAGS)

BuildHashIndex:	BuildHashIndex.cpp SPMRecord.cpp Directory.cpp HashBucket.cpp
	$(CXX) -o $@ $^ $(CXX_FLAGS)

query:	query.cpp SPMRecord.cpp Directory.cpp HashBucket.cpp
	$(CXX) -o $@ $^ $(CXX_FLAGS)

clean:
	rm CreateDBFile BuildHashIndex query
