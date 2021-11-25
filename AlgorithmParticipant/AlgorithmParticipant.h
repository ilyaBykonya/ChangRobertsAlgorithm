#pragma once
#include "../ThreadsafeQueue/ThreadsafeQueue.h"
#include "../ThreadsafeCout/ThreadsafeCout.h"
#include <condition_variable>
#include <stdexcept>
#include <cstdint>
#include <variant>
using namespace std;

class AlgorithmParticipant
{
private:
    struct CandidateProposal
    {
        uint16_t _candidateUid;
    };
    struct LeaderSelected
    {
        uint16_t _leaderUid;
    };
    struct InitializeEleection
    {};

    enum State
    {
        NonParticipant,
        Candidate,
        Leader
    };

    mutex _locker;
    condition_variable _messagesObserver;

    State _state = NonParticipant;
    ThreadsafeQueue<variant<CandidateProposal, LeaderSelected, InitializeEleection>> _messagesQueue;

    uint16_t _uid;
    uint16_t _leaderUid = 0;
    AlgorithmParticipant* _next = nullptr;

public:
    AlgorithmParticipant(uint16_t uid);
    void setNextParticipant(AlgorithmParticipant* next) { _next = next; }
    uint16_t leaderUid() const { return _leaderUid; }

    void initializeElection();
    void execute();//Запустить цикл событий участника

private:
    void receiveMessage(CandidateProposal message);
    void receiveMessage(LeaderSelected message);

    void handleMessage(InitializeEleection message);
    void handleMessage(CandidateProposal message);
    void handleMessage(LeaderSelected message);
};

