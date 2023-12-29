#define CATCH_CONFIG_MAIN
#include "../include/state.h"
#include "catch2/catch_test_macros.hpp"


TEST_CASE("neighbours", "[neighbours]"){
    StateMatrix state = {{{false, true, false}, {true, false, true}, {true, true, true}}};

    // 0 1 0
    // 1 0 1
    // 1 1 1

    SECTION("central"){
        int num = get_number_of_neighbours(1, 1, state);
        REQUIRE(num == 6);
    }

    SECTION("edge"){
        int num = get_number_of_neighbours(0, 0, state);
        REQUIRE(num == 2);
    }
}

bool states_equal(StateMatrix current, StateMatrix next){
    for(int i = 0; i<map_size; i++){
        for(int j=0; j<map_size;j++){
            if(current[i][j] != next[i][j]){
                return false;
            }
        }
    }
    return true;
}

TEST_CASE("next state", "[next_state]"){
    // O 0 X    0 X 0
    // X X 0 -> X X X
    // 0 X 0    X X 0
    StateMatrix current = {{{false, false, true}, {true, true, false}, {false, true, false}}};
    StateMatrix next = {{{false, true, false}, {true, true, true}, {true, true, false}}};

    StateMatrix result = next_state(current);

    REQUIRE(states_equal(next, result));
}