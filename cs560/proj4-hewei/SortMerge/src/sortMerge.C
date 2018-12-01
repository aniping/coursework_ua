
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <string>
#include "sortMerge.h"

using namespace std;
// Error Protocall:

//-------------------------------------------------------------
// scan files
//-------------------------------------------------------------
struct _rec2 {
	int	key;
	char	filler[4];
};

void scan_file(HeapFile *hf)
{
	Status s;
	Scan *scan = hf->openScan(s);
	assert(s == OK);
	int len;
	RID	rid;
	char	rec[sizeof(struct _rec2)*2];
	cout << "----- scan_file begin -----" << endl;
	for (s = scan->getNext(rid, rec, len); s == OK; s = scan->getNext(rid, rec, len)) 
	{
	  cout << (*((struct _rec2*)&rec)).key << /*"\t" << (*((struct _rec2*)&rec[8])).key <<*/ endl;
	}
	cout << "------ scan_file end ------" << endl << endl << endl;
	delete scan;
	
	//return s;
}



enum ErrCodes {SORT_FAILED, HEAPFILE_FAILED};

static const char* ErrMsgs[] =  {
  "Error: Sort Failed.",
  "Error: HeapFile Failed."
  // maybe more ...
};

static error_string_table ErrTable( JOINS, ErrMsgs );

static int postfix = 0;

enum MergeStatus {R_FORWARDING, S_FORWARDING, R_REPEATING, S_REPEATING, MERGING};
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
){
	// fill in the body
	int i, j, k;
	
	//sort s & r
	Status sta, sta_r, sta_s;
	char fn1[20];
	char fn2[20];
	//char *fn1 = "sorted_r";
	//char *fn2 = "sorted_s";
	strcpy(fn1, "sorted_r");
	strcpy(fn2, "sorted_s");
	
	//itoa(postfix, tmp_buf, 10);
	const char *tmp_buf = std::to_string(postfix).c_str();
	strcat(fn1, tmp_buf);
	strcat(fn2, tmp_buf);
	++postfix;
	
	//HeapFile *sorted_r = new HeapFile(fn1, sta);
	//HeapFile *sorted_s = new HeapFile(fn2, sta);
	HeapFile *res_file = new HeapFile(filename3, sta);
	HeapFile *file_r = new HeapFile(filename1, sta);
	HeapFile *file_s = new HeapFile(filename2, sta);
	
	//*scan_file(file_r);
	//*scan_file(file_s);
	
	Sort *ss = new Sort(filename2, fn2, len_in2, in2, t2_str_sizes, join_col_in2, order, amt_of_mem, sta);
	HeapFile *sorted_s = new HeapFile(fn2, sta);
	//*scan_file(sorted_s);
	//*scan_file(sorted_s);
	
	Sort *sr = new Sort(filename1, fn1, len_in1, in1, t1_str_sizes, join_col_in1, order, amt_of_mem, sta);
	//HeapFile *sorted_s = new HeapFile(fn2, sta);
	HeapFile *sorted_r = new HeapFile(fn1, sta);
	//scan_file(sorted_s);
	//*scan_file(sorted_r);
	//*scan_file(sorted_r);
	
	Scan *scan_r = sorted_r->openScan(sta);
	Scan *scan_s = sorted_s->openScan(sta);
	
	//compute size of s & r tuple
	int st_size = 0, rt_size = 0;
	for (i = 0; i < len_in1; ++i)
		rt_size += t1_str_sizes[i];
	for (i = 0; i < len_in2; ++i)
		st_size += t2_str_sizes[i];
	
	
	bool reach_end = false;
	bool r_end = false;
	bool s_end = false;
	MergeStatus merge_status = R_FORWARDING;
	int cmp_res = -2;
	RID rcurr_rid, scurr_rid;
	char *r_recptr = (char *)malloc(rt_size);
	char *s_recptr = (char *)malloc(st_size);
	char *r_lastrec = (char *)malloc(rt_size);
	char *s_lastrec = (char *)malloc(st_size);
	char *res_buffer = (char *)malloc(st_size + rt_size);
	RID r_equal_begin, r_equal_end, s_equal_begin, s_equal_end; 
	
	sta_r = scan_r->getNext(rcurr_rid, r_recptr, rt_size);
	sta_s = scan_s->getNext(scurr_rid, s_recptr, st_size);
	if (sta_r == DONE || sta_s == DONE)
		reach_end = true;
		
	while (reach_end == false)
	{
		cmp_res = tupleCmp(r_recptr, s_recptr);
		
		if (merge_status == R_FORWARDING)
		{
			if (cmp_res < 0)
			{
				memcpy(r_lastrec, r_recptr, rt_size);
				sta_r = scan_r->getNext(rcurr_rid, r_recptr, rt_size);
				if (sta_r == DONE)
					reach_end = true;
			}
			else if (cmp_res > 0)
			{
				/*memcpy(s_lastrec, s_recptr, st_size);
				sta_s = scan_s->getNext(scurr_rid, s_recptr, st_size);
				if (sta_s == DONE)
					reach_end = true;*/
				merge_status = S_FORWARDING;
			}
			else if (cmp_res == 0)
			{
				memcpy(r_lastrec, r_recptr, rt_size);
				//memcpy(s_lastrec, s_recptr, st_size);
				r_equal_begin = r_equal_end = rcurr_rid;
				s_equal_begin = s_equal_end = scurr_rid;
				sta_r = scan_r->getNext(rcurr_rid, r_recptr, rt_size);
				//sta_s = scan_s->getNext(scurr_rid, s_recptr, st_size);
				if (sta_r == DONE)
				{
					r_end = true;
					//merge_status = MERGING;
				}
				/*if (sta_s == DONE)
				{	
					s_end = true;
				}*/
				
				if (!r_end && tupleCmp(r_lastrec, r_recptr) == 0)
				{
					r_equal_end = rcurr_rid;
					merge_status = R_REPEATING;
				}
				else
				{
					memcpy(s_lastrec, s_recptr, st_size);
					sta_s = scan_s->getNext(scurr_rid, s_recptr, st_size);
					if (sta_s == DONE)
					{	
						s_end = true;
					}
					 if (!s_end && tupleCmp(s_lastrec, s_recptr) == 0)
					{
						s_equal_end = scurr_rid;
						merge_status = S_REPEATING;
					}
				}
				if (merge_status != S_REPEATING && merge_status != R_REPEATING)
					merge_status = MERGING;
			}
		}
		else if (merge_status == S_FORWARDING)
		{
			cmp_res = tupleCmp(r_recptr, s_recptr);
			
			if (cmp_res > 0)
			{
				memcpy(s_lastrec, s_recptr, st_size);
				sta_s = scan_s->getNext(scurr_rid, s_recptr, st_size);
				if (sta_s == DONE)
					reach_end = true;
			}
			else if (cmp_res < 0)
			{
				merge_status = R_FORWARDING;
			}
			else if (cmp_res == 0)
			{
				memcpy(r_lastrec, r_recptr, rt_size);
				//memcpy(s_lastrec, s_recptr, st_size);
				r_equal_begin = r_equal_end = rcurr_rid;
				s_equal_begin = s_equal_end = scurr_rid;
				sta_r = scan_r->getNext(rcurr_rid, r_recptr, rt_size);
				//sta_s = scan_s->getNext(scurr_rid, s_recptr, st_size);
				if (sta_r == DONE)
				{
					r_end = true;
					//merge_status = MERGING;
				}
				/*if (sta_s == DONE)
				{	
					s_end = true;
				}*/
				
				if (!r_end && tupleCmp(r_lastrec, r_recptr) == 0)
				{
					r_equal_end = rcurr_rid;
					merge_status = R_REPEATING;
				}
				else
				{
					memcpy(s_lastrec, s_recptr, st_size);
					sta_s = scan_s->getNext(scurr_rid, s_recptr, st_size);
					if (sta_s == DONE)
					{	
						s_end = true;
					}
					 if (!s_end && tupleCmp(s_lastrec, s_recptr) == 0)
					{
						s_equal_end = scurr_rid;
						merge_status = S_REPEATING;
					}
				}
				if (merge_status != S_REPEATING && merge_status != R_REPEATING)
					merge_status = MERGING;
			}
			
		}
		else if (merge_status == R_REPEATING)
		{
			memcpy(r_lastrec, r_recptr, rt_size);
			sta_r = scan_r->getNext(rcurr_rid, r_recptr, rt_size);
			if (sta_r == DONE)
			{
				r_end = true;
			}
			if (!r_end && tupleCmp(r_lastrec, r_recptr) == 0)
			{
				r_equal_end = rcurr_rid;
				merge_status = R_REPEATING;
			}
			else if (!r_end && tupleCmp(r_lastrec, r_recptr) != 0)
			{
				memcpy(s_lastrec, s_recptr, st_size);
				sta_s = scan_s->getNext(scurr_rid, s_recptr, st_size);
				if (sta_s == DONE)
				{
					s_end = true;
					merge_status = MERGING;
				}
				if (!s_end && tupleCmp(s_lastrec, s_recptr) == 0)
				{
					s_equal_end = scurr_rid;
					merge_status = S_REPEATING;
				}
				else if (!s_end && tupleCmp(s_lastrec, s_recptr) != 0)
				{
					merge_status = MERGING;
				}
			}
		}
		else if (merge_status == S_REPEATING)
		{
			memcpy(s_lastrec, s_recptr, st_size);
			sta_s = scan_s->getNext(scurr_rid, s_recptr, st_size);
			if (sta_s == DONE)
			{
				s_end = true;
				merge_status = MERGING;
			}
			if (!s_end && tupleCmp(s_lastrec, s_recptr) == 0)
			{
				s_equal_end = scurr_rid;
				merge_status = S_REPEATING;
			}
			else if (!s_end && tupleCmp(s_lastrec, s_recptr) != 0)
			{
				merge_status = MERGING;
			}
		}
		else if (merge_status == MERGING)
		{
			//todo: merge
			RID r_iter, s_iter, res_rid;
			r_iter = r_equal_begin;
			s_iter = s_equal_begin;
			bool outer_end = false, inner_end = false;
			
			//insert first match
			scan_r->position(r_iter);
			scan_s->position(s_iter);
			
			//nested loop for multiple matches
			//sta_r = scan_r->getNext(r_iter, r_recptr, rt_size);	
			//sta_s = scan_s->getNext(s_iter, s_recptr, st_size);
			while (!outer_end)
			{
				//sta_r = sorted_r->getRecord(r_iter, r_recptr, rt_size);
				sta_r = scan_r->getNext(r_iter, r_recptr, rt_size);	
				if (sta_r == DONE)
				{
					outer_end = true;
					continue;
				}
				while (!inner_end)
				{
					//sta_s = sorted_s->getRecord(s_iter, s_recptr, st_size);
					sta_s = scan_s->getNext(s_iter, s_recptr, st_size);
					if (sta_s == DONE)
					{
						inner_end = true;
						continue;
					}
					memcpy(res_buffer, r_recptr, rt_size);
					memcpy(res_buffer + rt_size, s_recptr, st_size);
					res_file->insertRecord(res_buffer, rt_size + st_size, res_rid);
					//cout << "insert record: " << "(pid=" << r_iter.pageNo << ", slot=" << r_iter.slotNo << ") " << ((_rec2 *)r_recptr)->key << "\t" << "(pid=" << s_iter.pageNo << ", slot=" << s_iter.slotNo << ") " << ((_rec2 *)s_recptr)->key << endl;
					
					if (s_iter == s_equal_end)
						inner_end = true;
					//else
					//	sta_s = scan_s->getNext(s_iter, s_recptr, st_size);
				}
				if (r_iter == r_equal_end)
				{
					outer_end = true;
				}
				inner_end = false;
				scan_s->position(s_equal_begin);
				//else	
				//{	
				//	sta_r = scan_r->getNext(r_iter, r_recptr, rt_size);
				//}
			}
			
			//set the curse of scan
			scan_r->position(rcurr_rid);
			scan_s->position(scurr_rid);
			
			//clean flags
			r_equal_begin.pageNo = -1;
			r_equal_end.pageNo = -1;
			s_equal_begin.pageNo = -1;
			s_equal_end.pageNo = -1;
			
			//if either one file reaches the end, no more merge
			if (s_end || r_end)
				reach_end = true;
				
			merge_status = R_FORWARDING;
			sta_r = scan_r->getNext(rcurr_rid, r_recptr, rt_size);
			sta_s = scan_s->getNext(scurr_rid, s_recptr, st_size);
			if (sta_r == DONE || sta_s == DONE)
				reach_end = true;
			
		}
	}
	
	s = OK;
	delete ss;
	delete sr;
	delete scan_r;
	delete scan_s;
	
	/*sta = sorted_s->deleteFile();
	if(sta != OK) MINIBASE_CHAIN_ERROR(JOINS,sta);
	delete sorted_s;
	
	sta = sorted_r->deleteFile();
	if(sta != OK) MINIBASE_CHAIN_ERROR(JOINS,sta);
	delete sorted_r;
	
	/*sta = res_file->deleteFile();
	if(sta != OK) MINIBASE_CHAIN_ERROR(JOINS,sta);
	delete res_file;
	
	sta = file_s->deleteFile();
	if(sta != OK) MINIBASE_CHAIN_ERROR(JOINS,sta);
	delete file_s;
	
	sta = file_r->deleteFile();
	if(sta != OK) MINIBASE_CHAIN_ERROR(JOINS,sta);
	delete file_r;*/
	
	/*RID tmp;
	while (sorted_s->getRecCnt() > 0)
		sorted_s->deleteRecord(tmp);*/
}

// sortMerge destructor
sortMerge::~sortMerge()
{
	// fill in the body
	
	/*sta = file_s->deleteFile();
	if(sta != OK) MINIBASE_CHAIN_ERROR(JOINS,sta);
	delete file_s;
	
	sta = file_r->deleteFile();
	if(sta != OK) MINIBASE_CHAIN_ERROR(JOINS,sta);
	delete file_r;*/
	
}
