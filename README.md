# БДЗ №1: Пассажирский поезд (Вариант 19)

Работа по ООП на C++23. Реализованы классы `Wagon` и `Train`, консольное меню и тесты на GoogleTest

## Что умеет прога

`Wagon` хранит тип вагона, вместимость и количество пассажиров. Можно сажать и высаживать пассажиров, получать процент занятости и пересаживать людей

`Train` хранит набор вагонов. Можно добавлять и удалять вагоны, обращаться к ним по индексу, сажать пассажиров в наиболее свободный вагон, перераспределять их между вагонами

## Структура проекта

```text
lib/
    wagon.h
    train.h
src/
    CMakeLists.txt
    wagon.cpp
    train.cpp
tests/
    CMakeLists.txt
    test.cpp
dialogue.cpp
CMakeLists.txt
README.md
```

## Сборка и запуск

Нужны CMake от 3.20 и компилятор с поддержкой C++23. GoogleTest скачивается автоматически при настройке проекта.

Все команды ниже выполняются из корня проекта

```bash
cmake -S . -B build
cmake --build build -j
./build/TrainApp
```

## Тесты

```bash
ctest --test-dir build --output-on-failure
```

Либо напрямую:

```bash
./build/tests/TrainTests
```

## Покрытие кода

Для измерения покрытия нужны GCC и gcovr

```bash
cmake -S . -B build-coverage \
    -DCMAKE_CXX_COMPILER=g++ \
    -DCMAKE_BUILD_TYPE=Debug \
    -DENABLE_COVERAGE=ON

cmake --build build-coverage -j
ctest --test-dir build-coverage --output-on-failure

gcovr --root . \
    --filter 'src/' \
    --filter 'lib/' \
    --html-details build-coverage/coverage.html \
    --print-summary \
    build-coverage
```

Отчёт по коду классов будет в `build-coverage/coverage.html`. Его можно открыть в браузере

## Документация

Документация собирается из комментариев в коде. Для генерации нужен Doxygen

```bash
cmake -S . -B build -DBUILD_DOCS=ON
cmake --build build --target docs
```

Открыть документацию можно через `build/docs/html/index.html`