#include "remove_duplicates.h"

using namespace std;

void RemoveDuplicates(SearchServer& search_server) {
    vector<int> dublicated_ids;
    set<set<string>> set_document_words;

    set<string> document_words;
    for (const auto& document_id : search_server) {
        document_words.clear();       

        map<string, double> word_frequencies = search_server.GetWordFrequencies(document_id);
        transform(word_frequencies.cbegin(), word_frequencies.cend(),
            inserter(document_words, document_words.begin()),
            [](const pair<string, double>& key_value)
            { return key_value.first; });

        if (set_document_words.find(document_words) == set_document_words.end()) {
            set_document_words.emplace(document_words);
        }
        else {
            dublicated_ids.push_back(document_id);
        }
    }

    for (const auto& document_id : dublicated_ids) {
        cout << "Found duplicate document id " << document_id << endl;
        search_server.RemoveDocument(document_id);
    }
}
