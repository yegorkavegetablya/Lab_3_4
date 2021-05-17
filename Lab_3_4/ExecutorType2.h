#ifndef __EXECUTOR_TYPE_2
#define __EXECUTOR_TYPE_2

#include <iostream>
#include <string>

#include "HierarchyObject.h"
#include "Executor.h"

using namespace std;

// Класс, полностью повторябщий функционал базового класса HierarchyObject (нужен для того, чтобы не использовать базовый класс)
class ExecutorType2 : public Executor {
public:
	// Конструктор класс, просто вызывает конструктор базового класса
	ExecutorType2(HierarchyObject* new_parent, string new_name, int new_state = 0) : Executor(new_parent, new_name, 2, new_state) {}
};

#endif