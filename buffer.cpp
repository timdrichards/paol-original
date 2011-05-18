
//////////Frame Linked List /////////////////////////////////////
void FrameLinkedList::push(cv::Mat frame)
{
  boost::mutex::scoped_lock lock(list_lock);
  
  //Store image locally
  cv::Mat newFrame = frame;
  
  //Create a pointer object to a frameListItem
  frameListItem* newItem;
  //Set the pointer to a new frameListItem (Making the new item above the scope of push
  newItem = new frameListItem;
  
  if (size == 0)
    {
      oldest = newItem;
      newest = newItem;
    }else
    {
      newest->next = newItem;
    };
  newest = newItem;
  newItem->frame = newFrame;
  size++;
};

cv::Mat FrameLinkedList::pop()
{
  bool moreThanTwo = false;
  cv::Mat toPop;
  while(!moreThanTwo)
    {
      { //Scoped so that the mutex will be released while the thread sleeps
	boost::mutex::scoped_lock lock(list_lock);
	boost::posix_time::seconds sleepTime(2);
	if (size > 2)
	  {
	    moreThanTwo = true;
	    toPop = oldest->frame;
	    frameListItem* toDelete;
	    toDelete = oldest;
	    oldest = toDelete->next;
	    delete toDelete;
	    return toPop;
	  }else
	  {
	    boost::this_thread::sleep(sleepTime);
	  };
      };
    };
};
