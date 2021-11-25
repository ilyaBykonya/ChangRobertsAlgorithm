#include "TestModule.h"

uint16_t start_test(const string& configFileName)
{
    ifstream configFile(configFileName);

    int amountOfParticipants;
    configFile >> amountOfParticipants;
    if(amountOfParticipants < 2)
        return amountOfParticipants;


    map<uint16_t, AlgorithmParticipant*> allParticipants;
    for(int i = 0; i < amountOfParticipants; ++i)
    {
        uint16_t participantUid;
        configFile >> participantUid;
        allParticipants.insert({ participantUid, new AlgorithmParticipant(participantUid) });
    }

    for(auto iter = allParticipants.begin(); iter != (--allParticipants.end()); )
    {
        auto iter_prev = iter++;
        iter_prev->second->setNextParticipant(iter->second);
    }
    allParticipants.rbegin()->second->setNextParticipant(allParticipants.begin()->second);

    list<thread> allExecutors;
    for(auto& participant: allParticipants)
        allExecutors.push_back(thread(&AlgorithmParticipant::execute, participant.second));

    int amountOfInitiators;
    configFile >> amountOfInitiators;
    for(int i = 0; i < amountOfInitiators; ++i)
    {
        uint16_t initiatorUid;
        configFile >> initiatorUid;
        allParticipants[initiatorUid]->initializeElection();
    }

    for(auto& threadObject: allExecutors)
        threadObject.join();

    uint16_t leader_uid = allParticipants.begin()->second->leaderUid();
    for(auto& participant: allParticipants)
        delete participant.second;

    return leader_uid;
}
