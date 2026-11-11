#pragma once
#include <vector>
#include <iostream>
#include "wagon.h"

/**
 * @brief Класс, представляющий пассажирский поезд
 */
class Train{
public:
    /**
     * @brief Пустой конструктор по умолчанию
     */
    Train();

    /**
     * @brief Конструктор поезда с заданным количеством вагонов
     * @param[in] wagon_count Количество вагонов
     */
    Train(const unsigned int wagon_count);

    /**
     * @brief Конструктор поезда из массива вагонов
     * @param[in] wagons Указатель на массив вагонов
     * @param[in] wagon_count Количество вагонов в массиве
     * @throw std::invalid_argument Если указатель нулевой при ненулевом количестве
     */
    Train(const Wagon* wagons, unsigned int wagon_count);

    /**
     * @brief Конструктор поезда из одного вагона
     * @param[in] w Начальный вагон
     */
    Train(const Wagon& w);

    /**
     * @brief Добавление нового вагона в состав
     * @param[in] w Добавляемый вагон
     * @return Ссылка на текущий поезд
     */
    Train& operator+=(const Wagon& w);

    /**
     * @brief Получение вагона по индексу. Допускается изменение вагона
     * @param[in] index Индекс вагона
     * @return Ссылка на вагон
     * @throw std::out_of_range Если индекс выходит за границы поезда
     */
    Wagon& operator[](unsigned int index);

    /**
     * @brief Получение вагона по индексу. Не допускается изменение вагона
     * @param[in] index Индекс вагона
     * @return Ссылка на вагон
     * @throw std::out_of_range Если индекс выходит за границы поезда
     */
    const Wagon& operator[](unsigned int index) const;
    /**
     * @brief Удаление вагона по индексу
     * @param[in] index Индекс удаляемого вагона
     * @throw std::out_of_range Если индекс выходит за границы поезда
     */
    void deleteWagon(int index);

    /**
     * @brief Посадка пассажиров в наиболее свободный вагон заданного класса
     * @param[in] passangers_count Количество пассажиров
     * @throw std::logic_error Если не смогли пересадить passangers_count ребят
     * @throw std::overflow_error Если мест в вагоне недостаточно
     */
    void sitDownMostFreeWagon(int passangers_count);

    /**
     * @brief Перераспределение пассажиров для максимально близкой занятости
     * @throw std::logic_error Если в поезде только вагоны-рестораны
     */
    void sitDownMostFreeWagons();

    /**
     * @brief Минимизация количества вагонов и удаление пустых
     */
    void minimizeWagons();

    /**
     * @brief Оптимальное размещение вагона-ресторана в поезде
     * @throw std::logic_error Если в поезде нет вагона-ресторана
     */
    void placeCaffeOptimal();

    /**
     * @brief Получение текущего количества вагонов в поезде
     * @return Количество вагонов
     */
    unsigned int size() const;

    /**
     * @brief Вывод состояния поезда в поток
     * @param[in] out Выходной поток
     * @return Ссылку на выходной поток
     */
    std::ostream& print(std::ostream& out) const;

private:
    /**
     * @brief Поиск индекса вагона с наибольшим процентом занятости
     * @return Индекс вагона
     */
    int searchBiggerPercent();

    /**
     * @brief Поиск индекса вагона с наименьшим процентом занятости
     * @return Индекс вагона или -1 если подходящих вагонов нет
     */
    int searchSmallerPercent();

    /**
     * @brief Поиск индекса вагона с максимальной вместимостью
     * @return Индекс вагона
     */
    int searchMaxCapacity();

    std::vector<Wagon> wagons_; ///< Вектор вагонов поезда
};

/**
 * @brief Оператор вывода поезда в поток
 * @param[in] out Выходной поток
 * @param[in] t Выводимый поезд
 * @return Ссылку на выходной поток
 */
std::ostream& operator<<(std::ostream& out, const Train& t);