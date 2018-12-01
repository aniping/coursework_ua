#ifndef _SPMRECORD_HPP_
#define _SPMRECORD_HPP_

#include <iostream>
#include <fstream>

using namespace std;

class SPMRecord {
public:
  int getObjID() const { return fields.objID; }
  void writeRecord(fstream& db_file, string line);
  void readRecord(fstream& db_file);
  void print() const;
  static int length() { return sizeof(fields); }

  void
  fetchRecord(fstream& db_file, int offset);

private:
  struct __attribute__((packed)) _fields {
    int objID; // field 1
    int RAh; // field 2
    int RAm;// field 3
    float RAs;// field 4
    char DEsign;// field 5
    int DEd;// field 6
    int DEm;// field 7
    float DEs;// field 8
    float RAdeg;// field 9
    float DEdeg;// field 10
    int e_RAdeg; // field 11
    int e_DEdeg;// field 12
    float pmRA;// field 13
    float pmDE;// field 14
    float e_pmRA;// field 15
    float e_pmDE; // field 16
    float Vmag;// field 17
    float Bmag;// field 18
    float BV;// field 19
    int e_Vmag;// field 20
    int e_Bmag;// field 21
    int Nfie;// field 22
    int o_Bmag;// field 23
    int o_Vmag;// field 24
    char F1;// field 25
    char F2;// field 26
    char F3;// field 27
  } fields;

  void parseLine(string line);
};

#endif
