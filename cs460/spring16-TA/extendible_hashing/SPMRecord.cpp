#include "SPMRecord.hpp"
#include <boost/algorithm/string.hpp>

using namespace boost::algorithm;

void
SPMRecord::writeRecord(fstream& db_file, string line)
{
  parseLine(line);
  db_file.write(reinterpret_cast<const char *>(&fields), sizeof(fields));
}

void
SPMRecord::parseLine(string line)
{
  fields.objID = stoi(line.substr(0, 8));

  string tmp;
  tmp = line.substr(9, 2);
  trim(tmp);
  fields.RAh = stoi(tmp);

  tmp = line.substr(12, 2);
  trim(tmp);
  fields.RAm = stoi(tmp);

  tmp = line.substr(15, 6);
  trim(tmp);
  fields.RAs = stof(tmp);

  fields.DEsign = line.at(22);

  tmp = line.substr(23, 2);
  trim(tmp);
  fields.DEd = stoi(tmp);

  tmp = line.substr(26, 2);
  trim(tmp);
  fields.DEm = stoi(tmp);

  tmp = line.substr(29, 5);
  trim(tmp);
  fields.DEs = stof(tmp);

  tmp = line.substr(35, 10);
  trim(tmp);
  fields.RAdeg = stof(tmp);

  tmp = line.substr(46, 10);
  trim(tmp);
  fields.DEdeg = stof(tmp);

  tmp = line.substr(56, 4);
  trim(tmp);
  fields.e_RAdeg = stoi(tmp);

  tmp = line.substr(60, 4);
  trim(tmp);
  fields.e_DEdeg = stoi(tmp);

  tmp = line.substr(65, 7);
  trim(tmp);
  fields.pmRA = stof(tmp);

  tmp = line.substr(73, 7);
  trim(tmp);
  fields.pmDE = stof(tmp);

  tmp = line.substr(80, 5);
  trim(tmp);
  fields.e_pmRA = stof(tmp);

  tmp = line.substr(85, 5);
  trim(tmp);
  fields.e_pmDE = stof(tmp);

  tmp = line.substr(91, 5);
  trim(tmp);
  fields.Vmag = stof(tmp);

  tmp = line.substr(97, 5);
  trim(tmp);
  fields.Bmag = stof(tmp);

  tmp = line.substr(103, 5);
  trim(tmp);
  fields.BV = stof(tmp);

  tmp = line.substr(108, 3);
  trim(tmp);
  fields.e_Vmag = stoi(tmp);

  tmp = line.substr(111, 3);
  trim(tmp);
  fields.e_Bmag = stoi(tmp);

  tmp = line.substr(117, 1);
  trim(tmp);
  fields.Nfie = stoi(tmp);

  tmp = line.substr(119, 2);
  trim(tmp);
  fields.o_Bmag = stoi(tmp);

  tmp = line.substr(122, 2);
  trim(tmp);
  fields.o_Vmag = stoi(tmp);

  fields.F1 = line.at(125);
  fields.F2 = line.at(126);
  fields.F3 = line.at(127);
}

void
SPMRecord::readRecord(fstream& db_file)
{
  if (db_file.eof()) {
    cout << "End of file reached!" << endl;
    exit(-1);
  }
  db_file.read(reinterpret_cast<char *>(&fields), sizeof(fields));
}

void
SPMRecord::fetchRecord(fstream& db_file, int offset)
{
  db_file.seekg(offset, ios_base::beg);
  db_file.read(reinterpret_cast<char *>(&fields), sizeof(fields));
}

void
SPMRecord::print() const
{
  cout << fields.objID << "," << fields.RAh << "," << fields.RAm
       << "," << fields.RAs << "," << fields.DEsign << ","
       << fields.DEd << "," << fields.DEm << "," << fields.DEs
       << "," << fields.RAdeg << "," << fields.DEdeg << ","
       << fields.e_RAdeg << "," << fields.e_DEdeg << ","
       << fields.pmRA << "," << fields.pmDE << "," << fields.e_pmRA
       << "," << fields.e_pmDE << "," << fields.Vmag << ","
       << fields.Bmag << "," << fields.BV << "," << fields.e_Vmag
       << "," << fields.e_Bmag << "," << fields.Nfie << ","
       << fields.o_Bmag << "," << fields.o_Vmag << "," << fields.F1
       << "," << fields.F2 << "," << fields.F3 << endl;
}
