#include <iostream>
#include "Application.h"

using namespace std;

// ������� ������� ���������
signed main() {
	// ������ ������ ����������
	Application* application = new Application(nullptr);

	// ������ ������ ��������
	application->construct_hierarchy_tree();

	// ��������� ���������� � ���������� ��������� ��� ���������
	return application->execute();
}