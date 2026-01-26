#pragma once
#include <helpers.hxx>
#include <map>
#include <optional>

#include "storage_types.hxx"
class IPackageReceiver{
public:
    virtual ElementID get_id() const = 0;
    virtual void receive_package(Package&&) = 0;
    virtual ~IPackageReceiver() = default;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
};
class ReceiverPreferences
{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    ReceiverPreferences(ProbabilityGenerator pg = probability_generator);
    void add_receiver(IPackageReceiver& r);
    void remove_receiver(IPackageReceiver& r);
    IPackageReceiver* choose_recveiver();
    const preferences_t& get_preferences()const {return preferences;};

private:
    preferences_t preferences;
    ProbabilityGenerator pg_;
    IPackageReceiver* receiver;
};
class PackageSender
{
public:
    ReceiverPreferences receiverPreferences_;
    void send_package();
    std::optional<Package>& get_sending_buffer() {return buffer;};
    protected:
        void push_package(Package&& package);
private:
    std::optional<Package> buffer;
};

class ramp : ReceiverPreferences{
public:
    ramp(ElementID id): ramp_id(id) {}
    ElementID get_id() const {return ramp_id;};
    int get_delivery_interval() const {return delivery_interval;};
    void deliver_goods();
private:
    ElementID ramp_id;
    int delivery_interval;
};
class storehouse : IPackageReceiver {
public:
    storehouse(ElementID id, std::unique_ptr<IPackageStockpile> stockpile_ptr): stockpile(std::move(stockpile_ptr)), storehouse_id(id) {};
    void add_products(std::list<Package>);
    ElementID get_id() const override {return storehouse_id;};
    void receive_package(Package&& package) override;
    IPackageStockpile::const_iterator cbegin() const override{ return stockpile->cbegin();};
    IPackageStockpile::const_iterator cend() const override{ return stockpile->cend();};
    IPackageStockpile::const_iterator begin() const override{ return stockpile->begin();};
    IPackageStockpile::const_iterator end() const override{ return stockpile->end();};
private:
    std::unique_ptr<IPackageStockpile> stockpile;
    ElementID storehouse_id;
};
class Worker : IPackageReceiver, ReceiverPreferences {
public:
    Worker(ElementID id, IPackageQueue* queue_ptr, Time proc_time, PackageQueueType q_type ): queue(queue_ptr), worker_id(id), processing_time(proc_time), queue_type(q_type) {}
    Worker(int i, int i1, const std::unique_ptr<::PackageQueue>& unique);;
    int get_processing_time() const {return processing_time;};
    void receive_package(Package&& package) override;
    IPackageStockpile::const_iterator cbegin() const override{ return queue->cbegin();};
    IPackageStockpile::const_iterator cend() const override{ return queue->cend();};
    IPackageStockpile::const_iterator begin() const override{ return queue->begin();};
    IPackageStockpile::const_iterator end() const override{ return queue->end();};
    ElementID get_id() const override {return worker_id;};
private:
    IPackageQueue* queue;
    ElementID worker_id;
    Time processing_time;
    PackageQueueType queue_type;
};
