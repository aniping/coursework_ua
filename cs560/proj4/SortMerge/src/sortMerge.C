
#include <string.h>
#include <assert.h>
#include "sortMerge.h"

// Error Protocall:
enum ErrCodes {SORT_FAILED, HEAPFILE_FAILED};

static const char* ErrMsgs[] =  {
  "Error: Sort Failed.",
  "Error: HeapFile Failed."
  // maybe more ...
};

struct Rec {
  int key;
  char data[4];
};

static error_string_table ErrTable( JOINS, ErrMsgs );

// sortMerge constructor
sortMerge::sortMerge(
    char*           filename1,      // Name of heapfile for relation R
    int             len_in1,        // # of columns in R.
    AttrType        in1[],          // Array containing field types of R.
    short           t1_str_sizes[], // Array containing size of columns in R
    int             join_col_in1,   // The join column of R 

    char*           filename2,      // Name of heapfile for relation S
    int             len_in2,        // # of columns in S.
    AttrType        in2[],          // Array containing field types of S.
    short           t2_str_sizes[], // Array containing size of columns in S
    int             join_col_in2,   // The join column of S

    char*           filename3,      // Name of heapfile for merged results
    int             amt_of_mem,     // Number of pages available
    TupleOrder      order,          // Sorting order: Ascending or Descending
    Status&         s               // Status of constructor
)
{
  Status status, status_r, status_s;
  RID rid;
  Rec rec;
  int len;
  int rand_num;

  //HeapFile *file_r = new HeapFile(filename1, status);
  //Scan *scan_r = file_r->openScan(status);
  //HeapFile *file_s = new HeapFile(filename2, status);
  //Scan *scan_s = file_s->openScan(status);
  HeapFile *file_res = new HeapFile(filename3, status);

  srand(time(NULL));

  char sortedfile1[100];
  memset(sortedfile1, 0, 100);
  memcpy(sortedfile1, filename1, strlen(filename1));
  strcat(sortedfile1, "_sorted");
  rand_num = rand();
  sprintf(sortedfile1+strlen(sortedfile1), "%d", rand_num);
  cout << "sorted flle: " << sortedfile1 << endl;
  Sort *sr = new Sort(filename1, sortedfile1, len_in1, in1, t1_str_sizes,
                      join_col_in1, order, amt_of_mem, status_r);
  HeapFile *sorted_r = new HeapFile(sortedfile1, status_r);
  Scan *scan_sorted_r = sorted_r->openScan(status_r);

  cout << "status: " << status_r << endl;
  // Only for Debugging
  cout << "Sorted keys in relation R:\n";
  // while ((status = scan_r->getNext(rid, (char *)&rec, len)) == OK) {
  //   cout << "unsorted: " << rec.key << endl;
  // }
  while ((status_r = scan_sorted_r->getNext(rid, (char *)&rec, len)) == OK) {
    cout << rec.key << endl;
  }

  char sortedfile2[100];
  memset(sortedfile2, 0, 100);
  memcpy(sortedfile2, filename2, strlen(filename2));
  strcat(sortedfile2, "_sorted");
  rand_num = rand();
  sprintf(sortedfile2+strlen(sortedfile2), "%d", rand_num);
  cout << "sorted flle: " << sortedfile2 << endl;
  Sort *ss = new Sort(filename2, sortedfile2, len_in2, in2, t2_str_sizes,
                      join_col_in2, order, amt_of_mem, status_s);
  HeapFile *sorted_s = new HeapFile(sortedfile2, status_s);
  Scan *scan_sorted_s = sorted_s->openScan(status_s);

  // Only for Debugging
  cout << "Sorted keys in relation S:\n";
  // while ((status_s = scan_s->getNext(rid, (char *)&rec, len)) == OK) {
  //   cout << "unsorted: " << rec.key << endl;
  // }
  while ((status_s = scan_sorted_s->getNext(rid, (char *)&rec, len)) == OK) {
    cout << rec.key << endl;
  }
}

void sortMerge::dumpRelation(char *fname)
{
  Status status;
  RID rid;
  Scan *scan = NULL;
  Rec rec;
  int len;

  cout << "\nDumpping the relation stored in file: " << fname << endl;
  HeapFile f(fname, status);

  if (status != OK) {
    cerr << "*** Error opening heap file\n";
    return;
  } else {
    scan = f.openScan(status);
    if (status != OK) {
      cerr << "*** Error opening scan\n";
      return;
    }
    while ((status = scan->getNext(rid, (char *)&rec, len)) == OK) {
      //cout << rec.key << "\t" << rec.data << endl;
      cout << rec.key << endl;
    }
  }
}

// sortMerge destructor
sortMerge::~sortMerge()
{
	// fill in the body
}
