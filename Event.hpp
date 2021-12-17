//
// Created by Leonardo Villalobos on 12/16/21.
//

#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <cassert>

class Event {
public:
    Event () = default;

    Event (std::string name, std::string date):
        name (std::move (name)), date (std::move (date)) {}

    [[nodiscard]] const std::string &getName () const { return name; }

    [[nodiscard]] const std::string &getDate () const { return date; }

    [[nodiscard]] const std::string &getCategory () const {return category;}

    void setName (const std::string &n_name) {
        assert(!n_name.empty());
        Event::name = n_name;
    }

    void setDate (const std::string &n_date) {
        assert(!n_date.empty());
        Event::date = n_date;
    }

    void setCategory (const std::string &n_category) {
        assert(!n_category.empty());
        Event::category = n_category;
    }

    [[nodiscard]] bool null () const {
        return name.empty() || date.empty() || category.empty();
    }

private:
    std::string category, name, date;
};

#endif //EVENT_HPP
