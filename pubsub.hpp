// Jarrod Humpel 2024
// https://github.com/Rebeljah/pubsub

#pragma once

#include <string>
#include <map>
#include <unordered_set>

using std::map;
using std::string;
using std::unordered_set;

namespace ps
{
    enum class SignalType : int
    {
        PlayerNameSet, // .str
        Click,         // .point
        Quit,          // n/a
    };

    struct Signal
    {
        SignalType type;
        string str; // always present, may be empty

        struct Vec2I
        {
            int x, y;
        };

        // refer to enum to check which meber to access
        union
        {
            Vec2I point;
        };

        Signal(SignalType type) : type(type) {}
    };

    class IObserver;

    class Subject
    {
    public:
        virtual ~Subject();
        virtual void addObserver(SignalType signalType, IObserver *observer);
        virtual void removeObserver(SignalType signalType, IObserver *observer);

    protected:
        virtual void emitSignal(const Signal &signal);

    private:
        using ObserverSet = unordered_set<IObserver *>;
        using ObserverMap = map<SignalType, ObserverSet>;
        ObserverMap m_observers;
    };

    class IObserver
    {
    public:
        virtual ~IObserver();
        virtual void addTrackingSubject(SignalType, Subject *);
        virtual void removeTrackingSubject(SignalType, Subject *);
        virtual void onSignal(const Signal &) = 0;

    private:
        using SubjectSet = unordered_set<Subject *>;
        using SubjectMap = map<SignalType, SubjectSet>;
        SubjectMap m_subjects;
    };
}; // namespace pubsub
