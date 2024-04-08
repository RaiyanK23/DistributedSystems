#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

int main()
{
    mongocxx::instance instance{}; // This should be done only once.
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);
    auto db = client["CPSC599_Project"];
    auto collection = db["StockMetrics"];

    std::string tickers[] = {"GOOGL", "AAPL", "MSFT", "AMZN", "FB", "BRK.B", "JNJ", "V", "JPM", "KO"};
    // Seed the random number generator
    srand(time(0));

    // Generate a single random value of either 3 or -2
    int randomValue = (rand() % 2 == 0) ? 3 : -2;

    auto filter = document{} << "Ticker" << "MSFT" << finalize;
    auto update = document{} << "$inc" << open_document << "Price" << randomValue << close_document << finalize;


    collection.update_one(
        make_document(kvp("StockName", "AAPL")),
        make_document(kvp("Price", 155))

    );

    // Update the document with given key-value pair.
}
