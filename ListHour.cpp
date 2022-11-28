#include "../hour/Hour.cpp"

class ListHour {
private:
    const static unsigned short int MAX_SIZE = 4;
    unsigned short int size = 0;
    Hour* hours[MAX_SIZE];

public:
    unsigned short int getSize() {
        return this->size;
    }

    Hour* getHour(int index) {
        return this->hours[index];
    }

    bool addHour(short int startHour, short int startMinute, short int endHour, short int endMinute) {
        if(this->size == MAX_SIZE) return false;
        this->hours[this->size] = new Hour(startHour, startMinute, endHour, endMinute);
        this->size++;
        return true;
    }

    void deleteHour(int index) {
        for (int i = index + 1; i < this->size; i++) {
            this->hours[index-1] = this->hours[index];
        }
        this->size--;
    }
};