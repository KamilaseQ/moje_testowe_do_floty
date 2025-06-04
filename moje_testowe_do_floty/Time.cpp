#include "Time.h"
#include <iomanip>


Time::Time(int days, int hours, int minutes)
{
    totalMinutes_ = days * 24 * 60 + hours * 60 + minutes;
}


int Time::getDays() const
{
    return totalMinutes_ / (24 * 60);
}


int Time::getHours() const
{
    return (totalMinutes_ / 60) % 24;
}


int Time::getMinutes() const
{
    return totalMinutes_ % 60;
}


Time Time::operator+(const Time& other) const
{
    return Time(0, 0, totalMinutes_ + other.totalMinutes_);
}


Time Time::operator-(const Time& other) const
{
    return Time(0, 0, totalMinutes_ - other.totalMinutes_);
}


Time& Time::operator+=(const Time& other)
{
    totalMinutes_ += other.totalMinutes_;
    return *this;
}


Time& Time::operator-=(const Time& other)
{
    totalMinutes_ -= other.totalMinutes_;
    return *this;
}


bool Time::operator==(const Time& other) const
{
    return totalMinutes_ == other.totalMinutes_;
}


bool Time::operator!=(const Time& other) const
{
    return !(*this == other);
}


Time& Time::operator++()
{
    ++totalMinutes_;
    return *this;
}


Time& Time::operator--()
{
    --totalMinutes_;
    return *this;
}


std::ostream& operator<<(std::ostream& os, const Time& time)
{
    os << "Day " << time.getDays() << ", "
        << std::setw(2) << std::setfill('0') << time.getHours() << ":"
        << std::setw(2) << std::setfill('0') << time.getMinutes();
    return os;
}
