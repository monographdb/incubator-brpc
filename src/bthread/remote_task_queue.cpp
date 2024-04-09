#include "remote_task_queue.h"

#include "task_group.h"
#include "task_group_inl.h"

namespace bthread {
    bool RemoteQueue::pop(bthread_t *task) {
        int tmp_cnt = _task_cnt.load(std::memory_order_acquire);
        if (_tasks.try_dequeue(*task)) {
            tmp_cnt--;
            TaskMeta* m = TaskGroup::address_meta(*task);
            if (!is_bound_queue && m != nullptr && m->bound_task_group != nullptr)
            {
                LOG(ERROR) << "group: " << tls_task_group->group_id_ << " pop from remote rq, a bounded tid: " << *task;
            }
            return true;
        } else {
            return false;
        }
        if (tmp_cnt > 0 &&
            _task_cnt.compare_exchange_strong(tmp_cnt, tmp_cnt - 1)) {
            if (_tasks.try_dequeue(*task)) {
                return true;
            } else {
                _task_cnt++;
                return false;
            }
        }
    }

    bool RemoteQueue::push(bthread_t task) {
        TaskMeta* m = TaskGroup::address_meta(task);
        if (!is_bound_queue && m != nullptr && m->bound_task_group != nullptr) {
//            LOG(INFO) << "test";
            LOG(ERROR) << "group: " << (tls_task_group != nullptr ? tls_task_group->group_id_ : -1) << " push into remote rq, a bounded tid: " << task;
            assert(false);
        }
        if (_tasks.enqueue(task)) {
//            LOG(INFO) << "group: " << tls_task_group->group_id_ << " pop from remote rq, tid: " << *task;
            _task_cnt++;
            return true;
        }
        return false;
    }
}