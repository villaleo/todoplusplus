//
// Created by Leonardo Villalobos on 12/16/21.
//

#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>

class Event {
public:
    Event () { isNull = true; }

    Event (std::string name, std::string date):
        name (std::move (name)), date (std::move (date)) {}

    [[nodiscard]] const std::string &getName () const { return name; }

    [[nodiscard]] const std::string &getDate () const { return date; }

    [[nodiscard]] const std::string &getCategory () const {return category;}

    void setName (const std::string &n_name) { Event::name = n_name; }

    void setDate (const std::string &n_date) { Event::date = n_date; }

    void setCategory (const std::string &n_category) {Event::category = n_category;}

    [[nodiscard]] bool null () const { return isNull; }

private:
    std::string category, name, date;
    bool isNull { false };
};

#endif //EVENT_HPP
