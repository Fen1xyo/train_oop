#pragma once
#include <iostream>
#include <stdexcept>

/**
 * @brief Перечисление типов вагонов поезда
 */
enum class WagonType{
    Sit,    ///< Сидячий вагон
    Econom, ///< Эконом-класс
    Luxe,   ///< Люкс-класс
    Caffe,   ///< Вагон-ресторан
    COUNT
};

/**
 * @brief Класс, представляющий вагон
 */
class Wagon{
public:
    /**
     * @brief Основной конструктор вагона
     * @param[in] max_capacity Максимальная вместимость вагона
     * @param[in] busy_count Количество занятых мест
     * @param[in] type Тип вагона
     * @throw std::invalid_argument Если вагон-ресторан имеет вместимость больше 0 или если занятых мест больше вместимости
     */
    Wagon(unsigned int max_capacity, unsigned int busy_count, WagonType type);

    /**
     * @brief Конструктор создания вагона по его типу
     * @param[in] type Тип вагона
     */
    Wagon(WagonType type);

    /**
     * @brief Конструктор по умолчанию
     */
    Wagon() noexcept;

    /**
     * @brief Получение процента занятости вагона
     * @return Процент занятых мест от 0 до 100
     */
    double getBusyPercent() const noexcept;

    /**
     * @brief Перемещение пассажиров между вагонами до выравнивания процента
     * @param[in] w Вагон в который пересаживаются пассажиры
     * @return Ссылка на текущий вагон
     */
    Wagon& operator>>(Wagon& w) noexcept;

    /**
     * @brief Посадка пассажиров в вагон
     * @param[in] passangers Число пассажиров
     * @throw std::overflow_error Если число пассажиров превышает свободные места
     */
    void sitDown(int passangers);

    /**
     * @brief Высадка пассажиров из вагона
     * @param[in] passangers Число высаживаемых пассажиров
     * @throw std::overflow_error Если число высаживаемых пассажиров больше чем занято мест
     */
    void standUp(int passangers);

    /**
     * @brief Получение максимальной вместимости вагона
     * @return Максимальная вместимость
     */
    int getCapacity() const noexcept;

    /**
     * @brief Получение количества занятых мест
     * @return Количество занятых мест
     */
    int getBusyCount() const noexcept;

    /**
     * @brief Получение типа вагона
     * @return Тип вагона
     */
    const WagonType getType() const noexcept;

    /**
     * @brief Установка максимальной вместимости вагона
     * @param[in] max_capacity Новая вместимость
     * @throw std::invalid_argument Если вагон-ресторан получает вместимость больше 0 или вместимость меньше занятых мест
     */
    void setCapacity(unsigned int max_capacity);

    /**
     * @brief Установка количества занятых мест
     * @param[in] busy_count Новое количество занятых мест
     * @throw std::invalid_argument Если занятых мест больше вместимости
     */
    void setBusyCount(unsigned int busy_count);

    /**
     * @brief Установка типа вагона
     * @param[in] type Новый тип вагона
     */
    void setType(WagonType type) noexcept;

    /**
     * @brief Вывод состояния вагона в поток
     * @param[in] out Выходной поток
     * @return Ссылку на выходной поток
     */
    std::ostream& print(std::ostream& out) const;

private:
    WagonType type_;            ///< Тип вагона
    unsigned int max_capacity_; ///< Максимальная вместимость
    unsigned int busy_count_;   ///< Количество занятых мест
};

/**
 * @brief Оператор вывода вагона в поток
 * @param[in] out Выходной поток
 * @param[in] w Выводимый вагон
 * @return Ссылку на выходной поток
 */
std::ostream& operator<<(std::ostream& out, const Wagon& w);