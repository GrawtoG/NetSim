#include "package.hxx"
std::set<ElementID> Package::freed_ids = {};
std::set<ElementID> Package::assigned_ids = {};

Package::Package()
{
  if (!freed_ids.empty())
  {
    Package_id = *freed_ids.begin();
    freed_ids.erase(freed_ids.begin());
    assigned_ids.insert(Package_id);
  }
  else
  {
    if(assigned_ids.empty())
    {
        Package_id = 1;
        assigned_ids.insert(Package_id);
    }
    else
    {
      Package_id = *assigned_ids.rbegin() + 1;
      assigned_ids.insert(Package_id);
    }
  }

}

Package::~Package() {
    assigned_ids.erase(Package_id);
    freed_ids.insert(Package_id);
}