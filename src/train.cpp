#include "train.h"
#include <stdexcept>
#include <limits>

Train::Train() : wagons_() {}

Train::Train(const unsigned int wagon_count) : wagons_(wagon_count) {}

Train::Train(const Wagon* wagons, unsigned int wagon_count){
    if (wagons == nullptr && wagon_count > 0){
        throw std::invalid_argument("Array pointer is null!");
    }
    for (unsigned int i = 0; i < wagon_count; i++){
        wagons_.push_back(wagons[i]);
    }
}

Train::Train(const Wagon& w) : wagons_{w} {}

Train& Train::operator+=(const Wagon& w){
    wagons_.push_back(w);
    return *this;
}

Wagon& Train::operator[](unsigned int index) { // исправл int на uint. ловилось исключение vector::_M_range_check: __n (which is 18446744073709551615) >= this->size() (which is 1) если тыкал отриц.число в восьмой пункт меню 
    return wagons_.at(index);
}

const Wagon& Train::operator[](unsigned int index) const { 
    return wagons_.at(index);
}


void Train::deleteWagon(int index){
    if (index < 0 || index >= wagons_.size()){
        throw std::out_of_range("Index out of range!");
    }
    wagons_.erase(wagons_.begin() + index);
}

unsigned int Train::size() const {
    return wagons_.size();
}

int Train::searchBiggerPercent(){
    int index = 0;
    int index_bigger_percent = 0;
    double mx = -1;
    for (const auto& wagon : wagons_){
        if (wagon.getBusyPercent() > mx){
            mx = wagon.getBusyPercent();
            index_bigger_percent = index;
        }
        index++;
    }
    return index_bigger_percent;
}

int Train::searchSmallerPercent(){
    int index = 0;
    int index_smaller_percent = -1;
    double mn = 101;
    for (const auto& wagon : wagons_){
        if (wagon.getType() == WagonType::Caffe){
            index++;
            continue;
        }
        if (wagon.getBusyPercent() < mn){
            mn = wagon.getBusyPercent();
            index_smaller_percent = index;
        }
        index++;
    }
    return index_smaller_percent;
}

int Train::searchMaxCapacity(){
    int index = 0;
    int index_max_capacity = 0;
    double mx = -1;
    for (const auto& wagon : wagons_){
        if (wagon.getCapacity() > mx){
            mx = wagon.getCapacity();
            index_max_capacity = index;
        }
        index++;
    }
    return index_max_capacity;
}

void Train::sitDownMostFreeWagon(int passangers_count){
    double min = 101;
    int index = 0;
    int index_free_wagon = -1;
    for (const auto& wagon : wagons_){
        if ((wagon.getType() != WagonType::Luxe) && (wagon.getType() != WagonType::Caffe) && wagon.getBusyPercent() < min){
            min = wagon.getBusyPercent();
            index_free_wagon = index;
        }
        else if ((wagon.getType() != WagonType::Caffe) && wagon.getBusyPercent() < min){
            min = wagon.getBusyPercent();
            index_free_wagon = index;
        }
        index++;
    }
    if (index_free_wagon == -1){
        throw std::logic_error("Not fount wagon for removing");
    }
    Wagon& w = wagons_.at(index_free_wagon);
    w.sitDown(passangers_count);
}

void Train::sitDownMostFreeWagons(){  // 
    if (wagons_.empty()){
        return;
    }

    int index_mn = searchSmallerPercent();
    if (index_mn == -1){
        throw std::logic_error("All wagons it's a caffe!");
    }

    int limit = wagons_.at(searchMaxCapacity()).getCapacity();

    for (int t = 0; t < static_cast<int>(WagonType::COUNT); t++){ // теперь можно добавлять новые типы выгонов и ничо не сломается!!!
        WagonType current_type = static_cast<WagonType>(t);

        for (int step = 0; step < limit; step++){
            int idx_mx = -1;
            int idx_mn = -1;
            double mx = -1;
            double mn = 101;

            for (int i = 0; i < wagons_.size(); i++){
                if (wagons_.at(i).getType() != current_type){
                    continue;
                }
                if (wagons_.at(i).getBusyPercent() > mx){
                    mx = wagons_.at(i).getBusyPercent();
                    idx_mx = i;
                }
                if (wagons_.at(i).getBusyPercent() < mn){
                    mn = wagons_.at(i).getBusyPercent();
                    idx_mn = i;
                }
            }

            if (idx_mx == -1 || idx_mn == -1 || idx_mx == idx_mn){
                break;
            }
            if (wagons_.at(idx_mx).getBusyCount() == 0){
                break;
            }
            if (wagons_.at(idx_mn).getBusyCount() == wagons_.at(idx_mn).getCapacity()){
                break;
            }

            wagons_.at(idx_mx).standUp(1);
            wagons_.at(idx_mn).sitDown(1);
        }
    }
}

void Train::minimizeWagons(){
    if (wagons_.empty()){
        return;
    }

    for (int i = wagons_.size() - 1; i >= 0; i--){
        if (wagons_.at(i).getType() == WagonType::Caffe){
            continue;
        }

        for (int j = 0; j < i; j++){
            if (wagons_.at(j).getType() == wagons_.at(i).getType()){
                while (wagons_.at(i).getBusyCount() > 0 &&
                       wagons_.at(j).getCapacity() - wagons_.at(j).getBusyCount() > 0){
                    wagons_.at(i).standUp(1);
                    wagons_.at(j).sitDown(1);
                }
            }
        }
    }

    for (int i = wagons_.size() - 1; i >= 0; i--){
        if (wagons_.at(i).getType() != WagonType::Caffe && wagons_.at(i).getBusyCount() == 0){
            deleteWagon(i);
        }
    }
}

void Train::placeCaffeOptimal(){
    int caffe_index = -1;
    for (int i = 0; i < wagons_.size(); i++){
        if (wagons_.at(i).getType() == WagonType::Caffe){
            caffe_index = i;
            break;
        }
    }

    if (caffe_index == -1){
        throw std::logic_error("No caffe wagon in train!");
    }

    Wagon caffe = wagons_.at(caffe_index);
    deleteWagon(caffe_index);

    if (wagons_.empty()){
        wagons_.push_back(caffe);
        return;
    }

    int best_pos = 0;
    int min_diff = std::numeric_limits<int>::max();

    for (int pos = 0; pos <= wagons_.size(); pos++){
        int left_passengers = 0;
        int right_passengers = 0;

        for (int i = 0; i < pos; i++){
            if (wagons_.at(i).getType() != WagonType::Luxe){
                left_passengers += wagons_.at(i).getBusyCount();
            }
        }

        for (int i = pos; i < wagons_.size(); i++){
            if (wagons_.at(i).getType() != WagonType::Luxe){
                right_passengers += wagons_.at(i).getBusyCount();
            }
        }

        int diff = left_passengers - right_passengers;
        if (diff < 0){
            diff = -diff;
        }

        if (diff < min_diff){
            min_diff = diff;
            best_pos = pos;
        }
    }

    wagons_.insert(wagons_.begin() + best_pos, caffe);
}

std::ostream& Train::print(std::ostream& out) const {
    out << wagons_.size() << std::endl;
    for (const auto& wagon : wagons_){
        out << wagon << std::endl;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Train& t){
    return t.print(out);
}