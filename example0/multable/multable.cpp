#include <graphenelib/asset.h>
#include <graphenelib/contract.hpp>
#include <graphenelib/contract_asset.hpp>
#include <graphenelib/dispatcher.hpp>
#include <graphenelib/global.h>
#include <graphenelib/multi_index.hpp>
#include <graphenelib/system.h>
#include <vector>
#include <list>

using namespace graphene;

class multable : public contract
{
  public:
    multable(uint64_t account_id)
        : contract(account_id)
        , tables(_self, _self)
    {
    }
    // @abi action
    void additem(uint32_t index){
        print("add item\n");
        
        tables.emplace(_self, [&](auto &obj) {
                obj.id = tables.available_primary_key();                      //o.owner = 1、2、3、4...
                obj.field1 = index;
                obj.field2 =2 ;
                obj.field3 = 3;
                obj.dfield4 = 4.123;
                obj.character5 = '5';

                obj.vec6.emplace_back(6);
                obj.str7 ="7";

                obj.sct9.number = 9;
                obj.sct9.result = 9;

                contract_asset assettemp(1000,0);
                obj.asset10 = assettemp;

                signature sigtemp = {1,2,3};
                obj.sig11 = sigtemp;

                /** 原始公钥 --->>> 钱包地址 
                struct binary_key
                {
                    binary_key() {}
                    uint32_t                 check = 0;
                    fc::ecc::public_key_data data;
                };
                public_key_type::operator std::string() const
                {
                    binary_key k;
                    k.data = key_data;
                    k.check = fc::ripemd160::hash( k.data.data, k.data.size() )._hash[0];
                    auto data = fc::raw::pack( k );
                    return GRAPHENE_ADDRESS_PREFIX + fc::to_base58( data.data(), data.size() );
                }
                **/

                public_key pktemp = {'H','e','l','l','o'};
                obj.pk12 = pktemp;

                checksum256 hash256temp = {1,3,5,7,9};
                obj.hash25613 = hash256temp;

                checksum160 hash160temp = {2,4,6,8,10};
                obj.hasn16014 = hash160temp;
        });
    }
    // @abi action
    void modifyitem(int16_t number){
        print("modify item 修改ID为0的对象的field2字段\n");
        //修改主键为0的对象
        auto itor = tables.find(0);
        graphene_assert(itor != tables.end(),"错误：未发现该项");
        //可以是对象的引用，也可以是迭代器，重载了两个版本
        tables.modify(itor,0,[&](auto &obj){
            obj.field2 = number;
        });
    }
    // @abi action
    void delitem(uint64_t id){
        print("delete item 删除指定对象\n");
        //删除主键id为参数id的对象
        auto itor = tables.find(id);
        graphene_assert(itor != tables.end(),"错误：未发现该项");
        //可以是对象的引用，也可以是迭代器，重载了两个版本
        tables.erase(itor);
    }
    // @abi action
    void finditem(){
        print("find item\n");

        // 使用主键索引（主键是唯一的）获取主键为0的项
        auto itor = tables.find(0);
        graphene_assert(itor != tables.end(),"错误：未发现该项");
        print("主键索引，ID：", itor->id);
        print("\n");
        print("\n");

        // 使用二级索引（等其他索引）获取主键为0的项
        auto tables2 = tables.get_index<N(field1)>();
        auto itor2 = tables2.find(1);
        graphene_assert(itor2 != tables2.end(),"错误：未发现该项");
        print("二级索引，ID：", itor->id);
        print("\n");
        print("\n");

        // 其他
        // lower_bound 与 upper_bound
        auto lo_itor = tables.lower_bound(0);       //返回的是指向ID为0的迭代器
        graphene_assert(lo_itor != tables.end(),"错误：未发现该项");
        while(lo_itor != tables.end()){
            print("lower_bound, ID:",lo_itor->id);
            print("\n");
            print("lower_bound, index:",lo_itor->field1);
            print("\n");
            lo_itor++;
        }
        print("\n");

        auto up_itor = tables.upper_bound(4);       //返回的是指向ID为5的迭代器
        graphene_assert(up_itor != tables.end(),"错误：未发现该项");
        print("upper_bound, ID:",up_itor->id);
        print("\n");
        print("upper_bound, index:",up_itor->field1);
        print("\n\n");
        // find与get方式（find返回的是迭代器，get返回的是对象的常量引用）
        auto obj = tables.get(0);
        graphene_assert(obj.id == 0,"错误：未发现该项");
        print("get方式获取主键, ID:",obj.id);
        print("\n");
    }

  private:
    
    struct substruct{
        int32_t number;
        int64_t result;
    };
    //@abi table mytable i64
    struct mytable {
        uint64_t id;
        uint32_t field1;
        int16_t  field2;
        int8_t   field3;
        double   dfield4;
        char     character5;
        
        std::vector<uint32_t>    vec6;
        std::string         str7; 
        // std::list<uint32_t>      list8;  wrong!!!     can only be: vector, struct, class or a built-in type.
        
        
        substruct sct9;

        contract_asset asset10;
        signature sig11;
        public_key pk12;
        checksum256 hash25613;
        checksum160 hasn16014;


        uint64_t primary_key() const { return id; }
        uint64_t byfield1() const { return field1; } 

        GRAPHENE_SERIALIZE(mytable, (id)(field1)
        (field2)(field3)
        (dfield4)
        (character5)
        (vec6)(str7)//(list8)
        (sct9)
        (asset10)
        (sig11)(pk12)(hash25613)(hasn16014)
        )
    };

    typedef graphene::multi_index<N(mytable), mytable,
        indexed_by<N(field1), const_mem_fun<mytable, uint64_t, &mytable::byfield1>>
        > table_index;

    table_index tables;
};

GRAPHENE_ABI(multable,(additem)(modifyitem)(delitem)(finditem))

