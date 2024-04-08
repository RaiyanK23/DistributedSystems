#include <cstdint>
#include <iostream>
#include <vector>

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
    //mongocxx::client client;

    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    /*
    try
    {
        
        mongocxx::uri uri("mongodb://localhost:27017");
        mongocxx::client client(uri);
        std::cout << "connected to port" << 27017 <<std::endl;
    }
    catch (const std::exception& e)
    {
        try
        {
            mongocxx::uri uri("mongodb://localhost:27018");
            mongocxx::client client(uri);
        }
        catch (const std::exception& e)
        {
            try
            {
                mongocxx::uri uri("mongodb://localhost:27019");
                mongocxx::client client(uri);
            }
            catch (const std::exception& e)
            {
                std::cout << "No working database replicas" << std::endl;
            }
        }
    }*/

    auto db = client["CPSC599_Project"];
    auto collection = db["StockMetrics"];

    auto doc_value = make_document(
        kvp("StockName", "KO"),
        kvp("Price", 50),
        kvp("MarketCap", 200000),
        kvp("PERatio", 15),
        kvp("MarketHigh", 55),
        kvp("MarketLow", 45),
        kvp("High52", 60),
        kvp("Low52", 40));

     

    auto doc_view = doc_value.view();

    auto insert_one_result = collection.insert_one(doc_view);

   // auto find_one_result = collection.find({});

   

    auto cursor = collection.find({});
    for (auto &&doc : cursor)
    {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
   
    return 0;
}
