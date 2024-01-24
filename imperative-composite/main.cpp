#include "Data.h"

TEST(main, main)
{
    Struct obj;
    obj.getMembers()["int"] = std::make_unique<Int>(7);
    obj.getMembers()["float"] = std::make_unique<Float>(2.3);
    obj.getMembers()["list"] = std::make_unique<List>();
    {
        auto& list = dynamic_cast<List&>(*obj.getMembers()["list"]);
        list.getValues().push_back(std::make_unique<Int>(1));
        list.getValues().push_back(std::make_unique<String>("deux"));
        list.getValues().push_back(std::make_unique<Float>(2.9999));
    }
    obj.getMembers()["obj"] = std::make_unique<Struct>();
    obj.getMembers()["str"] = std::make_unique<String>("whatever");

    DataPrinter().visit(obj);
}
