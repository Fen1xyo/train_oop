#include "wagon.h"

Wagon::Wagon(unsigned int max_capacity, unsigned int busy_count, WagonType type) : type_(type), max_capacity_(max_capacity), busy_count_(busy_count) {
    if (type_ == WagonType::Caffe && busy_count_ + max_capacity_ > 0){
        throw std::invalid_argument("Caffe wagon cannot include seats!");
    }
    if (busy_count_ > max_capacity_){
        throw std::invalid_argument("busy seats cannot be bigger then max seats count!");
    }
}

Wagon::Wagon(WagonType type) : type_(type) {
    switch (type_){
        using enum WagonType;
        case Sit:
            max_capacity_ = 50;
            break;
        case Econom:
            max_capacity_ = 80;
            break;
        case Luxe:
            max_capacity_ = 20;
            break;
        case Caffe:
            max_capacity_ = 0;
            break;
    }
    busy_count_ = 0;
}

Wagon::Wagon() noexcept{
    type_ = WagonType::Sit;
    busy_count_ = 0;
    max_capacity_ = 50;
}

double Wagon::getBusyPercent() const noexcept {
    if (max_capacity_ == 0){return 0;}
    return (static_cast<double>(busy_count_) / max_capacity_) * 100;
}

Wagon& Wagon::operator>>(Wagon& w) noexcept{ 
    while (getBusyPercent() > w.getBusyPercent()){
        if(w.busy_count_ == w.max_capacity_ || busy_count_ == 0){break;}
        busy_count_--;
        w.busy_count_++;
    }
    return *this;
}

void Wagon::sitDown(int passangers){
    if (busy_count_ + passangers > max_capacity_){
        throw std::overflow_error("We cant sit down this count of passangers!");
    }
    busy_count_ += passangers;
}

void Wagon::standUp(int passangers){
    if (passangers > busy_count_){
        throw std::overflow_error("We cant kick this count of passangers!");
    }
    busy_count_ -= passangers;
}

int Wagon::getCapacity() const noexcept {
    return max_capacity_;
}

int Wagon::getBusyCount() const noexcept {
    return busy_count_;
}

const WagonType Wagon::getType() const noexcept {
    return type_;
}

void Wagon::setCapacity(unsigned int max_capacity){
    if (type_ == WagonType::Caffe && max_capacity > 0){
        throw std::invalid_argument("Caffe wagon cannot include seats!");
    }
    if (busy_count_ > max_capacity){
        throw std::invalid_argument("busy seats cannot be bigger then max seats count!");
    }
    max_capacity_ = max_capacity;
}

void Wagon::setBusyCount(unsigned int busy_count){
    if (busy_count > max_capacity_){
        throw std::invalid_argument("busy seats cannot be bigger then max seats count!");
    }
    busy_count_ = busy_count;
}

void Wagon::setType(WagonType type) noexcept{
    if (type == WagonType::Caffe){
        max_capacity_ = 0;
        busy_count_ = 0;
    }

    if (type_ == WagonType::Caffe){
        switch (type){
            using enum WagonType;
            case Sit: max_capacity_ = 50; break;
            case Econom: max_capacity_ = 80; break;
            case Luxe: max_capacity_ = 20; break;
        }
    }
    type_ = type;
}

std::ostream& Wagon::print(std::ostream& out) const {
    out << static_cast<int>(type_) << " " << max_capacity_ << " " << busy_count_;
    return out;
}

std::ostream& operator<<(std::ostream& out, const Wagon& w){
    return w.print(out);
}