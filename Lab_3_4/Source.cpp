#include <iostream>
#include "Application.h"

using namespace std;

// главная функция программы
signed main() {
	// Создаём объект приложения
	Application* application = new Application(nullptr);

	// Строим дерево объектов
	application->construct_hierarchy_tree();

	// Выполняем приложение и возвращаем результат его отработки
	return application->execute();
}