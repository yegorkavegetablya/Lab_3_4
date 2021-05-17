#ifndef __APPLICATION
#define __APPLICATION

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Executor.h"
#include "ExecutorType2.h"
#include "ExecutorType3.h"

using namespace std;

typedef void(*Signal)(string&);
typedef void(*Handler)(HierarchyObject*, string&);

#define SIGNAL(current_signal) ((Signal) (&current_signal))
#define HANDLER(current_handler) ((Handler) (&current_handler))

//#define SIGNAL(current_signal) void(*current_signal)(string&)
//#define HANDLER(current_handler) void(*current_handler)(HierarchyObject*, string&)

// ����� ����������, ����������� �� ������ ������� �������� ������, �� ���������������� ��������� ���� ��������� ��������
class Application : public HierarchyObject {
public:
	map<int, pair<string, string>> all_connects;

	// ����������� ������, ������ �������� ����������� ������������� ������
	Application(HierarchyObject* new_parent, string new_name = "object") : HierarchyObject(new_parent, new_name, 1, 1) {}

	// ����� ���������� ��������� ������
	void construct_hierarchy_tree() {
		// ��������� ���������� ��� �������� ��� ��������� �������, �������� � ������ ������
		string root_name, parent_name = "1", new_child_name = "2";
		// ��������� ���������� ���� ������� � ��� ��������� ����������
		int new_type, new_state;

		// ������ ��� ��������� ��������
		cin >> root_name;
		// ����� ��� ��������� ��������
		set_name(root_name);

		// ������ ��� ��������
		cin >> parent_name;
		// ���� ��� �������� �� ����� "endtree" (������� ������ �� ����� ������):
		while (parent_name != "endtree") {
			// ������ ����� ���, ��� � ��������� ����������
			cin >> new_child_name >> new_type >> new_state;

			// ������� ��������� �������� � ������
			Executor* current_parent = (Executor*)this->find("//" + parent_name);
			// ��������� ������ ������ ������
			Executor* new_child;
			// � ����������� �� ���� ������ ������
			if (new_type == 2) {
				new_child = (Executor*)new ExecutorType2(current_parent, new_child_name, new_state);
			}
			else {
				new_child = (Executor*)new ExecutorType3(current_parent, new_child_name, new_state);
			}

			// ������������� ��� ������ ������ ��������
			new_child->set_parent(current_parent);

			// ������ ��������� ��� �������� 
			cin >> parent_name;
		}

		// ��������� ���������� ������ �������
		int signal_number;
		// ��������� ���������� ����� ����������� � ����� ����������
		string sender_name, receiver_name;

		// ���� �� ������ �� ����� ����...
		while (1) {
			// ������ ����� �������
			cin >> signal_number;
			// ���� ���� ����� ����� 0...
			if (signal_number == 0) {
				// ������� �� ����� 
				break;
			}

			// ������ ��� �����������
			cin >> sender_name;
			// ���� ��� ����������� ����� 0...
			if (sender_name == "0") {
				// ������� �� �����
				break;
			}

			// ������ ��� ����������
			cin >> receiver_name;
			// ���� ��� ���������� ����� 0...
			if (receiver_name == "0") {
				// ������� �� �����
				break;
			}

			// ��������� ����� ���������� � ������ ���� ����������
			all_connects[signal_number] = { sender_name, receiver_name };

			// ������� ������ �����������
			Executor* current_sender = (Executor*)this->find("//" + sender_name);
			// ������� ������ ����������
			Executor* current_receiver = (Executor*)this->find("//" + receiver_name);
			// ������������� � ����������� ����������
			current_sender->set_connect(SIGNAL(pointless_signal), current_receiver, HANDLER(pointless_handler));
		}
	}

	// ����� ������ �� �������� ���������� ���������
	void do_requests() {
		// ��������� ���������� ��� �������� ��������� ���� � ��������
		string current_path;
		// ������ �������� ���� � ��������
		cin >> current_path;
		// ���� �������� ���� �� ����� "//"...
		while (current_path != "//") {
			// ������� ������ �������
			HierarchyObject* result = find(current_path);
			// ������� ��������� � ������������ � ���, �������� �� �� ������ ��� ���
			if (result == nullptr) {
				cout << '\n' << current_path << " Object not found";
			}
			else {
				cout << '\n' << current_path << " Object name: " << result->name;
			}

			// ������ ����� �������� ���� � ��������
			cin >> current_path;
		}
	}

	// �����, ��������� �� ����� ��� ����������
	void print_all_connects() {
		// ������� ������ �����
		cout << "\nSet connects";

		// ��� ������� ����������...
		for (map<int, pair<string, string>>::iterator it = all_connects.begin(); it != all_connects.end(); it++) {
			// ������� ���������� � �� � ������ �������
			cout << '\n' << (*it).first << ' ' << (*it).second.first << ' ' << (*it).second.second;
		}
	}

	// �����, ����������� �������
	void do_signals() {
		// ������� ������ ���������
		cout << "\nEmit signals";

		// ��������� ��� ����������� � ���������
		string sender_name, message;

		// ������ ��� �����������
		cin >> sender_name;

		// ���� ��� ����������� �� ����� ������ "endsignals"...
		while (sender_name != "endsignals") {
			// ������ ���������
			cin >> message;

			// ������� ������ �����������
			Executor* current_sender = (Executor*)this->find("//" + sender_name);

			// ��������� ������
			current_sender->emit_signal(SIGNAL(pointless_signal), message);

			// ��� ������� ���������� ������� �����������...
			for (multimap<void (*) (string&), pair<HierarchyObject*, void (*) (HierarchyObject*, string&)>>::iterator it = current_sender->connects.begin(); it != current_sender->connects.end(); it++) {
				// ������� ��������� � ���, ��� ��� ������� ������
				cout << "\nSignal to " << (*it).second.first->name << " Text: " << current_sender->name << " -> " << message;
			}

			// ������ ��������� ��� �����������
			cin >> sender_name;
		}
	}

	// ����� ���������� ����������
	int execute() {
		// ������� ������ ���������
		cout << "Object tree";
		// ������� ������� ������ ��������
		DFS();
		// ������� ��� ����������
		print_all_connects();
		// ��������� ��� �������
		do_signals();
		// ���������� ��� ��������� ���������� ���������� (0)
		return 0;
	}
};

#endif