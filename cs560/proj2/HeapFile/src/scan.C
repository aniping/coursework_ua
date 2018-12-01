/*
 * implementation of class Scan for HeapFile project.
 * $Id: scan.C,v 1.1 1997/01/02 12:46:42 flisakow Exp $
 */

#include <stdio.h>
#include <stdlib.h>

#include "heapfile.h"
#include "scan.h"
#include "hfpage.h"
#include "buf.h"
#include "db.h"

// *******************************************
// The constructor pins the first page in the file
// and initializes its private data members from the private data members from hf
Scan::Scan (HeapFile *hf, Status& status)
{
  init(hf);
  status = OK;
}

// *******************************************
// The deconstructor unpin all pages.
Scan::~Scan()
{
  // put your code here
}

// *******************************************
// Retrieve the next record in a sequential scan.
// Also returns the RID of the retrieved record.
Status Scan::getNext(RID& rid, char *recPtr, int& recLen)
{
  static int last_rec_returned = FALSE;
  Status status, status1;
  rid = userRid;

  if (dataPage == NULL) return DONE;

  if (last_rec_returned) {
    last_rec_returned = FALSE;
    reset();
    return DONE;
  }
  status1 = dataPage->getRecord(userRid, recPtr, recLen);

  RID cur_rid = userRid;
  // try to move to next record on the current page
  status = dataPage->nextRecord(cur_rid, userRid);

  if (status == DONE) { // move to the next data page
    status = nextDataPage();
    if (status == DONE) {
      status = nextDirPage();
      if (status == DONE) {
        if (status1 == OK) {
          userRid.slotNo = -1;
          last_rec_returned = TRUE;
          return OK;
        }
        reset();
        return DONE;
      } else {
      }
    }
  }

  return OK;
}

// *******************************************
// Do all the constructor work.
Status Scan::init(HeapFile *hf)
{
  this->_hf = hf;

  Page *pptr;
  Status status;

  // pin the dir header page (first page in the heap file)
  status = MINIBASE_BM->pinPage(hf->firstDirPageId, pptr);
  dirPage = (HFPage *)pptr;
  dirPageId = hf->firstDirPageId;

  firstDataPage();

  return OK;
}

// *******************************************
// Reset everything and unpin all pages.
Status Scan::reset()
{
  Page *pptr;

  //MINIBASE_BM->unpinPage(_hf->firstDirPageId); // unpin the current dir
  MINIBASE_BM->unpinPage(dirPageId); // unpin the current dir
  while (dirPage->getPrevPage() != INVALID_PAGE) {
    MINIBASE_BM->pinPage(dirPage->getPrevPage(), pptr); // pin data page
    MINIBASE_BM->unpinPage(dirPage->getPrevPage());
    MINIBASE_BM->unpinPage(dirPage->getPrevPage());
    dirPage = (HFPage *) pptr;
  }
  return OK;
}

// *******************************************
// Copy data about first page in the file.
Status Scan::firstDataPage()
{
  Status status;
  Page *pptr;

  // bring the first data page
  status = dirPage->firstRecord(dataPageRid);
  if (status != DONE) {
    DataPageInfo dpinfo;
    int rec_len;
    status = dirPage->getRecord(dataPageRid, (char *)&dpinfo, rec_len);
    status = MINIBASE_BM->pinPage(dpinfo.pageId, pptr); // pin data page
    dataPage = (HFPage *) pptr;
    dataPageId = dpinfo.pageId;

    dataPage->firstRecord(userRid); // rid of the first record
  } else { // current dir page is empty, try next dir page
    PageId next_dir_page = dirPage->getNextPage();
    MINIBASE_BM->unpinPage(dirPageId); // unpin current dir page
    if (next_dir_page != INVALID_PAGE) {
      MINIBASE_BM->pinPage(next_dir_page, pptr); // pin next dir page
      dirPage = (HFPage *)pptr;
      dirPageId = next_dir_page;
      return firstDataPage(); // recursively find the first data page
    } else {
      //cout << "First data page is NULL!!!\n";
      dataPage = NULL;
    }
    return DONE;
  }

  return OK;
}

// *******************************************
// Retrieve the next data page.
Status Scan::nextDataPage()
{
  Status status;
  DataPageInfo dpinfo;
  Page *pptr;
  int rec_len;
  RID cur_dp_rid = dataPageRid;

  MINIBASE_BM->unpinPage(dataPageId); // unpin the current data page
  status = dirPage->nextRecord(cur_dp_rid, dataPageRid);
  if (status != DONE) {
    status = dirPage->getRecord(dataPageRid, (char *)&dpinfo, rec_len);
    status = MINIBASE_BM->pinPage(dpinfo.pageId, pptr); // pin next data page
    dataPage = (HFPage *) pptr;
    dataPageId = dpinfo.pageId;
    dataPage->firstRecord(userRid); // rid of the first record
  } else {
    return DONE;
  }
  return OK;
}

// *******************************************
// Retrieve the next directory page.
Status Scan::nextDirPage()
{
  Status status;
  Page *pptr;
  //MINIBASE_BM->unpinPage(dirPageId); // unpin the current dir
  PageId next_dir_pid = dirPage->getNextPage();
  if (next_dir_pid == INVALID_PAGE) {
    return DONE;
  } else {
    dirPageId = next_dir_pid;
    // pin the dir header page (first page in the heap file)
    status = MINIBASE_BM->pinPage(dirPageId, pptr);
    dirPage = (HFPage *)pptr;

    firstDataPage();
  }
  return OK;
}
