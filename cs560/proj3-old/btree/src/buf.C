/*****************************************************************************/
/*************** Implementation of the Buffer Manager Layer ******************/
/*****************************************************************************/


#include "buf.h"


// Define buffer manager error messages here
//enum bufErrCodes  {...};

// Define error message here
static const char* bufErrMsgs[] = { 
  // error message strings go here
  "Not enough memory to allocate hash entry",
  "Inserting a duplicate entry in the hash table",
  "Removing a non-existing entry from the hash table",
  "Page not in hash table",
  "Not enough memory to allocate queue node",
  "Poping an empty queue",
  "OOOOOOPS, something is wrong",
  "Buffer pool full",
  "Not enough memory in buffer manager",
  "Page not in buffer pool",
  "Unpinning an unpinned page",
  "Freeing a pinned page"
};

// Create a static "error_string_table" object and register the error messages
// with minibase system 
static error_string_table bufTable(BUFMGR,bufErrMsgs);

//*************************************************************
//** This is the implementation of BufMgr
//************************************************************

BufMgr::BufMgr (int numbuf, Replacer *replacer)
{
  int i;

  /* initialize buffer pool and frame descriptor */
  numBuffers = numbuf;
  bufPool = new Page[numBuffers];
  frameDesc = new FrameDesc[numBuffers];

  for (i = 0; i < (int)numBuffers; ++i) {
    frameDesc[i].pid = INVALID_PAGE;
    frameDesc[i].pin_count = 0;
    frameDesc[i].dirtybit = FALSE;
  }

  // set all frames as hated at the beginning,
  // indicating they are all replacible.
  for (i = numBuffers-1; i >= 0; --i) {
    hatedMRU.push(i);
  }

  // initialize hash table
  htable.init();

  //  cout << "number of bufPool: " << numbuf << endl;
  // cout << "sizeof page: " << sizeof(Page) << endl;
}

//*************************************************************
//** This is the implementation of ~BufMgr
//************************************************************
BufMgr::~BufMgr()
{
  // put your code here
}

//*************************************************************
//** This is the implementation of pinPage
//************************************************************
Status BufMgr::pinPage(PageId PageId_in_a_DB, Page*& page, int emptyPage)
{
  Status status;

  // check if buffer pool is full
  if (hatedMRU.empty() && lovedLRU.empty()) {
    return MINIBASE_FIRST_ERROR(BUFMGR, 7);
  }

  // find if the requested page is in the buffer pool or not?
  int frame_num = htable.lookup(PageId_in_a_DB);
  if (frame_num < 0) { // not in the buffer pool
    // find a frame to replace
    int frame2replace;
    if (!hatedMRU.empty()) {
      // TODO: handle the case where a page was loved and hated at the same time
      frame2replace = hatedMRU.top();
      hatedMRU.pop();
      //cout << "popping frame "<< frame2replace << " from hated list" << endl;
    } else if (!lovedLRU.empty()) {
      frame2replace = lovedLRU.front();
      lovedLRU.pop();
      //cout << "popping frame "<< frame2replace << " from loved list" << endl;
    } else { // buffer pool full
      //cout << "buffer pool is full!!!\n";
      return MINIBASE_FIRST_ERROR(BUFMGR, 7);
    }

    if (frameDesc[frame2replace].pid != INVALID_PAGE) {
      // flush the old page if it's dirty
      if (frameDesc[frame2replace].pin_count == 0 &&
          frameDesc[frame2replace].dirtybit == TRUE) {
        flushPage(frameDesc[frame2replace].pid);
      }
      // remove the entry for the old page from the hash table
      htable.remove(frameDesc[frame2replace].pid);
    }
    // read the requested page into the frame
    status = MINIBASE_DB->read_page(PageId_in_a_DB, &bufPool[frame2replace]);
    if (status != OK) {
      return status;
    }

    // return the pointer to the requested page for the upper level to use
    page = &(bufPool[frame2replace]);

    frameDesc[frame2replace].pid = PageId_in_a_DB;
    frameDesc[frame2replace].pin_count = 1; // pin the page
    frameDesc[frame2replace].dirtybit = FALSE;

    htable.insert(PageId_in_a_DB, frame2replace); // update hash table with the new entry
  } else {
    frameDesc[frame_num].pin_count += 1;
    page = &(bufPool[frame_num]);
  }

  return OK;
}//end pinPage

//*************************************************************
//** This is the implementation of unpinPage
//************************************************************
Status BufMgr::unpinPage(PageId page_num, int dirty=FALSE, int hate = FALSE)
{
  int frame_num = htable.lookup(page_num);

  if (frame_num < 0) {
    return MINIBASE_FIRST_ERROR(BUFMGR, 3);
  }

  //cout << "frame " << frame_num << " holds page " << page_num << endl;
  if (frameDesc[frame_num].pin_count <= 0) {
    return MINIBASE_FIRST_ERROR(BUFMGR, 10);
  }

  frameDesc[frame_num].pin_count--;
  frameDesc[frame_num].dirtybit = dirty;

  if (frameDesc[frame_num].pin_count == 0) {
    if (hate == TRUE) {
      int found = FALSE;
      // check if 'frame_num' is already in the list
      stack<int> hatedMRU_copy(hatedMRU);
      while (!hatedMRU_copy.empty()) {
        if (hatedMRU_copy.top() == frame_num) {
          found = TRUE;
          break;
        }
        hatedMRU_copy.pop();
      }
      if (found != TRUE)
        hatedMRU.push(frame_num);
    } else {
      int found = FALSE;
      queue<int> lovedLRU_copy(lovedLRU);
      while (!lovedLRU_copy.empty()) {
        if (lovedLRU_copy.front() == frame_num) {
          found = TRUE;
          //cout << frame_num << " is already in the loved LRU" << endl;
          break;
        }
        lovedLRU_copy.pop();
      }

      if (found != TRUE)
        lovedLRU.push(frame_num);
    }

    if (frameDesc[frame_num].dirtybit == TRUE) {
      flushPage(frameDesc[frame_num].pid);
    }

  }

  return OK;
}

//*************************************************************
//** This is the implementation of newPage
//************************************************************
Status BufMgr::newPage(PageId& firstPageId, Page*& firstpage, int howmany)
{
  assert(howmany > 0);

  //cout << numBuffers << ", " << hatedMRU.size() << "," << lovedLRU.size() << endl;
  MINIBASE_DB->allocate_page(firstPageId, howmany);

  // check if buffer pool is full
  if (hatedMRU.empty() && lovedLRU.empty()) {
    // cout << "buffer pool is full" << endl;
    MINIBASE_DB->deallocate_page(firstPageId, howmany);
    return MINIBASE_FIRST_ERROR(BUFMGR, 7);
  }

  pinPage(firstPageId, firstpage, 0);

  return OK;
}

//*************************************************************
//** This is the implementation of freePage
//************************************************************
Status BufMgr::freePage(PageId globalPageId)
{
  int frame_num = htable.lookup(globalPageId);
  if (frameDesc[frame_num].pin_count > 0) {
    return MINIBASE_FIRST_ERROR(BUFMGR, 11);
  }

  return MINIBASE_DB->deallocate_page(globalPageId);
}

//*************************************************************
//** This is the implementation of flushPage
//************************************************************
Status BufMgr::flushPage(PageId pageid)
{
  int frame_num = htable.lookup(pageid);

  MINIBASE_DB->write_page(pageid, &bufPool[frame_num]);
  return OK;
}

//*************************************************************
//** This is the implementation of flushAllPages
//************************************************************
Status BufMgr::flushAllPages()
{
  for (unsigned int i = 0; i < numBuffers; ++i) {
    if (frameDesc[i].pid != INVALID_PAGE && frameDesc[i].dirtybit == TRUE) {
      if (frameDesc[i].pin_count > 0) {
        cout << "TODO: report an error: flushing an unpinned page" << endl;
      } else {
        flushPage(frameDesc[i].pid);
      }
    }
  }
  return OK;
}


/*** Methods for compatibility with project 1 ***/
//*************************************************************
//** This is the implementation of pinPage
//************************************************************
Status BufMgr::pinPage(PageId PageId_in_a_DB, Page*& page, int emptyPage, const char *filename)
{
  return pinPage(PageId_in_a_DB, page, emptyPage);
}

//*************************************************************
//** This is the implementation of unpinPage
//************************************************************
Status BufMgr::unpinPage(PageId globalPageId_in_a_DB, int dirty, const char *filename)
{
  return unpinPage(globalPageId_in_a_DB, dirty, FALSE);
}

//*************************************************************
//** This is the implementation of getNumUnpinnedBuffers
//************************************************************
unsigned int BufMgr::getNumUnpinnedBuffers()
{
  int unpinned_counter = 0;

  for (int i = 0; i < (int)numBuffers; ++i) {
    if (frameDesc[i].pin_count <= 0) {
      unpinned_counter++;
    } else {
      //cout << "Page " << frameDesc[i].pid << " is still pinnped: "
      //   << frameDesc[i].pin_count << endl;
    }
  }

  //cout << "Num buffers: " << getNumBuffers() << endl;
  //cout << "Num unpinned buffers: " << unpinned_counter << endl;
  return unpinned_counter;
}
