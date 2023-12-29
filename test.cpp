#include "include/state.h"
#include <catch2/catch_test_macros.hpp>


TEST_CASE("neighbours", "[neighbours]"){
    StateMatrix state = {{{false, true, false}, {true, false, true}, {true, true, true}}};

    // 0 1 0
    // 1 0 1
    // 1 1 1

    SECTION("central"){
        int num = get_number_of_neighbours(1, 1, state);

        REQUIRE(num == 6);
    }

}