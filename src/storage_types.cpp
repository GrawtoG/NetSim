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
    Package package = std::move(queue_container.front());
    queue_container.pop_front();
    return package;
};
