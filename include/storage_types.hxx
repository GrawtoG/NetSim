#pragma once
#include "package.hxx"
#include <list>
#include <bits/unique_ptr.h>

#include "types.hxx"
enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
    public:
      using const_iterator = std::list<Package>::const_iterator;
      virtual void push(Package&&) = 0;
      virtual bool empty() = 0;
      virtual ~IPackageStockpile() = default;
      virtual const_iterator begin() const = 0;
      virtual const_iterator end() const = 0;
      virtual const_iterator cbegin() const = 0;
      virtual const_iterator cend() const = 0;
      virtual size_type size() const = 0;
};

class IPackageQueue: public IPackageStockpile {
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;
};
class PackageQueue : public IPackageQueue{
public:
    PackageQueue(PackageQueueType q_type): queue_type(q_type) {};
    PackageQueue(PackageQueueType type, std::list<Package> cont): queue_type(type), queue_container(std::move(cont)) {}
    const_iterator cbegin() const override{ return queue_container.cbegin();};
    const_iterator cend() const override{ return queue_container.cend();};
    const_iterator begin() const override{ return queue_container.begin();};
    const_iterator end() const override{ return queue_container.end();};
    size_type size() const override{ return static_cast<size_type>(queue_container.size());};
    void push(Package&& package) override;
    bool empty() override {return queue_container.empty();};
    Package pop() override;
    PackageQueueType get_queue_type() override{return queue_type;};
private:
    PackageQueueType queue_type;
    std::list<Package> queue_container ;
};
class ramp {
    public:
        ramp(ElementID id): ramp_id(id) {}
        ElementID get_ramp_id() const {return ramp_id;};
        int get_delivery_interval() const {return delivery_interval;};
    private:
        ElementID ramp_id;
        int delivery_interval;
};
class storehouse {
    public:
        storehouse(ElementID id, IPackageStockpile* stockpile_ptr): stockpile(stockpile_ptr), storehouse_id(id) {};
        IPackageStockpile* get_products() const {return stockpile; };
        void add_products(std::list<Package>);
    private:
        IPackageStockpile* stockpile;
        ElementID storehouse_id;
};
class Worker {
    public:
        Worker(ElementID id, IPackageQueue* queue_ptr, Time proc_time, PackageQueueType q_type ): queue(queue_ptr), worker_id(id), processing_time(proc_time), queue_type(q_type) {}
        Worker(int i, int i1, const std::unique_ptr<::PackageQueue>& unique);;
        ElementID get_worker_id() const {return worker_id;};
        int get_processing_time() const {return processing_time;};
    private:
        IPackageQueue* queue;
        ElementID worker_id;
        Time processing_time;
        PackageQueueType queue_type;
};

