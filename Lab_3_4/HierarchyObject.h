#ifndef __HIERARCHY_OBJECT
#define __HIERARCHY_OBJECT

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cstring>
#include <map>

using namespace std;

class HierarchyObject;

typedef void(*Signal)(string&);
typedef void(*Handler)(HierarchyObject*, string&);

#define SIGNAL(current_signal) ((Signal) (&current_signal))
#define HANDLER(current_handler) ((Handler) (&current_handler))

//#define SIGNAL(current_signal) void(*current_signal)(string&)
//#define HANDLER(current_handler) void(*current_handler)(HierarchyObject*, string&)

// �����, ����������� ������ �������� ������
class HierarchyObject {
public:
	string name;									// ��� (��������) �������
	HierarchyObject* parent;						// �������� (�������� �������) ������� � ������
	static queue<HierarchyObject*> objects_queue;	// ������� �� ��������
	vector<HierarchyObject*> children;				// ���� (��������� �������) ������� �������

	int type; 	// ��� (������ �����) 
	int state;	// ��������� ����������

	multimap<void (*) (string&), pair<HierarchyObject*, void (*) (HierarchyObject*, string&)>> connects; // ���������� ������� ������� � �������

	// �����������, ����������� ������ �� �������� � ����� ��� (�� ��������� object)
	HierarchyObject(HierarchyObject* new_parent, string new_name, int new_type, int new_state) {
		// ����� �������� ��������, ����� �������, ���� ������� � ��� ��������� ����������
		parent = new_parent;
		name = new_name;
		type = new_type;
		state = new_state;
	}

	// �����, �������� ��� �������
	void set_name(string new_name) {
		name = new_name;
	}
	// �����, ������������ ��� �������
	string get_name() {
		return name;
	}

	// �����, �������� �������� �������
	void set_parent(HierarchyObject* new_parent) {
		// ������� ������ ������ �� ������ ����� ������� ��������
		for (vector<HierarchyObject*>::iterator i = parent->children.begin(); i != parent->children.end(); i++) {
			// ���� ������ ������ �������� �������� ������ �������, �� ������� ��� �� ������ ����� ��������
			if (*i == this) {
				i = parent->children.erase(i);
				break;
			}
		}
		// ����� ������ ��������
		parent = new_parent;

		// ���� ����� �������� ������
		if (parent != nullptr) {
			// ��������� ������ ������ � ������ ����� ������ ��������
			parent->children.push_back(this);
		}
	}
	// �����, ������������ �������� �������
	HierarchyObject* get_parent() {
		return parent;
	}

	// �����, ������ ������� ������ �� ����
	HierarchyObject* findByPath() {
		// ���� ��������� ��� � ����
		char* cur_target = strtok(nullptr, "/");

		// ���� ��� ��� ������, ������ ������ ������ - �������, ���������� ���
		if (cur_target == nullptr) {
			return this;
		}

		// ��� ������� ������...
		for (HierarchyObject* child : children) {
			// ���� ��� ����� �������, �� �������� ����� �� ���� ��� ����
			if (child->name == cur_target) {
				return child->findByPath();
			}
		}

		// ���� �� ���� �� ��� ��������� ���������, ������ ������ ����� ������ �� �����, ���������� ������
		return nullptr;
	}

	// �����, ������ ������� ������ �� �����
	HierarchyObject* findByName(string target_name, int mode = 0) {
		// ���� ��� ������� �������� ��������� � ������ ������...
		if (("//" + name) == target_name) {
			// ���������� ������ �� ������ ������
			return this;
		}

		// ���� ����� ����������...
		if (!mode) {
			// ���� ��� �������� �������...
			if (parent == nullptr) {
				// �������� ����� ��� ������ �������, �� � ���������� ������, � ���������� ���������
				return findByName(target_name, 1);
			}
			// �����...
			else {
				// �������� ����� � ���������� ������ ��� �������� � ���������� ���������
				return parent->findByName(target_name);
			}
		}
		// �����...
		else {
			// ��� ������� ������...
			for (HierarchyObject* child : children) {
				// �������� ����� � ���������� ������ ��� ������� ������
				HierarchyObject* current_result = child->findByName(target_name, 1);

				// ���� ��������� ������ ������...
				if (current_result != nullptr) {
					// ���������� ���
					return current_result;
				}
			}
		}

		// ���� �� ���� �� ��� ��������� ���������, ������ ������ ����� ������ �� �����, ���������� ������
		return nullptr;
	}

	// �����, ������ ������� � ������� ������ �� ��������
	HierarchyObject* find(string target) {
		// ���� ������ ��� ������� - �����, �� ���� �� ���� (����������)
		if (target.size() > 1 && target[0] == '/' && target[1] == '/') {
			return findByName(target);
		}
		// ����� ���� �� �����
		else {
			// �������� ����������, ������� ����� ������� ���� � ���� ������� ��������
			char* target_char = new char[0];
			// ����������� ����-������ � ����-������
			strcpy(target_char, target.c_str());
			// ����� ������ ���� �� ����
			strtok(target_char, "/");

			// ����� ������ ������� ��� ������ ������ �� ����
			return findByPath();
		}
	}

	// DFS (depth-first search) - �������� ������ ����� � ������� (� ������ ������ ����� ������ ����� ������� � ������ ����)
	void DFS(int depth = 0) {
		cout << '\n';

		// ������� ���������� ��������� ������ ������ ������� ���� 
		for (int i = 0; i < depth; i++) {
			cout << "    ";
		}
		// ������� ��� ����
		cout << name;

		// �������� DFS ��� ���� ����� ������� �������
		for (HierarchyObject* child : children) {
			// ������� ������� �� ���� ������, ��� � ������� ��������
			child->DFS(depth + 1);
		}
	}

	// �����, ��������������� ����������
	void set_connect(Signal signal, HierarchyObject* receiver, Handler handler) {
		// ��� ������� ����������...
		for (multimap<void (*) (string&), pair<HierarchyObject*, void (*) (HierarchyObject*, string&)>>::iterator it = connects.begin(); it != connects.end(); it++) {
			// ���� ��� ���������� ��� ����������, ��...
			if ((*it).first == signal && receiver == (*it).second.first && handler == (*it).second.second) {
				// ������� �� ������
				return;
			}
		}

		// ���� �� ����� ���� ������ ������ ���������� ��� ���, ��������� ���
		connects.insert({ signal, {receiver, handler} });
	}

	// �����, ��������� ����������
	void delete_connect(Signal signal, HierarchyObject* receiver, Handler handler) {
		// ��� ������� ����������...
		for (multimap<void (*) (string&), pair<HierarchyObject*, void (*) (HierarchyObject*, string&)>>::iterator it = connects.begin(); it != connects.end(); it++) {
			// ���� ��� ������ ����������, ��...
			if ((*it).first == signal && receiver == (*it).second.first && handler == (*it).second.second) {
				// ������� ���
				connects.erase(it);
				// ������� �� ������
				return;
			}
		}
	}

	// �����, ����������� ������
	void emit_signal(Signal signal, string& message) {
		// ���� �� ���� ���������� �� ����������� ��� ��� ���������� � ������ ��������
		if (connects.empty() || connects.count(signal) == 0) {
			// ������� �� ������
			return;
		}

		// ��������� ������
		(*signal)(message);

		// ��� ������� ����������... 
		for (multimap<void (*) (string&), pair<HierarchyObject*, void (*) (HierarchyObject*, string&)>>::iterator it = connects.begin(); it != connects.end(); it++) {
			// ���� � ������ ���������� ���������� ������ ������, ��...
			if ((*it).first == signal) {
				// ������������ ������
				(*((*it).second.second))((*it).second.first, message);
			}
		}
	}

	// ����� �������, ���� ��� ��� �����������
	static void pointless_signal(string& message) {}

	// ������ �����������, ���� ��� ��� �����������
	static void pointless_handler(HierarchyObject* receiver, string& message) {}

	// DFS �� 1 ����
	void DFS_primal() {
		// ������� ������ �����
		cout << "\nThe object " << name << " is ";

		// ���� ��������� ������ ����...
		if (state > 0) {
			// ������� �����, ����������, ��� ������ �����
			cout << "ready";
		}
		// �����...
		else {
			// ������� �����, ����������, ��� ������ �� �����
			cout << "not ready";
		}

		// ��� ������� ������ ������� �������...
		for (HierarchyObject* child : children) {
			// ��������� ���� �����
			child->DFS_primal();
		}
	}
};

// ��� ������� ����� ��� ������������ ����
queue<HierarchyObject*> HierarchyObject::objects_queue = queue<HierarchyObject*>();

#endif