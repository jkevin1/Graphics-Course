// lists of objects in scene
#ifndef OBJECTLIST_HPP
#define OBJECTLIST_HPP

// other classes we use DIRECTLY in our interface
#include "Intersection.hpp"
#include "Ray.hpp"

// system includes
#include <list>

// classes we only use by pointer or reference
class Object;

class ObjectList {
	// list of objects
	typedef std::list<Object*> t_List;
	t_List d_list;

public:	// constructor & destructor
	ObjectList() {}
	~ObjectList();

public:
	// Add an object to the list. Objects should be allocated with
	// new. Objects will be deleted when this ObjectList is destroyed
	void addObject(Object *obj) { d_list.push_back(obj); }
  
public: // computational members
	// trace ray r through all objects, returning first intersection
	const Intersection trace(Ray r) const;
};

#endif
