#include "task_meta.h"

#include "task_group.h"


namespace bthread {
    void TaskMeta::SetBoundGroup(bthread::TaskGroup *group, int call_place) {
        LOG(INFO) << "group: " << tls_task_group->group_id_ << " set tid: " << tid << " bound group to : "
                <<(group != nullptr? group->group_id_ : -1) << "caller place: " << call_place;
        bound_task_group = group;
    }
}