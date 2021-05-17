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

// Класс, реализующий объект иерархии дерева
class HierarchyObject {
public:
	string name;									// имя (название) объекта
	HierarchyObject* parent;						// родитель (головной элемент) объекта в дереве
	static queue<HierarchyObject*> objects_queue;	// очередь из объектов
	vector<HierarchyObject*> children;				// дети (подчинённе объекты) данного объекта

	int type; 	// тип (данный класс) 
	int state;	// состояние готовности

	multimap<void (*) (string&), pair<HierarchyObject*, void (*) (HierarchyObject*, string&)>> connects; // соединения данного объекта с другими

	// Конструктор, принимающий ссылку на родителя и новое имя (по умолчанию object)
	HierarchyObject(HierarchyObject* new_parent, string new_name, int new_type, int new_state) {
		// Задаём значения родителя, имени объекта, типа объекта и его состояния готовности
		parent = new_parent;
		name = new_name;
		type = new_type;
		state = new_state;
	}

	// Метод, задающий имя объекта
	void set_name(string new_name) {
		name = new_name;
	}
	// Метод, возвращающий имя объекта
	string get_name() {
		return name;
	}

	// Метод, задающий родителя объекта
	void set_parent(HierarchyObject* new_parent) {
		// Удаляем данный объект из списка детей старого родителя
		for (vector<HierarchyObject*>::iterator i = parent->children.begin(); i != parent->children.end(); i++) {
			// Если данный ребёнок родителя является данным оъектом, то удаляем его из списка детей родителя
			if (*i == this) {
				i = parent->children.erase(i);
				break;
			}
		}
		// Задаём нового радителя
		parent = new_parent;

		// Если новый родитель непуст
		if (parent != nullptr) {
			// Добавляем данный объект в список детей нового родителя
			parent->children.push_back(this);
		}
	}
	// Метод, возвращающий родителя объекта
	HierarchyObject* get_parent() {
		return parent;
	}

	// Метод, ищущий элемент дерева по пути
	HierarchyObject* findByPath() {
		// берём следующее имя в пути
		char* cur_target = strtok(nullptr, "/");

		// если это имя пустое, значит данный объект - искомый, возвращаем его
		if (cur_target == nullptr) {
			return this;
		}

		// Для каждого ребёнка...
		for (HierarchyObject* child : children) {
			// Есди имя равно нужному, то вызываем поиск по пути для него
			if (child->name == cur_target) {
				return child->findByPath();
			}
		}

		// Если ни разу не был возвращён результат, значит данный метод ничего не нашёл, возвращаем ничего
		return nullptr;
	}

	// Метод, ищущий элемент дерева по имени
	HierarchyObject* findByName(string target_name, int mode = 0) {
		// Если имя данного элемента совпадает с нужным именем...
		if (("//" + name) == target_name) {
			// возвращаем ссылку на данный объект
			return this;
		}

		// Если режим восходящий...
		if (!mode) {
			// Если это корневой элемент...
			if (parent == nullptr) {
				// вызываем поиск для данной вершины, но в нисходящем режиме, и возвращаем результат
				return findByName(target_name, 1);
			}
			// Иначе...
			else {
				// Вызываем поиск в восходящем режиме для родителя и возвращаем результат
				return parent->findByName(target_name);
			}
		}
		// Иначе...
		else {
			// Для каждого ребёнка...
			for (HierarchyObject* child : children) {
				// Вызываем поиск в нисходящем режиме для данного ребёнка
				HierarchyObject* current_result = child->findByName(target_name, 1);

				// Если результат посика непуст...
				if (current_result != nullptr) {
					// Возвращаем его
					return current_result;
				}
			}
		}

		// Если ни разу не был возвращён результат, значит данный метод ничего не нашёл, возвращаем ничего
		return nullptr;
	}

	// Метод, ищущий элемент с помощью одного из способов
	HierarchyObject* find(string target) {
		// Если первые два символа - слеши, то ищем по пути (координате)
		if (target.size() > 1 && target[0] == '/' && target[1] == '/') {
			return findByName(target);
		}
		// иначе ищем по имени
		else {
			// Создадим переменную, которая будет хранить путь в виде массива символов
			char* target_char = new char[0];
			// Преобразуем путь-строку в путь-массив
			strcpy(target_char, target.c_str());
			// Уберём первый слеш из пути
			strtok(target_char, "/");

			// Найдём нужный элемент при помощи поиска по пути
			return findByPath();
		}
	}

	// DFS (depth-first search) - алгоритм обхода графа в глубину (в данном случае обход дерева слева направо и сверху вниз)
	void DFS(int depth = 0) {
		cout << '\n';

		// Выводим количество табуляций равное уровню данного узла 
		for (int i = 0; i < depth; i++) {
			cout << "    ";
		}
		// Выводим имя узла
		cout << name;

		// Вызываем DFS для всех детей данного объекта
		for (HierarchyObject* child : children) {
			// Передаём уровень на один больше, чем у данного элемента
			child->DFS(depth + 1);
		}
	}

	// метод, устанавливающий соединение
	void set_connect(Signal signal, HierarchyObject* receiver, Handler handler) {
		// для каждого соединения...
		for (multimap<void (*) (string&), pair<HierarchyObject*, void (*) (HierarchyObject*, string&)>>::iterator it = connects.begin(); it != connects.end(); it++) {
			// если это соединение уже существует, то...
			if ((*it).first == signal && receiver == (*it).second.first && handler == (*it).second.second) {
				// выходим из метода
				return;
			}
		}

		// если мы дошли сюда значит такого соединения ещё нет, добавляем его
		connects.insert({ signal, {receiver, handler} });
	}

	// метод, удаляющий соединение
	void delete_connect(Signal signal, HierarchyObject* receiver, Handler handler) {
		// для каждого соединения...
		for (multimap<void (*) (string&), pair<HierarchyObject*, void (*) (HierarchyObject*, string&)>>::iterator it = connects.begin(); it != connects.end(); it++) {
			// если это нужное соединение, то...
			if ((*it).first == signal && receiver == (*it).second.first && handler == (*it).second.second) {
				// удаляем его
				connects.erase(it);
				// выходим из метода
				return;
			}
		}
	}

	// метод, испускающий сигнал
	void emit_signal(Signal signal, string& message) {
		// если ни одно соединение не установлено или нет соединения с данным сигналом
		if (connects.empty() || connects.count(signal) == 0) {
			// выходим из метода
			return;
		}

		// испускаем сигнал
		(*signal)(message);

		// для каждого соединения... 
		for (multimap<void (*) (string&), pair<HierarchyObject*, void (*) (HierarchyObject*, string&)>>::iterator it = connects.begin(); it != connects.end(); it++) {
			// если в данном соединении учавствует данный сигнал, то...
			if ((*it).first == signal) {
				// обрабатываем сигнал
				(*((*it).second.second))((*it).second.first, message);
			}
		}
	}

	// метод сигнала, пока что без функционала
	static void pointless_signal(string& message) {}

	// метода обработчика, пока что без функционала
	static void pointless_handler(HierarchyObject* receiver, string& message) {}

	// DFS из 1 лабы
	void DFS_primal() {
		// Выводим нужные слова
		cout << "\nThe object " << name << " is ";

		// если состояние больше нуля...
		if (state > 0) {
			// выводим слово, означающее, что объект готов
			cout << "ready";
		}
		// иначе...
		else {
			// выводим слово, означающее, что объект не готов
			cout << "not ready";
		}

		// для каждого ребёнка данного объекта...
		for (HierarchyObject* child : children) {
			// выполняем этот вывод
			child->DFS_primal();
		}
	}
};

// Эта строчка нужна для статического поля
queue<HierarchyObject*> HierarchyObject::objects_queue = queue<HierarchyObject*>();

#endif