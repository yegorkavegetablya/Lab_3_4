#ifndef __EXECUTOR
#define __EXECUTOR

#include <iostream>
#include <string>

#include "HierarchyObject.h"

using namespace std;

// Класс, полностью повторябщий функционал базового класса HierarchyObject (нужен для того, чтобы не использовать базовый класс)
class Executor : public HierarchyObject {
public:
	// Конструктор класс, просто вызывает конструктор базового класса
	Executor(HierarchyObject* new_parent, string new_name, int new_type, int new_state = 0) : HierarchyObject(new_parent, new_name, new_type, new_state) {}
};

#endif