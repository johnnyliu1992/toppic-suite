
#ifndef PROT_THREAD_POOL_HPP_
#define PROT_THREAD_POOL_HPP_

#include <vector>
#include <queue>
#include <iostream>
#include "boost/thread/thread.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"

#include "prsm/prsm_xml_writer.hpp"

namespace prot {

typedef std::shared_ptr<boost::thread> ThreadPtr;

template <typename T>
class ThreadPool {
 public:

  // Constructor.
  ThreadPool(int threads, std::string output_file_name);

  // Destructor.
  ~ThreadPool();

  // Adds task to a task queue.
  void Enqueue(std::function<void()> f);

  // Shut down the pool.
  void ShutDown();

  int getQueueSize() {return tasks.size();}

  int getThreadNum() {return threadPool.size();}

  std::shared_ptr<T> getWriter(boost::thread::id thread_id);

 private:
  // Thread pool storage.
  std::vector<ThreadPtr> threadPool;

  // prsm writer pool
  std::vector<std::pair<boost::thread::id, std::shared_ptr<T>>> writerPool;

  // Queue to keep track of incoming tasks.
  std::queue<std::function<void()> > tasks;

  // Task queue mutex.
  boost::mutex tasksMutex;

  // Condition variable.
  boost::condition_variable condition;

  // Indicates that pool needs to be shut down.
  bool terminate;

  // Indicates that pool has been terminated.
  bool stopped;

  // Function that will be invoked by our threads.
  void Invoke();

  //boost::mutex mtx;

  //int index = 0;

};

//typedef std::shared_ptr<ThreadPool> ThreadPoolPtr;

}

#include "threadpool_impl.hpp"

#endif