//
// Created by Leonardo Villalobos on 12/16/21.
//

#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <cassert>
#include <type_traits>

/**
 * Create an Event, consisting of a name and a date.
 */
class Event {
public:
    Event () = default;

    Event (std::string name, std::string date): name (std::move (name)), date (std::move (date)) {}

    // Accessors
    [[nodiscard]] const std::string &getName () const { return name; }

    [[nodiscard]] const std::string &getDate () const { return date; }

    // Mutators
    void setName (const std::string &n_name) {
        assert(!n_name.empty ());
        Event::name = n_name;
    }

    void setDate (const std::string &n_date) {
        assert(!n_date.empty ());
        Event::date = n_date;
    }

    /**
     * An Event is equal to another Event iff the name and date are exactly
     * the same.
     * @param lhs The first Event.
     * @param rhs The second Event.
     * @return If the events' members are identical
     */
    friend bool operator== (const Event &lhs, const Event &rhs) {
        return lhs.name == rhs.name && lhs.date == rhs.date;
    }

private:
    std::string name, date;
};

#endif //EVENT_HPP
