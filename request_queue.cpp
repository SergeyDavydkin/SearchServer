#include "request_queue.h"

using namespace std;

RequestQueue::RequestQueue(const SearchServer& search_server)
    : search_server_(search_server)
    , empty_count_(0)
{}

vector<Document> RequestQueue::AddFindRequest(const string& raw_query) {
    return AddFindRequest(raw_query, [](int document_id, DocumentStatus document_status, int rating) {
        return document_status == DocumentStatus::ACTUAL;
        });
}

vector<Document> RequestQueue::AddFindRequest(const string& raw_query, DocumentStatus status) {
    return AddFindRequest(raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
        return document_status == status;
        });
}

int RequestQueue::GetNoResultRequests() const {
    return empty_count_;
}

void RequestQueue::RequestsCount(int result) {
    if (requests_.size() >= min_in_day_) {
        if (requests_.front().results != 0) {
            --empty_count_;
        }
        requests_.pop_back();
    }
    if (!result) {
        ++empty_count_;
    }
    requests_.push_front({ result });
}
