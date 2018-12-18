#ifndef MUTEX_H
#define MUTEX_H

class Mutex
{
  private:
    pthread_mutex_t mutex; 

  public:    
    int init()
    {
      mutex = PTHREAD_MUTEX_INITIALIZER;
      return pthread_mutex_init(&mutex, NULL);
    }          
    void lock()
    {
      if (pthread_mutex_lock(&mutex) != 0)
        printf("problem with lock coarse-grained\n");
    }

    void unlock()
    {
      if (pthread_mutex_unlock(&mutex) != 0)
        printf("problem with unlock coarse-grained\n");
    }
};

#endif