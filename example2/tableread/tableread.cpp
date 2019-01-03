#include <graphenelib/asset.h>
#include <graphenelib/contract.hpp>
#include <graphenelib/contract_asset.hpp>
#include <graphenelib/dispatcher.hpp>
#include <graphenelib/global.h>
#include <graphenelib/multi_index.hpp>
#include <graphenelib/system.h>

using namespace graphene;

class tableread : public contract
{
  public:
    tableread(uint64_t account_id)
        : contract(account_id)
    {
    }
    struct table {
        uint64_t owner;
        std::string str;
        uint64_t primary_key() const { return owner; }

        GRAPHENE_SERIALIZE(table, (owner)(str))
    };
    typedef graphene::multi_index<N(table), table> table_index;

    // @abi action
    void readitem()
    {
        const std::string init_str="tableinit02";   //读取tableinit02合约的table
        auto init_id = get_account_id(init_str.c_str(),init_str.size());

        table_index tables(init_id,init_id);

        auto itor = tables.find(0);         //获取hello 字符串所在项的迭代器
        graphene_assert(itor!=tables.end(),"未发现该项");
        print(itor->str);
    }

};

GRAPHENE_ABI(tableread,(readitem))
