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

// Класс приложения, наследуется от класса объекта иерархии дерева, по совместительству выполняет роль корневого элемента
class Application : public HierarchyObject {
public:
	map<int, pair<string, string>> all_connects;

	// Конструктор класса, просто вызывает конструктор родительского класса
	Application(HierarchyObject* new_parent, string new_name = "object") : HierarchyObject(new_parent, new_name, 1, 1) {}

	// Метод построения структуры дерева
	void construct_hierarchy_tree() {
		// Объявляем переменные для хранения имён корневого объекта, родителя и нового ребёнка
		string root_name, parent_name = "1", new_child_name = "2";
		// Объявляем переменные типа объекта и его состояние готовности
		int new_type, new_state;

		// Вводим имя корневого элемента
		cin >> root_name;
		// Задаём имя корневого элемента
		set_name(root_name);

		// Вводим имя родителя
		cin >> parent_name;
		// Пока имя родителя не равно "endtree" (условие выхода из ввода данных):
		while (parent_name != "endtree") {
			// Вводим новое имя, тип и состояние готовности
			cin >> new_child_name >> new_type >> new_state;

			// Находим нынешнего родителя в дереве
			Executor* current_parent = (Executor*)this->find("//" + parent_name);
			// Объявляем объект нового ребёнка
			Executor* new_child;
			// В зависимости от типа создаём ребёнка
			if (new_type == 2) {
				new_child = (Executor*)new ExecutorType2(current_parent, new_child_name, new_state);
			}
			else {
				new_child = (Executor*)new ExecutorType3(current_parent, new_child_name, new_state);
			}

			// Устанавливаем лдя нового ребёнка родителя
			new_child->set_parent(current_parent);

			// Вводим следующее имя родителя 
			cin >> parent_name;
		}

		// Объявляем переменную номера сигнала
		int signal_number;
		// Объявляем переменные имени отправителя и имени получателя
		string sender_name, receiver_name;

		// пока не выйдем из цикла сами...
		while (1) {
			// вводим номер сигнала
			cin >> signal_number;
			// если этот номер равен 0...
			if (signal_number == 0) {
				// выходим из цикла 
				break;
			}

			// вводим имя отправителя
			cin >> sender_name;
			// если имя отправителя равно 0...
			if (sender_name == "0") {
				// выходим из цикла
				break;
			}

			// вводим имя получателя
			cin >> receiver_name;
			// если имя получателя равно 0...
			if (receiver_name == "0") {
				// выходим из цикла
				break;
			}

			// добавляем новое соединение в список всех соединений
			all_connects[signal_number] = { sender_name, receiver_name };

			// Находим объект отправителя
			Executor* current_sender = (Executor*)this->find("//" + sender_name);
			// Находим объект получателя
			Executor* current_receiver = (Executor*)this->find("//" + receiver_name);
			// Устанавливаем у отправителя соединение
			current_sender->set_connect(SIGNAL(pointless_signal), current_receiver, HANDLER(pointless_handler));
		}
	}

	// Метод ответа на завпросы нахождения элементов
	void do_requests() {
		// Объявляем переменную для хранения нынешнего пути к элементу
		string current_path;
		// Вводим нынешний путь к элементу
		cin >> current_path;
		// Пока нынешний путь не равен "//"...
		while (current_path != "//") {
			// Находим нужный элемент
			HierarchyObject* result = find(current_path);
			// Выводим сообщение в соответствии с тем, является ли он пустым или нет
			if (result == nullptr) {
				cout << '\n' << current_path << " Object not found";
			}
			else {
				cout << '\n' << current_path << " Object name: " << result->name;
			}

			// Вводим новый нынешний путь к элементу
			cin >> current_path;
		}
	}

	// метод, выводящий на экран все соединения
	void print_all_connects() {
		// выводим нужные слова
		cout << "\nSet connects";

		// для каждого соединения...
		for (map<int, pair<string, string>>::iterator it = all_connects.begin(); it != all_connects.end(); it++) {
			// выводим информацию о нём в нужном формате
			cout << '\n' << (*it).first << ' ' << (*it).second.first << ' ' << (*it).second.second;
		}
	}

	// метод, выполняющий сигналы
	void do_signals() {
		// выводим нужное сообщение
		cout << "\nEmit signals";

		// Объявляем имя отправителя и сообщение
		string sender_name, message;

		// вводим имя отправителя
		cin >> sender_name;

		// пока имя отправителя не равно строке "endsignals"...
		while (sender_name != "endsignals") {
			// вводим сообщение
			cin >> message;

			// Находим объект отправителя
			Executor* current_sender = (Executor*)this->find("//" + sender_name);

			// испускаем сигнал
			current_sender->emit_signal(SIGNAL(pointless_signal), message);

			// для каждого соединения данного отправителя...
			for (multimap<void (*) (string&), pair<HierarchyObject*, void (*) (HierarchyObject*, string&)>>::iterator it = current_sender->connects.begin(); it != current_sender->connects.end(); it++) {
				// выводим сообщение о том, что был испущен сигнал
				cout << "\nSignal to " << (*it).second.first->name << " Text: " << current_sender->name << " -> " << message;
			}

			// вводим слудующее имя отправителя
			cin >> sender_name;
		}
	}

	// Метод выполнения приложения
	int execute() {
		// Выводим нужное сообщение
		cout << "Object tree";
		// Выводим объекты дерева иерархии
		DFS();
		// выводим все соединения
		print_all_connects();
		// исполняем все сигналы
		do_signals();
		// Возвращаем код успешного выполнения приложения (0)
		return 0;
	}
};

#endif