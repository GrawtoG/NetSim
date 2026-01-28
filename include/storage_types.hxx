#pragma once
#include "package.hxx"
#include <list>
#include <optional>
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


