class Hour {
private:
    short int startHour;
    short int startMinute;
    short int endHour;
    short int endMinute;
public:
    Hour(short int startHour, short int startMinute, short int endHour, short int endMinute) {
        this->startHour = startHour;
        this->startMinute = startMinute;
        this->endHour = endHour;
        this->endMinute = endMinute;
    }

    short int getHoursStart() {
        return this->startHour;
    }
    short int getMinutesStart() {
        return this->startMinute;
    }
    short int getHoursEnd() {
        return this->endHour;
    }
    short int getMinutesEnd() {
        return this->endMinute;
    }
};

