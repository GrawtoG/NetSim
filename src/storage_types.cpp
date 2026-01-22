#include "storage_types.hxx"
void PackageQueue::push(Package&& package) {
    if (queue_type == PackageQueueType::FIFO){
        queue_container.push_back(std::move(package));
    }
    else{
        queue_container.push_front(std::move(package));
    }
};
Package PackageQueue::pop() {

    switch (queue_type) {
        case PackageQueueType::FIFO:
        case PackageQueueType::LIFO:
            Package p=queue_container.front();
            queue_container.pop_front();
            return p;
            break;
    }

    return -1;
};
