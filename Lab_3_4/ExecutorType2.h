#ifndef __EXECUTOR_TYPE_2
#define __EXECUTOR_TYPE_2

#include <iostream>
#include <string>

#include "HierarchyObject.h"
#include "Executor.h"

using namespace std;

// �����, ��������� ����������� ���������� �������� ������ HierarchyObject (����� ��� ����, ����� �� ������������ ������� �����)
class ExecutorType2 : public Executor {
public:
	// ����������� �����, ������ �������� ����������� �������� ������
	ExecutorType2(HierarchyObject* new_parent, string new_name, int new_state = 0) : Executor(new_parent, new_name, 2, new_state) {}
};

#endif