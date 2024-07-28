#include "pubsub.hpp"

ps::Subject::~Subject()
{
    for (auto kv : m_observers)
    {
        SignalType signalType = kv.first;
        ObserverSet &observers = kv.second;
        for (auto pObserver : observers)
        {
            pObserver->removeTrackingSubject(signalType, this);
        }
    }
}

void ps::Subject::addObserver(SignalType signalType, IObserver *observer)
{
    // init observer set for this signal type
    if (m_observers.find(signalType) == m_observers.end())
    {
        m_observers.emplace(signalType, ObserverSet());
    }

    m_observers.at(signalType).emplace(observer);
    observer->addTrackingSubject(signalType, this);
}

void ps::Subject::removeObserver(SignalType signalType, IObserver *observer)
{
    auto &observerSet = m_observers.at(signalType);
    observerSet.erase(observer);

    // erase this signal from map if subscriber was last observer
    if (observerSet.empty())
        m_observers.erase(signalType);
}

void ps::Subject::emitSignal(const Signal &signal)
{
    if (m_observers.find(signal.type) == m_observers.end())
        return; // signal has no observers so return

    for (auto pObserver : m_observers.at(signal.type))
    {
        pObserver->onSignal(signal);
    }
}

ps::IObserver::~IObserver()
{
    for (auto kv : m_subjects)
    {
        SignalType signalType = kv.first;
        SubjectSet &subjects = kv.second;
        for (auto pSubject : subjects)
        {
            pSubject->removeObserver(signalType, this);
        }
    }
}

void ps::IObserver::addTrackingSubject(SignalType signalType, Subject *subject)
{
    if (m_subjects.find(signalType) == m_subjects.end())
    {
        m_subjects.emplace(signalType, SubjectSet());
    }

    m_subjects.at(signalType).emplace(subject);
}

void ps::IObserver::removeTrackingSubject(SignalType signalType, Subject *subject)
{
    auto &subjectSet = m_subjects.at(signalType);
    subjectSet.erase(subject);

    // erase this signal from map if subscriber was last observer
    if (subjectSet.empty())
        m_subjects.erase(signalType);
}
