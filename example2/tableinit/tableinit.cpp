#include <graphenelib/asset.h>
#include <graphenelib/contract.hpp>
#include <graphenelib/contract_asset.hpp>
#include <graphenelib/dispatcher.hpp>
#include <graphenelib/global.h>
#include <graphenelib/multi_index.hpp>
#include <graphenelib/system.h>

using namespace graphene;

class tableinit : public contract
{
  public:
    tableinit(uint64_t account_id)
        : contract(account_id)
    {
    }

    // @abi action
    void additem()
    {
        print("add item \n");
        table_index tables(_self, _self);
        tables.emplace(_self, [&](auto &obj) {
                obj.owner = tables.available_primary_key();                      //o.owner = 1、2、3、4...
                obj.str = "Hello";
        });
    }

  private:
    //@abi table table i64
    struct table {
        uint64_t owner;
        std::string str;
        uint64_t primary_key() const { return owner; }

        GRAPHENE_SERIALIZE(table, (owner)(str))
    };

    typedef graphene::multi_index<N(table), table> table_index;
};

GRAPHENE_ABI(tableinit,(additem))
