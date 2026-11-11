#include <gtest/gtest.h>
#include <sstream>
#include "wagon.h"
#include "train.h"

// =============================================================================
// ТЕСТЫ WAGON
// =============================================================================

// Проверка конструктора по умолчанию
TEST(WagonTest, DefaultConstructor) {
    Wagon w;
    EXPECT_EQ(w.getType(), WagonType::Sit);
    EXPECT_EQ(w.getCapacity(), 50);
    EXPECT_EQ(w.getBusyCount(), 0);
    EXPECT_DOUBLE_EQ(w.getBusyPercent(), 0.0);
}

// Проверка конструктора по типу
TEST(WagonTest, TypeConstructor) {
    Wagon sit(WagonType::Sit);
    EXPECT_EQ(sit.getCapacity(), 50);

    Wagon econom(WagonType::Econom);
    EXPECT_EQ(econom.getCapacity(), 80);

    Wagon luxe(WagonType::Luxe);
    EXPECT_EQ(luxe.getCapacity(), 20);

    Wagon caffe(WagonType::Caffe);
    EXPECT_EQ(caffe.getCapacity(), 0);
    EXPECT_EQ(caffe.getBusyCount(), 0);
    EXPECT_DOUBLE_EQ(caffe.getBusyPercent(), 0.0);
}

// Проверка исключений при создании вагона
TEST(WagonTest, ConstructorExceptions) {
    // В ресторан нельзя сажать людей или задавать вместимость > 0
    EXPECT_THROW(Wagon(10, 0, WagonType::Caffe), std::invalid_argument);
    EXPECT_THROW(Wagon(0, 5, WagonType::Caffe), std::invalid_argument);

    // Занятых мест больше, чем вместимость
    EXPECT_THROW(Wagon(50, 51, WagonType::Sit), std::invalid_argument);

    // Корректное создание не должно бросать исключений
    EXPECT_NO_THROW(Wagon(60, 30, WagonType::Econom));
}

// Проверка посадки и высадки пассажиров
TEST(WagonTest, SitAndStand) {
    Wagon w(50, 10, WagonType::Sit);

    w.sitDown(15);
    EXPECT_EQ(w.getBusyCount(), 25);
    EXPECT_DOUBLE_EQ(w.getBusyPercent(), 50.0);

    w.standUp(5);
    EXPECT_EQ(w.getBusyCount(), 20);

    // Переполнение вагона
    EXPECT_THROW(w.sitDown(31), std::overflow_error);

    // Попытка высадить больше людей, чем есть
    EXPECT_THROW(w.standUp(21), std::overflow_error);
}

// Проверка оператора пересаживания >>
TEST(WagonTest, OperatorShift) {
    Wagon w1(100, 80, WagonType::Econom);
    Wagon w2(100, 20, WagonType::Econom);

    w1 >> w2;

    // Пассажиры должны уравновеситься (по 50%)
    EXPECT_EQ(w1.getBusyCount(), 50);
    EXPECT_EQ(w2.getBusyCount(), 50);
    EXPECT_DOUBLE_EQ(w1.getBusyPercent(), 50.0);
    EXPECT_DOUBLE_EQ(w2.getBusyPercent(), 50.0);
}


// =============================================================================
// ТЕСТЫ TRAIN
// =============================================================================

// Создание поезда и добавление вагонов
TEST(TrainTest, CreationAndAdd) {
    Train train(2); // 2 вагона по умолчанию
    EXPECT_EQ(train.size(), 2);

    Wagon w(20, 5, WagonType::Luxe);
    train += w;
    EXPECT_EQ(train.size(), 3);
    EXPECT_EQ(train[2].getType(), WagonType::Luxe);
    EXPECT_EQ(train[2].getBusyCount(), 5);
}

// Доступ по индексу и удаление
TEST(TrainTest, IndexAndRemoval) {
    Wagon w1(50, 10, WagonType::Sit);
    Train train(w1); // конструктор от одного вагона

    EXPECT_EQ(train[0].getBusyCount(), 10);
    EXPECT_THROW(train[1], std::out_of_range);

    train.deleteWagon(0);
    EXPECT_EQ(train.size(), 0);
}

// Посадка пассажиров в наиболее свободный вагон
TEST(TrainTest, SitDownMostFreeWagon) {
    Train train(0);
    train += Wagon(100, 60, WagonType::Econom); // 60%
    train += Wagon(100, 20, WagonType::Econom); // 20% 

    train.sitDownMostFreeWagon(10);
    EXPECT_EQ(train[1].getBusyCount(), 30);
    EXPECT_EQ(train[0].getBusyCount(), 60);
}

// Распределение пассажиров между вагонами
TEST(TrainTest, SitDownMostFreeWagonsBalancing) {
    Train train(0);
    train += Wagon(100, 80, WagonType::Econom);
    train += Wagon(100, 20, WagonType::Econom);

    train.sitDownMostFreeWagons();

    // Проценты должны стать примерно равными
    EXPECT_NEAR(train[0].getBusyPercent(), train[1].getBusyPercent(), 2.0);
}

// Исключение при распределении, если в поезде только рестораны
TEST(TrainTest, SitDownMostFreeWagonsOnlyCaffeException) {
    Train train(0);
    train += Wagon(WagonType::Caffe);
    EXPECT_THROW(train.sitDownMostFreeWagons(), std::logic_error);
}

// Минимизация количества вагонов
TEST(TrainTest, MinimizeWagons) {
    Train train(0);
    train += Wagon(100, 30, WagonType::Econom);
    train += Wagon(100, 20, WagonType::Econom); // 30 + 20 = 50 челов поместятся в 1 вагон

    train.minimizeWagons();

    EXPECT_EQ(train.size(), 1);
    EXPECT_EQ(train[0].getBusyCount(), 50);
}

// Оптимальное размещение вагона-ресторана
TEST(TrainTest, PlaceCaffeOptimal) {
    Train train(0);
    // Люкс игнорируется при расчете еды
    train += Wagon(20, 20, WagonType::Luxe);
    train += Wagon(100, 50, WagonType::Econom); // 50 чел
    train += Wagon(WagonType::Caffe);           // Стоит в середине
    train += Wagon(100, 50, WagonType::Econom); // 50 чел

    // Должен переставить кафе так, чтобы слева и справа было одинаково
    EXPECT_NO_THROW(train.placeCaffeOptimal());

    // Если кафе нет вообще - бросает logic_error
    Train trainWithoutCaffe(0);
    trainWithoutCaffe += Wagon(100, 10, WagonType::Econom);
    EXPECT_THROW(trainWithoutCaffe.placeCaffeOptimal(), std::logic_error);
}

// Дополнительные тесты (утром допишу комменты)

TEST(WagonTest, SetCapacity) {
    Wagon w(50, 20, WagonType::Sit);
    w.setCapacity(40);
    EXPECT_EQ(w.getCapacity(), 40);
    EXPECT_EQ(w.getBusyCount(), 20);
    EXPECT_DOUBLE_EQ(w.getBusyPercent(), 50.0);

    EXPECT_THROW(w.setCapacity(19), std::invalid_argument);
    EXPECT_EQ(w.getCapacity(), 40);
    EXPECT_EQ(w.getBusyCount(), 20);

    Wagon caffe(WagonType::Caffe);
    EXPECT_NO_THROW(caffe.setCapacity(0));
    EXPECT_THROW(caffe.setCapacity(1), std::invalid_argument);
    EXPECT_EQ(caffe.getCapacity(), 0);
}

TEST(WagonTest, SetBusyCount) {
    Wagon w(50, 10, WagonType::Sit);
    w.setBusyCount(50);
    EXPECT_EQ(w.getBusyCount(), 50);
    EXPECT_DOUBLE_EQ(w.getBusyPercent(), 100.0);

    EXPECT_THROW(w.setBusyCount(51), std::invalid_argument);
    EXPECT_EQ(w.getBusyCount(), 50);

    w.setBusyCount(0);
    EXPECT_EQ(w.getBusyCount(), 0);
    EXPECT_DOUBLE_EQ(w.getBusyPercent(), 0.0);
}

TEST(WagonTest, Output) {
    const Wagon w(80, 25, WagonType::Econom);
    std::ostringstream out;
    EXPECT_EQ(&(out << w), &out);
    EXPECT_EQ(out.str(), "1 80 25");
}

TEST(WagonTest, ShiftToCaffeDoesNotMovePassengers) {
    Wagon w(50, 20, WagonType::Sit);
    Wagon caffe(WagonType::Caffe);
    EXPECT_EQ(&(w >> caffe), &w);
    EXPECT_EQ(w.getBusyCount(), 20);
    EXPECT_EQ(caffe.getBusyCount(), 0);
}

TEST(TrainTest, ArrayConstructorCopiesWagons) {
    Wagon wagons[] = {
        Wagon(50, 10, WagonType::Sit),
        Wagon(20, 5, WagonType::Luxe)
    };
    Train train(wagons, 2);
    ASSERT_EQ(train.size(), 2u);
    EXPECT_EQ(train[0].getBusyCount(), 10);
    EXPECT_EQ(train[1].getType(), WagonType::Luxe);
    EXPECT_EQ(train[1].getBusyCount(), 5);

    wagons[0].sitDown(1);
    EXPECT_EQ(train[0].getBusyCount(), 10);
}

TEST(TrainTest, NullArrayConstructor) {
    EXPECT_THROW(Train(nullptr, 1), std::invalid_argument);
    Train train(nullptr, 0);
    EXPECT_EQ(train.size(), 0u);
}

TEST(TrainTest, ConstIndex) {
    const Train train(Wagon(50, 12, WagonType::Sit));
    EXPECT_EQ(train[0].getBusyCount(), 12);
    EXPECT_THROW(train[1], std::out_of_range);
}

TEST(TrainTest, InvalidRemovalKeepsTrain) {
    Train train(Wagon(50, 12, WagonType::Sit));
    EXPECT_THROW(train.deleteWagon(-1), std::out_of_range);
    EXPECT_THROW(train.deleteWagon(1), std::out_of_range);
    ASSERT_EQ(train.size(), 1u);
    EXPECT_EQ(train[0].getBusyCount(), 12);
}

TEST(TrainTest, EmptyTrainOperations) {
    Train train;
    EXPECT_EQ(train.size(), 0u);
    EXPECT_NO_THROW(train.sitDownMostFreeWagons());
    EXPECT_NO_THROW(train.minimizeWagons());
    EXPECT_THROW(train.sitDownMostFreeWagon(1), std::logic_error);
    EXPECT_THROW(train.placeCaffeOptimal(), std::logic_error);
    EXPECT_EQ(train.size(), 0u);
}

TEST(TrainTest, Output) {
    Train train(Wagon(50, 10, WagonType::Sit));
    train += Wagon(WagonType::Caffe);
    std::ostringstream out;
    EXPECT_EQ(&(out << train), &out);
    EXPECT_EQ(out.str(), "2\n0 50 10\n3 0 0\n");
}
