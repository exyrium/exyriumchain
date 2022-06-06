#pragma once
#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/transaction.hpp>

#define EXYRIUMADMIN "exyriumchain"
#define EXYRIUMSTAKE "pool.exyrium"
#define EXYRIUMDRAW "draw.exyrium"

#define EXYRIUMTOKEN "exyrium.org"

#define VEXTOKEN "vex.token"
#define VXTOKEN "vexwrap.exy"
#define VYNTOKEN "vyndaoutoken"
#define DJVTOKEN "djvtokenvexa"

#define EXYRIUM_SYMBOL eosio::symbol("EXYRIUM", 8)

#define VEX_SYMBOL eosio::symbol("VEX", 4)
#define VX_SYMBOL eosio::symbol("VX", 4)
#define VYN_SYMBOL eosio::symbol("VYN", 8)
#define DJV_SYMBOL eosio::symbol("DJV", 8)

#define NO_TIME   2000000000
#define SECONDS_THREE_DAY  259200
#define MIN_STAKE_VEX 1000000 //100 VEX
#define MIN_STAKE_VX 100000000 //10K VX
#define MIN_STAKE_VYN 10000000 //0.1 VYN
#define MIN_STAKE_DJV 10000 // 0.0001 djv

//10k
#define STAGE_MINING_TOTAL eosio::asset(1000000000000000, EXYRIUM_SYMBOL)

using eosio::extended_asset;
using namespace eosio;
 
static constexpr int64_t max_amount  = 100000000000000000;

namespace vexdt {
    class [[eosio::contract("exyriumchain")]] exyriumchain :public eosio::contract {
    public:
        exyriumchain(eosio::name receiver, eosio::name code, eosio::datastream<const char *> ds) :
                eosio::contract(receiver, code, ds),
                _global(eosio::name(EXYRIUMADMIN), eosio::name(EXYRIUMADMIN).value),
                _users(eosio::name(EXYRIUMADMIN), eosio::name(EXYRIUMADMIN).value){
        }

        //更新抵押
        ACTION doissue(const uint64_t &idfrom, const uint64_t &idto);

        ACTION init(); 

        ACTION setstop(const uint8_t &state);

        ACTION claim(const eosio::name &from, const std::string &pooltype);

        ACTION exit(const eosio::name &from, const std::string &pooltype);

        bool isvextoken(const eosio::extended_asset &quantity)
        {
            //print("quantity.contract", quantity.contract); 
            if ((quantity.contract == eosio::name(VEXTOKEN)) && (quantity.quantity.symbol == VEX_SYMBOL))
            {
                return true;
            }
            return false;
        }

        bool isvxtoken(const eosio::extended_asset &quantity)
        {
            //print("quantity.contract", quantity.contract); 
            if ((quantity.contract == eosio::name(VXTOKEN)) && (quantity.quantity.symbol == VX_SYMBOL))
            {
                return true;
            }
            return false;
        }

        bool isvyntoken(const eosio::extended_asset &quantity)
        {
            //print("quantity.contract", quantity.contract); 
            if ((quantity.contract == eosio::name(VYNTOKEN)) && (quantity.quantity.symbol == VYN_SYMBOL))
            {
                return true;
            }
            return false;
        }

        bool isdjvtoken(const eosio::extended_asset &quantity)
        {
            //print("quantity.contract", quantity.contract); 
            if ((quantity.contract == eosio::name(DJVTOKEN)) && (quantity.quantity.symbol == DJV_SYMBOL))
            {
                return true;
            }
            return false;
        }

        void apply(eosio::name code, eosio::name action);

        void stake(const eosio::name &from,
                const eosio::asset &quantity);

        void onTransfer(const eosio::name &from,
            const eosio::name &to,
            const eosio::extended_asset &quantity,
            const std::string &memo);

        TABLE global {
            uint8_t initState;
            uint8_t stopState;

            uint64_t checkfromid;
            uint64_t maxstakeid;
            uint64_t check_update_time;

            eosio::asset all_pool_reward;

            eosio::asset total_staked_vex;
            eosio::asset vex_pool_reward;
            eosio::asset vex_pool_day_reward;
            eosio::asset total_staked_vx;
            eosio::asset vx_pool_reward;
            eosio::asset vx_pool_day_reward;
            eosio::asset total_staked_vyn;
            eosio::asset vyn_pool_reward;
            eosio::asset vyn_pool_day_reward;
            eosio::asset total_staked_djv;
            eosio::asset djv_pool_reward;
            eosio::asset djv_pool_day_reward;
            eosio::asset total_mininged;
        };
        typedef eosio::singleton<"global"_n, global> global_table;


        TABLE st_user {
            uint64_t    id = 0;
            eosio::name holder;
            eosio::asset vex_stake;
            eosio::asset vx_stake;
            eosio::asset vyn_stake;
            eosio::asset djv_stake;

            //eosio::asset available_bonus; //可取分红

            eosio::asset vex_bonus;
            eosio::asset vx_bonus;
            eosio::asset vyn_bonus;
            eosio::asset djv_bonus;


            uint64_t primary_key() const { return id; }
            uint64_t by_holder() const { return holder.value; }
        };

        typedef eosio::multi_index<"user"_n, st_user,
                                eosio::indexed_by<"holder"_n, eosio::const_mem_fun<st_user, uint64_t, &st_user::by_holder>>
                                    > user_table;


        global_table _global;
        user_table _users;


        ACTION test()
        {
            require_auth(eosio::name(EXYRIUMADMIN));

            //新阶段 需要更新日奖池
            // auto global = _global.get_or_default();
            // eosio::asset all = eosio::asset(100000000, VYN_SYMBOL);//单倍池奖励 第3阶段 960s一次
            // eosio::asset one = all/20;
            // global.vex_pool_day_reward = one * 7 * 90; //日奖池
            // global.vx_pool_day_reward = one * 10 * 90;
            // global.vyn_pool_day_reward = one * 3 * 90;
            // _global.set(global, eosio::name(EXYRIUMADMIN));

            _global.remove( );
            auto itr = _users.begin();
            while(itr != _users.end()){
                itr = _users.erase(itr);
            }
        }
    };


    struct st_transfer
    {
        eosio::name from;
        eosio::name to;
        eosio::asset quantity;
        std::string memo;
    };

    void exyriumchain::apply(eosio::name code, eosio::name action)
    {
        auto &thiscontract = *this;

        if (action == eosio::name("transfer") && (code == eosio::name(VEXTOKEN) || code == eosio::name(VXTOKEN) || code == eosio::name(DJVTOKEN) || code == eosio::name(VYNTOKEN)))
        {
            //print("  apply code name", code); //code 为那个账户触发的通知
            auto transfer_data = eosio::unpack_action_data<st_transfer>();
            onTransfer(transfer_data.from, transfer_data.to, eosio::extended_asset(transfer_data.quantity, code), transfer_data.memo);
            return;
        }
        
        if (code != eosio::name(EXYRIUMADMIN))
            return;
        if( code == eosio::name(EXYRIUMADMIN) ) {
            switch(action.value) {
                case eosio::name("init").value: 
                    execute_action(eosio::name(EXYRIUMADMIN), eosio::name(code), &exyriumchain::init); 
                    break;
                case eosio::name("setstop").value: 
                    execute_action(eosio::name(EXYRIUMADMIN), eosio::name(code), &exyriumchain::setstop); 
                    break;
                case eosio::name("test").value: 
                    execute_action(eosio::name(EXYRIUMADMIN), eosio::name(code), &exyriumchain::test); 
                    break;
                case eosio::name("doissue").value: 
                    execute_action(eosio::name(EXYRIUMADMIN), eosio::name(code), &exyriumchain::doissue); 
                    break;
                case eosio::name("claim").value: 
                    execute_action(eosio::name(EXYRIUMADMIN), eosio::name(code), &exyriumchain::claim); 
                    break;
                // case eosio::name("unstake").value: 
                //     execute_action(eosio::name(EXYRIUMADMIN), eosio::name(code), &exyriumchain::unstake); 
                //     break;
                case eosio::name("exit").value: 
                    execute_action(eosio::name(EXYRIUMADMIN), eosio::name(code), &exyriumchain::exit); 
                    break;
            }
        }
    }


    extern "C"
    {
        [[noreturn]] void apply(uint64_t receiver, uint64_t code, uint64_t action) {
            eosio::datastream<const char*> ds( nullptr, 0 );
            exyriumchain p(eosio::name(receiver), eosio::name(code), ds);
            p.apply(eosio::name(code), eosio::name(action));
            eosio_exit(0);
        }
    }

} /// namespace eosio
