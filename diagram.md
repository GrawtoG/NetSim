```plantuml
@startuml
class worker
{
+queue_type(): Package_Queue_Type
+get_worker_id(): uint32
+get_processing_time():char
-queue_type: Package_Queue_Type
-worker_id: ElementID
-processing_time: int
-queue: IPackageQueue*
}
class ramp{
+get_ramp_id(): ElementID
+get_delivery_interval(): int
-ramp_id: int
-delivery_interval: int
}
class storehouse{
-storehouse_id: ElementID
+get_products(): Package
-stockpile: IPackageStockpile*
+add_products(std::list<Package>): void
}




    interface IPackageStockpile <<interface>> {
	    + {abstract} push(Package&&) : void
	    + {abstract} empty() : bool {query}
	    + {abstract} size() : size_type {query}
	    + {method} /iteratory/
	    + ~IPackageStockpile()
	}

	interface IPackageQueue <<interface>> {
	    + {abstract} pop() : Package
	    + {abstract} get_queue_type() : PackageQueueType {query}
	}
	

	enum PackageQueueType <<enumeration>> {
		FIFO
		LIFO
	}
	

	class PackageQueue {
	    - queue_type: PackageQueueType
	    - queue_container: std::list<Package> 
	    + PackageQueue(PackageQueueType)
	    
	}
	

	class Package {
		+ Package()
		+ Package(ElementID)
		+ Package(Package&&)
		+ operator=(Package&&) : Package&
		+ get_id(): ElementID {query}
		+ ~Package()
		-Package_id: ElementID
	}

worker *-- PackageQueueType
worker *-- IPackageQueue
IPackageStockpile *-- Package
PackageQueue ..|> IPackageQueue
worker *-- ElementID
IPackageQueue --|> IPackageStockpile
storehouse *-- ElementID
Package *-- ElementID
PackageQueue *-- Package
ramp *-- ElementID
storehouse o-- IPackageStockpile
	class ElementID << (T,orchid) primitive>>


@enduml
```
