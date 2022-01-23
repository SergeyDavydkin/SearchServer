#pragma once

#include <deque>
#include "document.h"
#include "search_server.h"

class RequestQueue {
public:

    explicit RequestQueue(const SearchServer& search_server);

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;

private:
    const SearchServer& search_server_;
    struct QueryResult {
        int results;
    };
    int empty_count_;
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;

    void RequestsCount(int result);

};

template <typename DocumentPredicate>
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
    std::vector<Document> result = search_server_.FindTopDocuments(raw_query, document_predicate);
    RequestsCount(result.size());
    return result;
}
