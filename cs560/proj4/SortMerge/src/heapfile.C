#include "heapfile.h"

using namespace std;

// ******************************************************
// Error messages for the heapfile layer

static const char *hfErrMsgs[] = {
    "bad record id",
    "bad record pointer", 
    "end of file encountered",
    "invalid update operation",
    "no space on page for record", 
    "page is empty - no records",
    "last record on page",
    "invalid slot number",
    "file has already been deleted",
};

static int Debug = 0;

static error_string_table hfTable( HEAPFILE, hfErrMsgs );

// ********************************************************
// Constructor
HeapFile::HeapFile( const char *name, Status& returnStatus )
{
  Status status;
  HFPage *dirHeader; // dir header page

  // try opening the file, if not exist, create a new one.
  status = MINIBASE_DB->get_file_entry(name, firstDirPageId);
  if (status != OK) { // heapfile of name 'name' doesn't exists in DB
    //cout << "heap file " << name << " not in the DB" << endl;
    Page *first_page_ptr;
    // create a new dir page via buffer manager.
    // TODO: check status
    status = MINIBASE_BM->newPage(firstDirPageId, first_page_ptr);

    // add a new file entry into DB with the page id returned by BM
    // TODO: check status
    status = MINIBASE_DB->add_file_entry(name, firstDirPageId);

    // set up HFPage for the directory header page
    dirHeader = (HFPage *) first_page_ptr;
    dirHeader->init(firstDirPageId);

    // let BM unpin the header page and set dirty bit as True.
    MINIBASE_BM->unpinPage(firstDirPageId, TRUE);
  } else { // heapfile 'name' is already in DB
    //cout << "heap file " << name << " already in the DB" << endl;
  }

  this->fileName = (char *) malloc(strlen(name) + 1);
  strncpy(this->fileName, name, strlen(name) + 1);
  this->file_deleted = 0; // false

  if (Debug) {
    cout << "DB Name: " << MINIBASE_DB->db_name() << endl;
    cout << "Heap file: " << this->fileName << " set up with first dir page id: "
         << firstDirPageId << endl;
  }

  returnStatus = OK;
}

// ******************
// Destructor
HeapFile::~HeapFile()
{
  //MINIBASE_DB->delete_file_entry(this->fileName);
}

// *************************************
// Return number of records in heap file
int HeapFile::getRecCnt()
{
  HFPage *dir_page;
  Page *pptr;
  Status status;
  PageId pid_runner = firstDirPageId;
  int rec_count = 0;

  while (TRUE) {
    // pin current dir page
    status = MINIBASE_BM->pinPage(pid_runner, pptr);
    dir_page = (HFPage *)pptr;

    /* iterate through entries to get count of records */
    RID cur_rid;
    status = dir_page->firstRecord(cur_rid);
    if (status == OK) {
      DataPageInfo dpinfo;
      int rec_len;
      status = dir_page->getRecord(cur_rid, (char *)&dpinfo, rec_len);
      rec_count += dpinfo.recct;
      RID next_rid;
      while (dir_page->nextRecord(cur_rid, next_rid) == OK) {
        status = dir_page->getRecord(next_rid, (char *)&dpinfo, rec_len);
        rec_count += dpinfo.recct;
        cur_rid = next_rid;
      }
    }

    MINIBASE_BM->unpinPage(pid_runner);
    PageId next_dir_pid = dir_page->getNextPage();
    if (next_dir_pid == INVALID_PAGE) {
      break;
    } else {
      pid_runner = next_dir_pid;
    }
  }

  return rec_count;
}

// *****************************
// Insert a record into the file
Status HeapFile::insertRecord(char *recPtr, int recLen, RID& outRid)
{
  HFPage *dir_page; // dir header page
  HFPage *data_page; // data page
  Page *pptr;
  Status status;
  PageId pid_runner = firstDirPageId;
  int rec_inserted = FALSE;

  while (TRUE) { // outer loop: iterate through dir pages
    // pin current dir page
    status = MINIBASE_BM->pinPage(pid_runner, pptr);
    dir_page = (HFPage *)pptr;

    /* iterate through entries to find an avaiable data page to insert */
    RID cur_rid;
    status = dir_page->firstRecord(cur_rid);
    if (status == DONE) { // dir page is empty
      DataPageInfo dpinfo;
      newDataPage(&dpinfo); // need to bring in a data page

      // insert the dpinfo record into dir page
      RID dpinfo_rid;
      PageId dir_pid;

      status = MINIBASE_BM->pinPage(dpinfo.pageId, pptr); // pin data page
      data_page = (HFPage *)pptr;
      // cout << "Create data page " << dpinfo.pageId << endl;

      RID rec_rid;
      if (data_page->available_space() < recLen) {
        // record length too long to fit in an empty page! unpin pages, log error and return
        MINIBASE_BM->unpinPage(dpinfo.pageId); // unpin data page
        MINIBASE_BM->unpinPage(pid_runner); // unpin dir page
        return MINIBASE_FIRST_ERROR(HEAPFILE, 4);
      }
      data_page->insertRecord(recPtr, recLen, rec_rid); // no need to check space since page is just created
      dpinfo.availspace = data_page->available_space();
      dpinfo.recct++;
      allocateDirSpace(&dpinfo, dir_pid, dpinfo_rid);

      MINIBASE_BM->unpinPage(dpinfo.pageId, TRUE);
      rec_inserted = TRUE; // let the outer loop know we're done
    } else { // dir page not empty, iterate through its entries
      while (TRUE) { // inner loop: iterate through entries of the current dir page
        DataPageInfo dpinfo;
        int rec_len;
        status = dir_page->getRecord(cur_rid, (char *)&dpinfo, rec_len);
        status = MINIBASE_BM->pinPage(dpinfo.pageId, pptr); // pin data page
        data_page = (HFPage *)pptr;

        if (data_page->available_space() >= recLen) {
          RID rec_rid;
          data_page->insertRecord(recPtr, recLen, rec_rid);
          MINIBASE_BM->unpinPage(dpinfo.pageId, TRUE);

          // udpate dpinfo and write it back to dir page
          dpinfo.availspace = data_page->available_space();
          dpinfo.recct++;
          int reclen; char *recptr;
          status = dir_page->returnRecord(cur_rid, recptr, reclen);
          memcpy(recptr, &dpinfo, reclen);

          rec_inserted = TRUE; // let the outer loop know we're done
          // cout << "Insert a record into data page " << dpinfo.pageId << endl;
          // cout << ", avaiable space:  " << ((DataPageInfo *)recptr)->availspace << endl;
          break;
        } else { // no space on the current data page, try to get next
          RID next_rid;
          MINIBASE_BM->unpinPage(dpinfo.pageId, TRUE);
          status = dir_page->nextRecord(cur_rid, next_rid);
          if (status == DONE) { // no more dpinfo entry, create one if there is space on dir page
            if (dir_page->available_space() >= sizeof(DataPageInfo)) {
              DataPageInfo dpinfo;
              newDataPage(&dpinfo); // need to bring in a new data page
              // cout << "Create data page " << dpinfo.pageId << endl;

              status = MINIBASE_BM->pinPage(dpinfo.pageId, pptr); // pin data page
              data_page = (HFPage *)pptr;

              if (data_page->available_space() < recLen) {
                // record length too long to fit in an empty page! log error and return
                MINIBASE_BM->unpinPage(dpinfo.pageId); // unpin data page
                MINIBASE_BM->unpinPage(pid_runner); // unpin dir page
                return MINIBASE_FIRST_ERROR(HEAPFILE, 4);
              }

              RID rec_rid;
              data_page->insertRecord(recPtr, recLen, rec_rid); // no need to check space since page is just created???

              dpinfo.availspace = data_page->available_space();
              dpinfo.recct++;
              // insert the dpinfo record into dir page
              RID dpinfo_rid;
              PageId dir_pid;
              allocateDirSpace(&dpinfo, dir_pid, dpinfo_rid);

              MINIBASE_BM->unpinPage(dpinfo.pageId, TRUE);
              rec_inserted = TRUE; // let the outer loop know we're done
              break;
            } else { // if current dir page has no space, move to the next dir page
              rec_inserted = FALSE; // let the outer loop know we're not done
              break;
            }
          } else {
            cur_rid = next_rid;
          }
        }
      }
    }

    if (rec_inserted) { // done
      MINIBASE_BM->unpinPage(pid_runner, TRUE);
      break;
    } else { // go to the next dir page
      MINIBASE_BM->unpinPage(pid_runner, TRUE);
      HFPage *hfptr;
      PageId next_dir_pid = dir_page->getNextPage();
      if (next_dir_pid == INVALID_PAGE) {
        Page *pptr;
        // create a new dir page via buffer manager.
        status = MINIBASE_BM->newPage(next_dir_pid, pptr);

        // set up HFPage for the new directory page
        hfptr = (HFPage *) pptr;
        hfptr->init(next_dir_pid);
        dir_page->setNextPage(next_dir_pid);
        hfptr->setPrevPage(pid_runner);

        // let BM unpin the header page and set dirty bit as True.
        MINIBASE_BM->unpinPage(next_dir_pid, TRUE);
      }

      pid_runner = next_dir_pid;
    }
  }

  return OK;
}


// ***********************
// delete record from file
Status HeapFile::deleteRecord (const RID& rid)
{
  HFPage *dir_page; // dir header page
  HFPage *data_page; // data page
  Page *pptr;
  Status status;
  PageId dir_pid_runner = firstDirPageId;
  int rec_deleted = FALSE;
  DataPageInfo dpinfo;
  RID cur_dir_rid, next_dir_rid;

  while (TRUE) { // outer loop: iterate through dir pages
    // pin current dir page
    status = MINIBASE_BM->pinPage(dir_pid_runner, pptr);
    dir_page = (HFPage *)pptr;

    /* iterate through dir entries to find an avaiable data page to insert */
    //RID cur_dir_rid, next_dir_rid;
    status = dir_page->firstRecord(cur_dir_rid);
    if (status == OK) {
      while (TRUE) {
        //DataPageInfo dpinfo;
        int rec_len;
        RID cur_rec_rid, next_rec_rid;
        dir_page->getRecord(cur_dir_rid, (char *)&dpinfo, rec_len);
        MINIBASE_BM->pinPage(dpinfo.pageId, pptr); // pin data page
        data_page = (HFPage *) pptr;

        data_page->firstRecord(cur_rec_rid); // rid of the first record
        for (int i = 0; i < dpinfo.recct; ++i) {
          if (cur_rec_rid == rid) {
            data_page->deleteRecord(rid);
            rec_deleted = TRUE;
            dpinfo.recct--; // update record count
            dpinfo.availspace = data_page->available_space(); // update space

            int reclen; char *recptr;
            status = dir_page->returnRecord(cur_dir_rid, recptr, reclen);
            memcpy(recptr, &dpinfo, reclen); // update dpinfo in the dir page
            break;
          } else {
            if ((i+1) < dpinfo.recct) {
              data_page->nextRecord(cur_rec_rid, next_rec_rid);
              cur_rec_rid = next_rec_rid;
            }
          }
        }
        if (rec_deleted) {
          // uppin the current data page, set the dirty bit as TRUE
          MINIBASE_BM->unpinPage(dpinfo.pageId, TRUE);
          break;
        } else { // go to the next data page of the current dir page
          MINIBASE_BM->unpinPage(dpinfo.pageId);
          status = dir_page->nextRecord(cur_dir_rid, next_dir_rid);
          if (status == OK) cur_dir_rid = next_dir_rid;
          else break;
        }
      }
    }

    if (rec_deleted) {
      MINIBASE_BM->unpinPage(dir_pid_runner, TRUE); // unpin the current dir page
      break;
    } else {
      MINIBASE_BM->unpinPage(dir_pid_runner);
      PageId next_dir_pid = dir_page->getNextPage();
      if (next_dir_pid == INVALID_PAGE) {
        return DONE;
      } else {
        dir_pid_runner = next_dir_pid;
      }
    }
  }

  if (rec_deleted) {
    // TODO: need to free the data_page or dir_page if they become empty
    //cout << "Data page " << dpinfo.pageId << " reccnt: "
    //<< dpinfo.recct << " available space: " << dpinfo.availspace << endl;

    if (dpinfo.recct == 0) {
      status = MINIBASE_BM->freePage(dpinfo.pageId);
      if (status == OK) {
        // cout << "Data page " << dpinfo.pageId << " is deleted!\n";

        status = dir_page->deleteRecord(cur_dir_rid);
        if (status == OK) {
          //cout << "Data page info (" << dpinfo.pageId << ", "
          //   << dpinfo.recct << ", " << dpinfo.availspace << ") is deleted\n";
       }
      }
    }
  }

  return OK;
}

// *******************************************
// updates the specified record in the heapfile.
Status HeapFile::updateRecord (const RID& rid, char *recPtr, int recLen)
{
  PageId dir_page_id, data_page_id;
  HFPage *dir_page, *data_page;
  RID dpinfo_rid;
  Status status;

  status = findDataPage(rid, dir_page_id, dir_page, data_page_id, data_page, dpinfo_rid);
  if (status == OK) {
    char *rec_ptr; int rec_len;
    data_page->returnRecord(rid, rec_ptr, rec_len);
    if (recLen != rec_len) { // log error and return
      MINIBASE_BM->unpinPage(dir_page_id);
      MINIBASE_BM->unpinPage(data_page_id);
      return MINIBASE_FIRST_ERROR(HEAPFILE, 3);
      //return HEAPFILE; // error: update can't change record length
    }
    else {
      memcpy(rec_ptr, recPtr, rec_len); // update record
      MINIBASE_BM->unpinPage(dir_page_id);
      MINIBASE_BM->unpinPage(data_page_id, TRUE);
      return OK;
    }
  } else {
    return DONE;
  }

  return OK;
}

// ***************************************************
// read record from file, returning pointer and length
Status HeapFile::getRecord (const RID& rid, char *recPtr, int& recLen)
{
  PageId dir_page_id, data_page_id;
  HFPage *dir_page, *data_page;
  RID dpinfo_rid;
  Status status;

  status = findDataPage(rid, dir_page_id, dir_page, data_page_id, data_page, dpinfo_rid);
  if (status == OK) {
    char *rec_ptr; int rec_len;
    data_page->getRecord(rid, recPtr, rec_len);
    MINIBASE_BM->unpinPage(dir_page_id);
    MINIBASE_BM->unpinPage(data_page_id);
    return OK;
  } else {
    return DONE;
  }
}

// **************************
// initiate a sequential scan
Scan *HeapFile::openScan(Status& status)
{
  Status s;
  Scan *scanner = new Scan(this, s);

  return scanner;
}

// ****************************************************
// Wipes out the heapfile from the database permanently. 
Status HeapFile::deleteFile()
{
  if (file_deleted) {
    return MINIBASE_FIRST_ERROR(HEAPFILE, 8);
  }

  HFPage *dir_page; // dir header page
  HFPage *data_page; // data page
  Page *pptr;
  Status status;
  PageId dir_pid_runner = firstDirPageId;
  int rec_found = FALSE;

  //cout << MINIBASE_BM->getNumBuffers() << endl;
  //cout << MINIBASE_BM->getNumUnpinnedBuffers() << endl;

  MINIBASE_BM->pinPage(dir_pid_runner, pptr);
  dir_page = (HFPage *)pptr;
  RID first_rid; int rec_len;
  dir_page->firstRecord(first_rid);
  DataPageInfo dpinfo;
  dir_page->getRecord(first_rid, (char *)&dpinfo, rec_len);
  MINIBASE_BM->unpinPage(dpinfo.pageId);

  MINIBASE_BM->unpinPage(dir_pid_runner);
  MINIBASE_BM->unpinPage(dir_pid_runner);

  while (TRUE) { // outer loop: iterate through dir pages
    // pin current dir page
    status = MINIBASE_BM->pinPage(dir_pid_runner, pptr);
    dir_page = (HFPage *)pptr;

    /* iterate through dir entries to find an avaiable data page to insert */
    RID cur_dir_rid, next_dir_rid;
    dir_page->firstRecord(cur_dir_rid);
    while (TRUE) {
      DataPageInfo dpinfo;
      int rec_len;
      RID cur_rec_rid, next_rec_rid;
      dir_page->getRecord(cur_dir_rid, (char *)&dpinfo, rec_len);

      status = MINIBASE_BM->freePage(dpinfo.pageId); // delete data page
      //      cout << "free data page " << dpinfo.pageId << endl;
      if (status != OK) {
        cout << "Error on free data page " << dpinfo.pageId << endl;
        cout << MINIBASE_BM->getNumBuffers() << endl;
        cout << MINIBASE_BM->getNumUnpinnedBuffers() << endl;
        MINIBASE_SHOW_ERRORS();
      }

      status = dir_page->nextRecord(cur_dir_rid, next_dir_rid);
      if (status == OK) cur_dir_rid = next_dir_rid;
      else break;
    }

    PageId next_dir_pid = dir_page->getNextPage();
    status = MINIBASE_BM->unpinPage(dir_pid_runner);
    if (status != OK) {
      cout << "Couldn't unpin dir page " << dir_pid_runner << endl;
      MINIBASE_SHOW_ERRORS();
    }

    status = MINIBASE_BM->freePage(dir_pid_runner);
    //cout << "free dir page " << dir_pid_runner << endl;
    if (status != OK) {
      cout << "Error on free dir page " << dir_pid_runner << endl;
      MINIBASE_SHOW_ERRORS();
    }

    if (next_dir_pid == INVALID_PAGE) {
      break;
    } else {
      dir_pid_runner = next_dir_pid;
    }
  }

  MINIBASE_DB->delete_file_entry(fileName);

  //  cout << "# of unpinned buffer: " << MINIBASE_BM->getNumUnpinnedBuffers() << endl;
  // cout << "# of buffer: " << MINIBASE_BM->getNumBuffers() << endl;
  return OK;
}

// ****************************************************************
// Get a new datapage from the buffer manager and initialize dpinfo
// (Allocate pages in the db file via buffer manager)
Status HeapFile::newDataPage(DataPageInfo *dpinfop)
{
  Status status;
  PageId pid;
  HFPage *hfpptr;
  Page *pptr;

  // TODO: check status
  status = MINIBASE_BM->newPage(pid, pptr);
  hfpptr = (HFPage *)pptr;
  hfpptr->init(pid);
  dpinfop->availspace = hfpptr->available_space();
  dpinfop->recct = 0;
  dpinfop->pageId = pid;

  MINIBASE_BM->unpinPage(pid, TRUE); // unpin it

  return OK;
}

// ************************************************************************
// Internal HeapFile function (used in getRecord and updateRecord): returns
// pinned directory page and pinned data page of the specified user record
// (rid).
//
// If the user record cannot be found, rpdirpage and rpdatapage are 
// returned as NULL pointers.
//
Status HeapFile::findDataPage(const RID& rid,
                    PageId &rpDirPageId, HFPage *&rpdirpage,
                    PageId &rpDataPageId,HFPage *&rpdatapage,
                    RID &rpDataPageRid)
{
  HFPage *dir_page; // dir header page
  HFPage *data_page; // data page
  Page *pptr;
  Status status;
  PageId dir_pid_runner = firstDirPageId;
  int rec_found = FALSE;

  while (TRUE) { // outer loop: iterate through dir pages
    // pin current dir page
    status = MINIBASE_BM->pinPage(dir_pid_runner, pptr);
    dir_page = (HFPage *)pptr;

    /* iterate through dir entries to find an avaiable data page to insert */
    RID cur_dir_rid, next_dir_rid;
    dir_page->firstRecord(cur_dir_rid);
    while (TRUE) {
      DataPageInfo dpinfo;
      int rec_len;
      RID cur_rec_rid, next_rec_rid;
      dir_page->getRecord(cur_dir_rid, (char *)&dpinfo, rec_len);
      MINIBASE_BM->pinPage(dpinfo.pageId, pptr); // pin data page
      data_page = (HFPage *) pptr;

      data_page->firstRecord(cur_rec_rid); // rid of the first record
      for (int i = 0; i < dpinfo.recct; ++i) {
        if (cur_rec_rid == rid) {
          rec_found = TRUE;
          rpDirPageId = dir_pid_runner; // return value
          rpdirpage = dir_page; // return value
          rpDataPageId = dpinfo.pageId; // return value
          rpdatapage = data_page; // return value
          rpDataPageRid = cur_dir_rid; // return the rid of the data page contains the record
          return OK;
          //break;
        } else {
          if ((i+1) < dpinfo.recct) {
            data_page->nextRecord(cur_rec_rid, next_rec_rid);
            cur_rec_rid = next_rec_rid;
          }
        }
      }
      // record rid not found in current data page, move to the next
      MINIBASE_BM->unpinPage(dpinfo.pageId);
      status = dir_page->nextRecord(cur_dir_rid, next_dir_rid);
      if (status == OK) cur_dir_rid = next_dir_rid;
      else break;
    }

    MINIBASE_BM->unpinPage(dir_pid_runner);
    PageId next_dir_pid = dir_page->getNextPage();
    if (next_dir_pid == INVALID_PAGE) {
      return DONE;
    } else {
      dir_pid_runner = next_dir_pid;
    }
  }

  return OK; // should never get to here
}

// *********************************************************************
// Allocate directory space for a heap file page 

Status HeapFile::allocateDirSpace(struct DataPageInfo * dpinfop,
                                  PageId &allocDirPageId,
                                  RID &allocDataPageRid)
{
  HFPage *dir_ptr; // dir header page
  Page *pptr;
  Status status;
  PageId pid_runner;
  RID rid;

  pid_runner = firstDirPageId;
  while (TRUE) {
    // pin dir page
    MINIBASE_BM->pinPage(pid_runner, pptr);
    dir_ptr = (HFPage *)pptr;

    if (dir_ptr->available_space() >= sizeof(DataPageInfo)) {
      dir_ptr->insertRecord((char *)dpinfop, sizeof(DataPageInfo), rid);

      MINIBASE_BM->unpinPage(pid_runner, TRUE); // unpin the current dir page, dirty: TRUE
      break;
    } else {
      MINIBASE_BM->unpinPage(pid_runner); // unpin the current dir page
      pid_runner = dir_ptr->getNextPage(); // move to the next dir page
      if (pid_runner == INVALID_PAGE) {
        cout << "TODO: need to create a new dir page!!!" << endl; // never get to here
        break;
      }
    }
  }

  return OK;
}

// *******************************************
