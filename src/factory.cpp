#include "factory.hxx"

void Factory::do_deliveries(Time t) {
    for (auto &ramp : container_ramps)
        ramp.deliver_goods(t);
}

void Factory::do_work(Time t ) {
    for (auto& worker : container_workers)
        worker.do_work(t);
}

void Factory::do_package_passing() {
    for (auto &ramp : container_ramps)
        ramp.send_package();

    for (auto &worker : container_workers)
        worker.send_package();
}