#include <iostream>
#include <limits>
#include <stdexcept>
#include "wagon.h"
#include "train.h"

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

const char* wagonTypeName(WagonType type) {
    switch (type) {
        case WagonType::Sit:    return "Сидячий";
        case WagonType::Econom: return "Эконом";
        case WagonType::Luxe:   return "Люкс";
        case WagonType::Caffe:  return "Ресторан";
        default:                return "Неизвестный";
    }
}

void printTrainInfo(Train& train) {
    unsigned int size = train.size();
    std::cout << "\n=== Текущий состав поезда (" << size << " ваг.) ===\n";
    if (size == 0) {
        std::cout << "Поезд пуст!\n";
        return;
    }
    for (int i = 0; i < size; ++i) {
        const Wagon& w = train[i];
        std::cout << "[" << i << "] Тип: " << wagonTypeName(w.getType())
                  << " | Вместимость: " << w.getCapacity()
                  << " | Занято: " << w.getBusyCount()
                  << " | Загрузка: " << w.getBusyPercent() << "%\n";
    }
}

int main() {
    Train train(0);
    int command = -1;

    while (command != 0) {
        std::cout << "\n--- МЕНЮ УПРАВЛЕНИЯ ПОЕЗДОМ ---\n"
                  << "1. Показать состояние поезда\n"
                  << "2. Добавить новый вагон\n"
                  << "3. Удалить вагон по индексу\n"
                  << "4. Посадить пассажиров в наиболее свободный вагон\n"
                  << "5. Перераспределить пассажиров поровну\n"
                  << "6. Минимизировать число вагонов\n"
                  << "7. Оптимально разместить вагон-ресторан\n"
                  << "8. Пересадить пассажиров между двумя вагонами поровну\n"
                  << "0. Выход\n"
                  << "Выберите пункт: ";

        if (std::cin.eof()){
            std::cout << "Завершение программы" << std::endl;
            return 0;
        }
        
        if (!(std::cin >> command)){
            std::cout << "Ошибка: введено не число!\n";
            clearInput();
            continue;
        }

        try {
            switch (command) {
                case 1: {
                    printTrainInfo(train);
                    break;
                }
                case 2: {
                    int type_choice = 0;
                    std::cout << "Выберите тип вагона (0 - Сидячий, 1 - Эконом, 2 - Люкс, 3 - Ресторан): ";
                    if (!(std::cin >> type_choice) || type_choice < 0 || type_choice > 3) {
                        std::cout << "Ошибка ввода типа вагона!\n";
                        clearInput();
                        break;
                    }

                    int init_type = 0;
                    std::cout << "Инициализировать вручную (1) или по умолчанию (2)? ";
                    if (!(std::cin >> init_type)) {
                        clearInput();
                        break;
                    }

                    WagonType type = static_cast<WagonType>(type_choice);

                    if (init_type == 1) {
                        unsigned int cap = 0, busy = 0;
                        std::cout << "Введите максимальную вместимость и занятые места: ";
                        if (!(std::cin >> cap >> busy)) {
                            std::cout << "Ошибка ввода чисел!\n"; // добавил обработку ошибки в потоке ввода
                            clearInput();
                            break;
                        }
                        train += Wagon(cap, busy, type);
                    } else {
                        train += Wagon(type);
                    }

                    std::cout << "Вагон успешно добавлен!\n";
                    break;
                }
                case 3: {
                    int index = 0;
                    std::cout << "Введите индекс вагона для удаления: ";
                    if (!(std::cin >> index)) {
                        clearInput();
                        break;
                    }
                    train.deleteWagon(index);
                    std::cout << "Вагон с индексом " << index << " удален.\n";
                    break;
                }
                case 4: {
                    int pass = 0;
                    std::cout << "Введите количество пассажиров для посадки: ";
                    if (!(std::cin >> pass)) {
                        clearInput();
                        break;
                    }
                    train.sitDownMostFreeWagon(pass);
                    std::cout << "Пассажиры усажены в самый свободный вагон.\n";
                    break;
                }
                case 5: {
                    train.sitDownMostFreeWagons();
                    std::cout << "Пассажиры успешно перераспределены по вагонам.\n";
                    break;
                }
                case 6: {
                    train.minimizeWagons();
                    std::cout << "Вагоны минимизированы. Новое количество вагонов: " << train.size() << '\n';
                    break;
                }
                case 7: {
                    train.placeCaffeOptimal();
                    std::cout << "Вагон-ресторан перемещен в оптимальную позицию.\n";
                    break;
                }
                case 8: {
                    int from = 0, to = 0;
                    std::cout << "Введите индекс вагона-донора и вагона-приемника: ";
                    if (!(std::cin >> from >> to)) {
                        clearInput();
                        break;
                    }
                    train[from] >> train[to];
                    std::cout << "Операция >> выполнена успешно.\n";
                    break;
                }
                case 0: {
                    std::cout << "Завершение программы.\n";
                    break;
                }
                default:
                    std::cout << "Неизвестный пункт меню!\n";
                    break;
            }
        } catch (const std::exception& e) {
            std::cout << "Поймано исключение: " << e.what() << '\n';
        }
    }

    return 0;
}