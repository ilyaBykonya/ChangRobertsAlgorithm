#include "AlgorithmParticipant.h"

AlgorithmParticipant::AlgorithmParticipant(uint16_t uid)
    :_uid{ uid }
    {
    }


void AlgorithmParticipant::initializeElection()
{
    lock_guard<mutex> lk(_locker);
    _messagesQueue.push(InitializeEleection{});
    _messagesObserver.notify_one();
}
void AlgorithmParticipant::receiveMessage(CandidateProposal message)
{
    lock_guard<mutex> lk(_locker);
    _messagesQueue.push(message);
    _messagesObserver.notify_one();
}
void AlgorithmParticipant::receiveMessage(LeaderSelected message)
{
    lock_guard<mutex> lk(_locker);
    _messagesQueue.push(message);
    _messagesObserver.notify_one();
}


void AlgorithmParticipant::execute()
{
    while (_leaderUid == 0)
    {
        unique_lock<mutex> lk(_locker);
        if(_messagesQueue.empty()) {
            _messagesObserver.wait(lk, [this]() ->bool { return !_messagesQueue.empty(); });
        }

        auto message = _messagesQueue.pop();
        if(message.index() == 0) {
            handleMessage(get<CandidateProposal>(message));
        } else if(message.index() == 1) {
            handleMessage(get<LeaderSelected>(message));
        } else {
            handleMessage(get<InitializeEleection>(message));
        }
    }
}


void AlgorithmParticipant::handleMessage(InitializeEleection message)
{
    ThreadsafeCout{} << _uid << ": " << "initialize election" << endl;
    _state = Candidate;
    _next->receiveMessage(CandidateProposal{ _uid });
}
void AlgorithmParticipant::handleMessage(CandidateProposal message)
{
    ThreadsafeCout{} << _uid << ": " << "Receive candidate_proposal message: " << message._candidateUid << endl;
    if(message._candidateUid > _uid) {
        _next->receiveMessage(message);
    } else if(message._candidateUid < _uid) {
        if(_state == NonParticipant) {
            _state = Candidate;
            _next->receiveMessage(CandidateProposal{ _uid });
        }
    } else {
        _state = Leader;
        _leaderUid = _uid;
        _next->receiveMessage(LeaderSelected{ _uid });
    }
}
void AlgorithmParticipant::handleMessage(LeaderSelected message)
{
    ThreadsafeCout{} << _uid << ": " << "Receive leader_selected message: " << message._leaderUid << endl;
    if(message._leaderUid == _uid) {
        return;
    } else {
        if(_leaderUid == 0) {
            _state = NonParticipant;
            _leaderUid = message._leaderUid;
            _next->receiveMessage(message);
        }
    }
}
























