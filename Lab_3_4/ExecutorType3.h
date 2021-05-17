#ifndef __EXECUTOR_TYPE_3
#define __EXECUTOR_TYPE_3

#include <iostream>
#include <string>

#include "HierarchyObject.h"
#include "Executor.h"

using namespace std;

// �����, ��������� ����������� ���������� �������� ������ HierarchyObject (����� ��� ����, ����� �� ������������ ������� �����)
class ExecutorType3 : public Executor {
public:
	// ����������� �����, ������ �������� ����������� �������� ������
	ExecutorType3(HierarchyObject* new_parent, string new_name, int new_state = 0) : Executor(new_parent, new_name, 3, new_state) {}
};

#endif