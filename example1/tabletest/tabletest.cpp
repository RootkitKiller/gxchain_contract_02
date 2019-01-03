#include <graphenelib/asset.h>
#include <graphenelib/contract.hpp>
#include <graphenelib/contract_asset.hpp>
#include <graphenelib/dispatcher.hpp>
#include <graphenelib/global.h>
#include <graphenelib/multi_index.hpp>
#include <graphenelib/system.h>
#include <vector>

using namespace graphene;

//@abi table tablea i64
struct tableb {
    uint64_t owner;
    std::vector<contract_asset> balances;

    uint64_t primary_key() const { return owner; }

    GRAPHENE_SERIALIZE(tableb, (owner)(balances))
};

typedef graphene::multi_index<N(tableb), tableb> tableb_index;

// @abi action
// @abi payable
void additemb()
{
    print("additemb\n");
    auto current_id = current_receiver();
    tableb_index tableb_s(current_id,current_id);
    tableb_s.emplace(current_id, [&](auto &obj) {
            obj.owner = tableb_s.available_primary_key();                                                 //o.owner = 1、2、3、4...
            contract_asset asset_balance {10000, 0};
            obj.balances.emplace_back(asset_balance);
    });
}

class tabletest : public contract
{
  public:
    tabletest(uint64_t account_id)
        : contract(account_id)
        , tablea_s(_self, _self)
    {
    }

    // @abi action
    // @abi payable
    void additema()
    {
        print("additema\n");
        
        tablea_s.emplace(_self, [&](auto &obj) {
                obj.owner = tablea_s.available_primary_key();                      //o.owner = 1、2、3、4...
                contract_asset asset_balance {10000, 0};
                obj.balances.emplace_back(asset_balance);
        });
        print("additema\n");
    }

  private:
    //@abi table tablea i64
    struct tablea {
        uint64_t owner;
        std::vector<contract_asset> balances;

        uint64_t primary_key() const { return owner; }

        GRAPHENE_SERIALIZE(tablea, (owner)(balances))
    };

    typedef graphene::multi_index<N(tablea), tablea> tablea_index;

    tablea_index tablea_s;
};

extern "C" { 
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { 
      auto self = receiver; 
      if (code == self) { 
         tabletest thiscontract( self ); 
         switch( action ) { 
            case N(additemb):
                additemb();
                break;
            default:
            GRAPHENE_API( tabletest, (additema) ) 
         } 
      }
   } 
} 

