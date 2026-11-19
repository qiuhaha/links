// pthread_cond_xxx()的简化实现代码：
pthread_cond_wait(mutex, cond):
    count = cond->count; // 关键：释放mutex前先记录当前count
    pthread_mutex_unlock(mutex);
    // 此时pthread_cond_signal()可能被调用并修改cond->count
    pthread_mutex_lock(cond->mutex);
    if (count == cond->count)
    {
        // 创建队列节点me
        me->next_cond = cond->waiter;
        cond->waiter = me;
        pthread_mutex_unlock(cond->mutex);
        unable_to_run(me); // 阻塞自己
    }
    else
        pthread_mutex_unlock(cond->mutex);
    pthread_mutex_lock(mutex);


pthread_cond_signal(cond):
    pthread_mutex_lock(cond->mutex);
    cond->count++;
    if (cond->waiter)
    {
        sleeper = cond->waiter;
        cond->waiter = sleeper->next_cond;
        able_to_run(sleeper); // 唤醒等待线程
    }
    pthread_mutex_unlock(cond->mutex);

