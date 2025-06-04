#pragma once
#include <iostream>
class Time {

    friend class Time;

public:

    Time(int days = 0, int hours = 0, int minutes = 0);

    int getDays() const;
    int getHours() const;
    int getMinutes() const;

    Time operator+(const Time& other) const;
    Time operator-(const Time& other) const;
    Time& operator+=(const Time& other);
    Time& operator-=(const Time& other);

    bool operator==(const Time& other) const;
    bool operator!=(const Time& other) const;

    Time& operator++();
    Time& operator--();

    friend std::ostream& operator<<(std::ostream& os, const Time& time);

private:
    int totalMinutes_;

};

